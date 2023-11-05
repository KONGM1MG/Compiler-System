#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <stack>
#include <set>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <list>
using namespace std;


// 运算符优先级
int priority(char c) 
{
    if(c == '*') return 3;
    else if(c == '.') return 2;
    else if(c == '|') return 1;
    else return 0;
}

class DFA
{
public:
    set<int> states;
    map<int, map<char, int>> transitions; 
    int startState;
    set<int> acceptStates;
    set<char> inputs;
    
    // 添加状态
    void addState(int state) 
    {
        states.insert(state);
    }
    // 添加接受状态
    void addAcceptState(int state) 
    {
        acceptStates.insert(state);
    }
    // 添加转换
    void addTransition(int from, char input, int to) 
    {
        if (transitions.find(from) == transitions.end()) 
        {
            transitions[from] = map<char, int>();
        }
        transitions[from][input] = to;
    }

    void printDFA()
    {
        cout << "DFA States:" << endl;
        for (const auto& state : states) 
        {
            cout << state;
            if (state == startState) 
            {
                cout << " [Start]";
            }
            if (acceptStates.find(state) != acceptStates.end()) 
            {
                cout << " [Accept]";
            }
            cout << endl;
        }

        cout << "\nDFA Transitions:" << endl;
        for (const auto& transition : transitions) 
        {
            for (const auto& input : transition.second) 
            {
                cout << "State " << transition.first << " with input '" << input.first << "' goes to state " << input.second << endl;
            }
        }

        cout << "\nDFA Start State:" << endl;
        cout << startState << endl;

        cout << "\nDFA Accept States:" << endl;
        for (const auto& acceptState : acceptStates) {
            cout << acceptState << " ";
        }
        cout << endl;
    }

    DFA minimize() 
    {
    map<int, int> partitions; 

    // 初始分区
    int nonAcceptingPartition = 1;
    int acceptingPartition = 0;

    for (int state : states) 
    {
        if (acceptStates.find(state) != acceptStates.end()) 
        {
            partitions[state] = acceptingPartition; 
        } 
        else 
        {
            partitions[state] = nonAcceptingPartition; 
        }
    }
    int nextPartition = 2;

    map<pair<int, char>, set<int>> transitionGroups;
    
    bool changed;
    do 
    {
        changed = false;
        map<int, int> newPartitions = partitions;
        for (int state : states) 
        {
            for (char input : inputs) 
            {
                if(transitions[state].count(input) > 0) 
                {
                    int targetState = transitions[state][input];
                    transitionGroups[{partitions[state], input}].insert(state);
                }
            }
        }

        for (const auto& group : transitionGroups) 
        {
            if(group.second.size() == 1) continue;
            int representativeState = *group.second.begin();
            for (int state : group.second) 
            {
                bool isSame = true;
                for (char input : inputs) 
                {
                    if(transitions[state].count(input) > 0 && transitions[representativeState].count(input) > 0) 
                    {
                        if(partitions[transitions[state][input]] != partitions[transitions[representativeState][input]]) 
                        {
                            isSame = false;
                            break;
                        }
                    } 
                    else if (transitions[state].count(input) != transitions[representativeState].count(input)) 
                    {
                        isSame = false;
                        break;
                    }
                }
                if(!isSame) 
                {
                    newPartitions[state] = nextPartition++;
                    changed = true;
                }
            }
        }

        partitions = newPartitions;
        transitionGroups.clear();
    } while (changed);

    // 创建新的DFA
    DFA newDFA;
    for (int state : states) 
    {
        int newState = partitions[state];
        newDFA.states.insert(newState);
        if (state == startState) 
        {
            newDFA.startState = newState;
        }
        if (acceptStates.find(state) != acceptStates.end()) 
        {
            newDFA.acceptStates.insert(newState);
        }
    }

    // 重建转换
    for (auto& trans : transitions) 
    {
        for (auto& inp : trans.second) 
        {
            newDFA.transitions[partitions[trans.first]][inp.first] = partitions[inp.second];
        }
    }

    newDFA.inputs = inputs;
    return newDFA;
}




};

class NFA 
{
public:
    string regex;      // 正则表达式    
    string postfix_regex;      // 正则表达式的后缀形式
    int stateNum;   // 状态数
    pair<int, int> se;      // 起点和终点状态编号
    vector<vector<pair<int, char> > > graph;  // NFA状态关系图, graph[i] 代表第i个状态中与第i个状态转换的所有状态和动作
    vector<char> inputs;
public:
    NFA(string regex)
    {
        this->regex = regex;
        stateNum = 0;
        graph.push_back(vector<pair<int, char> >());
    }
    
    // 中缀转后缀
    void re2Pe()
    {
        stack<char> op; // 符号栈
        for(auto c : regex) 
        {
            if(c == '(') op.push(c);
            else if(c == ')') 
            {   // 遇到 ） 则将栈中的符号弹出，直到遇到 （
                while(op.top() != '(') 
                {
                    postfix_regex += op.top();
                    op.pop();
                }
                op.pop();
            }
            else if(c == '*' || c == '.' || c == '|') 
            {   
                while(op.size()) 
                {
                    if(priority(c) <= priority(op.top())) 
                    {
                        postfix_regex += op.top();
                        op.pop();
                    }
                    else break;
                }
                op.push(c);
            }
            else postfix_regex += c;
        }
        while(op.size()) 
        {
            postfix_regex += op.top();
            op.pop();
        }
    }
    int newState() 
    {
        graph.push_back(vector<pair<int, char> >());    // 生成新状态的边集
        return ++stateNum;
    }
    // 后缀转NFA
    void pe2NFA()
    {
        stack<pair<int, int> > states;      // 状态栈
        int s, e;       // 状态边起点和终点状态编号
        // 遍历后缀表达式
        for(auto c : postfix_regex)
        {
            // 识别输出
            if(c != '*' && c != '.' && c != '|')
            {
                s = newState();
                e = newState();
                states.push(make_pair(s, e));
                graph[s].push_back(make_pair(e, c));
                inputs.push_back(c);
                continue;
            }
            switch (c)
            {
            case '*': 
            {
                pair<int, int> origin = states.top(); states.pop();
                s = newState();
                e = newState();
                states.push(make_pair(s, e));
                graph[s].push_back(make_pair(origin.first, ' '));
                graph[s].push_back(make_pair(e, ' '));
                graph[origin.second].push_back(make_pair(e, ' '));
                graph[origin.second].push_back(make_pair(origin.first, ' '));
                break;
            }
            case '.': 
            {
                pair<int, int> right = states.top(); states.pop();
                pair<int, int> left = states.top(); states.pop();
                states.push(make_pair(left.first, right.second));
                graph[left.second].push_back(make_pair(right.first, ' '));
                break;
            }
            case '|': 
            {
                pair<int, int> down = states.top(); states.pop();
                pair<int, int> up = states.top(); states.pop();
                s = newState();
                e = newState();
                states.push(make_pair(s, e));
                graph[s].push_back(make_pair(up.first, ' '));
                graph[s].push_back(make_pair(down.first, ' '));
                graph[up.second].push_back(make_pair(e, ' '));
                graph[down.second].push_back(make_pair(e, ' '));
                break;
            }
            default:
                break;
            }
        }
        se = make_pair(states.top().first, states.top().second);
    }

    void printNFA()
    {
        cout<< "regex: " << regex << "\n"
            << "stateNum: " << stateNum << "\n"
            << "start: " << se.first << "\n"
            << "end: " << se.second << endl;
        for(int i = 1; i <= stateNum; i++) 
        {
            for(auto edge : graph[i]) 
            {
                cout << i << "----" << edge.second << "---->" << edge.first << "\n";
            }
        }
        cout << endl;
    }

    DFA subsetConstruction() 
    {
        DFA dfa;
        map<set<int>, int> stateMapping; // 将NFA状态集映射到DFA状态
        queue<set<int>> processingQueue; // 使用队列
        int dfaStateCount = 0;
        set<char> dfaInputs; // 用于跟踪在构造过程中遇到的所有非空边字符

        // 初始状态
        set<int> startClosure = epsilonClosure({se.first});
        processingQueue.push(startClosure);
        stateMapping[startClosure] = dfaStateCount;
        dfa.startState = dfaStateCount;
        dfa.addState(dfaStateCount++);

        while (!processingQueue.empty()) {
            set<int> current = processingQueue.front();
            processingQueue.pop();
            int currentStateId = stateMapping[current];
            
            // 接受状态检查
            if (current.find(se.second) != current.end()) 
            {
                dfa.addAcceptState(currentStateId);
            }

            // 转换检查
            for (char input : inputs) 
            { // inputs 需要是NFA中所有的非空边字符的集合

                set<int> newState;
                for (int state : current) {
                    for (auto edge : graph[state]) 
                    {
                        if (edge.second == input) 
                        {
                            newState.insert(edge.first);
                            if (input != ' ') // 如果输入不是epsilon-transition
                            {
                                dfaInputs.insert(input);
                            }
                        }
                    }
                }
                set<int> newClosure = epsilonClosure(newState);
                if (!newClosure.empty()) {
                    if (stateMapping.find(newClosure) == stateMapping.end()) 
                    {
                        stateMapping[newClosure] = dfaStateCount;
                        processingQueue.push(newClosure);
                        dfa.addState(dfaStateCount++);
                    }
                    dfa.addTransition(currentStateId, input, stateMapping[newClosure]);
                }
            }
        }
        dfa.inputs = dfaInputs;
        return dfa;
    }

    
    set<int> epsilonClosure(set<int> states)
    {
        set<int> closure = states;
        stack<int> toProcess;

        for (int state : states)
        {
            toProcess.push(state);
        }

        while (!toProcess.empty())
        {
            int currentState = toProcess.top();
            toProcess.pop();

            for (auto edge : graph[currentState])
            {
                if (edge.second == ' ' && closure.find(edge.first) == closure.end())
                {
                    closure.insert(edge.first);
                    toProcess.push(edge.first);
                }
            }
        }

        return closure;
    }
};



void generateNFA_DOT(const NFA &nfa, const string &filename) {
    ofstream dotFile(filename);
    dotFile << "digraph NFA {" << endl;
    dotFile << "rankdir=LR;" << endl;

    for (int i = 1; i <= nfa.stateNum; i++) {
        dotFile << i;
        if (i == nfa.se.first) {
            dotFile << " [shape=point]";
        } else if (i == nfa.se.second) {
            dotFile << " [shape=doublecircle]";
        } else {
            dotFile << " [shape=circle]";
        }
        dotFile << ";" << endl;
    }

    for (int i = 1; i <= nfa.stateNum; i++) {
        for (auto edge : nfa.graph[i]) {
            dotFile << i << " -> " << edge.first << " [label=\"" << edge.second << "\"];" << endl;
        }
    }

    dotFile << "}" << endl;
    dotFile.close();
}

void generateDFA_DOT(const DFA& dfa, const string& filename)
{
    ofstream dotFile(filename);

    dotFile << "digraph DFA {" << endl;
    dotFile << "rankdir=LR;" << endl;
    dotFile << "node [shape = circle];" << endl;

    dotFile << "node [shape = doublecircle];";
    for (int state : dfa.acceptStates) 
    {
        dotFile << " " << state;
    }
    dotFile << ";\n";
    dotFile << "node [shape = circle];" << endl;

    for (const auto& sourcePair : dfa.transitions) 
    {
        int sourceState = sourcePair.first;
        for (const auto& transPair : sourcePair.second) 
        {
            char input = transPair.first;
            int destState = transPair.second;
            dotFile << "    " << sourceState << " -> " << destState << " [ label = \"" << input << "\" ];\n";
        }
    }

    dotFile << "}" << endl;
    dotFile.close();
}






int main(void)
{
    string regex;
    cin>>regex;

    NFA nfa(regex);
    nfa.re2Pe();
    nfa.pe2NFA();
    //nfa.printNFA();
    generateNFA_DOT(nfa, "nfa.dot");

    DFA dfa = nfa.subsetConstruction();
    for (auto a : dfa.inputs)
    {
        cout << a <<endl;
    }
    dfa.printDFA();
    generateDFA_DOT(dfa, "dfa.dot");
    DFA dfa1 = dfa.minimize();
    dfa1.printDFA();
    generateDFA_DOT(dfa1, "dfa1.dot");
    
    return 0;
}

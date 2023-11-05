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
    void addState(int state) {
        states.insert(state);
    }
    // 添加接受状态
    void addAcceptState(int state) {
        acceptStates.insert(state);
    }

    // 添加转换的方法
    void addTransition(int from, char input, int to) {
        if (transitions.find(from) == transitions.end()) {
            transitions[from] = map<char, int>();
        }
        transitions[from][input] = to;
    }

    void printDFA()
    {
        cout << "DFA States:" << endl;
        for (const auto& state : states) {
            cout << state;
            if (state == startState) {
                cout << " [Start]";
            }
            if (acceptStates.find(state) != acceptStates.end()) {
                cout << " [Accept]";
            }
            cout << endl;
        }

        cout << "\nDFA Transitions:" << endl;
        for (const auto& transition : transitions) {
            for (const auto& input : transition.second) {
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

    DFA minimize() {
        map<int, int> partitions; // 将每个状态映射到其分区
        int nextPartition = 0;

        // 初始分区
        for (int state : states) {
            if (acceptStates.find(state) != acceptStates.end()) {
                partitions[state] = 0; // 接受状态分区
            } else {
                partitions[state] = 1; // 非接受状态分区
            }
        }
        nextPartition = 2;

        bool changed = true;
        while (changed) {
            changed = false;
            map<int, vector<int>> newPartitions;

            for (int state : states) {
                for (char input : inputs) {
                    int targetState = transitions[state][input];
                    int targetPartition = partitions[targetState];

                    if (newPartitions.find(targetPartition) == newPartitions.end()) {
                        newPartitions[targetPartition] = vector<int>();
                    }
                    newPartitions[targetPartition].push_back(state);
                }
            }

            for (const auto& partition : newPartitions) {
                if (partition.second.size() > 1) {
                    for (int state : partition.second) {
                        partitions[state] = nextPartition++;
                    }
                    changed = true;
                }
            }
        }

        // 创建新的DFA
        DFA newDFA;
        for (int state : states) {
            int newState = partitions[state];
            newDFA.states.insert(newState);
            if (state == startState) {
                newDFA.startState = newState;
            }
            if (acceptStates.find(state) != acceptStates.end()) {
                newDFA.acceptStates.insert(newState);
            }
            for (char input : inputs) {
                newDFA.transitions[newState][input] = partitions[transitions[state][input]];
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

    // 子集构造法将NFA转换为DFA
    // map<set<int>, map<char, set<int>>> subsetConstruction()
    // {
    //     DFA todfa;
    //     map<set<int>, map<char, set<int>>> dfa; // DFA状态转移表
    //     stack<set<int>> toProcess;  // 待处理状态集
    //     set<set<int>> processed;    // 已处理状态集

    //     // 初始状态
    //     set<int> startState;
    //     startState.insert(se.first);    // 起点状态
    //     set<int> startClosure = epsilonClosure(startState); // 起点状态的ε闭包

    //     toProcess.push(startClosure);   // 将起点状态的ε闭包加入待处理状态集
    //     processed.insert(startClosure); // 将起点状态的ε闭包加入已处理状态集

    //     while (!toProcess.empty())
    //     {
    //         set<int> currentState = toProcess.top();    // 当前状态
    //         toProcess.pop();    // 将当前状态从待处理状态集中弹出

    //         map<char, set<int>> transitions;    // 当前状态的转换表
    //         // 遍历当前状态的所有状态
    //         for (int state : currentState)
    //         {
    //             // 遍历当前状态的所有边
    //             for (auto edge : graph[state])
    //             {
    //                 // 如果边上有输入字符，则将该状态加入转换表
    //                 if (edge.second != ' ')
    //                 {
    //                     transitions[edge.second].insert(edge.first);
    //                 }
    //             }
    //         }
    //         // 遍历转换表
    //         for (auto transition : transitions)
    //         {
    //             set<int> nextState; // 下一个状态
    //             for (int state : transition.second)
    //             {
    //                 nextState.insert(state);    // 将转换表中的状态加入下一个状态
    //             }
    //             set<int> nextClosure = epsilonClosure(nextState);   // 下一个状态的ε闭包
                
    //             if (processed.find(nextClosure) == processed.end())
    //             {
    //                 toProcess.push(nextClosure);
    //                 processed.insert(nextClosure);
    //             }

    //             dfa[currentState][transition.first] = nextClosure;
    //         }
    //     }
        
    //     todfa.startState = epsilonClosure(startState);
    //     todfa.acceptStates = processed;
    //     todfa.states = processed;
    //     todfa.transitions = dfa;

    //     return dfa;
    // }

    DFA subsetConstruction() 
    {
        DFA dfa;
        map<set<int>, int> stateMapping; // 将NFA状态集映射到DFA状态
        queue<set<int>> processingQueue; // 使用队列代替栈
        int dfaStateCount = 0;

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
            if (current.find(se.second) != current.end()) {
                dfa.addAcceptState(currentStateId);
            }

            // 转换检查
            for (char input : inputs) { // inputs 需要是NFA中所有的非空边字符的集合
                set<int> newState;
                for (int state : current) {
                    for (auto edge : graph[state]) {
                        if (edge.second == input) {
                            newState.insert(edge.first);
                        }
                    }
                }
                set<int> newClosure = epsilonClosure(newState);
                if (!newClosure.empty()) {
                    if (stateMapping.find(newClosure) == stateMapping.end()) {
                        stateMapping[newClosure] = dfaStateCount;
                        processingQueue.push(newClosure);
                        dfa.addState(dfaStateCount++);
                    }
                    dfa.addTransition(currentStateId, input, stateMapping[newClosure]);
                }
            }
        }
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






int main(void)
{
    string regex;
    cin>>regex;

    NFA nfa(regex);
    nfa.re2Pe();
    nfa.pe2NFA();
    nfa.printNFA();
    generateNFA_DOT(nfa, "nfa.dot");

    DFA dfa = nfa.subsetConstruction();
    dfa.printDFA();
    dfa.minimize();
    dfa.printDFA();

    
    return 0;
}

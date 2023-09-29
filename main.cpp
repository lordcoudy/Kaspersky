#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <stack>

using namespace std;

struct Computer {
    string name;
    int macAddress;
    bool infected;

    Computer(string name, int macAddress) : name(name), macAddress(macAddress), infected(false) {}
};

void dfs(unordered_map<int, vector<int>>& graph, vector<Computer>& computers, int start, unordered_set<int>& visited) {
    stack<int> s;
    s.push(start);
    visited.insert(start);

    while (!s.empty()) {
        int current = s.top();
        s.pop();

        // Check if the computer is infected
        if (computers[current].name.find("infected") != string::npos) {
            computers[current].infected = true;
        }

        // Traverse adjacent computers
        for (int neighbor : graph[current]) {
            if (visited.find(neighbor) == visited.end()) {
                s.push(neighbor);
                visited.insert(neighbor);

                // Propagate infection to adjacent computers
                if (computers[current].infected) {
                    computers[neighbor].infected = true;
                }
            }
        }
    }
}

vector<string> scanComputers(int N, vector<pair<string, int>>& computerData, int M, vector<pair<int, int>>& connections) {
    // Create graph and sort computers based on MAC addresses
    unordered_map<int, vector<int>> graph;
    vector<Computer> computers;

    for (int i = 0; i < N; ++i) {
        computers.emplace_back(computerData[i].first, computerData[i].second);
    }

    sort(computers.begin(), computers.end(), [](const Computer& c1, const Computer& c2) {
        return c1.macAddress < c2.macAddress;
    });

    for (int i = 0; i < M; ++i) {
        int computer1 = connections[i].first;
        int computer2 = connections[i].second;
        graph[computer1].push_back(computer2);
        graph[computer2].push_back(computer1);
    }

    // Perform DFS to scan the network
    unordered_set<int> visited;
    dfs(graph, computers, 0, visited);

    // Collect infected computer names
    vector<string> infectedComputers;
    for (const Computer& computer : computers) {
        if (computer.infected) {
            infectedComputers.push_back(computer.name);
        }
    }

    return infectedComputers;
}

int main() {
    int N;
    cin >> N;

    vector<pair<string, int>> computerData(N);
    for (int i = 0; i < N; ++i) {
        cin >> computerData[i].first >> computerData[i].second;
    }

    int M;
    cin >> M;

    vector<pair<int, int>> connections(M);
    for (int i = 0; i < M; ++i) {
        cin >> connections[i].first >> connections[i].second;
    }

    vector<string> infectedComputers = scanComputers(N, computerData, M, connections);

    for (const string& computer : infectedComputers) {
        cout << computer << endl;
    }

    return 0;
}
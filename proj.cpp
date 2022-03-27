#include <iostream>
#include <vector>
using namespace std;

typedef vector<vector<int>> graph;

graph transposed_graph = graph();
vector<int> colors = vector<int>(); // 0 -> não visitado, 1 -> visitado, 2 -> bloqueado, 3 -> solução, 4 -> inválido, 5 -> bloqueado2.

int n_nodes, n_edges, v1, v2, total;

int dfs_visit(int node) {
    int adj = transposed_graph[node].size();
    colors[node] = 1;
    
    for (int i = 0; i < adj; i++) {
        if (colors[transposed_graph[node][i]] == 0) dfs_visit(transposed_graph[node][i]);
        else if (colors[transposed_graph[node][i]] == 1) return 1;
    }

    colors[node] = 2;

    return 0;
}

int parse() {

    int father, son;

    transposed_graph.resize(n_nodes+1, vector<int>());
    colors.resize(n_nodes+1, 0);

    for (int i = 0; i < n_edges; i++) {
        cin >> son;
        cin >> father;
        total++;
        transposed_graph[father].push_back(son); // cada indice do grafo tem o vetor de pais, e como é transposto são os adjacentes
        if (transposed_graph[father].size() > 2) return 1;
    }
    
    for (int i = 1; i < n_nodes+1; i++) {
        if (colors[i] == 0 && dfs_visit(i) == 1) return 1;
        if (colors[i] == 1) return 1;
    }
    return 0;
}

int dfs_visit_v1(int node) {
    int adj = transposed_graph[node].size(); // nao visitado

    for (int i = 0; i < adj; i++) {
        if(colors[transposed_graph[node][i]] == 0) {
            colors[transposed_graph[node][i]] = 1;
            dfs_visit_v1(transposed_graph[node][i]);
        }
    }
    colors[node] = 2;

    return 0;
}

// se devolver diferente de 0 não temos solução
int get_ancestors_number(int node) {
    int adj = transposed_graph[node].size();

    for (int i = 0; i < adj; i++) {
        if (colors[node] == 3 || colors[node] == 4) {
            colors[transposed_graph[node][i]] = 4;
        }
        get_ancestors_number(transposed_graph[node][i]);
    }
    return 0;
}

int dfs_visit_v2(int node) {
    int adj = transposed_graph[node].size();

    for (int i = 0; i < adj; i++) {
        // magia de ver se os ancestrais dele são todos diferentes de 0 e se forem meter a 3
        if (colors[transposed_graph[node][i]] == 0) {
            colors[transposed_graph[node][i]] = 1;
            dfs_visit_v2(transposed_graph[node][i]);
        }
        if (colors[transposed_graph[node][i]] == 2) {
            colors[transposed_graph[node][i]] = 3;
            dfs_visit_v2(transposed_graph[node][i]);
        }
    }
    get_ancestors_number(node);
    if(colors[node] == 1)
        colors[node] = 5;
    return 0;
}

int find_closest_common_ancestors() {
    int answers = 0;
    colors.clear();
    colors.resize(n_nodes+1, 0);
    // todos os vertices estão bloqueados right now
    colors[v1] = 1;
    dfs_visit_v1(v1);
    if (colors[v2] == 0) {
        colors[v2] = 1; // meter os ancestrais do v1 a nao visitado
        dfs_visit_v2(v2);
    } else if (colors[v2] == 2) colors[v2] = 3;

    for (int i = 1; i < n_nodes+1; i++) {
        if (colors[i] == 3) {
            cout << i << " ";
            answers++;
        }
    }
    if (answers == 0) cout << "-";
    cout << endl;

    return 0;
}

int main() {
    ios::sync_with_stdio(false);

    cin >> v1;
    cin >> v2;
    cin >> n_nodes;
    cin >> n_edges;
    
    if (parse() == 1) {
        cout << 0 << endl;
        return 0;
    }

    find_closest_common_ancestors();
    cout << "Total: " << total << endl; 

    return 0;
}
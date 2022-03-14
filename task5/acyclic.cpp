#include<iostream>
#include<vector>
#include<set>
#include<stack>

using namespace std;

int Max(int a, int b) {
    return a > b ? a : b;
}

int Min(int a, int b) {
    return a < b ? a : b;
}

void dfs1(vector<vector<int>>& out, vector<int>& t, set<int>& all, int start, int parent, stack<int>& peaks) {
    all.erase(start);
    t[start] = 1;
    
    for (int i = 0; i < out[start].size(); i++) {
        if (out[start][i] == parent) {
            continue;
        } else if (t[out[start][i]] == -1) {
            dfs1(out, t, all, out[start][i], start, peaks);
        }
    }
    peaks.push(start);
}

void dfs2(vector<vector<int>>& in, vector<int>& colors, int start, int parent, int color, vector<int>& ind, vector<int>& v) {
    colors[start] = color;
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == start) {
            ind[i] = 1;
            break;
        }
    }
    
    for (int i = 0; i < in[start].size(); i++) {
        if (in[start][i] == parent) {
            continue;
        } else if (colors[in[start][i]] == -1) {
            dfs2(in, colors, in[start][i], start, color, ind, v);
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    vector<pair<int, int>> edges;
    vector<vector<int>> out(n + 1);
    vector<vector<int>> in(n + 1);
    for (int i = 0; i < m; i++) {
        pair<int, int> peak;
        int x, y;
        cin >> x >> y;
        peak.first = x;
        peak.second = y;
        edges.push_back(peak);
        out[x].push_back(y);
        in[y].push_back(x);
    }
    
    //dfs1
    
    set<int> all;
    for (int i = 1; i <= n; i++) {
        all.insert(i);
    }
    
    stack<int> peaks;
    vector<int> t(n + 1, -1);
    while (!all.empty()) {
        set<int> :: iterator it = all.begin();
        dfs1(out, t, all, *it, 0, peaks);
    }
    
    vector<int> v(n);
    vector<int> ind(n, -1);
    for (int i = 0; i < n; i++) {
        v[i] = peaks.top();
        peaks.pop();
    }
    
    //dfs2
    
    vector<int> colors(n + 1, -1);
    int color = 0;
    while (true) {
        int index = -1;
        for (int i = 0; i < n; i++) {
            if (ind[i] == -1) {
                index = i;
                break;
            }
        }
        if (index == -1) {
            break;
        }
        color++;
        dfs2(in, colors, v[index], 0, color, ind, v);
    }
    
    int count = 0;
    set<pair<int, int>> result;
    for (int i = 0; i < edges.size(); i++) {
        if (colors[edges[i].first] != colors[edges[i].second]) {
            pair<int, int> x, inv;
            x.first = colors[edges[i].first];
            x.second = colors[edges[i].second];
            if (result.find(x) == result.end()) {
                result.insert(x);
            }
        }
    }
    
    cout << result.size();
    
    return 0;
}
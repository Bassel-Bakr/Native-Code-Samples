#include "bits/stdc++.h""
using namespace std;
using ll = long long;

tuple<vector<int>, set<pair<int, int>>, int> dfs(
    int const u,                              // node
    int const p,                              // parent
    vector<vector<int>> const& g,             // graph
    vector<pair<int, int>> const& queries,    // [v, d]
    vector<vector<int>> const& node_queries,  // [v] => queries of this node
    vector<int>& depth,                       // node depth from root
    vector<int>& stack,                       // dfs stack,
    vector<int>& ans) {
  stack.emplace_back(u);
  depth[u] = depth[p] + 1;

  int offset = 0;
  vector<int> chain;
  set<pair<int, int>> pending;

  for (auto v : g[u]) {
    if (v == p) continue;

    auto&& [v_chain, v_pending, v_offset] =
        dfs(v, u, g, queries, node_queries, depth, stack, ans);

    /* try to solve */
    // solve (v) subtree using previous values
    for (auto it = v_pending.begin(); it != v_pending.end();
         v_pending.erase(it++)) {
      auto [d, idx] = *it;
      d -= v_offset;

      if (d < 0) continue;
      
      if (int(chain.size()) < d) break;

      ans[idx] = *(chain.rbegin() + d - 1);
    }

    // solve previous values using (v) subtree
    for (auto it = pending.begin(); it != pending.end(); pending.erase(it++)) {
      auto [d, idx] = *it;
      d -= offset;

      if (d < 0) continue;

      if (int(v_chain.size()) < d) break;

      ans[idx] = *(v_chain.rbegin() + d - 1);
    }

    // combine
    if (chain.size() < v_chain.size()) swap(chain, v_chain);

    if (pending.size() < v_pending.size()) {
      swap(offset, v_offset);
      swap(pending, v_pending);
    }

    for (auto [d, idx] : v_pending) {
      d -= v_offset;
      pending.emplace(d + offset, idx);
    }
  }

  // add (u) to the long chain
  chain.emplace_back(u);

  // solve for (u):
  for_each(node_queries[u].begin(), node_queries[u].end(), [&](auto const idx) {
    auto [v, d] = queries[idx];

    // up?
    if (0 <= d and d < int(stack.size())) {
      ans[idx] = *(stack.rbegin() + d);
      return;
    }

    // down?
    if (0 <= d and d < int(chain.size())) {
      ans[idx] = *(chain.rbegin() + d);
      return;
    }

    // nothing for now ... add to (pending) list
    pending.emplace(d + offset, idx);
  });

  stack.pop_back();

  return make_tuple(move(chain), move(pending), offset + 1);
}

int main() {
  cin.tie(0);
  cin.sync_with_stdio(0);

  int n, q;
  cin >> n >> q;

  vector<vector<int>> g(n + 1);

  for (int i = 0; i < n - 1; ++i) {
    int u, v;
    cin >> u >> v;

    g[u].emplace_back(v);
    g[v].emplace_back(u);
  }

  vector<pair<int, int>> queries(q);
  vector<vector<int>> node_queries(n + 1);

  for (int i = 0; i < q; ++i) {
    auto& [v, d] = queries[i];
    cin >> v >> d;
    node_queries[v].emplace_back(i);
  }

  vector<int> ans(q);
  vector<int> stack, depth(n + 1);

  dfs(1, 0, g, queries, node_queries, depth, stack, ans);

  for (auto x : ans) {
    cout << x << '\n';
  }
}

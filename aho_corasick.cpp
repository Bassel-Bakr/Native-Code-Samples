#include <stdio.h>
#include <bits/stdc++.h>

using namespace std;

struct node {
  node* f = 0;
  node* kids[26] = {};
  set<int> mark;
};

struct aho_corasick {
  node ROOT;
  node* root = &ROOT;

  void insert(char* pattern, int id) {
    auto p = root;
    for(int i = 0; pattern[i]; ++i) {
      int idx = pattern[i] - 'a';
      if(not p->kids[idx])
        p->kids[idx] = new node;
      p = p->kids[idx];
    }
    p->mark.insert(id);
  }

  void build() {
    queue<node*> q;
    q.push(root);

    while(not q.empty()) {
      auto u = q.front(); q.pop();
      for(int i = 0; i < 26; ++i) {
        auto v = u->kids[i];
        if(not v) continue;

        q.push(v);
        auto f = u->f;

        while(f && not f->kids[i])
          f = f->f;

        v->f = f ? f->kids[i] : root;

        if(v->f != root)
          for(int x : v->f->mark)
            v->mark.insert(x);
      }
    }
  }

  vector<int> match(char* txt, int k) {
    vector<int> occ(k);
    auto p = root;
    for(int i = 0; txt[i]; ++i) {
      int idx = txt[i] - 'a';

      while(p && not p->kids[idx])
        p = p->f;

      p = p ? p->kids[idx] : root;

      for(int x : p->mark)
        occ[x]++;
    }

    return occ;
  }
};

int main() {
  aho_corasick ac;
  ac.insert("hers", 0);
  ac.insert("help", 1);
  ac.insert("he", 2);
  ac.insert("she", 3);
  ac.insert("son", 4);
  ac.insert("person", 5);

  ac.build();
  auto v = ac.match("shershelperson", 6);
  for(int x : v)
    printf("%d\n", x);
}

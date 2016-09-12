#include <bits/stdc++.h>

using namespace std;

template<class T>
struct node {
  T x;
  int size = 1;
  stack<node*> kids;

  node(T const& v) {
    x = v;
  }

  ~node() {
    while(kids.size()) {
      delete kids.top();
      kids.pop();
    }
  }
};

template<class T>
struct fib_heap {
  typedef node<T> tnode;

  int cnt = 0;
  tnode* choosen = 0;
  tnode* old_choosen = 0;
  stack<tnode*> buckets[32];

  ~fib_heap() {
    for(int id = 0; id < 32; ++id) {
      join_bucket(id);
      if(buckets[id].size())
        delete buckets[id].top();
    }
  }

  bool empty() {
    return cnt == 0;
  }

  void update_min(tnode* p) {
    if(not choosen)
      choosen = p;
    else
      choosen = (choosen->x <= p->x ? choosen : p);
  }

  void insert(T const& x) {
    ++cnt;
    auto p = new tnode(x);
    update_min(p);
    buckets[0].push(p);
  }

  T top() {
    return choosen->x;
  }

  void pop_kids(node<T>* p) {
    while(p->kids.size()) {
      auto u = p->kids.top(); p->kids.pop();
      int idx = __builtin_ctz(u->size);
      buckets[idx].push(u);
    }
  }

  void join_bucket(int id) {
    while(buckets[id].size() >= 2) {
      auto u = buckets[id].top(); buckets[id].pop();
      auto v = buckets[id].top(); buckets[id].pop();

      if(old_choosen == u) {
        buckets[id].push(v);
        continue;
      }

      if(old_choosen == v) {
        buckets[id].push(u);
        continue;
      }

      if(u->x <= v->x)
        u->size += v->size, u->kids.push(v), buckets[id+1].push(u);
      else
        v->size += u->size, v->kids.push(u), buckets[id+1].push(v);
    }

    if(buckets[id].size() == 1 && buckets[id].top() == old_choosen)
      buckets[id].pop();

  }

  T pop() {
    --cnt;
    T val = top();
    old_choosen = choosen;
    pop_kids(old_choosen);
    delete old_choosen;
    choosen = 0;
    for(int id = 0; id < 32; ++id) {
      join_bucket(id);
      if(buckets[id].size())
        update_min(buckets[id].top());
    }
    return val;
  }
};

int main() {
  int n = 10;

  fib_heap<int> h;
  for(int i = 0; i < n; ++i)
    h.insert(rand());

  while(not h.empty())
    printf("%d\n", h.pop());
}

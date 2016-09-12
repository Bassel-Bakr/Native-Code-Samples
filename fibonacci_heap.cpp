#include <bits/stdc++.h>

using namespace std;

template<class T>
struct node {
  T x;
  int size = 1;
  list<node*> kids;

  node(T const& v) {
    x = v;
  }

  ~node() {
    while(kids.size()) {
      delete kids.front();
      kids.erase(kids.begin());
    }
  }
};

template<class T>
struct fib_heap {
  typedef node<T> tnode;

  int cnt = 0;
  tnode* choosen = 0;
  tnode* old_choosen = 0;
  list<tnode*> buckets[32];

  ~fib_heap() {
    for(int id = 0; id < 32; ++id) {
      join_bucket(id);
      if(buckets[id].size())
        delete buckets[id].front();
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
    buckets[0].push_front(p);
  }

  T top() {
    return choosen->x;
  }

  void pop_kids(node<T>* p) {
    while(p->kids.size()) {
      auto u = p->kids.front(); p->kids.erase(p->kids.begin());
      int idx = __builtin_ctz(u->size);
      buckets[idx].push_front(u);
    }
  }

  void join_bucket(int id) {
    while(buckets[id].size() >= 2) {
      auto u = buckets[id].front(); buckets[id].erase(buckets[id].begin());
      auto v = buckets[id].front(); buckets[id].erase(buckets[id].begin());

      if(old_choosen == u) {
        buckets[id].push_front(v);
        continue;
      }

      if(old_choosen == v) {
        buckets[id].push_front(u);
        continue;
      }

      if(u->x <= v->x)
        u->size += v->size, u->kids.push_front(v), buckets[id+1].push_front(u);
      else
        v->size += u->size, v->kids.push_front(u), buckets[id+1].push_front(v);
    }

    if(buckets[id].size() == 1 && *buckets[id].begin() == old_choosen)
      buckets[id].clear();

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
        update_min(buckets[id].front());
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

#include <iostream>

using namespace std;

struct node {
  int x;
  int s = 1;

  node* l = 0;
  node* r = 0;
  node* p = 0;

  node(int v) {
    x = v;
  }

  node* upd() {
    s = 1;
    if(l) s += l->s;
    if(r) s += r->s;
    return this;
  }

  node* rtl() {
    p->r = l;
    if(l) l->p = p;

    l = p;

    auto g = p->p;    
    if(g) (g->l == p ? g->l : g->r) = this;
    
    p->p = this;
    p->upd();    
    p = g;    
    return upd();
  }

  node* rtr() {
    p->l = r;
    if(r) r->p = p;

    r = p;

    auto g = p->p;
    if(g) (g->l == p ? g->l : g->r) = this;
    
    p->p = this;
    p->upd();
    p = g;
    return upd();
  }

  node* splay() {
    while(p) {
      auto g = p->p;

      if(not g && p->l == this)
        rtr();
      else if(not g && p->r == this)
        rtl();
      else if(g->l == p && p->l == this)
        p->rtr(), rtr();
      else if(g->r == p && p->r == this)
        p->rtl(), rtl();
      else if(g->l == p && p->r == this)
        rtl(), rtr();
      else
        rtr(), rtl();
    }
    return this;
  }

  node* splay_right() {
    auto u = this;
    while(u->r)
      u = u->r;
    return u->splay();
  }

  node* splay_left() {
    auto u = this;
    while(u->l)
      u = u->l;
    return u->splay();
  }
};

struct splay_tree {
  node* root = 0;

  ~splay_tree() {
    if(not root)
      return;

    vector<node*> q;
    for(q.emplace_back(root); q.size(); ) {
      auto u = q.back(); q.pop_back();
      if(u->l) q.emplace_back(u->l);
      if(u->r) q.emplace_back(u->r);
      delete u;
    }
  }

  node* join(node* l, node* r) {
    if(not l) return r;
    if(not r) return l;

    auto lf = l->splay_right();

    lf->splay();
    lf->r = r;
    r->p = lf;
    return lf->upd();
  }

  node* lower_bound(node* p, int x) {
    if(not p)
      return 0;

    node* u = p;

    while(p) {
      u = p;
      p = (x < p->x) ? p->l : p->r;
    }

    return u->splay();
  }

  pair<node*, node*> split(node* p, int x) {
    if(not p)
      return make_pair(nullptr, nullptr);

    auto lf = lower_bound(p, x);
    if(lf->x <= x) {
      auto l = lf;
      auto r = lf->r;
      l->r = 0;
      if(r) r->p = 0;
      l->upd();
      return make_pair(l, r);
    } else {
      auto r = lf;
      auto l = lf->l;
      r->l = 0;
      if(l) l->p = 0;
      r->upd();
      return make_pair(l, r);
    }
  }

  void insert(int x) {
    auto t = split(root, x);
    root = join(t.first, join(new node(x), t.second));
  }

  void erase(int x) {
    auto t = split(root, x);
    if(t.first) {
      t.first = t.first->splay_right();
      if(t.first->x == x) {
        auto tmp = t.first->l;
        if(tmp) tmp->p = 0;        
        delete t.first;    
        t.first = tmp;     
      }
    }

    root = join(t.first, t.second);
  }

  int print() {
    return print(root);
  }

  int print(node* p) {
    if(not p) return 0;
    int l = print(p->l);
    cout << p->x << endl;
    int r = print(p->r);
    return 1 + max(l, r);
  }
};


int main() {
  cin.tie(0);
  cin.sync_with_stdio(0);


  splay_tree t;
  int n = 100;

  for(int i = 0; i < n; ++i)
    t.insert(i);

  t.erase(21);
  t.erase(12);

  cout << "Height: " << t.print() << endl;
}

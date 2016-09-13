#include <bits/stdc++.h>
 
using namespace std;
 
struct node {
  int x;
  int rev = 0;
 
  node* l = 0;
  node* r = 0;
  node* p = 0;
  node* pp = 0;
 
  node(int v) {
    x = v;
  }
};
 
struct link_cut_tree {
  int n;
  vector<node*> nodes;
 
  link_cut_tree(int cnt) {
    n = cnt;
    nodes.resize(n+1);
    for(int i = 1; i <= n; ++i)
      nodes[i] = new node(i);
  }
 
  ~link_cut_tree() {
    for(int i = 1; i <= n; ++i)
      delete nodes[i];
  }
 
  // splay_tree
  inline void set_parent(node* p, node* c) {
    if(c) c->p = p;
  }
 
  inline void push(node* p) {
    if(not p || not p->rev) return;

    p->rev = 0;
    swap(p->l, p->r);
    if(p->l) p->l->rev ^= 1;
    if(p->r) p->r->rev ^= 1;
  }
 
  node* rtr(node* p) {
    auto g = p->p;
    auto c = p->l;
 
    p->l = c->r;
    c->r = p;
 
    set_parent(g, c);
    set_parent(c, p);
    set_parent(p, p->l);
 
    if(g) {
      if(g->l == p)      g->l = c;
      else if(g->r == p) g->r = c;
    }
 
    c->pp = p->pp;
    p->pp = 0;
    return c;
  }
 
  node* rtl(node* p) {
    auto g = p->p;
    auto c = p->r;
 
    p->r = c->l;
    c->l = p;
 
    set_parent(g, c);
    set_parent(c, p);
    set_parent(p, p->r);
 
    if(g) {
      if(g->l == p)      g->l = c;
      else if(g->r == p) g->r = c;
    }
 
    c->pp = p->pp;
    p->pp = 0;
    return c;
  }

  void splay(node* c) {
    while(c->p) {
      auto p = c->p;
      auto g = p->p;
 
      push(g);
      push(p);
      push(c);
 
      if(c == p->l) {
        if(not g)          rtr(p);
        else if(g->l == p) rtr(g), rtr(p);
        else               rtr(p), rtl(g);
      } else {
        if(not g)          rtl(p);
        else if(g->r == p) rtl(g), rtl(p);
        else               rtl(p), rtr(g);
      }
    }
 
    push(c);
  }
 
  // link_cut_tree
  inline void detach_right(node* p) {
    if(p->r) {
      p->r->pp = p;
      p->r->p = 0;
      p->r = 0;
    }
  }
 
  inline void detach_left(node* p) {
    if(p->l) {
      p->l->rev ^= 1;
      p->l->pp = p;
      p->l->p = 0;
      p->l = 0;      
    }
  }

  inline void attach_child(node* p, node* c) {
    detach_right(p);
    p->r = c;
    c->p = p;
    c->pp = 0;
  }
 
  node* access(int u) {
    node *p = nodes[u];
    splay(p);
    while(p->pp) {
      node* pp = p->pp;
      splay(pp); 
      attach_child(pp, p);
      p = pp;
    }
    splay(nodes[u]);
    return p;
  }
 
  inline void make_root(int u) {
    access(u);
    auto p = nodes[u];
    detach_left(p);
  }
 
  inline int find_root(int v) {
    access(v);
    node* p = nodes[v];
    while(p->l) p = p->l;
    splay(p);
    return p->x;
  }
 
  inline void link(int u, int v) {
    access(u);
    make_root(v);
    nodes[v]->pp = nodes[u];
  }
 
  inline void cut(int u, int v) {
    make_root(u);
    access(v);
 
    if(nodes[v]->l == 0) {
      nodes[v]->pp = 0;
    } else {
      nodes[v]->l->p = 0;
      nodes[v]->l = 0;
    }
  }
};
 
 
int main() {
  int n, m;
  scanf("%d%d", &n, &m);
  link_cut_tree t(n);
  while(m--) {
    int u, v;
    char w[8];
    scanf(" %s %d %d", w, &u, &v);

    if('a' == w[0])
      t.link(u, v);
    else if('r' == w[0])
      t.cut(u, v);
    else
      puts(t.find_root(u) == t.find_root(v) ? "YES" : "NO");
  }
} 

/* INPUT */
// 5 11
// conn 1 5
// add 1 2
// add 1 3
// add 3 4
// add 5 4
// conn 1 5
// rem 4 5
// conn 1 5
// rem 3 4
// add 3 5
// conn 1 5

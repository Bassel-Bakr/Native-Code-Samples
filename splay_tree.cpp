#include <bits/stdc++.h>

using namespace std;

struct node {
  int x;

  node* p = 0;
  node* l = 0;
  node* r = 0;

  node(int v) {
    x = v;
  }

  node(node* pp, int v) {
    p = pp;
    x = v;    
  }

  ~node() {
    delete l;
    delete r;
  }
};

struct splay_tree {
  node* root = 0;

  ~splay_tree() {
    delete root;
  }

  void set_parent(node* p, node* c) {
    if(c) c->p = p;
  }

  node* rtr(node* p) {
    auto g = p->p;

    auto c = p->l;
    p->l = c->r;
    c->r = p;

    set_parent(g, c);
    set_parent(c, p);
    set_parent(p, p->l);
    set_parent(p, p->r);

    if(g) {
      if(g->l == p)      g->l = c;
      else if(g->r == p) g->r = c;
      else               abort();
    }

    return c;
  }

  node* rtl(node* p) {
    auto g = p->p;

    auto c = p->r;
    p->r = c->l;
    c->l = p;

    set_parent(g, c);
    set_parent(c, p);
    set_parent(p, p->l);
    set_parent(p, p->r);

    if(g) {
      if(g->l == p)      g->l = c;
      else if(g->r == p) g->r = c;
      else               abort();
    }

    return c;
  }

  void splay(node*& root, node* c) {
    while(c->p) {
      auto p = c->p;
      auto g = p->p;

      if(c == p->l) {
        if(not g) rtr(p);
        else if(g->l == p) rtr(g), rtr(p);
        else rtr(p), rtl(g);
      } else {
        if(not g) rtl(p);
        else if(g->r == p) rtl(g), rtl(p);
        else rtl(p), rtr(g);
      }
    }

    root = c;
  }

  void splay(node* c) {
    splay(root, c);
  }

  void find(node*& root, int x) {
    node* prev = 0;
    node* p = root;

    while(p) {
      prev = p;
      if(x == p->x)     p = 0;
      else if(x < p->x) p = p->l;
      else              p = p->r;
    }

    if(prev)
      splay(root, prev);
  }

  void find(int x) {
    find(root, x);
  }

  void insert(int x) {
    if(not root) {
      root = new node(x);
      return;
    }

    find(x);
    node *l, *r;
    if(x > root->x) {
      l = root;
      r = l->r;
      l->r = 0;
    } else {
      r = root;
      l = r->l;
      r->l = 0;
    }

    root = new node(x);
    root->l = l;
    root->r = r;

    set_parent(root, l);
    set_parent(root, r);
  }

  void erase(int x) {
    find(x);
    if(not root || x != root->x) return;

    node* l = root->l;
    node* r = root->r;
    set_parent(0, l);
    set_parent(0, r);

    root->l = root->r = 0;
    delete root;

    if(not l || not r) {
      root = l ?: r;
    } else {
      find(r, -1e9);
      r->l = l;
      root = r;
    }
  }

  int print(node* p, int h = -1) {
    if(not p)
      return h;
    int l = print(p->l, h+1);
    printf("%d\n", p->x);
    int r = print(p->r, h+1);
    return max(l, r);
  }

  int print() {
    return print(root);
  }
};

int main() {
  int n = 1e5;

  splay_tree t;

  for(int i = 0; i < n; ++i)
    t.insert(i);

  for(int i = 0; i < 16; ++i)
    t.find(i);

  t.erase(99997);
  t.erase(99999);
  t.erase(99980);

  int h = t.print();

  printf("height = %d\n", h);
}

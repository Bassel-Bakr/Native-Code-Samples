#include <bits/stdc++.h>

using namespace std;

struct node {
  int x;
  int s = 1;
  node* l = 0;
  node* r = 0;

  node(int v) {
    x = v;
  }

  ~node() {
    delete l, l = 0;
    delete r, r = 0;
  }
};

struct sbt {
  node* root = 0;

  sbt() = default;

  sbt(node* p) {
    root = p;
  }

  ~sbt() {
    delete root, root = 0;
  }

  inline int size(node* p) {
    return p ? p->s : 0;
  }

  void update(node* p) {
    p->s = size(p->l) + 1 + size(p->r);
  }

  node* rtr(node* p) {
    auto c = p->l;
    p->l = c->r;
    c->r = p;
    update(p);
    return c;
  }

  node* rtl(node* p) {
    auto c = p->r;
    p->r = c->l;
    c->l = p;
    update(p);
    return c;
  }

  node* balance(node* p) {
    while(size(p->l) < size(p->r)>>1)
      p = rtl(p);
    while(size(p->r) < size(p->l)>>1)
      p = rtr(p);
    update(p);
    return p;
  }

  void split(int x, node* p, node*& l, node*& r) {
    if(not p) return void(l = r = 0);

    if(x < p->x)
      split(x, p->l, l, p->l), r = p;
    else
      split(x, p->r, p->r, r), l = p;
  }

  void split_at(int idx, node* p, node*& l, node*& r) {
    if(not p) return void(l = r = 0);

    int l_size = size(p->l);
    if(idx < l_size)
      split_at(idx, p->l, l, p->l), r = p;
    else
      split_at(idx-l_size-1, p->r, p->r, r), l = p;
  }

  node* join(node* l, node* r) {
    if(not l or not r) return l ? : r;

    if(l->s > r->s) {
      l->r = join(l->r, r);
      return balance(l);
    } else {
      r->l = join(l, r->l);
      return balance(r);
    }
  }

  void insert(int x) {
    node*l,*r;
    split(x,root,l,r);
    root = new node(x);
    root = join(l, join(root, r));
  }

  void erase(int x) {
    node *l, *r;
    split(x, root, l, r);

    node *l1, *l2;
    split(x-1, l, l1, l2);

    // erase one instance
    if(l2) {
      node*one,*rest;
      split_at(0, l2, one, rest); 
      delete one;
      l1 = join(l1, rest);
    }
    // erase all instances
    // delete l2;

    root = join(l1, r);
  }

  int print(node* p, int h) {
    int l = p->l ? print(p->l, h + 1) : h;
    printf("%d\n",p->x);
    int r = p->r ? print(p->r, h + 1) : h;
    return max(l,r);
  }

  int print() {
    return root ? print(root,0) : 0;
  }
};

int main() {
  sbt t;
  int n = 1e5;
  for(int i = 0; i < n; ++i)
    t.insert(rand());

  int h = t.print();
  printf("Height = %d\n", h);
}

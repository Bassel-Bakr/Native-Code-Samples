#include <bits/stdc++.h>

using namespace std;

inline int abs2(int const& x) {
  return (x) ^ (x >> 31);
}

struct node {
  int v;
  int sz = 1;
  node*r=nullptr,*l=nullptr;
  
  node(int val) {
    v = val;
  }
  
  ~node() {
    if(r) delete r;
    if(l) delete l;
  }
};

struct tree {
  int cnt = 0;
  
  node* root = nullptr;
  
  ~tree() {
    if(root) delete root;
  }
  
  inline int size(node* n) {
    return n ? n->sz : 0;
  }
  
  inline void update(node* n) {
   if(n) n->sz = size(n->r) + 1 + size(n->l);
  }
  
  inline int balance(node *n) {
    return size(n->r) - size(n->l);
  }
    
  inline node* rtr(node*n) {
    auto c = n->l;
    
    if(abs2(balance(n)) <= abs2(size(c->l) - size(c->r) - size(n->r) - 1)) {
      return n;
    }
    
    n->l = c->r;
    c->r = n;
    update(n);
    return c;
  }
  
  inline node* rtl(node*n) {
    auto c = n->r;
    
    if(abs2(balance(n)) <= abs2(size(c->r) - size(c->l) - size(n->l) - 1)) {
      return n;
    }
    
    n->r = c->l;
    c->l = n;
    update(n);
    return c;
  }
  
  node* insert(node *p, int key) {
    if(!p&&++cnt)
      return new node(key);
    else if(key > p->v)
      p->r = insert(p->r, key), update(p = rtl(p));
    else if(key < p->v)
      p->l = insert(p->l, key), update(p = rtr(p));
    else
      return p;
      
    return p;
  }
  
  void insert(int key) {
    root = insert(root, key); 
  }
  
  int inorder(node*p,int h) {
    int l = p->l?inorder(p->l,h+1):h;
    //printf("%d\n", p->v);
    int r = p->r?inorder(p->r,h+1):h;
    return max(r, l);
  }
  
  int inorder() {
    return root ? inorder(root, 0) : 0;
  }
};

int main() {
  tree t;
  int n = 100000;
  for(int i = 0; i < n; ++i)
    t.insert(i);
  
  printf("Depth = %d\n", t.inorder());
  
  printf("%d == %d\n", t.cnt, t.root->sz);
}

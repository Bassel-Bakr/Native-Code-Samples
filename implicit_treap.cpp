#include<bits/stdc++.h>

using namespace std;
using ll = long long;

int const N = 100 * 1000 + 16;

struct node {
  int x;
  int s = 1;
  int y = rand();
  
  node* l = 0;
  node* r = 0;
  
  node(int v) {
    x = v;
  }
  
  ~node() {
    delete l;
    delete r;
  }
  
  node* update() {
    s = 1;
    if(l) s += l->s;
    if(r) s += r->s;
    return this;
  }
};

struct treap {
  node* root = 0;
  
  ~treap() {
    delete root;
  }
  
  node* join(node* l, node* r) {
    if(not l) return r;
    if(not r) return l;
    
    if(l->y < r->y) {
      l->r = join(l->r, r);
      return l->update();
    } else {
      r->l = join(l, r->l);
      return r->update();
    }
  }
  
  pair<node*, node*> split(node* p, int idx) {
    if(not p)
      return make_pair(nullptr, nullptr);
    
    int lf = (p->l ? p->l->s : 0);
    
    if(idx < lf) {
      auto t = split(p->l, idx);
      p->l = t.second;
      t.second = p->update();
      return t;
    } else {
      auto t = split(p->r, idx-lf-1);
      p->r = t.first;
      t.first = p->update();
      return t;
    }
  }
  
  void insert(int idx, int x) {
    auto t = split(root, idx-1);
    root = join(t.first, join(new node(x),  t.second));
  }
  
  void erase(int idx) {
    auto t1 = split(root, idx-1);
    auto t2 = split(t1.second, 0);
    if(t2.first) delete t2.first;
    t1.second = t2.second;
    root = join(t1.first, t1.second);
  }
  
  int print() {
    return print(root);
  }
  
  int print(node* p) {
    if(not p) return 0;
    int l = print(p->l);
    cout << p->x << "\n";
    int r = print(p->r);
    return 1 + max(l, r);
  }
};

int main()
{
	cin.tie(0);
	cin.sync_with_stdio(0);
	
	int n = 10;
	treap t;
	for(int i = 1; i <= n; ++i)
	  t.insert(1, i);
	  
	t.erase(2);
	
	int h = t.print();
	cout << "Height: " << endl;
}

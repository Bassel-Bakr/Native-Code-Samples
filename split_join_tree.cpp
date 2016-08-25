#include <bits/stdc++.h>

using namespace std;

typedef unsigned long long ull;
typedef long double   ld;
typedef long long     ll;
typedef pair<int,int> pii;
typedef vector<int>   vi;
typedef vector<pii>   vpii;
typedef pair<ll,ll>   pll;
typedef vector<ll>    vl;
typedef vector<pll>   vpll;

#define gc getchar
#define lsb(x) (x&-x)
#define blt(x) __builtin_##x
#define clz blt(clz)
#define ctz blt(ctz)
#define ones blt(popcount)
#define gcd(x,y) __gcd(x,y)
#define lcm(x,y) (x/__gcd(x,y)*y)

#define rep(i,a,b) for(int i = a; i < b; ++i)
#define repq(i,a,b) for(int i = a; i <= b; ++i)

#define rev(i,a,b) for(int i = a; i >= b; --i)
#define revn(i,a,b) for(int i = a; i > b; --i)

#define all(x) x.begin(),x.end()
#define rall(x) x.rbegin(),x.rend()

#define pb push_back
#define eb emplace_back
#define mp make_pair
#define fi first
#define se second

int readln(char *w) {
  scanf(" %[^\n]\n", w);
  return strlen(w);
}

double eps = numeric_limits<double>::epsilon();
double PI = acos(-1);
int INF = 1e9;
ll MAX = 1e18;
int const N = 1e5 + 16;
int const M = 1e9 + 7;
int const M2 = 999999983;

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
    while(size(p->l) * 2 < size(p->r))
      p = rtl(p);
    while(size(p->r) * 2 < size(p->l))
      p = rtr(p);
    update(p);
    return p;
  }

  void split(int x, node* p, node*& l, node*& r) {
    if(not p) return void(l = r = 0);

    if(x < p->x) {
      split(x, p->l, l, p->l);
      p = balance(p);
      r = p;
    } else {
      l = p;
      split(x, p->r, p->r, r);
      p = balance(p);
      l = p;
    }
  }

  void split_at(int idx, node* p, node*& l, node*& r) {
    if(not p) return void(l = r = 0);

    int l_size = size(p->l);
    if(idx < l_size) {
      split_at(idx, p->l, l, p->l);
      p = balance(p);
      r = p;
    } else {
      l = p;
      split_at(idx-l_size-1, p->r, p->r, r);
      p = balance(p);
      l = p;
    }
  }

  node* join(node* l, node* r) {
    if(not l or not r) return l ? : r;

    if(l->s > r->s) {
      l->r = join(l->r, r);
      l = balance(l);
      return l;
    } else {
      r->l = join(l, r->l);
      r = balance(r);
      return r;
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
  rep(i,0,n)
    t.insert(i);

  int h = t.print();
  printf("Height = %d\n", h);
}

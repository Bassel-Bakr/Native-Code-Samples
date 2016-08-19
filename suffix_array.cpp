// Author: BasselBakr
// suffix array construction in O(n log n)

#include <algorithm>
#include <iostream>
#include <vector>
#include <utility>
#include <array>
#include <set>
#include <map>
#include <bitset>
#include <stack>
#include <queue>
#include <sstream>
#include <forward_list>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef pair < int, int >pii;
typedef vector < int >vi;
typedef vector < pii > vpii;
typedef pair < ll, ll > pll;
typedef vector < ll > vl;
typedef vector < pll > vpll;

#ifndef ONLINE_JUDGE
FILE *__redirect__ = freopen("in", "r", stdin);
#else
#  include <bits/stdc++.h>
#endif

#define eps 1e-9
#define lsb(x) (x&-x)
#define del(x) if(x) delete x

#define rep(i,a,b) for(int i = a; i < b; ++i)
#define repq(i,a,b) for(int i = a; i <= b; ++i)

#define rev(i,a,b) for(int i = a; i >= b; --i)
#define revn(i,a,b) for(int i = a; i > b; --i)

#define del(x) if(x)delete x;
#define ctz(x) __builtin_ctz(x)
#define clz(x) __builtin_clz(x)
#define gcd(x,y) __gcd(x,y)
#define lcm(x,y) (x/gcd(x,y)*y)
#define all(x) x.begin(),x.end()
#define rall(x) x.rbegin(),x.rend()

#define pb push_back
#define eb emplace_back
#define mp make_pair
#define fi first
#define se second

int INF = 1e9;
int const N = 1e6 + 16;
int const M = 1e9 + 7;
int const M2 = 999999983;

int n = 1e5;
char w[N];

int sa[N];
int lcp[N];
int idx[N];

struct suffix {
  int i, major, minor;
} suf[N], tmp[N];

int const msk = (1<<19)-1;
int cnt[msk+1];

void radix_sort_init() {
  rep(i,0,n) tmp[i] = suf[i];
  rep(i,0,n) ++cnt[msk&w[tmp[i].i]];
  repq(i,1,256) cnt[i]+=cnt[i-1];
  rev(i,n-1,0) suf[--cnt[msk&w[tmp[i].i]]] = tmp[i];
}

void radix_sort_minor() {
  rep(i,0,n) tmp[i] = suf[i];
  fill(cnt,cnt+msk+1,0);
  rep(i,0,n)  ++cnt[msk&tmp[i].minor];
  repq(i,1,msk) cnt[i]+=cnt[i-1];
  rev(i,n-1,0) suf[--cnt[msk&tmp[i].minor]] = tmp[i];
}

void radix_sort_major() {
  rep(i,0,n) tmp[i] = suf[i];
  fill(cnt,cnt+msk+1,0);
  rep(i,0,n) ++cnt[msk&tmp[i].major];
  repq(i,1,msk) cnt[i]+=cnt[i-1];
  rev(i,n-1,0) suf[--cnt[msk&tmp[i].major]] = tmp[i];
}

void build_sa() {
  rep(i, 0, n) suf[i].i = i;
  radix_sort_init();
  // major rank
  suf[0].major = 1;
  rep(i, 1, n) suf[i].major= suf[i - 1].major + (w[suf[i].i] != w[suf[i - 1].i]);

  for (int j = 1; (1 << j) <= n; j <<= 1) {
    // length
    rep(i, 0, n) idx[n - suf[i].i] = i;
    // minor rank
    rep(i, 0, n) {
      int len = n - suf[i].i;
      suf[i].minor = (len - j <= 0) ? 0 : suf[idx[len - j]].major;
    }
    radix_sort_minor(), radix_sort_major();
    // major rank
    pii prev = mp(suf[0].major, suf[0].minor);
    suf[0].major = 1;    
    rep(i, 1, n) {
      pii rank = mp(suf[i].major, suf[i].minor);
      int x = (prev != rank);
      prev = rank;
      suf[i].major = suf[i - 1].major + x;
    }
    if (suf[n - 1].major == n) break;
  }
  rep(i, 0, n) sa[i] = suf[i].i, idx[n-sa[i]] = i;
}

// kasai
void build_lcp() {
  for(int len = n, k = 0; len; --len, k -= k>0) {
    int i = idx[len];
    if(i == n-1) {
      lcp[i] = k = 0;
      continue;
    }
    while(w[sa[i]+k] == w[sa[i+1]+k]) ++k;
    lcp[i] = k;
  }
}

int main() {
  rep(i, 0, n)
    w[i] = 'a' + rand() % 26;
  build_sa();
  build_lcp();
  rep(i,0,n)
    printf("lcp = %d | str = %.10s\n", lcp[i], w+sa[i]);
}

#include <bits/stdc++.h>

using namespace std;

int const N = 2 * 100 * 1000 + 16;

struct suffix {
  int i;
  pair<int, int> rank;
};

int n;
int sa[N];
int lcp[N];
int index_of_length[N];
suffix suf[N];
char w[N];

void compute_big_rank() {
  pair<int, int> prev = suf[0].rank;
  suf[0].rank.first = 1;
  sa[0] = suf[0].i;
  for(int i = 1; i < n; ++i) {
    auto cur = suf[i].rank;
    suf[i].rank.first = suf[i-1].rank.first + (cur != prev);
    prev = cur;
    sa[i] = suf[i].i;
    index_of_length[n - sa[i]] = i;
  }
}

void compute_min_rank(int p) {
  for(int i = 0; i < n; ++i) {
    int len = n - suf[i].i - p;
    suf[i].rank.second = (len < 1) ? len : suf[index_of_length[len]].rank.first;
  }
}

void build_sa() {
  for(int i = 0; i < n; ++i)
    (suf[i].i = i), (suf[i].rank.first = w[i]), (suf[i].rank.second = w[i+1]);

  int p = 1;
  do {
    sort(suf, suf + n, [] (suffix const& x, suffix const& y) {return x.rank < y.rank;});
    compute_big_rank();
    compute_min_rank(p <<= 1);
  } while(suf[n-1].rank.first < n);
}

void build_lcp() {
  for(int len = n, k = 0; len; --len, k -= (k>0)) {
    int i = index_of_length[len];
    if(i == n - 1) {
      lcp[i] = 0;
      continue;
    }
    while(w[sa[i] + k] == w[sa[i+1] + k]) ++k;
    lcp[i] = k;
  }
}

int main() {
  scanf("%s", w);
  n = strlen(w);
  build_sa();
  build_lcp();
  for(int i = 0; i < n; ++i)
    printf("[%d] %s\n", lcp[i], w + sa[i]);
}

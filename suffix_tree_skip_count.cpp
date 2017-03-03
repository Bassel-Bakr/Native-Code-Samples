#include<iostream>
#include<iomanip>
#include<cmath>
#include<algorithm>
#include<vector>
#include<map>
#include<cassert>
#include<cstring>

typedef long long ll;
using namespace std;

int const N = 100 * 1000 + 16;
int const M = 1000 * 1000 * 1000 + 7;
int const inf = 1000 * 1000 * 1000;

int n, m, k;

struct node
{
	int l, r;
	node *kids[27] = { };

	node(int lf, int rg, int length = 0)
	{
		l = lf;
		r = rg;
	}

	~node()
	{
	  for (auto x:kids)
			delete x;
	}
};

struct suftree
{
	char *w;
	int n = 0;
	int rem = 0;
	int len = 0;
	int edge = 0;
	node *root = new node(0, -1, 0);

	  suftree(char *ww)
	{
		w = ww;
		for (int i = 0; w[i]; ++i)
			add(i);
	}

	inline int get(char c)
	{
		return c == '$' ? 0 : c - ('a' - 1);
	}

	void add(int i)
	{
		n++;
		rem++;
		int c = get(w[i]);
		while (rem)
		{
			// skip count
			auto p = root;
			int e = edge;
			// e - edge = p->len
			while ((e - edge) + (p->r - p->l + 1) < len)
			{
				e += p->r - p->l + 1;
				p = p->kids[get(w[e])];
			}

			int idx = len - (e - edge);

			if (p->l + idx > p->r)
			{
				if (p->kids[c])
				{
					edge = p->kids[c]->l - len;
					len++;
					break;
				}
				p->kids[c] = new node(i, inf);
			}
			else
			{
				int c2 = get(w[p->l + idx]);
				if (c == c2)
				{
					len++;
					break;
				}
				auto tmp = new node(p->l + idx, p->r);
				copy(p->kids, p->kids + 27, tmp->kids);
				fill(p->kids, p->kids + 27, nullptr);
				p->kids[c] = new node(i, inf);
				p->kids[c2] = tmp;
				p->r = tmp->l - 1;
			}
			if (--rem == 0)
				break;
			len--;
			edge++;
		}
	}

	void dfs(node * p, int len = 0)
	{
		int c = 0;
		for (int i = 0; i < 27; ++i)
		{
			auto x = p->kids[i];
			if (not x)
				continue;
			c++;
			dfs(x, len + p->r - p->l + 1);
		}
		if (not c)
		{
			int idx = n - p->l + len;
			cout << (w + n - idx) << "\n";
		}
	}

	void dfs()
	{
		dfs(root, 0);
	}
};

char a[N];

int main()
{
	srand(rand());
	int lim = N - 5;
	for (int i = 0; i + 1 < lim; ++i)
		a[i] = 'a' + rand() % 26;
	a[lim - 1] = '$';
	suftree t("banaban$");
	cout << t.len << ", " << t.rem << endl;
	t.dfs();
}

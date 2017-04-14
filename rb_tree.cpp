#include<iostream>
#include<iomanip>
#include<cmath>
#include<algorithm>
#include<vector>
#include<map>
#include<set>
#include<cassert>
#include<cstring>
#include<bitset>
#include<iterator>
#include<queue>
#include<unordered_map>

typedef unsigned long long ll;
using namespace std;

int const N = 1000 * 1000 + 16;
int const M = 1000 * 1000 * 1000 + 7;
int const inf = 1000 * 1000 * 1000;


struct node
{
	int x;
	int c = 1;

	node *l = 0;
	node *r = 0;

	  node(int v)
	{
		x = v;
	}

	 ~node()
	{
		delete l;
		delete r;
	}
};


struct rb_tree
{
	node *root = 0;

	 ~rb_tree()
	{
		delete root;
	}

	bool red(node * p)
	{
		return p && p->c;
	}

	bool rec(node * p)
	{
		if (not red(p) && red(p->l) && red(p->r))
		{
			p->c ^= 1;
			p->l->c ^= 1;
			p->r->c ^= 1;
			return true;
		}
		return false;
	}

	node *rtr(node * p)
	{
		auto c = p->l;
		p->l = c->r;
		c->r = p;
		swap(p->c, c->c);
		return c;
	}

	node *rtl(node * p)
	{
		auto c = p->r;
		p->r = c->l;
		c->l = p;
		swap(p->c, c->c);
		return c;
	}

	node *fix(node * p)
	{
		if (not p)
			return p;

		auto old = p;
		do
		{
			old = p;
			if (rec(p))
				return p;

			if (red(p->r))
			{
				if (red(p->r->r))
					p = rtl(p);
				else if (red(p->r->l))
				{
					p->r = rtr(p->r);
					p = rtl(p);
				}
			}

			if (red(p->l))
			{
				if (red(p->l->l))
					p = rtr(p);
				else if (red(p->l->r))
				{
					p->l = rtl(p->l);
					p = rtr(p);
				}
			}

		}
		while (p != old);

		return p;
	}

	void insert(int x)
	{
		root = insert(root, x);
		root->c = 0;
	}

	node *insert(node * p, int x)
	{
		if (not p)
			return new node(x);

		if (x < p->x)
			p->l = insert(p->l, x);
		else
			p->r = insert(p->r, x);

		return fix(p);
	}

	node *red_right(node * p)
	{
		if (not p->r || p->r->c)
			return p;

		if (red(p->l))
			return rtr(p);

		p->c ^= 1;
		p->l->c ^= 1;
		p->r->c ^= 1;

		return p;
	}

	node *red_left(node * p)
	{
		if (not p->l || p->l->c)
			return p;

		if (red(p->r))
			return rtl(p);

		p->c ^= 1;
		p->l->c ^= 1;
		p->r->c ^= 1;

		return p;
	}

	void remove(int x)
	{
		if (root)
			root->c = 1;
		root = remove(root, x);
		if (red(root))
			root->c = 0;
	}

	node *remove(node * p, int x)
	{
		if (not p)
			return p;

		if (x == p->x)
		{
			auto l = p->l;
			auto r = p->r;

			if (not l || not r)
			{
				p->l = p->r = 0;
				delete p;
				return l ? : r;
			}

			auto succ = p->r;
			while (succ->l)
				succ = succ->l;

			p->x = succ->x;
			p = red_right(p);
			p->r = remove(p->r, succ->x);
			return fix(p);
		}

		if (x < p->x)
		{
			p = red_left(p);
			p->l = remove(p->l, x);
		}
		else
		{
			p = red_right(p);
			p->r = remove(p->r, x);
		}

		return fix(p);
	}

	bool find(int x)
	{
		auto p = root;
		while (p)
		{
			if (x == p->x)
				return true;
			if (x < p->x)
				p = p->l;
			else
				p = p->r;
		}
		return false;
	}

	int print()
	{
		return print(root);
	}

	int print(node * p)
	{
		if (not p)
			return 0;

		// no double reds
		if (red(p))
			assert(not red(p->l) && not red(p->r));

		int l = print(p->l);
		cout << p->x << '\n';
		int r = print(p->r);

		// black height
		assert(l == r);

		return l + 1 - p->c;
	}
};


int main()
{
	cin.tie(0);
	cin.sync_with_stdio(0);

	rb_tree t;
	int n = 100000;
	for (int i = 0; i < n; ++i)
		t.insert(i);

	int h = t.print();

	cout << "depth: " << h;
	cout << flush;
}
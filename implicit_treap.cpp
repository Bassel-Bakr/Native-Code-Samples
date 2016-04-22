#include <iostream>
#include <algorithm>
#include <random>
#include <stack>

using namespace std;

template < class T > struct node {
	T key;
	int priority;
	int size = 1;
	int sum;
	int low;
	int high;
	node *r = nullptr;
	node *l = nullptr;
	node(){}

	 node(T const &key) {
		low = high = sum = this->key = key;
		priority = rand();
	} ~node() {
		if (r)
			delete r;
		r = nullptr;
		if (l)
			delete l;
		l = nullptr;
	}
};

template < class T > struct treap {
	typedef node < T > tnode;
	int cnt = 0;
	tnode *root = nullptr;
	 treap() {
	} treap(tnode * r, int c) {
		root = r;
		cnt = c;
	}
	~treap() {
		if (root)
			delete root;
		root = nullptr;
	}

	int _print(tnode * p, int h) {
		auto l = p->l ? _print(p->l, h + 1) : h;
		//cout << p->key << '\n';
		auto r = p->r ? _print(p->r, h + 1) : h;
		return max(l, r);
	}

	int print() {
		return root ? _print(root, 0) : 0;
	}

	void clr() {
		root = nullptr;
		cnt = 0;
	}

	int size(tnode * t) {
		return t ? t->size : 0;
	}

	int low(tnode * t) {
		return t ? t->low : 2e9;
	}

	int high(tnode * t) {
		return t ? t->high : -2e9;
	}

	int sum(tnode * t) {
		return t ? t->sum : 0;
	}

	void update(tnode * t) {
		if (t) {
			t->size = size(t->l) + 1 + size(t->r);
			t->low = min(t->key, min(low(t->l), low(t->r)));
			t->high = max(t->key, max(high(t->l), high(t->r)));
			t->sum = sum(t->l) + t->key + sum(t->r);
		}
	}

	tnode *rotate_right(tnode * p) {
		auto c = p->l;
		p->l = c->r;
		c->r = p;
		update(p);
		update(c);
		return c;
	}

	tnode *rotate_left(tnode * p) {
		auto c = p->r;
		p->r = c->l;
		c->l = p;
		update(p);
		update(c);
		return c;
	}

	tnode *rtl(tnode * p) {
		if (p->priority < p->r->priority)
			return rotate_left(p);
		return p;
	}

	tnode *rtr(tnode * p) {
		if (p->priority < p->l->priority)
			return rotate_right(p);
		return p;
	}

	tnode *pull(tnode * p, int i) {
		auto lsize = size(p->l);
		if (i == lsize)
			return p;
		else if (i > lsize) {
			p->r = pull(p->r, i - lsize - 1);
			return rotate_left(p);
		} else {
			p->l = pull(p->l, i);
			return rotate_right(p);
		}
	}
	tnode *pull(int i) {
		return root = pull(root, i);
	}

	tnode *append(tnode * p, T const &key) {
		if (!p && ++cnt)
			return new tnode(key);
		p->r = append(p->r, key);
		update(p->r);
		return rtl(p);
	}

	tnode *append(T const &key) {
		root = append(root, key);
		update(root);
		return root;
	}

	tnode *prepend(tnode * p, T const &key) {
		if (!p && ++cnt)
			return new tnode(key);
		p->l = prepend(p->l, key);
		update(p->l);
		return rtr(p);
	}

	tnode *prepend(T const &key) {
		root = prepend(root, key);
		update(root);
		return root;
	}

	// [l...i] [i+1...r]
	void split(int i, treap & l, treap & r) {
		if (i < 0) {
			l.root = nullptr;
			l.cnt = 0;
			r.root = root;
			r.cnt = cnt;
		} else if (i >= cnt) {
			r.root = nullptr;
			r.cnt = 0;
			l.root = root;
			l.cnt = cnt;
		} else {
			pull(i);
			l.root = root;
			r.root = root->r;

			root->r = nullptr;
			l.update(root);
			r.update(root);

			l.cnt = size(l.root);
			r.cnt = size(r.root);
		}
		clr();
	}

	void merge(treap & l, treap & r) {
		if (l.cnt == 0) {
			root = r.root;
			cnt = r.cnt;
		} else if (r.cnt == 0) {
			root = l.root;
			cnt = l.cnt;
		} else {
			l.pull(l.cnt - 1);
			r.pull(0);
			root = l.root;
			root->r = r.root;
		}

		update(root);
		cnt = size(root);
		l.clr();
		r.clr();
	}

	void insert_at(int i, T const &key) {
		if (i <= 0) {
			prepend(key);
			return;
		} else if (i + 1 >  cnt) {
			append(key);
			return;
		}
		treap a, b;
		split(i - 1, a, b);
		a.append(key);
		merge(a, b);
	}
	
	void clone(tnode &n, tnode& t) {
	  n.sum = t.sum;
	  n.high = t.high;
	  n.low = t.low;
	  n.size = t.size;
	  n.priority = t.priority;
	}
	
	tnode query(int l, int r) {
	  treap a, b, c;
	  split(l - 1, a, b);
	  b.split(r - l, *this, c);
	  tnode q;
	  clone(q, *root);
	  b.merge(*this, c);
	  merge(a, b);
	  return q;	  
	}
};

int main()
{
	treap < int >t;
	for (int i = 1; i <= 100000; ++i)
		t.append(i);

	cout << t.query(0, 9).sum <<'\n';
}

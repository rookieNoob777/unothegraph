// unothelinkedlist.cpp : Defines the entry point for the console application.
//
//#include "stdafx.h"
#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include <math.h>
#include <string>
#include <algorithm>
#include <unordered_map>
#include "string.h"

using namespace std;

#define BP (cout<<endl)

#define DOCK() do{	\
	int dock;	\
	cin >> dock;	\
}while(0)

template<typename T>
void printContainer(vector<T> &container)
{
	for (auto it = container.begin(); it != container.end(); ++it)
	{
		cout << *it;
		if (it != container.end() - 1)
			cout << ", ";
	}
}

// Disjoint-set union
class DSU
{
private:
	vector<int> v;
public:
	DSU(int n)
	{
		v.resize(n, 0);
		for (int i = 0; i < n; ++i)
			v[i] = i;
	}

	/**
	 * "Path Compressing" and "quick find"
	 * Find the root and compress the path in parallel, after a find,
	 * all nodes on the path will point to the root node.
	 */
	int finds(int p)
	{
		if (v[p] != p)
			v[p] = finds(v[p]);
		return v[p];
	}

	// corresponding to "quick find", this is "slow union"
	void unions(int p, int q)
	{
		
		/**
		 * q's root will point to p's root,
		 * that means p's root will be the new root of q tree
		 */
		v[finds(q)] = finds(p);
	}
};

class Graph
{
public:
	void BFS(int n, vector<vector<int>>& e)
	{
		vector<vector<pair<int, int>>> g(n);
		queue<int> v;
		vector<int> seen(n, 0);

		// covert edge to adjacency list
		for (auto ei : e)
		{
			g[ei[0]].push_back({ei[1], ei[2]});
			g[ei[1]].push_back({ei[0], ei[2]});
		}

		cout << "BFS: ";

		for (int i = 0; i < n; ++i)
		{
			if (seen[i])
				continue;

			v.push(i);

			while (v.size())
			{
				int vi = v.front();
				v.pop();

				if (seen[vi])
					continue;

				cout << vi << " ";
				seen[vi]++;

				for (auto vw : g[vi])
				{
					if (!seen[vw.first])
						v.push(vw.first);
				}
			}
		}

		cout << endl;
	}

	void DFS(int n, vector<vector<int>>& e)
	{
		vector<vector<pair<int, int>>> g(n);
		stack<int> v;
		vector<int> seen(n, 0);

		for (auto ei : e)
		{
			g[ei[0]].push_back({ei[1], ei[2]});
			g[ei[1]].push_back({ei[0], ei[2]});
		}

		cout << "DFS: ";

		for (int i = 0; i < n; ++i)
		{
			if (seen[i])
			continue;

			v.push(i);

			while (v.size())
			{
				int vi = v.top();
				v.pop();

				if (seen[vi])
					continue;

				cout << vi << " ";
				seen[vi]++;

				for (auto vw : g[vi])
				{
					if (!seen[vw.first])
						v.push(vw.first);
				}
			}
		}

		cout << endl;
	}

	void dfsr(vector<vector<pair<int, int>>>& g, int v, vector<int>& seen)
	{
		cout << v << " ";
		seen[v]++;

		for (auto vw : g[v])
		{
			if (!seen[vw.first])
			dfsr(g, vw.first, seen);
		}
	}

	void DFSR(int n, vector<vector<int>>& e)
	{
		vector<vector<pair<int, int>>> g(n);
		vector<int> seen(n, 0);

		for (auto ei : e)
		{
			g[ei[0]].push_back({ei[1], ei[2]});
			g[ei[1]].push_back({ei[0], ei[2]});
		}

		cout << "DFSR: ";

		for (int i = 0; i < n; ++i)
		{
			if (seen[i])
			continue;

			dfsr(g, i, seen);
		}

		cout << endl;
	}

	void prim(int n, vector<vector<int>>& e)
	{
		vector<vector<pair<int, int>>> g(n);
		priority_queue<pair<int, int>> q;
		vector<int> seen(n, 0);
		int min_cost = 0;

		for (auto ei : e)
		{
			g[ei[0]].push_back({ei[1], ei[2]});
			g[ei[1]].push_back({ei[0], ei[2]});
		}

		cout << "prim's: ";

		for (int i = 0; i < n; ++i)
		{
			if (seen[i])
			continue;

			if (i > 0)
			cout << endl <<
			"New entry for disconnection graph: "
			<< i << endl;

			q.push({0, i});

			while (q.size())
			{
				int w = -q.top().first;
				int vi = q.top().second;
				q.pop();

				if (seen[vi])
					continue;

				cout << vi << " ";
				seen[vi]++;
				min_cost += w;
				for (auto vw : g[vi])
				{
					if (!seen[vw.first])
					q.push({-vw.second, vw.first});
				}
			}
		}

		cout << endl;
		cout << "Min cost of prim's: " << min_cost << endl;
	}

	void kruskal(int n, vector<vector<int>>& e)
	{
		priority_queue<vector<int>> q;
		int num_edge = 0;
		int min_cost = 0;
		DSU dsu(n);

		for (auto ei : e)
			q.push({-ei[2], ei[0], ei[1]});

		while (num_edge < n - 1)
		{
			if (q.empty())
			{
				cout << "Void Kruskal" << endl;
				return;
			}

			int w = -q.top()[0]; 
			int vp = q.top()[1];
			int vq = q.top()[2];
			q.pop();

			if (dsu.finds(vp) == dsu.finds(vq))
			continue;
			
			cout << vp << " " << vq << ",";
			min_cost += w;
			dsu.unions(vp, vq);
			num_edge++;
		}

		cout << endl;
		cout << "Min cost of kruskal: " << min_cost << endl;
	}
};

// Definition for a Node.
class Node {
public:
	int val;
	vector<Node*> neighbors;

	Node() {
		val = 0;
		neighbors = vector<Node*>();
	}

	Node(int _val) {
		val = _val;
		neighbors = vector<Node*>();
	}

	Node(int _val, vector<Node*> _neighbors) {
		val = _val;
		neighbors = _neighbors;
	}
};

class Solution
{
public:
	// 997. Find the Town Judge
	// int findJudge(int N, vector<vector<int>>& trust)
	// {
	// 	vector<int> in_edges(N + 1, 0);
	// 	vector<int> out_edges(N + 1, 0);

	// 	for (auto t : trust)
	// 	{
	// 		++in_edges[t[1]];
	// 		++out_edges[t[0]];
	// 	}

	// 	for (int i = 1; i <= N; ++i)
	// 	{
	// 		if (in_edges[i] - out_edges[i] == N - 1)
	// 		return i;
	// 	}

	// 	return -1;
	// }

	int findJudge(int N, vector<vector<int>>& trust)
	{
		vector<int> trusts(N + 1, 0);

		for (auto t : trust)
		{
			++trusts[t[1]];
			--trusts[t[0]];
		}

		for (int i = 1; i <= N; ++i)
		{
			if (trusts[i] == N - 1)
			return i;
		}

		return -1;
	}

	// 1042. Flower Planting With No Adjacent
	vector<int> gardenNoAdj(int N, vector<vector<int>>& paths)
	{
		vector<vector<int>> g(N);
		vector<int> GNA(N, 0);

		for (auto path : paths)
		{
			g[path[0]-1].push_back(path[1]-1);
			g[path[1]-1].push_back(path[0]-1);
		}

		for (int i = 0; i < N; ++i)
		{
			vector<int> flowers = {INT_MAX, 1, 2, 3, 4};
			for (auto ad : g[i])
			{
				// if (GNA[ad] == 0)
				//     continue;
				// remove(flowers.begin(), flowers.end(), GNA[ad]);
				flowers[GNA[ad]] = INT_MAX;
			}
			// GNA[i] = flowers.front();
			GNA[i] = *min_element(flowers.begin(), flowers.end());
		}

		return GNA;
	}

	// 133. Clone Graph
	// Node* cloneGraph(Node* node)
	// {
	// 	if (!node)
	// 	return node;

	// 	Node *cnode = nullptr;
	// 	queue<Node*> q;
	// 	unordered_map<int, Node*> hmap;
	// 	vector<int> seen(101, 0);

	// 	q.push(node);

	// 	while (q.size())
	// 	{
	// 		node = q.front();
	// 		q.pop();

	// 		if (seen[node->val]++)
	// 			continue;

	// 		Node* clone = nullptr;
	// 		auto it = hmap.find(node->val);
	// 		if (it == hmap.end())
	// 		{
	// 			clone = new Node(node->val);
	// 			hmap.insert({clone->val, clone});
	// 			if (clone->val == 1)
	// 			cnode = clone;
	// 		}
	// 		else
	// 			clone = it->second;

	// 		for (auto nb : node->neighbors)
	// 		{
	// 			it = hmap.find(nb->val);
	// 			if (it == hmap.end())
	// 			{
	// 			Node* cnb = new Node(nb->val);
	// 			clone->neighbors.push_back(cnb);
	// 			hmap.insert({cnb->val, cnb});
	// 			}
	// 			else
	// 			clone->neighbors.push_back(it->second);

	// 			if (!seen[nb->val])
	// 			q.push(nb);
	// 		}
	// 	}

	// 	return cnode;
	// }

	unordered_map<int, Node*> created;

	Node* cloneGraph(Node* node)
	{
		if (!node)
			return node;

		Node* clone = nullptr;
		auto it = created.find(node->val);
		if (it == created.end())
		{
			clone = new Node(node->val);
			created.insert({clone->val, clone});
			for (auto nb : node->neighbors)
			clone->neighbors.push_back(cloneGraph(nb));
		}
		else
			clone = it->second;

		return clone;
	}
};

int main()
{
	// adjacency matrix
	// vector<vector<int>> m;
	/*
		    0   1    2   3    4   5
		0   \   3   -1   2   -1  -1
		1   
		2
		3
		4
		5
	*/

	// adjacency list
	// vector<vector<pair<int, int>>> g; 
	/*
	{
		0:  {<1, 3>, <3, 4>, <5, 2>}
		1:  {<0, 3>, <2, 1>}
		2:  ...
		.
		.
	}
	*/

	// edge list
	// vector<vector<int>> e = {{0, 1, 4}, {1, 2, 2}, {1, 3, 2}, {2, 4, 1}, {2, 6, 2}, {3, 5, 3}};
	vector<vector<int>> e = {{0, 1, 2}, {0, 2, 1}, {0, 3, 4}, {1, 2, 3}, {1, 3, 2}};

	Graph graph;
	Solution solu;

	graph.BFS(4, e);
	// graph.DFS(4, e);
	// graph.DFSR(4, e);
	// graph.prim(4, e);
	// graph.kruskal(4, e);

	// 997. Find the Town Judge
	// vector<vector<int>> trust = { {1,3}, {1,4}, {2,3}, {2,4}, {4,3} };
	// cout << "The label of the Town Judge is: " << solu.findJudge(4, trust) << endl << endl;

	// 1042. Flower Planting With No Adjacent
	// int N = 3;
	// vector<vector<int>> paths = {{1,2}, {2,3}, {3,1}};
	// vector<int> GNA = solu.gardenNoAdj(N, paths);
	// printContainer(GNA);

	// 133. Clone Graph


	DOCK();

	return 0;
}


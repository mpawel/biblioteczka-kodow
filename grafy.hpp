#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <cstdio>
#include <cstring>

using namespace std;
typedef unsigned long long ull;
typedef ull Ct;

struct Et {
  int from,to;
  Ct cost;
  Et(int _from,int _to,Ct _cost): from(_from), to(_to), cost(_cost) { }
};

struct Vt {
  vector<Et*> out;
  vector<Et*> in;
};

struct Gt {
  vector<Et*> e;
  vector<Vt> v;
  Gt() {}
  Gt(int nV): v(nV) {}
  ~Gt() {
    for (int i=0; i<e.size(); ++i) delete e[i];
  }
  void conn(int a, int b, Ct c) {
    Et* edge = new Et(a,b,c);
    e.push_back(edge);
    v[a].out.push_back(edge);
    v[b].in.push_back(edge);
  }
  Vt& operator[](int i) { return v[i]; }
  int size() {return v.size();}
};

Gt G;

int bellman_ford(int start, Ct* dist, int* prev) {
  int nv = G.size();
  for (int i=0; i<nv; ++i) {
    dist[i] = LLONG_MAX;
    prev[i] = -1;
  }
  dist[start] = 0;
  for (int i=1; i<nv; ++i) {
    typedef vector<Et*>::iterator iT;
    for (iT ei=G.e.begin(); ei!=G.e.end(); ++ei) {
      Et* edge = *ei;
      if(dist[edge->to] > dist[edge->from] + edge->cost) {
	dist[edge->to] = dist[edge->from] + edge->cost;
	prev[edge->to] = edge->from;
      }
    }
  }
  return 0;
}

struct dist_cmp {
  dist_cmp(Ct* _dist): dist(_dist) {}
  Ct* dist;
  bool operator()(int a, int b) {
    return dist[a] < dist[b];
  }
};

int dijkstra(int start, Ct* dist, int* prev) {
  int nv = G.size();
  for (int i=0; i<nv; ++i) {
    dist[i] = LLONG_MAX;
    prev[i] = -1;
  }
  dist[start] = 0;
  dist_cmp cmp(dist);
  vector<int> Q;
  Q.push_back(start);
  while(Q.size()) {
    pop_heap(Q.begin(), Q.end(), cmp);
    int u = Q.back(); Q.pop_back();
    typedef vector<Et*>::iterator iT;
    vector<Et*>* edges = &(G[u].out);
    for (iT ie=edges->begin(); ie!=edges->end(); ++ie) {
      Et* edge = *ie;
      if(dist[edge->to] > dist[edge->from] + edge->cost) {
	dist[edge->to] = dist[edge->from] + edge->cost;
	prev[edge->to] = edge->from;
	Q.push_back(edge->to); push_heap(Q.begin(), Q.end(), cmp);
      }
    }
  }
  return 0;
}

int floyd(Ct** d, int** p) {
  int s = G.size();
  for (int i=0; i<s; ++i) {
    for (int j=0; j<s; ++j) {
      d[i][j] = LLONG_MAX;
      p[i][j] = -1;
    }
    d[i][i] = 0;
  }
  for (int i=0; i<G.e.size(); ++i) {
    Et* e = G.e[i];
    d[e->from][e->to] = e->cost;
    p[e->from][e->to] = e->from;
  }
  for (int u=0; u<s; ++u) {
    for (int v1=0; v1<s; ++v1) {
      for (int v2=0; v2<s; ++v2) {
	if (d[v1][v2] > d[v1][u] + d[u][v2]) {
	  d[v1][v2] = d[v1][u] + d[u][v2];
	  p[v1][v2] = p[u][v2];
	}
      }
    }
  }
  return 0;
}

struct union_find {
  int *buff, size;
  union_find(int s): size(s) {
    buff = new int[size];
    for (int i=0; i<size; ++i)
      buff[i] = i;
  }
  ~union_find() { delete[] buff; }
  int f(int s) {
    int i = s;
    while(buff[i] != i)
      i = buff[i];
    return buff[s] = i;
  }
  void u(int a, int b) { buff[f(a)] = f(b); }
};

struct len_cmp {
  bool operator()(Et* a, Et* b) {
    return a->cost < b->cost;
  }
};

struct rev_len_cmp {
  bool operator()(Et* a, Et* b) {
    return a->cost > b->cost;
  }
};

int kruskal(Et** mst, Ct& cost) {
  len_cmp cmp;
  union_find uf(G.size());
  vector<Et*> edges = vector<Et*>(G.e.begin(), G.e.end());
  sort(edges.begin(), edges.end(), cmp);
  for (vector<Et*>::iterator i=edges.begin(); i!= edges.end(); ++i) {
    Et* e = *i;
    if(uf.f(e->from) != uf.f(e->to)) {
      *mst++ = e;
      uf.u(e->from, e->to);
      cost += e->cost;
    }
  }
  return 0;
}

int prim(Et** mst, Ct& cost) {
  rev_len_cmp cmp;
  union_find uf(G.size());
  vector<Et*> Q;
  for(vector<Et*>::iterator i = G[0].out.begin(); i != G[0].out.end(); ++i) {
    Q.push_back(*i); push_heap(Q.begin(), Q.end(), cmp);
  }
  while(Q.size()) {
    pop_heap(Q.begin(), Q.end(), cmp);
    Et* e = Q.back(); Q.pop_back();
    if(uf.f(e->from) != uf.f(e->to)) {
      *mst++ = e;
      uf.u(e->from, e->to);
      cost += e->cost;
      int u = e->to;
      for(vector<Et*>::iterator i = G[u].out.begin(); i != G[u].out.end(); ++i) {
	Q.push_back(*i); push_heap(Q.begin(), Q.end(), cmp);
      }
    }
  }
  return 0;
}

int dfs(int start) {
  vector<int> stack;
  int s = G.size();
  int pre_c  = 0, *pre  = new int[s];
  int post_c = 0, *post = new int[s];
  for (int i=0; i<s; ++i) {
    pre[i] = post[i] = 0;
  }
  stack.push_back(start);
  while(stack.size()) {
    int curr = stack.back();
    if(pre[curr]) {
      stack.pop_back();
      post[curr] = ++post_c;
      // printf("post %d\n", curr+1); //
    } else {
      pre[curr] = ++pre_c;
      // printf("pre %d\n", curr+1); //
      for (int i=0; i<G[curr].out.size(); ++i) {
	Et* e = G[curr].out[i];
	if(pre[e->to]) {
	  if(post[e->to]) {
	    // printf("cross-bridge from %d to %d\n", e->from+1, e->to+1); //
	  } else {
	    //printf("back-bridge from %d to %d\n", e->from+1, e->to+1); //
	  }
	} else {
	  // printf("push %d\n", e->to+1); //
	  stack.push_back(e->to);
	}
      }
    }
  }
}

int bfs() {
  queue<int> Q;
  int s = G.size();
  int pre_c  = 0, *pre  = new int[s];
  for (int i=0; i<s; ++i) pre[i] = 0;
  for (int start=0; start<s; ++start) {
    if(pre[start]) continue;
    Q.push(start);
    while(Q.size()) {
      int curr = Q.front(); Q.pop();
      if(pre[curr]) continue; // wierzchołek osiągalny kilkoma drogami
      pre[curr] = ++pre_c;
      // printf("visiting %d\n", curr+1); //
      for (int i=0; i<G[curr].out.size(); ++i) {
	Et* e = G[curr].out[i];
	if(pre[e->to]) {
	  // printf("  back-bridge to %d\n", e->to+1); //
	} else {
	  // printf("  push %d\n", e->to+1); //
	  Q.push(e->to);
	}
      }
    }    
  }
  return 0;
}

int paint2(int* vc) {
  queue<int> Q;
  for (int i=0; i<G.size(); ++i) vc[i] = 0;
  for (int start=0; start<G.size(); ++start) {
    if(vc[start]) continue;
    vc[start] = 1; Q.push(start);
    while(Q.size()) {
      int curr = Q.front(); Q.pop();
      for (int i=0; i<G[curr].out.size(); ++i) {
	Et* e = G[curr].out[i];
	if(vc[e->to]) {
	  if(vc[e->to] != (vc[curr] ^ 3)) return 1;
	} else {
	  vc[e->to] = vc[curr] ^ 3; Q.push(e->to);
	}
      }
    }
  }
  return 0;
}

struct hopcroft {
  int *dist, *assoc, *colors;
  bool *done;

  bool dfs(int x) {
    done[x] = true;
    for (int i=0; i<G[x].out.size(); ++i) { // przechodzi przez wszyskie y (b), które zna dany x (a)
      Et* e = G[x].out[i];
      int y = e->to;
      if(assoc[y] == -1) {
	assoc[y] = x;
	assoc[x] = y;
	return true;
      } else {
	int z = assoc[y];
	if(!done[z] && dist[z] == dist[x]+1) {
	  if(dfs(z)) {
	    assoc[y] = x;
	    assoc[x] = y;
	    return true;
	  }
	}
      }
    } // for y known to x
    return false;
  }

  int run() {
    int s = G.size();
    colors = new int[s];
    dist   = new int[s];
    assoc  = new int[s];
    done   = new bool[s];

    if(paint2(colors)) return 1;

    vector<int> a, b;
    for (int i=0; i<s; ++i) {
      assoc[i] = -1;
      if(colors[i] == 1) a.push_back(i);
      else               b.push_back(i);
    }

    bool keep_searching;
    do {
      for (int i=0; i<a.size(); ++i) dist[a[i]] = -1;
      queue<int> Q;
      for (int i=0; i<a.size(); ++i) {
	if(assoc[a[i]] == -1) {
	  dist[a[i]] = 0;
	  Q.push(a[i]);
	}
      }
      while(Q.size()) {
	int x = Q.front(); Q.pop();
	for (int i=0; i<G[x].out.size(); ++i) { // przechodzi przez wszyskie b, które zna dany x (a)
	  Et* e = G[x].out[i];
	  int y = e->to;
	  if(assoc[y] != -1){
	    int z = assoc[y];
	    if(dist[z] == -1) {
	      dist[z] = dist[x] + 1;
	      Q.push(z);
	    }
	  }
	}
      }
    
      for (int i=0; i<a.size(); ++i) done[a[i]] = 0;

      keep_searching = false;
      for (int i=0; i<a.size(); ++i) {
	int x = a[i];
	if(assoc[x] == -1) {
	  bool dfs_result = dfs(x);
	  keep_searching = dfs_result or keep_searching;
	} // if not assigned
      } // for each a
      
    } while(keep_searching);
    
    return 0;
  }
};

int low() {
  vector<int> stack;
  int s = G.size();
  int pre_c  = 0, *pre  = new int[s];
  int *low = new int[s];
  int *par = new int[s];
  for (int i=0; i<s; ++i) {
    pre[i] = low[i] = 0;
    par[i] = -1;
  }
  for (int start=0; start<s; ++start) {
    if(pre[start] == 0) {
      stack.push_back(start);
      par[start] = -1;
    }
    while(stack.size()) {
      int x = stack.back();
      if(pre[x]) {
	//printf(" stepping off %d (to %d)\n", x+1, par[x] + 1);
	stack.pop_back();
	for (int i=0; i<G[x].out.size(); ++i) {
	  int y = G[x].out[i]->to;
	  if(y == par[x]) continue; // nie wracaj do poprzednika
	  if(low[x] > low[y]) {
	    //printf(" lowering low to %d\n", low[y]);
	    low[x] = low[y];
	  }
	}
      } else {
	low[x] = pre[x] = ++pre_c;
	//printf("Visiting %d\n", x+1);
	for (int i=0; i<G[x].out.size(); ++i) {
	  int y = G[x].out[i]->to;
	  if(y == par[x]) continue; // nie wracaj do poprzednika
	  if(pre[y]) {
	    if(low[x] > pre[y]) low[x] = pre[y];
	  } else {
	    stack.push_back(y);
	    par[y] = x;
	  }
	}
      }
    }    
  }
  // korekta funkcji low
  for (int x=0; x<s; ++x) {
    low[x] = pre[x];
    for (int i=0; i<G[x].out.size(); ++i) {
      int y = G[x].out[i]->to;
      if(y == par[x]) continue;
      if(low[x] > low[y]) low[x] = low[y];
    }
  }
  // Wyznaczanie punktów artykulacji
  vector<int> art_points;
  for (int i=0; i<s; ++i) {
    //printf("node %2d { pre:%2d low:%2d }", i+1, pre[i], low[i]);
    if(par[i] == -1) {
      int sons = 0;
      for (int j=0; j<G[i].out.size(); ++j) {
	int y = G[i].out[j]->to;
	if(par[y] == i) ++sons;
      }
      if(sons > 1) art_points.push_back(i);
    } else {
      bool art = false;
      for (int j=0; j<G[i].out.size(); ++j) {
	int y = G[i].out[j]->to;
	if(low[y] >= pre[i]) {
	  art = true;
	  break;
	}
      }
      if(art) art_points.push_back(i);
    }
  }
  printf("Punkty artykulacji: ");
  for (int i=0; i<art_points.size(); ++i) {
    if(i) printf(", ");
    printf("%d", art_points[i]+1);
  }
  printf("\n");
  // Wyznaczanie mostów
  for (int x=0; x<s; ++x) {
    if(par[x] != -1 && low[x] == pre[x]) {
      printf("Most %d - %d\n", x+1, par[x]+1);
    }
  }
}

void show_graph() {
  for (int i=0; i<G.size(); ++i) {
    typedef vector<Et*>::iterator iT;
    vector<Et*>* edges = &(G[i].out);
    for (iT ie=edges->begin(); ie!=edges->end(); ++ie) {
      Et* edge = *ie;
      printf("Edge from %d to %d, with cost %llu\n", edge->from, edge->to, edge->cost);	
    }
  }
}

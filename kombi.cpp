#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

struct permutator {
  vector<int>* v;
  void (*fun)(vector<int>&);
  void perm(int m) {
    if(!m) fun(*v);
    else for (int i=0; i<m; ++i) {
	perm(m-1);
	if(i<m) {
	  swap((*v)[i], (*v)[m-1]);
	  reverse(&(*v)[0], &(*v)[m]);
	}
      }
  }
  permutator(vector<int>& v_, void(*f)(vector<int>&)): v(&v_), fun(f) {}
  void run() {
    perm(v->size() - 1);
  }
};

void print(vector<int>& v) {
  printf("v: ");
  for (int i=0; i<v.size(); ++i) {
    if(i) printf(", ");
    printf("%2d", v[i]);
  }
  printf("\n");
}

int main(int argc, char *argv[]) {
  vector<int> v;
  for (int i=0; i<4; ++i) {
    v.push_back(rand()%100);
  }
  sort(v.begin(), v.end());
  //print(v);
  permutator p(v, print);
  p.run();
  return 0;
}

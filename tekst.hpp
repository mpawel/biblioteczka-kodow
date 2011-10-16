#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <cstdio>
#include <cstring>

using namespace std;

const char* A = "klamka";
const char* B = "lama2";

template<class T>
struct arr2 {
  T* a;
  int x, y;
  arr2(int _x, int _y): x(_x), y(_y) { a = new T[x*y]; }
  ~arr2() { /*delete[] a;*/ }
  T& operator()(int px, int py) { return a[px*x + py]; }
};

template<class T>
int edist(T* s, int m, T* t, int n) {
  arr2<int> d(m+1,n+1);
  for (int i=0; i<=m; ++i) d(i, 0) = i;
  for (int j=0; j<=n; ++j) d(0, j) = j;
  
  for (int j=1; j<=n; ++j) {
    for (int i=1; i<=m; ++i) {
      if (s[i-1] == t[j-1]) {
	d(i, j) = d(i-1, j-1);
      } else {
        d(i, j) = min(min(d(i-1, j) + 1 , // a deletion
			  d(i, j-1) + 1), // an insertion
		        d(i-1, j-1) + 1); // a substitution
      }
    }
  }
  int ret = d(m,n);
  return ret;
}

// KMP: W-wzorzec, T-prefikso-sufiksy, S-ciąg do przeszukania

void preffix_suffix(char* W, int* T) {
  int i = 2, j = 0;
  T[0] = -1; T[1] = 0;
  while(W[i]) {
    if(W[i-1] == W[j]) T[i++] = ++j;
    else if(j)         j      = T[j];
    else               T[i++] = 0;
  }
}

int kmp_search(char* S, char* W) {
  int m = 0, i = 0, len = strlen(W);
  int* T = new int[len];
  preffix_suffix(W, T);
  while(S[m + i]) {
    if(W[i] == S[m + i]) {
      ++i;
      if(W[i] == 0) return m;
    } else {
      m = m + i - T[i];
      if(i) i = T[i];
    }
  }
  delete[] T;
  return m + i;
}

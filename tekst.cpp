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
	//printf("%s[%d] == %s[%d] => d[%d][%d] = %d\n", s, i, t, j, i, j,d(i-1, j-1));
	d(i, j) = d(i-1, j-1);
      } else {
        d(i, j) = min(min(d(i-1, j) + 1 , // a deletion
			  d(i, j-1) + 1), // an insertion
		        d(i-1, j-1) + 1); // a substitution
      }
    }
  }
  /*  
  printf("    ");
  for (int j=0; j<=n; ++j) {
    printf("%2c", t[j]);
  }
  printf("\n");
  for (int i=0; i<=m; ++i) {
    if (i) printf("%2c", s[i-1]);
    else printf("  ");
    
    for (int j=0; j<=n; ++j) {
      printf("%2d", d(i,j));
    }
    printf("\n");
    }*/
  int ret = d(m,n);
  return ret;
}

int main(int argc, char *argv[])
{
  int la = strlen(A), lb = strlen(B);
  int ed = edist(A,la,B,lb);
  printf("Levenshtein distance (%s, %s): %d (should be 3)\n", A, B, ed);
  return 0;
}

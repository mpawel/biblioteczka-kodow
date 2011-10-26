#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <stdint.h>


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

void make_delta1(int *delta1, uint8_t *pat, int patlen) {
  for (int i=0; i < 256; i++) delta1[i] = patlen;
  for (int i=0; i < patlen; i++) delta1[pat[i]] = patlen-1 - i;
}

int is_prefix(uint8_t *word, int wordlen, int pos) {
    int i;
    int suffixlen = wordlen - pos;
    for (i = 0; (i < suffixlen) && (word[i] == word[pos + i]); i++);
    return !(i < suffixlen);
}
 
int suffix_length(uint8_t *word, int wordlen, int pos) { // długość najdłuższego prefiksu kończącego się na danej pozycji
    int i;
    for (i = 0; (word[pos-i] == word[wordlen-1-i]) && (i < pos); i++);
    return i;
}

void make_delta2(int *delta2, uint8_t *pat, int32_t patlen) {
  int p;
  int last_prefix_index = patlen-1;
 
  for (p=patlen-1; p>=0; p--) {
    if (is_prefix(pat, patlen, p+1)) last_prefix_index = p+1;
    else ++last_prefix_index;
        
    delta2[p] = last_prefix_index;
  }
 
  for (p=0; p < patlen-1; p++) {
    int slen = suffix_length(pat, patlen, p);
    delta2[patlen-1 - slen] = patlen-1 - p + slen;
  }
}
 
int boyer_moore (uint8_t *string, uint32_t stringlen, uint8_t *pat, uint32_t patlen) {
  int i;
  int delta1[256];
  int *delta2 = new int[patlen];
  make_delta1(delta1, pat, patlen);
  make_delta2(delta2, pat, patlen);
 
  i = patlen-1;
  while (i < stringlen) {
    int j = patlen-1;
    while (j >= 0 && (string[i] == pat[j])) {
      --i;
      --j;
    }
    if (j < 0) {
      delete[] delta2;
      return i+1;
    }
 
    i += max(delta1[string[i]], delta2[j]);
  }
  delete[] delta2;
  return -1;
}

#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <climits>
#include <cstring>

#define ALPHABET_SIZE 128
#define tonum(c) (c >= 'A' && c <= 'Z' ? c - 'A' : c - 'a' + 26)

using namespace std;

/*** Rabin-Karp ***/

int64_t mod(int a,int p,int m){
     if (p == 0) return 1;
     int64_t sqr = mod(a,p/2,m) % m;
     sqr = (sqr * sqr) % m;
     if (p & 1) return ((a % m) * sqr) % m;
     else return sqr;
}

void rk(char *T,char *P,int d,int q) {
     int64_t i,j,p,t,n,m,h,match;
     n = strlen(T);
     m = strlen(P);
     h = mod(d,m-1,q);
     p = t = 0;

     for (i=0; i<m; i++)   {
	  p = (d*p + tonum(P[i])) % q;
	  t = (d*t + tonum(T[i])) % q;
     }

     for (i=0; i<=n-m; i++)     {
	  if (p == t)     {
	       match = 1;
	       for (j=0; j<m; j++)
		   if (P[j] != T[i+j])    {
		       match = 0;
		       break;
		   }
	       if (match) printf("%Ld\n",i);
	  } else
	       t = (d*(t - ((tonum(T[i])*h) % q)) + tonum(T[i+m])) % q;
     }
}


/*** Boyer-Moore ***/

void pre_badcharacter(const char *x, int m, int bmBc[]) {
   int i;

   for (i = 0; i < ALPHABET_SIZE; ++i)
      bmBc[i] = m;
   for (i = 0; i < m - 1; ++i)
      bmBc[x[i]] = m - i - 1;
}

void compute_suffix(const char *x, int m, int *suff) {
   int f, g, i;

   suff[m - 1] = m;
   g = m - 1;
   for (i = m - 2; i >= 0; --i) {
      if (i > g && suff[i + m - 1 - f] < i - g)
	 suff[i] = suff[i + m - 1 - f];
      else {
	 if (i < g)
	    g = i;
	 f = i;
	 while (g >= 0 && x[g] == x[g + m - 1 - f])
	    --g;
	 suff[i] = f - g;
      }
   }
}

void pre_suffix(const char *x, int m, int bmGs[]) {
   int i, j, suff[m];

   compute_suffix(x, m, suff);

   for (i = 0; i < m; ++i)
      bmGs[i] = m;
   j = 0;
   for (i = m - 1; i >= 0; --i)
      if (suff[i] == i + 1)
	 for (; j < m - 1 - i; ++j)
	    if (bmGs[j] == m)
	       bmGs[j] = m - 1 - i;
   for (i = 0; i <= m - 2; ++i)
      bmGs[m - 1 - suff[i]] = m - 1 - i;
}


void bm(const char *P, int m,const  char *T, int64_t n) {

   int i, j, suffix[m], bad_character[ALPHABET_SIZE];

   pre_suffix(P, m, suffix);
   pre_badcharacter(P, m, bad_character);

   j = 0;
   while (j <= n - m) {
      for (i = m - 1 ;  i >= 0 && P[i] == T[i + j] ; --i) ;
      if (i < 0) {
	 printf("%d\n",j);
	 j += suffix[0];
      }
      else
	 j += max(suffix[i], bad_character[T[i + j]] - m + 1 + i);
   }
}

/***  Knuth–Morris–Pratt  ***/
void compute_prefix (const char * P, int * pref, int n ) {

    pref[1]=0;
    int k=0;
    for (int64_t q=1 ;  q<n ; q++ ) {
	while (k > 0 &&  P[k]!=P[q])
	    k=pref[k];
	if(P[k] == P[q])
	    k++;
	pref[q+1]=k;
    }
}

void kmp ( const char * P, int plen, const char *T, int tlen) {

    int pref[plen+1];
    pref[0]=0;
    compute_prefix(P,pref, plen);

    int k=0;
    for ( int64_t i=0;i<tlen; i++) {
	while( k>0  &&  P[k]!=T[i] )
	    k=pref[k];
	if(P[k]==T[i])
		k++;
	    if (k==plen) {
		printf("%ld\n",i-plen+1);
		k=pref[k];
	    }
    }
}


/*** TEST ***/

int main () {
//    stdin = fopen("test.kmp_spoj","r");
    int test_n;
    scanf("%d",&test_n);
    for ( int t =0; t<test_n; t++) {
	int n;
	scanf("%d",&n);
	char p[n+1], t[5000000];
	scanf("%s\n",p);
	scanf("%s\n",t);

	kmp(p,n,t,strlen(t));
	bm(p,n,t,strlen(t));
	rk(t,p,10,UINT_MAX);
    }
}

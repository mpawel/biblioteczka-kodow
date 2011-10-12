#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <climits>
#include <cstring>

#define ALPHABET_SIZE 128
#define tonum(c) (c >= 'A' && c <= 'Z' ? c - 'A' : c - 'a' + 26)

using namespace std;

/*** Rabin-Karp ***/

/* return a^p mod m */
int64_t mod(int a,int p,int m){
     if (p == 0) return 1;
     int64_t sqr = mod(a,p/2,m) % m;
     sqr = (sqr * sqr) % m;
     if (p & 1) return ((a % m) * sqr) % m;
     else return sqr;
}

void RabinKarpMatch(char *T,char *P,int d,int q)
{
     int64_t i,j,p,t,n,m,h,found;
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
	       found = 1;
	       for (j=0; j<m; j++)
		   if (P[j] != T[i+j])    {
		       found = 0;
		       break;
		   }
	       if (found) printf("%d\n",i);
	  } else
	       t = (d*(t - ((tonum(T[i])*h) % q)) + tonum(T[i+m])) % q;
     }
//     return -1;
}


/*** Boyer-Moore ***/

void preBmBc(const char *x, int m, int bmBc[]) {
   int i;

   for (i = 0; i < ALPHABET_SIZE; ++i)
      bmBc[i] = m;
   for (i = 0; i < m - 1; ++i)
      bmBc[x[i]] = m - i - 1;
}

void suffixes(const char *x, int m, int *suff) {
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

void preBmGs(const char *x, int m, int bmGs[]) {
   int i, j, suff[m];

   suffixes(x, m, suff);

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


void bm(const char *P, int m, char *T, int64_t n) {

   int i, j, bmGs[m], bmBc[ALPHABET_SIZE];

   /* Preprocessing */
   preBmGs(P, m, bmGs);
   preBmBc(P, m, bmBc);

   /* Searching */
   j = 0;
   while (j <= n - m) {
      for (i = m - 1; i >= 0 && P[i] == T[i + j]; --i);
      if (i < 0) {
	 printf("%d\n",j);
	 j += bmGs[0];
      }
      else
	 j += max(bmGs[i], bmBc[T[i + j]] - m + 1 + i);
   }
}

/***  Knuth–Morris–Pratt  ***/
void compute_prefix (const char * P, int * pref, int n ) {
    pref[1]=0;
    int k=0,q=0;
    for (uint32_t q=2; q<=n;q++) {
	while (k &&  P[k+1]!=P[q])
	    k=pref[k];
	if(P[k+1]==P[q])
	    k=k+1;
	pref[q]=k;
    }
}

void kmp ( const char * P, int plen, const char *T, int tlen) {

    int pref[plen];
    compute_prefix(P,pref, plen);

    int q=0;
    for ( uint32_t i=1;i<=tlen; i++) {
	while( q &&  P[q+1]!=T[i])
	    q=pref[q];
	if(P[q+1]==T[i])
		q=q+1;
	    if (q==plen) {
		printf("%d\n", i-q);
		q=pref[q];
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
	//indeksowanie znaków w napisach 1..n
	scanf("%s\n",p+1);
	scanf("%s\n",t+1);
	p[0]='\0';
//	kmp(p,n,t,strlen(t+1));
//	bm(p+1,n,t+1,strlen(t+1));
	RabinKarpMatch(t+1,p+1,10,UINT_MAX);
    }
}

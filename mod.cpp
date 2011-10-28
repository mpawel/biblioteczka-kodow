#include <cstdio>
#include <cmath>

#define ASSIGN(a,b,c,d) (tmp = (c), (b) = (d), (a) = tmp)

typedef long long unsigned int llu;

llu pow(llu a, llu c, llu q) {
  llu ret = 1; // tak duże jak (q-1)*(q-1)*a
  llu len = 0; // tak duże jak c
  while(c >> len) ++len;
  for(len = 1<<(len-1); len; len>>=1) {
    if(c&len) ret *= ret*a;
    else ret *= ret;
    ret %= q;
  }
  return ret;
}

struct primegen {
  int primes[1000000];
  int n_primes;

  void run(int end, int start=2) {
    n_primes = 0;
    for(int i = start; i<end; ++i) {
      int top = (int)sqrt(i);
      for(int j = 0; j<n_primes; ++j) {
        if((i % primes[j]) == 0) goto not_a_prime;
        if(j>=top) break;
      }
      primes[n_primes] = i;
      ++n_primes;
    not_a_prime:
      i = i;
    }
  }
};

int rozszerzony_nwd(int a, int b, int *xi, int *yi) {
  int x = 0, y = 1, lastx = 1, lasty = 0, divisor, tmp;
  while(b) {
    divisor = a/b;
    ASSIGN(a, b, b, a%b);
    ASSIGN(x, lastx, lastx-divisor*x, x);
    ASSIGN(y, lasty, lasty-divisor*y, y);
  }
  *xi = lastx;
  *yi = lasty;
  return a;
}

long long int cto_(int m1, int m2, int n1, int n2) {
  int p1, p2;
  rozszerzony_nwd(n1, n2, &p1, &p2);
  return n1*p1*m2 + n2*p2*m1;
}

int cto(int* reszty, int* dzielniki, int len) {
  long long int E = 0;
  long long int N = 1;
  int i;
  for(i = 0; i<len; ++i) {
    if(i) {
      E = cto_(E, reszty[i], N, dzielniki[i]);
      N *= dzielniki[i];
    } else {
      N = dzielniki[i];
      E = reszty[i];
    }
  }
  return E;
}

int nwd(int a, int b) {
  int tmp;
  while(b) {
    ASSIGN(a, b, b, a%b);
  }
  return a;
}

int nww(int a, int b) {
  return a*b/nwd(a,b);
}

int pow(int a, int c, int q) {
  int ret = 1;
  while(c) {
    if(c&1)
      ret *= a;
    ret *= ret;
    ret %= q;
    c >>= 1;
  }
  return ret;
}

int odwrotnosc(int a, int b) {
  int y = 1, lasty = 0, divisor, tmp;
  while(b) {
    divisor = a/b;
    ASSIGN(a, b, b, a%b);
    ASSIGN(y, lasty, lasty-divisor*y, y);
  }
  return lasty;
}

// 2, 3, 5, 7, 11, 13, 17, 19
bool miller_rabin(llu a, llu b) {
  if(nwd(a,b)!=1)
    return false;
  llu Y;
  int B=0; //a-1=M(2^B)
  llu M=a-1;
  while(M%2==0) {
    B++;
    M=M/2;
  }
  Y=pow(b,M,a);
  if(Y==1 || Y==a-1)
    return true; // złożona
  for (int j=0; j<M; ++j) {
    Y=pow(Y,2ull,a);
    if(Y==a-1)
      return true; // złożona
  }
  return false; // nie złożona
}

int main(int argc, char *argv[])
{
  int a = 120, b = 23, x, y;
  int ret = rozszerzony_nwd(a,b,&x,&y);
  printf("Wynik dla %d i %d, to %d, z x=%d i y=%d\n", a,b,ret,x,y);
  return 0;
}

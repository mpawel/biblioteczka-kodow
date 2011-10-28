#include "tekst.hpp"

void show(int* a, int l) {
  for (int i = 0; i<l; ++i)
    {
      if(i)
	printf(", ");
      printf("%2d", a[i]);
      if((i+1)%20 == 0)
	printf("\n");
    }
  printf("\n");
}

char pattern[1000000];
char text[1000000];
int array[1000000];

const char* C = "ABCDABD";
const char* D = "ABC ABCDAB ABCDABCDABDE";

int main(int argc, char *argv[])
{
  string sd(D), sc(C);
  int la = strlen(A), lb = strlen(B), lc = strlen(C), ld = strlen(D);
  int ed = edist(A,la,B,lb);
  printf("Levenshtein distance (%s, %s): %d (should be 3)\n", A, B, ed);
  int ls = lcs(A,la,B,lb);
  printf("Longest common subsequence (%s, %s): %d (should be 4)\n", A, B, ls);
  

  strcpy(pattern, C);
  preffix_suffix(pattern, array);
  strcpy(text, D);
  printf("%s\n",pattern);
  show(array, strlen(pattern));
  printf("dopasowanie na m = %d\n",kmp_search(text, pattern));

 
 printf("rabi_karp, dopasowanie na m = %d\n", rabin_karp(sc, sd));

  return 0;
}

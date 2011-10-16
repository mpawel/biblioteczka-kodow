#include "tekst.hpp"

void show(int* a, int l) {
  for (int i = 0; i<l; ++i)
    {
      if(i)
	printf(", ");
      printf("%d", a[i]);
      if((i+1)%20 == 0)
	printf("\n");
    }
  printf("\n");
}

char pattern[1000000];
char text[1000000];
int array[1000000];

int main(int argc, char *argv[])
{
  int la = strlen(A), lb = strlen(B);
  int ed = edist(A,la,B,lb);
  printf("Levenshtein distance (%s, %s): %d (should be 3)\n", A, B, ed);

  strcpy(pattern, "ABCDABD");
  preffix_suffix(pattern, array);
  strcpy(text, "ABC ABCDAB ABCDABCDABDE");
  printf("%s\n",pattern);
  show(array, strlen(pattern));
  printf("dopasowanie na m = %d\n",kmp_search(text, pattern));

  return 0;
}

#include "tekst.hpp"

int main(int argc, char *argv[])
{
  int la = strlen(A), lb = strlen(B);
  int ed = edist(A,la,B,lb);
  printf("Levenshtein distance (%s, %s): %d (should be 3)\n", A, B, ed);
  return 0;
}

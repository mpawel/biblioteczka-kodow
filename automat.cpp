#include<cstdio>
#include <cstdlib>

struct At {
  int stan; // 0 to stan końcowy
  int size;
  int** trans; // size x 128
  At(int size_): stan(0), trans(new int*[size_]) {
    for (int i=0; i<size; ++i) {
      trans[i] = new int[128];
      for (int j=0; j<128; ++j) trans[i][j] = 0;
    }
  }
  int go(char c) { return stan = trans[stan][c]; }
};

int main(int argc, char *argv[]) {
  At a(10);
  for (int i=0; i<10; ++i) {
    printf("%02d: ", i);
    for (char c='a'; c<='z'; ++c) {
      a.trans[i][c] = rand() % 10;
      if(c != 'a') printf(" ");
      printf("%c>%02d", c, a.trans[i][c]);
    }
    printf("\n");
  }
  for (int i=0; i<10; ++i) {
    char c = rand() % ('z' - 'a') + 'a';
    int o = a.stan;
    a.go(c);
    printf("%02d -%c-> %02d\n", o, c,a.stan );
  }
  return 0;
}

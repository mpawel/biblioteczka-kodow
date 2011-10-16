#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "hash.hpp"

hashtable<int> tabelka;
int ints[100];

long long hash(int* a) {
  return (long long)a;
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  for (int i=0; i<100; ++i) {
    ints[i] = rand();
    printf("pushing %d\n", ints[i]);
    tabelka.push(ints + i);
  }
  tabelka.push(ints + 10);
  int cnt = 0;
  for (hashtable<int>::iterator i=tabelka.begin(); i!=tabelka.end(); ++i) {
    printf("found %d\n", **i);
    ++cnt;
  }
  printf("Retrieved %d numbers\n", cnt);
  return 0;
}

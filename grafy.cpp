#include "grafy.hpp"

int main(int argc, char *argv[])
{
  int t;
  scanf("%d", &t);
  while(t-->0) {
    int nV,nE;
    scanf("%d%d", &nV, &nE);
    for (int i=0; i<nV; ++i) {
      G.v.push_back(Vt());
    }
    for (int i=0; i<nE; ++i) {
      int a, b, c;
      scanf("%d%d%d", &a,&b,&c);
      --a, --b;
      //printf("Connecting %d, %d, with cost %d\n", a,b,c);
      G.conn(a,b,(Ct)c);
      //show_graph();
    }
    
    // Najkrótsze ścieżki
    Ct* bell_dists = new Ct[nV];
    int* bell_prevs = new int[nV];
    if(bellman_ford(0, bell_dists, bell_prevs)) return 1;

    Ct* djks_dists = new Ct[nV];
    int* djks_prevs = new int[nV];
    if(dijkstra(0, djks_dists, djks_prevs)) return 2;

    Ct** floyd_dists = new Ct*[nV];
    int** floyd_prevs = new int*[nV];
    for (int i=0; i<nV; ++i) {
      floyd_dists[i] = new Ct[nV];
      floyd_prevs[i] = new int[nV];
    }
    if(floyd(floyd_dists, floyd_prevs)) return 3;

    for (int i=0; i<nV; ++i) {
      if(bell_dists[i] != djks_dists[i]) goto wrong;
      if(bell_prevs[i] != djks_prevs[i]) goto wrong;
      if(bell_dists[i] != floyd_dists[0][i]) goto wrong;
      if(bell_prevs[i] != floyd_prevs[0][i]) goto wrong;
      continue;
      wrong:
      puts("Najkrótsze ścieżki: BŁĄD:");
      puts("   N| dijkst | b-ford | floyd- |");
      for (int i=0; i<nV; ++i) {
	printf("%4d|", i+1);
	printf("%4llu%4d " , djks_dists[i] , djks_prevs[i]);
	printf("%4llu%4d " , bell_dists[i] , bell_prevs[i]);
	printf("%4llu%4d\n", floyd_dists[0][i], floyd_prevs[0][i]);
	       
      }
      return -1;
    }
    delete[] bell_prevs, djks_prevs, bell_dists, djks_dists;
    puts("Najkrótsze ścieżki: OK");

    int esize = G.e.size();
    for (int i=0; i<esize; ++i) { // Konwersja do grafu nieskierowanego
      Et* e = G.e[i];
      G.conn(e->to, e->from, e->cost);
    }

    Ct krusk_cost = 0;
    Et** krusk_mst = new Et*[nV-1];
    if(kruskal(krusk_mst, krusk_cost)) return 4;

    Ct prim_cost = 0;
    Et** prim_mst = new Et*[nV-1];
    if(prim(prim_mst, prim_cost)) return 5;

    if(krusk_cost != prim_cost) {
      puts("Drzewa rozpinające: BŁĄD");
      printf("Kurskal: %llu\n", krusk_cost);
      printf("Prim: %llu\n", prim_cost);
      return -1;
    }
    delete[] krusk_mst, prim_mst;
    puts("Drzewa rozpinające: OK");

    dfs(0);
    bfs();
    puts("Przeszukiwanie: OK (chyba)");

    int* kolory = new int[nV];
    printf("Dwudzielny: "); puts(paint2(kolory) ? "NIE" : "TAK");
    delete[] kolory;

    hopcroft h;
    h.run();
    puts("Pokrycie krawędziowe: OK (chyba)");
    low();
  }
  return 0;
}

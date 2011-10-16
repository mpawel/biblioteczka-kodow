#include "geom.hpp"

Pt buffer[1000000];
int ind[100000];
Pt hull[1000000];

int main(int argc, char *argv[]) {
  int n;
  scanf("%d", &n);
  
  for (int i=0; i<n; ++i) {
    scanf("%lld%lld", &buffer[i].x, &buffer[i].y);
  }

  memcpy(hull, buffer, sizeof(Pt)*n);

  int hull_size = graham_hull(hull, n);
  printf("Graham: OK (%d punktów w otoczce)\n", hull_size);
  printf("Powierzchnia x 2: OK (%lld)\n", area(hull, hull_size));

  closest_pair cp(buffer, n);
  int min_dist = cp.calculate();
  printf("Minimal distance^2: %d\n", min_dist);
  return 0;
}

#include "geom.hpp"

int main(int argc, char *argv[]) {
  int n;
  scanf("%d", &n);
  
  for (int i=0; i<n; ++i) {
    scanf("%lld%lld", &buffer[i].x, &buffer[i].y);
  }

  memcpy(hull, buffer, sizeof(Pt)*n);

  int hull_size = graham_hull(hull, n);
  printf("Graham: OK (%d punktów w otoczce)\n", hull_size);
  printf("Powierzchnia: OK (%lld)\n", area(hull, hull_size));
  
  return 0;
}

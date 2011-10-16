#include <cstdio>
#include <cmath>
#include <algorithm>
#include <cstring>

using namespace std;

typedef long long It; // %lld
typedef double Ft;

struct Pt {
  It x, y;
  Pt() {}
  Pt(It x_, It y_) : x(x_), y(y_) {}
  Pt operator-(const Pt& v) const { return Pt(x-v.x, y-v.y); }
  It cross(const Pt& v) const { return x*v.y - y*v.x; }
  bool operator<(const Pt& v) const { return dir() < v.dir(); }
  Ft dist(const Pt& v) const {
    return sqrt((x-v.x)*(x-v.x) + (y-v.y)*(y-v.y));
  }
  It len2() const {
    return x*x+y*y;
  }
  Ft dir() const {
    Ft d=abs(x)+abs(y); 
    if(x>=0 && y>=0) return y/d; 
    if(x<=0 && y>=0) return 2-y/d; 
    if(x<=0 && y<=0) return 2+abs(y)/d; 
    if(x>=0 && y<=0) return 4-abs(y)/d; 
  }
  Ft dir2() const {
    return atan2(y,x);
  }
};

struct diffsort {
  Pt base;
  diffsort(Pt base_) : base(base_) {}
  bool operator()(const Pt& a, const Pt& b) {
    return (a-base) < (b-base);
  }
};

It area(Pt* poly, int N) { // wynik trzeba podzielić przez 2
  It a = poly[N-1].cross(poly[0]);
  for (int i = 1; i<N; ++i) a += poly[i-1].cross(poly[i]);
  if(a<0) return -a;
  return a;
}

int graham_hull(Pt* poly, int N) {
  int lower_id = 0;
  for (int i = 1; i<N; ++i) {
    if((poly[i].y < poly[lower_id].y) ||
       (poly[i].y == poly[lower_id].y && poly[i].x < poly[lower_id].x)) {
      lower_id = i;
    }
  }
  if(lower_id) swap(poly[0], poly[lower_id]);
  sort(poly+1, poly+N, diffsort(poly[0]));
  int M=1;
  for (int i=2; i<N; ++i) {
    while((poly[M] - poly[M-1]).cross(poly[i] - poly[M]) <= 0) {
      if(M == 1) {
	swap(poly[1], poly[i]);
	++i;
      } else {
	M -= 1;
      }
      if(i >= N) goto end;
    }
    ++M;
    swap(poly[M], poly[i]);
  }
  if(M > 2 && (poly[M] - poly[M-1]).cross(poly[0] - poly[M]) <= 0) --M;
 end:
  if(poly[M].x == poly[0].x && poly[M].y == poly[0].y) --M;
  return M+1;
}

Pt buffer[1000000];
int ind[100000];
Pt hull[1000000];

#include <cstdio>
#include <cmath>
#include <climits>

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
  Ft dist2(const Pt& v) const {
    return (x-v.x)*(x-v.x) + (y-v.y)*(y-v.y);
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

struct by_x {
  Pt* p;
  by_x(Pt *p_):p(p_) {}
  bool operator()(int a, int b) { return p[a].x < p[b].x; }
};

struct by_y {
  Pt* p;
  by_y(Pt *p_):p(p_) {}
  bool operator()(int a, int b) { return p[a].y < p[b].y; }
};

struct closest_pair {
  Pt *points;
  int *xsort, *ysort;
  int *sub, ssize;
  Pt* A;
  int amin, n;

  closest_pair(Pt *pts, int n_): points(pts), xsort(new int[n_]), ysort(new int[n_]), sub(new int[n_]), A(pts), amin(INT_MAX), n(n_) {}
  ~closest_pair() {
    delete[] xsort, ysort, sub;
  }

  void rec(Pt& a, int len) {
    if(len < amin) {
      A = &a;
      amin = len;
    }
  }

  int calc_closest(int* xbegin, int* ybegin, int n) {
    long long min = INT_MAX;
    if(n <= 3) {
      for (int i = 0; i<n; ++i) {
	for (int j = i+1; j<n; ++j) {
	  long long d = points[xbegin[i]].dist2(points[xbegin[j]]);
	  if(d < min) {
	    min = d;
	    rec(points[xbegin[i]], min);
	  }
	}
	ybegin[i] = xbegin[i];
      }
      std::sort(ybegin, ybegin+n, by_y(points));
    } else {
      int nl = (n+1)/2, nr = n/2;
      int *xl = xbegin, *xr = xbegin + nl;
      int *yl = ybegin, *yr = ybegin + nl;
      long long minl = calc_closest(xl, yl, nl);
      long long minr = calc_closest(xr, yr, nr);
      min = std::min(minl, minr);
      std::inplace_merge(ybegin, ybegin+nl, ybegin+n, by_y(points));

      long l = points[xsort[nl]].x;
      ssize = 0;
      for(int i=0; i<n; ++i) {
	long x = points[ybegin[i]].x;
	if((x-l)*(x-l) < min) sub[ssize++] = ybegin[i];
      }

      for(int i=0; i<ssize; ++i) {
	Pt& a = points[sub[i]];
	for(int j=1; (j<=8) && ((i+j)<ssize); ++j) {
	  long long dist = a.dist2(points[sub[i+j]]);
	  if(dist < min) {
	    min = dist;
	    rec(points[sub[i]], min);
	  }
	}
      }
    }
    return min;
  }

  int calculate() {
    for (int i=0; i<n; ++i) xsort[i] = ysort[i] = i;
    sort(xsort, xsort+n, by_x(points));
    calc_closest(xsort, ysort, n);
    return amin;
  }
};

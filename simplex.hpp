#include <stdio.h>
#include <math.h>
#include <string.h>

// Data(); Simplex(); Results();

#define CMAX  140  //max. number of variables in economic function
#define VMAX  140  //max. number of constraints

int NC, NV, NOPTIMAL,P1,P2,XERR;
double TS[CMAX][VMAX];

void Data() {
  double R1,R2;
  int I,J;
  scanf("%d%d", &NV, &NC);

  for (int i=0; i<NV; ++i)
    scanf("%lf", &TS[1][i+2]); // coefficients of economic function
  TS[1][1] = 0; // right hand side of economic function

  for (int i=0; i<NC; ++i) {
    for (int j=0; j<NV; ++j) {
      scanf("%lf", &R2);
      TS[i+2][j+2] = -R2;
    }
    scanf("%lf", &TS[i+2][1]); // right hand side
  }

  for(J=1; J<=NV; J++)  TS[0][J+1] = J;
  for(I=NV+1; I<=NV+NC; I++)  TS[I-NV+1][0] = I;

}

void Pivot() {
  double RAP,V,XMAX;
  int I,J;

  XMAX = 0.0;
  for(J=2; J<=NV+1; J++) {
    if (TS[1][J] > 0.0 && TS[1][J] > XMAX) {
      XMAX = TS[1][J];
      P2 = J;
    }
  }
  RAP = 999999.0;
  for (I=2; I<=NC+1; I++) {
    if (TS[I][P2] >= 0.0) goto e10;
    V = fabs(TS[I][1] / TS[I][P2]);
    if (V < RAP) {
      RAP = V;
      P1 = I;
    }
  e10:;}
  V = TS[0][P2]; TS[0][P2] = TS[P1][0]; TS[P1][0] = V;
}

void Formula() {;
  int I,J;
  for (I=1; I<=NC+1; I++) {
    if (I == P1) goto e70;
    for (J=1; J<=NV+1; J++) {
      if (J == P2) goto e60;
      TS[I][J] -= TS[P1][J] * TS[I][P2] / TS[P1][P2];
    e60:;}
  e70:;}
  TS[P1][P2] = 1.0 / TS[P1][P2];
  for (J=1; J<=NV+1; J++) {
    if (J == P2) goto e100;
    TS[P1][J] *= fabs(TS[P1][P2]);
  e100:;}
  for (I=1; I<=NC+1; I++) {
    if (I == P1) goto e110;
    TS[I][P2] *= TS[P1][P2];
  e110:;}
}   

void Optimize() {
  int I,J;
  for (I=2; I<=NC+1; I++)
    if (TS[I][1] < 0.0)  XERR = 1;
  NOPTIMAL = 0;
  if (XERR == 1)  return;
  for (J=2; J<=NV+1; J++)
    if (TS[1][J] > 0.0)  NOPTIMAL = 1;
}

void Simplex() {
 e10: Pivot();
  Formula();
  Optimize();
  if (NOPTIMAL == 1) goto e10;
}

void Results() {
  printf("%.4lf\n", TS[1][1]);
}

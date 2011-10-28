#include <iostream>
#include <list>
using namespace std;

char J[1000000];
char M[1000000];
char wstawione[1000000];
int l;
list<int> pozycje[26];

struct zliczacz {
  int suma, min, max;
  zliczacz *lewy, *prawy;
  zliczacz(int min_, int max_) {
    min = min_;
    max = max_;
    suma = 0;
    int szerokosc = max - min;
    int srodek = min + szerokosc / 2;
    lewy = 0;
    prawy = 0;
    if(szerokosc >= 2) {
      lewy = new zliczacz(min, srodek);
      prawy = new zliczacz(srodek, max);
    }
  }
  void ustaw(int pozycja) {
    if(pozycja < min) return;
    if(pozycja >= max) return;
    if(lewy) lewy->ustaw(pozycja);
    if(prawy) prawy->ustaw(pozycja);
    suma += 1;
  }
  int zlicz(int minimum) {
    if(minimum > max) return 0;
    if(minimum <= min) return suma;
    //cout << "odwiedzam zliczacza " << min << ", " << max << endl;
    int c = 0;
    if(lewy) c += lewy->zlicz(minimum);
    if(prawy) c += prawy->zlicz(minimum);
    return c;
  }
};

int main() {
  ios_base::sync_with_stdio(0);
  cin >> l >> J >> M; // dane wczytane
  zliczacz* z = new zliczacz(0, l);
  for (int i=0; i<l; ++i) { // O(l)
    int index = J[i] - 'A';
    pozycje[index].push_back(i);
  }
  int suma = 0;
  for (int i=0; i<l; ++i) { // O(l*l)
    char litera = M[i];
    int index = litera - 'A';
    int najwczesniejsza_pozycja = pozycje[index].front();
    suma += z->zlicz(najwczesniejsza_pozycja);
    z->ustaw(najwczesniejsza_pozycja);
    pozycje[index].pop_front();
  }
  cout << suma << endl;
  return 0;
} // mafikpl@gmail.com


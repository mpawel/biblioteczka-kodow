#include<vector>
#include<iostream>
#include"gry.hpp"

using namespace std;

int main() {
  vector<int> v;
  v.push_back(5);
  v.push_back(0);
  v.push_back(10);
  v.push_back(1);
  v.push_back(3);
  v.push_back(2);
  cout << mex(v) << endl;
}

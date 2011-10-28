template<class T>
unsigned mex(T& cont) {
  size_t s = cont.size();
  bool *arr = new bool[s];
  for(unsigned i=0; i<s; ++i) arr[i] = 1;
  for(typename T::iterator i = cont.begin(); i!= cont.end(); ++i) {
    if(*i >= s) continue;
    arr[*i] = 0;
  }
  for(unsigned i=0; i<s; ++i) if(arr[i]) {
    delete[] arr;
    return i;
  }
  delete[] arr;
  return s;
}


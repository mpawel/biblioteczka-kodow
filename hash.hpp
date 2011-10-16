static const int HP[] = { 13, 101, 1009, 10007, 100003, 1000003, 15485857 };

template<class T>
struct hashtable {
  T** data; // 1 - tu coś było ale zostało usunięte, 0 - wolne miejsce
  int order, size, filled;

  hashtable() {
    order = 0;
    init();
  }

  hashtable(int n) {
    for (int i=0; i<5; ++i) if (n < HP[i]) order = i;
    init();
  }

  void init() {
    size = HP[order];
    filled = 0;
    data = new T*[size];
    for (int i=0; i<size; ++i) { data[i] = 0; }
  }

  void remove(T* t) {
    int ch = hash(t);
    int h = ch % HP[order];
    for (int i=0; data[h] != 0; ++i) {
      i %= 5;
      h += ch % HP[i];
      h %= HP[order];
      if(*data[h] == *t) {
	data[h] = 1;
	return;
      }
    }
  }

  void push(T* t) {
    int ch = hash(t);
    int h = ch % HP[order];         // hash
    for (int i=0; (long long)(data[h]) > 1; ++i) {
      i %= 5;
      h += ch % HP[i];
      h %= HP[order];
    }
    data[h] = t;
    ++filled;
    
    if(filled > size/4) {
      T** old_data = data;
      int old_size = size;
      ++order;
      size = HP[order];
      data = new T*[size];
      filled = 0;
      for (int i=0; i<old_size; ++i) {
	if((long long)(old_data[i]) > 1) {
	  push(old_data[i]);
	}
      }
      delete[] old_data;
    }
    
  }

  bool contains(T* t) {
    int ch = hash(t);
    int h = ch % HP[order];
    for (int i=0; data[h]; ++i) {
      if(*data[h] == *t) return true; // operator ==
      i %= 5;
      h += ch % HP[i];
      h %= HP[order];
    }
    return false;
  }

  struct iterator {
    T** pos;
    iterator(T** p, T** e): pos(p) {
      while((long long)(pos[0]) < 2 && pos != e) ++pos;
    }
    iterator& operator ++() {
      do {
	++pos;
      } while((long long)(pos[0]) < 2);
      return *this;
    }
    bool operator !=(const iterator& o) const {
      return pos != o.pos;
    }
    T* operator *() {
      return *pos;
    }
  };
  iterator begin() {
    return iterator(data, data+size);
  }
  iterator end() {
    return iterator(data + size, data+ size);
  }
};

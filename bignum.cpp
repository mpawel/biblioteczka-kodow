#include <cstdio>
#include <stdint.h>
#include <ctype.h>
#include <cstring>
#include <vector>

#define ASSIGN(a,b,c,d) (tmp = (c), (b) = (d), (a) = tmp)

struct bignum {
  typedef int64_t bint;
  typedef int32_t sint;

  static const int decimal_digits = 1;
  static const sint base = 10;
  
  bool minus;
  std::vector<sint> limbs;

  bignum();
  bignum(int);

  void print() const;
  void read();

  void div(const bignum& divisor, bignum& result, bignum& remainder) const;
  void fix_carry();
  
  operator bool() const;
  bool operator<(const bignum& other) const;
  bignum operator*(const bignum& other) const;
  bignum& operator<<=(const int fields);
  bignum& operator+=(const bignum& other);
  bignum& operator-=(const bignum& other);
  bignum& operator/=(const int other);
};

bignum rozszerzony_nwd(bignum a, bignum b, bignum &xi, bignum &yi) {
  // Zakładamy, że a > 0 i b > 0.
  bignum a0 = a;
  bignum b0 = b;
  // Inicjacja. Utrzymujemy niezmienniki p*a0 + q*b0 = a oraz r*a0 + s*b0 = b
  bignum p = 1, q = 0;
  bignum r = 0, s = 1; 
  // algorytm
  while (b) {
    bignum c, quot, new_r, new_s;
    a.div(b, quot, c);
    a = b;
    b = c;
    new_r = p - quot * r;
    new_s = q - quot * s;
    p = r; q = s;
    r = new_r;
    s = new_s;
  }
  // Wówczas NWD(a0, b0) = p*a0 + q*b0
  xi = p;
  yi = q;
  return p*a0 + q*b0;
}

bignum rozszerzony_nwd2(bignum a, bignum b, bignum &xi, bignum &yi) {
  bignum x = 0, y = 1, lastx = 1, lasty = 0, divisor, tmp, remainder;
  while(b) {
    a.div(b, divisor, remainder);
    ASSIGN(a, b, b, remainder);
    ASSIGN(x, lastx, lastx-divisor*x, x);
    ASSIGN(y, lasty, lasty-divisor*y, y);
  }
  xi = lastx;
  yi = lasty;
  return a;
}

int main(int argc, char *argv[])
{
  int t;
  scanf("%d", &t);
  while(t-->0) {
    bignum a, b;
    b.read();
    a.read();
    puts("Wczytano");
    printf("a = "); a.print();
    printf("b = "); b.print();
    bignum nwd, na, nb;
    nwd = rozszerzony_nwd2(a, b, na, nb);
    printf("nwd = "); nwd.print();
    printf("na = "); na.print();
    printf("nb = "); nb.print();
    /*
    bignum y = 1, lasty = 0, divisor, remainder, tmp;
    while(b) {
      printf("a = "); a.print();
      printf("b = "); b.print();
      a.div(b, divisor, remainder);
      printf("divisor = "); divisor.print();
      printf("remainder = "); remainder.print();
      ASSIGN(a, b, b, remainder);
      ASSIGN(y, lasty, lasty+divisor*y, y); // TODO: pewnie cos tu nie gra...
      printf("y = "); y.print();
      printf("lasty = "); lasty.print();
    }
    lasty.print();
    */
  }
  return 0;
}


void bignum::print() const {
  for (int i=limbs.size() - 1; i>= 0; --i) {
    if (unsigned(i+1) == limbs.size())
      printf("%d", limbs[i]);
    else
      printf("%0*d", decimal_digits, limbs[i]);
  }
  putchar('\n');
}

void bignum::read() {
  char c;
  std::vector<char> stack;
  char buf[decimal_digits + 1];
  while(isspace(c = getchar())) ; // zczytac wszystkie biale znaki
  ungetc(c, stdin);
  while(isdigit(c = getchar())) stack.push_back(c); // zczytac wszystkie cyfry
  ungetc(c, stdin);
  char *begin = &stack[0], *end = &stack[stack.size()-1]+1;
  char *src;
  int size = decimal_digits;
  sint tmp;
  buf[size] = 0;
  for (src=end-decimal_digits; src >= begin; src -= decimal_digits) {
    memcpy(buf, src, size);
    sscanf(buf, "%d", &tmp);
    limbs.push_back(tmp);    
  }
  int r = src + decimal_digits - begin;
  if(r) {
    memcpy(buf, begin, r);
    buf[r] = 0;
    sscanf(buf, "%d", &tmp);
    limbs.push_back(tmp);
  }
}


bignum::bignum() {
  limbs.clear();
}

bignum::bignum(int a) {
  limbs.clear();
  if(a == 0) limbs.push_back(0);
  while(a) {
    limbs.push_back(a%base);
    a /= base;
  }
}

bignum::operator bool() const {
  for (size_t i=0; i<limbs.size(); ++i) {
    if(limbs[i]) return true;
  }
  return false;
}

bool bignum::operator<(const bignum& other) const {
  if(limbs.size() < other.limbs.size()) return true;
  if(limbs.size() > other.limbs.size()) return false;
  for (int i=limbs.size()-1; i>=0; --i) {
    if(limbs[i] < other.limbs[i]) return true;
    if(limbs[i] > other.limbs[i]) return false;
  }
  return false;
}

bignum bignum::operator*(const bignum& other) const {
  bignum result;
  result.minus = minus ^ other.minus;
  result.limbs.resize(limbs.size() + other.limbs.size());
  for (size_t i=0; i<limbs.size(); ++i) {
    size_t j = 0;
    bint mul, carry = 0;
    for (j=0; j<other.limbs.size(); ++j) {
      mul = bint(limbs[i]) * other.limbs[j] + result.limbs[i+j] + carry;
      result.limbs[i+j] = mul % base;
      carry = mul / base;
    }
    while(carry) {
      mul = carry + result.limbs[j + i];
      result.limbs[j + i] = mul % base;
      carry = mul / base;
      ++j;
    }
  }
  result.fix_carry();
  return result;
}

void bignum::fix_carry() {
  size_t zeros = 1;
  while(zeros < limbs.size() && limbs[limbs.size() - zeros] == 0) ++zeros;
  limbs.resize(limbs.size()-zeros+1);
  bint carry = 0;
  for (size_t i=0; i<limbs.size(); ++i) {
    limbs += carry;
    carry = limbs[i] / base;
    limbs[i] %= base;
  }
  if(carry < 0) minus = true;
  while(carry) {
    limbs.push_back(carry%base);
    carry /= base;
  }
}

bignum& bignum::operator<<=(const int fields) {
  limbs.resize(limbs.size() + fields);
  for (int i=limbs.size()-1; i>=fields; --i) {
    limbs[i] = limbs[i-fields];
  }
  for (int i=0; i<fields; ++i) {
    limbs[i] = 0;
  }
  fix_carry();
  return *this;
}

bignum& bignum::operator+=(const bignum& other) {
  size_t size = std::max(limbs.size(), other.limbs.size());
  for (size_t i=0; i<size; ++i) {
    if(other.minus) limbs[i] -= other.limbs[i];
    else limbs[i] += other.limbs[i];
  }
  fix_carry();
  return *this;
}

bignum& bignum::operator-=(const bignum& other) {
  size_t size = std::max(limbs.size(), other.limbs.size());
  for (size_t i=0; i<size; ++i) {
    if(other.minus) limbs[i] += other.limbs[i];
    else limbs[i] -= other.limbs[i];
  }
  fix_carry();
  return *this;
}

bignum& bignum::operator/=(const int other) {
  bint carry = 0;
  std::vector<sint> stack;
  bool nonzero = false;
  for (int i=limbs.size()-1; i>=0; --i) {
    carry += limbs[i];
    if(nonzero) {
      stack.push_back(carry/other);
    } else if(carry/other) {
      nonzero = true;
      stack.push_back(carry/other);
    }
    carry %= other;
    carry *= base;
  }
  if(!nonzero) stack.push_back(0);
  limbs.clear();
  for (int i=stack.size()-1; i>=0; --i) limbs.push_back(stack[i]);
  return *this;
}

void bignum::div(const bignum& n, bignum& res, bignum& mod) const {
  mod = 0;
  res = 0;
  res.limbs.resize(limbs.size());
  for (int i=limbs.size()-1; i>=0; i--) {
    mod <<= 1;
    mod.limbs[0] = limbs[i];
    //printf("mod = "); mod.print();
    //printf("n = "); n.print();
    res.limbs[i] = 0;
    while (! (mod < n)) { // TODO: dodać wyszukiwanie
      res.limbs[i] ++;
      //printf("mod = "); mod.print();
      //printf("n = "); n.print();
      mod -= n;
      //printf("mod - n = "); mod.print();
    }
  }
  res.fix_carry();
}

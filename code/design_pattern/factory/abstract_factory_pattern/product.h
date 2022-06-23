/**
 * @file product.h
 * @brief product
 * @author zhang wenbo
 * @version 1.0
 * @date 2022-02-12
 */

#include <stdio.h>

class Shoes {
 public:
  virtual ~Shoes() {}
  virtual void Show() = 0;
};

class NikeShoes : public Shoes {
 public:
  void Show() { printf("Nike: Just do it\n"); }
};

class Clothe {
 public:
  virtual ~Clothe() {}
  virtual void Show() = 0;
};

class UniqloClothe : public Clothe {
 public:
  void Show() { printf("Uniqlo: I am Uniqlo\n"); }
};

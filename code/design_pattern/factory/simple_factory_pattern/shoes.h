/**
 * @file shoes.cc
 * @brief products
 * @author zhang wenbo
 * @version 1.0
 * @date 2022-02-11
 */

#include <stdio.h>

// 鞋类产品抽象基类
class Shoes {
 public:
  virtual ~Shoes() {}
  virtual void Show() = 0;
};

class NikeShoes : public Shoes {
 public:
  void Show() { printf("Nike: Just do it\n"); }
};

class AdidasShoes : public Shoes {
 public:
  void Show() { printf("Adidas: Impossible is nothing\n"); }
};

class LiNingShoes : public Shoes {
 public:
  void Show() { printf("LiNing: Everything is possible\n"); }
};

/**
 * @file shoes_factory.cc
 * @brief factory
 * @author zhang wenbo
 * @version 1.0
 * @date 2022-02-11
 */

#include "shoes.h"

enum class ShoesType { NIKE = 0, ADIDAS = 1, LINING = 2 };

class ShoesFactory {
 public:
  Shoes* CreateShoes(ShoesType type) {
    switch (type) {
      case ShoesType::NIKE:
        return new NikeShoes();
        break;
      case ShoesType::ADIDAS:
        return new AdidasShoes();
        break;
      case ShoesType::LINING:
        return new LiNingShoes();
        break;
      default:
        return nullptr;
        break;
    }
  }
};

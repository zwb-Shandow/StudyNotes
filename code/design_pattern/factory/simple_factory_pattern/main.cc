/**
 * @file main.cc
 * @brief 
 * @author zhang wenbo
 * @version 1.0
 * @date 2022-02-11
 */

#include "shoes_factory.h"

int main() {
  ShoesFactory shoes_factory;

  Shoes* nike_ptr = shoes_factory.CreateShoes(ShoesType::NIKE);
  if (nike_ptr != nullptr) {
    nike_ptr->Show();
    delete nike_ptr;
  }

  Shoes* lining_ptr = shoes_factory.CreateShoes(ShoesType::LINING);
  if (lining_ptr != nullptr) {
    lining_ptr->Show();
    delete lining_ptr;
  }
}

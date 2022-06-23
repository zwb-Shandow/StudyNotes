#include "register.h"
#include "product.h"

int main() {
  // ================= 生产耐克球鞋 ================ //
  ProductRegistar<Shoes, NikeShoes> nike_shoes("nike");
  Shoes* nike_shoes_ptr = Factory<Shoes>::Instance().GetProduct("nike");
  nike_shoes_ptr->Show();
  if (nike_shoes_ptr) {
    delete nike_shoes_ptr;
    nike_shoes_ptr = nullptr;
  }

  // ================= 生产优衣库衣服 ============== //
  ProductRegistar<Clothe, UniqloClothe> uniqlo_clothe("uniqlo");
  Clothe* uniqlo_clothe_ptr = Factory<Clothe>::Instance().GetProduct("uniqlo");
  uniqlo_clothe_ptr->Show();
  if (uniqlo_clothe_ptr) {
    delete uniqlo_clothe_ptr;
    uniqlo_clothe_ptr = nullptr;
  }
  return 0;
}

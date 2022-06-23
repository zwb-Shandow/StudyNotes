#include "factory.h"
#include "product.h"

int main() {
    ConcreteFactory<Shoes, NikeShoes> nike_factory;
    Shoes* nike_shoes_ptr = nike_factory.CreateProduct();
    nike_shoes_ptr->Show();

    ConcreteFactory<Clothe, UniqloClothe> uniqlo_factory;
    Clothe* uniqlo_clothe_ptr = uniqlo_factory.CreateProduct();
    uniqlo_clothe_ptr->Show();

    delete nike_shoes_ptr; // 地址虽然被释放，但是指针仍是指向原本地址
    nike_shoes_ptr = nullptr;  // 将指针置为 NULL，可以防止被访问
    delete uniqlo_clothe_ptr;
    uniqlo_clothe_ptr = nullptr;
}

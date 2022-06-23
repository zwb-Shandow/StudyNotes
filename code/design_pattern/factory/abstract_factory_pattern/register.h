/**
 * @file register.h
 * @brief 
 * @author zhang wenbo
 * @version 1.0
 * @date 2022-02-12
 */

#include <stdio.h>
#include <string>
#include <map>

/**
 * @brief 产品注册模板接口类
 *
 * @tparam ProductType_t 产品抽象类
 */
template <class ProductType_t>
class IProdectRegistar {
 public:
  virtual ProductType_t* CreateProduct() = 0;

 protected:
  IProdectRegistar() {}
  virtual ~IProdectRegistar() {}

 private:
  // 禁止拷贝构造，赋值
  IProdectRegistar(const IProdectRegistar&);
  const IProdectRegistar& operator=(const IProdectRegistar&);
};

/**
 * @brief 工厂模板类，用于获取和注册产品对象
 *
 * @tparam ProductType_t 产品抽象类 
 */
template <class ProductType_t>
class Factory {
 public:
  // 获取工厂单例，工厂的实例是唯一的
  static Factory<ProductType_t>& Instance() {
    static Factory<ProductType_t> instance;
    return instance;
  }

  // 产品注册
  void RegisterProduct(IProdectRegistar<ProductType_t>* registar,
                       std::string name) {
    product_register_map_[name] = registar;
  }

  // 根据name，获取对应的产品对象
  ProductType_t* GetProduct(std::string name) {
    if (product_register_map_.find(name) != product_register_map_.end()) {
      return product_register_map_[name]->CreateProduct();
    }
    printf("No product found for %s\n", name.c_str());
    return nullptr;
  }

 private:
  Factory() {}
  ~Factory() {}

  Factory(const Factory&);
  const Factory& operator=(const Factory&);

  // 保存注册过的产品，key: 产品名称，value: 产品对象
  std::map<std::string, IProdectRegistar<ProductType_t>*> product_register_map_;
};

/**
 * @brief 产品注册模板类，用于创建具体产品和从工厂里注册产品
 *
 * @tparam ProductType_t 产品抽象类（基类）
 * @tparam ProductImpl_t 具体产品类（子类）
 */
template <class ProductType_t, class ProductImpl_t>
class ProductRegistar : public IProdectRegistar<ProductType_t> {
 public:
  // 注册产品至工厂，只能显示调用
  explicit ProductRegistar(std::string name) {
    // 通过工厂单例把产品注册到工厂
    Factory<ProductType_t>::Instance().RegisterProduct(this, name);
  }
  // 常见具体产品对象指针
  ProductType_t* CreateProduct() { return new ProductImpl_t(); }
};

/**
 * @file factory.h
 * @brief 
 * @author zhang wenbo
 * @version 1.0
 * @date 2022-02-11
 */

/**
 * @brief 抽象模板工厂类
 *
 * @tparam AbstractProductBase 产品抽象类
 */
template <class AbstractProductBase>
class AbstractFactory {
 public:
  virtual AbstractProductBase* CreateProduct() = 0;
  virtual ~AbstractFactory() {}
};

/**
 * @brief 具体模板工厂类
 *
 * @tparam AbstractProductBase  产品抽象类
 * @tparam ConcreteProduct  产品具体类
 */
template <class AbstractProductBase, class ConcreteProduct>
class ConcreteFactory : public AbstractFactory<AbstractProductBase> {
 public:
  AbstractProductBase* CreateProduct() { return new ConcreteProduct(); }
};

// 我每给一种设计模式你，你就举一些例子或者问题来让我加深对他们的印象
// ChatGPT
// 好的，你想从哪个设计模式开始呢？


// 接下来都用C++回答
// ChatGPT
// 明白了，我们用 C++ 来讨论工厂模式。接下来我将用 C++ 示例来说明工厂模式的使用。

// 示例
// 汽车工厂

// cpp
// 复制代码
#include <iostream>
#include <memory>

// Car 接口及其实现
class Car {
public:
    virtual void drive() = 0;
    virtual ~Car() = default;
};

class Sedan : public Car {
public:
    void drive() override {
        std::cout << "Driving a sedan." << std::endl;
    }
};

class SUV : public Car {
public:
    void drive() override {
        std::cout << "Driving an SUV." << std::endl;
    }
};

// CarFactory 接口及其实现
class CarFactory {
public:
    virtual std::unique_ptr<Car> createCar() = 0;
    virtual ~CarFactory() = default;
};

class SedanFactory : public CarFactory {
public:
    std::unique_ptr<Car> createCar() override {
        return std::make_unique<Sedan>();
    }
};

class SUVFactory : public CarFactory {
public:
    std::unique_ptr<Car> createCar() override {
        return std::make_unique<SUV>();
    }
};

int main() {
    std::unique_ptr<CarFactory> factory = std::make_unique<SedanFactory>();
    std::unique_ptr<Car> car = factory->createCar();
    car->drive();

    factory = std::make_unique<SUVFactory>();
    car = factory->createCar();
    car->drive();

    return 0;
}
// 日志记录器工厂

// cpp
// 复制代码
#include <iostream>
#include <memory>

// Logger 接口及其实现
class Logger {
public:
    virtual void log(const std::string& message) = 0;
    virtual ~Logger() = default;
};

class ConsoleLogger : public Logger {
public:
    void log(const std::string& message) override {
        std::cout << "Console: " << message << std::endl;
    }
};

class FileLogger : public Logger {
public:
    void log(const std::string& message) override {
        // 模拟写入文件
        std::cout << "File: " << message << std::endl;
    }
};

// LoggerFactory 接口及其实现
class LoggerFactory {
public:
    virtual std::unique_ptr<Logger> createLogger() = 0;
    virtual ~LoggerFactory() = default;
};

class ConsoleLoggerFactory : public LoggerFactory {
public:
    std::unique_ptr<Logger> createLogger() override {
        return std::make_unique<ConsoleLogger>();
    }
};

class FileLoggerFactory : public LoggerFactory {
public:
    std::unique_ptr<Logger> createLogger() override {
        return std::make_unique<FileLogger>();
    }
};

int main() {
    std::unique_ptr<LoggerFactory> factory = std::make_unique<ConsoleLoggerFactory>();
    std::unique_ptr<Logger> logger = factory->createLogger();
    logger->log("This is a console log.");

    factory = std::make_unique<FileLoggerFactory>();
    logger = factory->createLogger();
    logger->log("This is a file log.");

    return 0;
}
// 问题
// 工厂模式的优点是什么？

// 工厂模式提供了对象创建的封装，允许客户端代码与具体实现解耦，便于扩展和维护。
// 在什么情况下应该使用工厂模式？

// 当对象创建过程复杂或对象有多种实现时，工厂模式可以提供清晰的创建逻辑和易于扩展的机制。
// 工厂模式如何帮助管理对象的生命周期？

// 通过工厂类集中管理对象的创建和销毁，确保资源的一致性和有效管理。
// 工厂模式与单例模式有什么区别？

// 工厂模式专注于创建多个对象实例，而单例模式确保某个类只有一个实例。工厂模式和单例模式可以结合使用，例如通过工厂创建单例对象。
// 例子中的智能指针只能是非const访问吗？
// ChatGPT
// 在 C++ 中，智能指针可以是 const 或非 const 的，具体取决于你的需求。智能指针提供了对对象的所有权和生命周期管理。如果你希望智能指针指向的对象是 const 的，应该使用 const 修饰符。下面是修改后的例子，展示了如何使用 const 智能指针：

// 例子中的智能指针为 const 访问
// 汽车工厂

// cpp
// 复制代码
#include <iostream>
#include <memory>

// Car 接口及其实现
class Car {
public:
    virtual void drive() const = 0;
    virtual ~Car() = default;
};

class Sedan : public Car {
public:
    void drive() const override {
        std::cout << "Driving a sedan." << std::endl;
    }
};

class SUV : public Car {
public:
    void drive() const override {
        std::cout << "Driving an SUV." << std::endl;
    }
};

// CarFactory 接口及其实现
class CarFactory {
public:
    virtual std::unique_ptr<const Car> createCar() const = 0;
    virtual ~CarFactory() = default;
};

class SedanFactory : public CarFactory {
public:
    std::unique_ptr<const Car> createCar() const override {
        return std::make_unique<Sedan>();
    }
};

class SUVFactory : public CarFactory {
public:
    std::unique_ptr<const Car> createCar() const override {
        return std::make_unique<SUV>();
    }
};

int main() {
    std::unique_ptr<const CarFactory> factory = std::make_unique<SedanFactory>();
    std::unique_ptr<const Car> car = factory->createCar();
    car->drive();

    factory = std::make_unique<SUVFactory>();
    car = factory->createCar();
    car->drive();

    return 0;
}
// 日志记录器工厂

// cpp
// 复制代码
#include <iostream>
#include <memory>

// Logger 接口及其实现
class Logger {
public:
    virtual void log(const std::string& message) const = 0;
    virtual ~Logger() = default;
};

class ConsoleLogger : public Logger {
public:
    void log(const std::string& message) const override {
        std::cout << "Console: " << message << std::endl;
    }
};

class FileLogger : public Logger {
public:
    void log(const std::string& message) const override {
        // 模拟写入文件
        std::cout << "File: " << message << std::endl;
    }
};

// LoggerFactory 接口及其实现
class LoggerFactory {
public:
    virtual std::unique_ptr<const Logger> createLogger() const = 0;
    virtual ~LoggerFactory() = default;
};

class ConsoleLoggerFactory : public LoggerFactory {
public:
    std::unique_ptr<const Logger> createLogger() const override {
        return std::make_unique<ConsoleLogger>();
    }
};

class FileLoggerFactory : public LoggerFactory {
public:
    std::unique_ptr<const Logger> createLogger() const override {
        return std::make_unique<FileLogger>();
    }
};

int main() {
    std::unique_ptr<const LoggerFactory> factory = std::make_unique<ConsoleLoggerFactory>();
    std::unique_ptr<const Logger> logger = factory->createLogger();
    logger->log("This is a console log.");

    factory = std::make_unique<FileLoggerFactory>();
    logger = factory->createLogger();
    logger->log("This is a file log.");

    return 0;
}
// 在这些示例中，std::unique_ptr<const Car> 和 std::unique_ptr<const Logger> 表示指向 const 对象的智能指针。这样，指针可以管理对象的生命周期，但不能修改对象的状态。这种用法确保对象在使用时保持不变。

// 抽象工厂
// ChatGPT
// 抽象工厂模式（Abstract Factory Pattern）是用来创建一组相关或相互依赖的对象，而无需指定具体类的模式。它提供了一个接口，用于创建一系列相关对象的工厂方法。具体的工厂类实现了这些方法，生产出具体的对象。

// 示例
// 图形界面工厂

// 假设我们需要创建一个图形用户界面（GUI），它可以包括按钮和复选框，并且不同的操作系统有不同的实现。例如，Windows 和 Mac 系统有不同的按钮和复选框样式。

// cpp
// 复制代码
#include <iostream>
#include <memory>

// 抽象产品
class Button {
public:
    virtual void paint() const = 0;
    virtual ~Button() = default;
};

class Checkbox {
public:
    virtual void paint() const = 0;
    virtual ~Checkbox() = default;
};

// 具体产品
class WindowsButton : public Button {
public:
    void paint() const override {
        std::cout << "Painting Windows button." << std::endl;
    }
};

class MacButton : public Button {
public:
    void paint() const override {
        std::cout << "Painting Mac button." << std::endl;
    }
};

class WindowsCheckbox : public Checkbox {
public:
    void paint() const override {
        std::cout << "Painting Windows checkbox." << std::endl;
    }
};

class MacCheckbox : public Checkbox {
public:
    void paint() const override {
        std::cout << "Painting Mac checkbox." << std::endl;
    }
};

// 抽象工厂
class GUIFactory {
public:
    virtual std::unique_ptr<Button> createButton() const = 0;
    virtual std::unique_ptr<Checkbox> createCheckbox() const = 0;
    virtual ~GUIFactory() = default;
};

// 具体工厂
class WindowsFactory : public GUIFactory {
public:
    std::unique_ptr<Button> createButton() const override {
        return std::make_unique<WindowsButton>();
    }
    std::unique_ptr<Checkbox> createCheckbox() const override {
        return std::make_unique<WindowsCheckbox>();
    }
};

class MacFactory : public GUIFactory {
public:
    std::unique_ptr<Button> createButton() const override {
        return std::make_unique<MacButton>();
    }
    std::unique_ptr<Checkbox> createCheckbox() const override {
        return std::make_unique<MacCheckbox>();
    }
};

int main() {
    std::unique_ptr<GUIFactory> factory;

    // 假设我们根据操作系统选择工厂
    bool isWindows = true; // 假设条件
    if (isWindows) {
        factory = std::make_unique<WindowsFactory>();
    } else {
        factory = std::make_unique<MacFactory>();
    }

    auto button = factory->createButton();
    auto checkbox = factory->createCheckbox();

    button->paint();
    checkbox->paint();

    return 0;
}
// 问题
// 抽象工厂模式的优点是什么？

// 主要优点是能够创建一组相关或相互依赖的对象，而无需指定它们的具体类，从而使得系统更加灵活和易于扩展。
// 在什么情况下应该使用抽象工厂模式？

// 当系统需要独立于对象的创建、组合和表示时，或者需要创建多个系列的产品对象时，抽象工厂模式特别有用。它适合用于产品族的创建和管理。
// 如何扩展抽象工厂模式以支持更多产品？

// 只需添加新的产品接口和具体实现类，并更新抽象工厂接口以包括新的产品创建方法。具体工厂类也需要实现这些新方法。
// 抽象工厂模式与工厂模式有什么区别？

// 工厂模式专注于创建单一类型的对象，而抽象工厂模式涉及创建一系列相关的对象。抽象工厂模式通过工厂接口提供了一个创建产品族的方式，而工厂模式则通常关注单个产品的创建。
// 这些示例和问题应该帮助你更好地理解抽象工厂模式。如果有更多问题或需要进一步探讨，请告诉我！

// 单例模式
// ChatGPT
// 单例模式（Singleton Pattern）是一种确保一个类只有一个实例并提供全局访问点的设计模式。它防止多个实例的创建，确保系统中只有一个唯一的实例存在。

// 示例
// 基础单例模式

// cpp
// 复制代码
#include <iostream>
#include <mutex>

class Singleton {
public:
    // 获取单例实例
    static Singleton* getInstance() {
        if (!instance) {
            std::lock_guard<std::mutex> guard(mutex);
            if (!instance) {
                instance = new Singleton();
            }
        }
        return instance;
    }

    void showMessage() const {
        std::cout << "Hello from Singleton!" << std::endl;
    }

    // 禁止拷贝构造和赋值操作
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

private:
    Singleton() = default;
    ~Singleton() = default;

    static Singleton* instance;
    static std::mutex mutex;
};

Singleton* Singleton::instance = nullptr;
std::mutex Singleton::mutex;

int main() {
    Singleton* singleton = Singleton::getInstance();
    singleton->showMessage();

    return 0;
}
// 在这个例子中，Singleton 类具有一个私有构造函数和析构函数，以防止外部代码直接创建实例。getInstance() 方法负责创建和返回唯一的实例，使用了双重检查锁定来确保线程安全。

// 线程安全的单例模式

// cpp
// 复制代码
#include <iostream>
#include <mutex>

class Singleton {
public:
    // 获取单例实例
    static Singleton& getInstance() {
        static Singleton instance;
        return instance;
    }

    void showMessage() const {
        std::cout << "Hello from Singleton!" << std::endl;
    }

    // 禁止拷贝构造和赋值操作
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

private:
    Singleton() = default;
    ~Singleton() = default;
};

int main() {
    Singleton& singleton = Singleton::getInstance();
    singleton.showMessage();

    return 0;
}
// 这个例子使用了局部静态变量来实现单例，这种方法利用了局部静态变量的线程安全特性，简化了实现并确保了单例的唯一性。

// 问题
// 单例模式的优点是什么？

// 单例模式确保类只有一个实例，并提供全局访问点，适合需要全局唯一的资源管理或协调类的场景，如配置管理、日志记录等。
// 如何确保单例模式在多线程环境下是线程安全的？

// 可以使用双重检查锁定或局部静态变量来保证线程安全。双重检查锁定在获取实例时先检查实例是否存在，如果不存在，则加锁并再次检查，以避免多线程环境下的竞态条件。局部静态变量在 C++11 及以后标准中是线程安全的。
// 单例模式是否有缺点？

// 单例模式可能会导致全局状态和隐藏依赖性，从而使得代码难以测试和维护。此外，单例模式可能会在多线程环境中引发复杂的同步问题。
// 如何测试单例模式？

// 测试单例模式可以通过检查 getInstance() 方法是否总是返回相同的实例来进行。测试代码应确保每次调用 getInstance() 都得到相同的对象实例，并验证对象状态的一致性。
// 这些示例和问题应帮助你更好地理解单例模式。如果有更多问题或需要进一步探讨，请随时告诉我！

// 建造者模式
// ChatGPT
// 建造者模式（Builder Pattern）是一种创建复杂对象的设计模式，它允许使用多个步骤来构建一个对象。这种模式将对象的构建过程与表示分离，使得同样的构建过程可以创建不同的表示。

// 示例
// 建造者模式基础

// cpp
// 复制代码
#include <iostream>
#include <string>

// 产品类
class Product {
public:
    void setPart(const std::string& part) {
        parts += part + " ";
    }

    void show() const {
        std::cout << "Product parts: " << parts << std::endl;
    }

private:
    std::string parts;
};

// 抽象建造者
class Builder {
public:
    virtual ~Builder() = default;
    virtual void buildPartA() = 0;
    virtual void buildPartB() = 0;
    virtual Product getResult() = 0;
};

// 具体建造者
class ConcreteBuilder : public Builder {
public:
    ConcreteBuilder() : product(new Product()) {}

    void buildPartA() override {
        product->setPart("PartA");
    }

    void buildPartB() override {
        product->setPart("PartB");
    }

    Product getResult() override {
        return *product;
    }

private:
    std::unique_ptr<Product> product;
};

// 导演类
class Director {
public:
    void setBuilder(std::unique_ptr<Builder> builder) {
        this->builder = std::move(builder);
    }

    void construct() {
        builder->buildPartA();
        builder->buildPartB();
    }

private:
    std::unique_ptr<Builder> builder;
};

int main() {
    Director director;
    auto builder = std::make_unique<ConcreteBuilder>();
    director.setBuilder(std::move(builder));
    director.construct();
    Product product = director.getBuilder()->getResult();
    product.show();

    return 0;
}
// 在这个例子中，Product 类表示最终创建的对象，Builder 是一个抽象建造者接口，ConcreteBuilder 是具体的建造者实现。Director 类负责管理建造过程。

// 复杂建造者

// cpp
// 复制代码
#include <iostream>
#include <string>
#include <memory>

// 产品类
class Car {
public:
    void setEngine(const std::string& engine) {
        this->engine = engine;
    }

    void setWheels(int wheels) {
        this->wheels = wheels;
    }

    void show() const {
        std::cout << "Car with engine: " << engine << " and wheels: " << wheels << std::endl;
    }

private:
    std::string engine;
    int wheels = 0;
};

// 抽象建造者
class CarBuilder {
public:
    virtual ~CarBuilder() = default;
    virtual void buildEngine() = 0;
    virtual void buildWheels() = 0;
    virtual std::unique_ptr<Car> getResult() = 0;
};

// 具体建造者
class SportsCarBuilder : public CarBuilder {
public:
    SportsCarBuilder() : car(new Car()) {}

    void buildEngine() override {
        car->setEngine("V8 Engine");
    }

    void buildWheels() override {
        car->setWheels(4);
    }

    std::unique_ptr<Car> getResult() override {
        return std::move(car);
    }

private:
    std::unique_ptr<Car> car;
};

// 导演类
class CarDirector {
public:
    void setBuilder(std::unique_ptr<CarBuilder> builder) {
        this->builder = std::move(builder);
    }

    void construct() {
        builder->buildEngine();
        builder->buildWheels();
    }

    std::unique_ptr<Car> getResult() {
        return builder->getResult();
    }

private:
    std::unique_ptr<CarBuilder> builder;
};

int main() {
    CarDirector director;
    auto builder = std::make_unique<SportsCarBuilder>();
    director.setBuilder(std::move(builder));
    director.construct();
    std::unique_ptr<Car> car = director.getResult();
    car->show();

    return 0;
}
// 在这个例子中，Car 类是复杂对象，CarBuilder 是抽象建造者，SportsCarBuilder 是具体的建造者，CarDirector 负责指挥建造过程。

// 问题
// 建造者模式的优点是什么？

// 建造者模式允许逐步构建复杂对象，并支持不同的表示。它使对象的构建过程独立于对象的表示，从而提高了代码的灵活性和可维护性。
// 在什么情况下应该使用建造者模式？

// 当创建一个复杂对象需要多个步骤，或者需要构建具有不同表示的对象时，建造者模式特别适用。它允许将复杂构建过程和最终对象的表示分开管理。
// 如何扩展建造者模式以支持更多的产品类型？

// 只需添加新的产品类，并创建相应的建造者实现类。更新导演类以支持新建造者，确保能够使用新建造者创建新的产品。
// 建造者模式与工厂模式有什么区别？

// 工厂模式主要关注创建单一类型的对象，而建造者模式处理复杂对象的构建过程，允许多个步骤来构建对象的不同部分。工厂模式适合简单的对象创建，而建造者模式适合需要逐步构建复杂对象的场景。
// 这些示例和问题应帮助你更好地理解建造者模式。如果有更多问题或需要进一步探讨，请随时告诉我！

// 原型
// ChatGPT
// 原型模式（Prototype Pattern）是一种创建对象的设计模式，它通过复制现有的对象来创建新对象，而不是通过类的实例化。原型模式适用于对象创建成本较高或复杂的情况，可以通过克隆现有对象来节省创建时间和资源。

// 示例
// 基础原型模式

// cpp
// 复制代码
#include <iostream>
#include <memory>
#include <string>

// 抽象原型类
class Prototype {
public:
    virtual ~Prototype() = default;
    virtual std::unique_ptr<Prototype> clone() const = 0;
    virtual void show() const = 0;
};

// 具体原型类
class ConcretePrototype : public Prototype {
public:
    ConcretePrototype(const std::string& data) : data(data) {}

    std::unique_ptr<Prototype> clone() const override {
        return std::make_unique<ConcretePrototype>(*this);
    }

    void show() const override {
        std::cout << "ConcretePrototype with data: " << data << std::endl;
    }

private:
    std::string data;
};

int main() {
    // 创建原型对象
    ConcretePrototype original("Original");

    // 克隆原型对象
    std::unique_ptr<Prototype> clone = original.clone();

    // 显示两个对象
    original.show();
    clone->show();

    return 0;
}
// 在这个例子中，Prototype 类是抽象的原型接口，定义了 clone 方法。ConcretePrototype 类是具体的原型实现类，负责克隆自身。

// 带有复杂对象的原型模式

// cpp
// 复制代码
#include <iostream>
#include <memory>
#include <vector>

// 抽象原型类
class Prototype {
public:
    virtual ~Prototype() = default;
    virtual std::unique_ptr<Prototype> clone() const = 0;
    virtual void show() const = 0;
};

// 具体原型类
class ComplexPrototype : public Prototype {
public:
    ComplexPrototype(std::vector<int> data) : data(std::move(data)) {}

    std::unique_ptr<Prototype> clone() const override {
        return std::make_unique<ComplexPrototype>(*this);
    }

    void show() const override {
        std::cout << "ComplexPrototype with data: ";
        for (int num : data) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }

private:
    std::vector<int> data;
};

int main() {
    // 创建原型对象
    ComplexPrototype original({1, 2, 3, 4, 5});

    // 克隆原型对象
    std::unique_ptr<Prototype> clone = original.clone();

    // 显示两个对象
    original.show();
    clone->show();

    return 0;
}
// 在这个例子中，ComplexPrototype 类包含一个复杂的数据成员（std::vector<int>），并实现了 clone 方法来复制自身。

// 问题
// 原型模式的优点是什么？

// 原型模式通过复制现有对象而不是重新创建，减少了对象创建的开销，特别适合对象的创建成本高或复杂时。它简化了创建过程，并允许对象的配置在运行时进行。
// 在什么情况下应该使用原型模式？

// 当对象的创建成本较高时，尤其是当需要多个具有相似属性的对象时，原型模式可以有效节省创建成本。此外，当对象的配置在运行时需要动态调整时，原型模式也很有用。
// 如何处理深拷贝和浅拷贝？

// 深拷贝和浅拷贝的选择取决于对象的复杂性。如果对象内部包含指针或动态分配的资源，需要实现深拷贝，以确保新对象的资源独立于原对象。浅拷贝仅复制指针而不复制指针指向的对象，可能导致多个对象共享相同的资源，从而产生问题。
// 原型模式与工厂模式有什么区别？

// 原型模式通过复制现有对象来创建新对象，而工厂模式通过创建类的实例来创建新对象。原型模式适用于需要克隆现有对象的场景，而工厂模式适用于需要通过创建新的类实例来生成对象的场景。
// 这些示例和问题应帮助你更好地理解原型模式。如果有更多问题或需要进一步探讨，请随时告诉我！

// 适配器
// ChatGPT
// 适配器模式（Adapter Pattern）是一种结构型设计模式，用于将一个类的接口转换成客户期望的另一种接口。适配器模式可以让原本由于接口不兼容而无法一起工作的类一起工作，主要用于解决现有系统与新系统的接口不匹配问题。

// 示例
// 对象适配器模式

// cpp
// 复制代码
#include <iostream>
#include <memory>

// 目标接口
class Target {
public:
    virtual void request() const {
        std::cout << "Target request" << std::endl;
    }
    virtual ~Target() = default;
};

// 被适配者类
class Adaptee {
public:
    void specificRequest() const {
        std::cout << "Adaptee specific request" << std::endl;
    }
};

// 适配器类
class Adapter : public Target {
public:
    Adapter(std::unique_ptr<Adaptee> adaptee) : adaptee(std::move(adaptee)) {}

    void request() const override {
        adaptee->specificRequest();
    }

private:
    std::unique_ptr<Adaptee> adaptee;
};

int main() {
    auto adaptee = std::make_unique<Adaptee>();
    std::unique_ptr<Target> adapter = std::make_unique<Adapter>(std::move(adaptee));
    adapter->request();

    return 0;
}
// 在这个例子中，Target 是客户端期望的接口，Adaptee 是现有的但不兼容的接口。Adapter 类实现了 Target 接口，并在其 request 方法中调用了 Adaptee 的 specificRequest 方法，以适配客户端的请求。

// 类适配器模式

// cpp
// 复制代码
#include <iostream>

// 目标接口
class Target {
public:
    virtual void request() const {
        std::cout << "Target request" << std::endl;
    }
    virtual ~Target() = default;
};

// 被适配者类
class Adaptee {
public:
    void specificRequest() const {
        std::cout << "Adaptee specific request" << std::endl;
    }
};

// 适配器类
class Adapter : public Target, private Adaptee {
public:
    void request() const override {
        specificRequest();
    }
};

int main() {
    Adapter adapter;
    adapter.request();

    return 0;
}
// 在这个例子中，Adapter 类同时继承了 Target 和 Adaptee 类。Adapter 实现了 Target 的 request 方法，并在其中调用了 Adaptee 的 specificRequest 方法。这种方法使用类继承来实现适配器。

// 问题
// 适配器模式的优点是什么？

// 适配器模式允许类的接口与客户期望的接口不匹配时，让两个不兼容的接口协同工作。它促进了现有代码的复用，并提供了一种灵活的方式来集成不兼容的系统。
// 在什么情况下应该使用适配器模式？

// 当你需要将一个类的接口转换成客户端期望的接口，或将多个不兼容的接口整合成一个一致的接口时，适配器模式特别有用。这通常发生在系统集成和对现有系统进行改造时。
// 如何选择使用对象适配器还是类适配器？

// 对象适配器通过组合而不是继承来实现适配，这使得它在运行时更灵活，适合处理复杂的适配情况。类适配器通过继承来实现适配，适用于在编译时就确定了适配关系的简单情况。
// 适配器模式与代理模式有什么区别？

// 适配器模式用于解决接口不兼容的问题，使得不同的接口能够协同工作。而代理模式用于控制对对象的访问，可以用于延迟初始化、访问控制或记录操作等。
// 这些示例和问题应帮助你更好地理解适配器模式。如果有更多问题或需要进一步探讨，请随时告诉我！

// 桥接
// ChatGPT
// 桥接模式（Bridge Pattern）是一种结构型设计模式，用于将抽象部分与实现部分分离，使它们可以独立变化。它通过引入桥接接口，将抽象层与实现层分开，从而实现灵活的组合和扩展。

// 示例
// 基础桥接模式

// cpp
// 复制代码
#include <iostream>
#include <memory>

// 实现接口
class Implementor {
public:
    virtual void operationImpl() const = 0;
    virtual ~Implementor() = default;
};

// 具体实现类
class ConcreteImplementorA : public Implementor {
public:
    void operationImpl() const override {
        std::cout << "ConcreteImplementorA operation" << std::endl;
    }
};

class ConcreteImplementorB : public Implementor {
public:
    void operationImpl() const override {
        std::cout << "ConcreteImplementorB operation" << std::endl;
    }
};

// 抽象类
class Abstraction {
public:
    Abstraction(std::unique_ptr<Implementor> impl) : implementor(std::move(impl)) {}
    virtual void operation() const {
        implementor->operationImpl();
    }
    virtual ~Abstraction() = default;

private:
    std::unique_ptr<Implementor> implementor;
};

// 扩展抽象类
class RefinedAbstraction : public Abstraction {
public:
    RefinedAbstraction(std::unique_ptr<Implementor> impl) : Abstraction(std::move(impl)) {}
    void operation() const override {
        std::cout << "RefinedAbstraction: ";
        Abstraction::operation();
    }
};

int main() {
    auto implementorA = std::make_unique<ConcreteImplementorA>();
    Abstraction abstractionA(std::move(implementorA));
    abstractionA.operation();

    auto implementorB = std::make_unique<ConcreteImplementorB>();
    RefinedAbstraction abstractionB(std::move(implementorB));
    abstractionB.operation();

    return 0;
}
// 在这个例子中，Implementor 是实现接口，ConcreteImplementorA 和 ConcreteImplementorB 是具体实现类。Abstraction 类使用 Implementor，并提供抽象方法。RefinedAbstraction 是扩展的抽象类，重写了 operation 方法。

// 复杂桥接模式

// cpp
// 复制代码
#include <iostream>
#include <memory>

// 实现接口
class DrawingAPI {
public:
    virtual void drawCircle(double x, double y, double radius) const = 0;
    virtual ~DrawingAPI() = default;
};

// 具体实现类
class DrawingAPI1 : public DrawingAPI {
public:
    void drawCircle(double x, double y, double radius) const override {
        std::cout << "DrawingAPI1: Circle at (" << x << ", " << y << ") with radius " << radius << std::endl;
    }
};

class DrawingAPI2 : public DrawingAPI {
public:
    void drawCircle(double x, double y, double radius) const override {
        std::cout << "DrawingAPI2: Circle at (" << x << ", " << y << ") with radius " << radius << std::endl;
    }
};

// 抽象类
class Shape {
public:
    Shape(std::unique_ptr<DrawingAPI> api) : drawingAPI(std::move(api)) {}
    virtual void draw() const = 0;
    virtual void resizeByPercentage(double pct) = 0;
    virtual ~Shape() = default;

protected:
    std::unique_ptr<DrawingAPI> drawingAPI;
};

// 具体抽象类
class Circle : public Shape {
public:
    Circle(double x, double y, double radius, std::unique_ptr<DrawingAPI> api)
        : Shape(std::move(api)), x(x), y(y), radius(radius) {}

    void draw() const override {
        drawingAPI->drawCircle(x, y, radius);
    }

    void resizeByPercentage(double pct) override {
        radius *= (1 + pct / 100.0);
    }

private:
    double x, y, radius;
};

int main() {
    auto api1 = std::make_unique<DrawingAPI1>();
    Circle circle1(5, 10, 7, std::move(api1));
    circle1.draw();
    circle1.resizeByPercentage(10);
    circle1.draw();

    auto api2 = std::make_unique<DrawingAPI2>();
    Circle circle2(15, 20, 5, std::move(api2));
    circle2.draw();
    circle2.resizeByPercentage(20);
    circle2.draw();

    return 0;
}
// 在这个例子中，DrawingAPI 是实现接口，DrawingAPI1 和 DrawingAPI2 是具体实现类。Shape 是抽象类，定义了抽象方法。Circle 是具体的抽象类，利用桥接接口进行绘制。

// 问题
// 桥接模式的优点是什么？

// 桥接模式将抽象部分与实现部分分离，使得两者可以独立变化。这种分离减少了代码的耦合，提高了系统的灵活性和可维护性，支持多种扩展方式。
// 在什么情况下应该使用桥接模式？

// 当你需要在不改变抽象接口的情况下对实现进行多种变化时，或者当系统需要处理多个变化的维度（如不同的图形和不同的绘制方式）时，桥接模式特别有用。
// 如何扩展桥接模式以支持更多的抽象和实现？

// 只需扩展抽象层和实现层。添加新的抽象类和实现类，然后在抽象类中使用新的实现类。这样可以通过不同的组合实现新的功能。
// 桥接模式与适配器模式有什么区别？

// 桥接模式用于将抽象部分与实现部分分离，使它们可以独立变化，而适配器模式用于将不兼容的接口适配到客户端期望的接口。桥接模式关注于抽象和实现的分离，适配器模式关注于接口的兼容性。
// 这些示例和问题应帮助你更好地理解桥接模式。如果有更多问题或需要进一步探讨，请随时告诉我！

// 组合
// ChatGPT
// 组合模式（Composite Pattern）是一种结构型设计模式，用于将对象组合成树形结构以表示部分-整体层次结构。组合模式允许客户端以统一的方式对待单个对象和对象集合，从而简化了处理复杂的对象结构。

// 示例
// 基础组合模式

// cpp
// 复制代码
#include <iostream>
#include <vector>
#include <memory>

// 组件接口
class Component {
public:
    virtual void operation() const = 0;
    virtual ~Component() = default;
};

// 叶子节点
class Leaf : public Component {
public:
    void operation() const override {
        std::cout << "Leaf operation" << std::endl;
    }
};

// 组合节点
class Composite : public Component {
public:
    void add(std::unique_ptr<Component> component) {
        children.push_back(std::move(component));
    }

    void operation() const override {
        std::cout << "Composite operation" << std::endl;
        for (const auto& child : children) {
            child->operation();
        }
    }

private:
    std::vector<std::unique_ptr<Component>> children;
};

int main() {
    auto leaf1 = std::make_unique<Leaf>();
    auto leaf2 = std::make_unique<Leaf>();
    auto composite = std::make_unique<Composite>();

    composite->add(std::move(leaf1));
    composite->add(std::move(leaf2));

    composite->operation();

    return 0;
}
// 在这个例子中，Component 是组件接口，Leaf 是具体的叶子节点，Composite 是组合节点，它可以包含多个子组件（叶子或组合）。Composite 的 operation 方法会遍历并调用所有子组件的 operation 方法。

// 复杂组合模式

// cpp
// 复制代码
#include <iostream>
#include <vector>
#include <memory>
#include <string>

// 组件接口
class FileSystemComponent {
public:
    virtual void ls(int depth = 0) const = 0;
    virtual ~FileSystemComponent() = default;
};

// 叶子节点
class File : public FileSystemComponent {
public:
    File(const std::string& name) : name(name) {}

    void ls(int depth = 0) const override {
        std::cout << std::string(depth, '-') << name << std::endl;
    }

private:
    std::string name;
};

// 组合节点
class Directory : public FileSystemComponent {
public:
    Directory(const std::string& name) : name(name) {}

    void add(std::unique_ptr<FileSystemComponent> component) {
        components.push_back(std::move(component));
    }

    void ls(int depth = 0) const override {
        std::cout << std::string(depth, '-') << name << std::endl;
        for (const auto& component : components) {
            component->ls(depth + 2);
        }
    }

private:
    std::vector<std::unique_ptr<FileSystemComponent>> components;
    std::string name;
};

int main() {
    auto file1 = std::make_unique<File>("file1.txt");
    auto file2 = std::make_unique<File>("file2.txt");
    auto directory = std::make_unique<Directory>("directory");

    directory->add(std::move(file1));
    directory->add(std::move(file2));

    auto root = std::make_unique<Directory>("root");
    root->add(std::move(directory));

    root->ls();

    return 0;
}
// 在这个例子中，FileSystemComponent 是组件接口，File 是具体的文件类（叶子节点），Directory 是目录类（组合节点）。Directory 可以包含多个文件或目录，并以层次结构显示文件系统。

// 问题
// 组合模式的优点是什么？

// 组合模式简化了处理复杂树形结构的代码，使得客户端可以一致地对待单个对象和对象集合。它提高了系统的灵活性和扩展性，允许轻松地添加新类型的组件。
// 在什么情况下应该使用组合模式？

// 当你需要处理部分-整体层次结构时，组合模式特别有用。例如，图形系统中的树形结构（如图形和图层）、文件系统中的文件和目录等场景都适合使用组合模式。
// 如何扩展组合模式以支持更多的组件类型？

// 只需添加新的组件类并使其继承自组件接口。组合节点可以容纳这些新组件，客户端代码不需要改变，因为它们可以一致地处理所有组件。
// 组合模式与装饰模式有什么区别？

// 组合模式主要用于处理树形结构中的部分-整体关系，使客户端可以一致地对待单个对象和对象集合。装饰模式用于动态地为对象添加额外的功能，而不改变其接口。组合模式关注于结构，而装饰模式关注于行为的增强。
// 这些示例和问题应帮助你更好地理解组合模式。如果有更多问题或需要进一步探讨，请随时告诉我！

// 装饰
// ChatGPT
// 装饰模式（Decorator Pattern）是一种结构型设计模式，用于动态地为对象添加额外的功能，而不改变对象的接口。装饰模式允许你通过创建装饰类来包装现有对象，并在运行时增加或修改其行为，从而增强其功能。

// 示例
// 基础装饰模式

// cpp
// 复制代码
#include <iostream>
#include <memory>

// 组件接口
class Coffee {
public:
    virtual double cost() const = 0;
    virtual ~Coffee() = default;
};

// 具体组件
class SimpleCoffee : public Coffee {
public:
    double cost() const override {
        return 5.0; // 基础咖啡的价格
    }
};

// 装饰器基类
class CoffeeDecorator : public Coffee {
public:
    CoffeeDecorator(std::unique_ptr<Coffee> coffee) : coffee(std::move(coffee)) {}
    virtual ~CoffeeDecorator() = default;

protected:
    std::unique_ptr<Coffee> coffee;
};

// 具体装饰器
class MilkDecorator : public CoffeeDecorator {
public:
    MilkDecorator(std::unique_ptr<Coffee> coffee) : CoffeeDecorator(std::move(coffee)) {}

    double cost() const override {
        return coffee->cost() + 2.0; // 加奶的价格
    }
};

class SugarDecorator : public CoffeeDecorator {
public:
    SugarDecorator(std::unique_ptr<Coffee> coffee) : CoffeeDecorator(std::move(coffee)) {}

    double cost() const override {
        return coffee->cost() + 1.0; // 加糖的价格
    }
};

int main() {
    auto coffee = std::make_unique<SimpleCoffee>();
    std::cout << "Cost of simple coffee: " << coffee->cost() << std::endl;

    auto milkCoffee = std::make_unique<MilkDecorator>(std::move(coffee));
    std::cout << "Cost of milk coffee: " << milkCoffee->cost() << std::endl;

    auto sugarMilkCoffee = std::make_unique<SugarDecorator>(std::move(milkCoffee));
    std::cout << "Cost of sugar milk coffee: " << sugarMilkCoffee->cost() << std::endl;

    return 0;
}
// 在这个例子中，Coffee 是组件接口，SimpleCoffee 是具体的组件类。CoffeeDecorator 是装饰器基类，它持有一个 Coffee 对象并可以扩展功能。MilkDecorator 和 SugarDecorator 是具体的装饰器，实现了额外的功能（如加奶和加糖）。

// 复杂装饰模式

// cpp
// 复制代码
#include <iostream>
#include <memory>
#include <string>

// 组件接口
class Text {
public:
    virtual std::string getContent() const = 0;
    virtual ~Text() = default;
};

// 具体组件
class SimpleText : public Text {
public:
    std::string getContent() const override {
        return "Hello, world!";
    }
};

// 装饰器基类
class TextDecorator : public Text {
public:
    TextDecorator(std::unique_ptr<Text> text) : text(std::move(text)) {}
    virtual ~TextDecorator() = default;

protected:
    std::unique_ptr<Text> text;
};

// 具体装饰器
class UppercaseDecorator : public TextDecorator {
public:
    UppercaseDecorator(std::unique_ptr<Text> text) : TextDecorator(std::move(text)) {}

    std::string getContent() const override {
        std::string content = text->getContent();
        for (char& c : content) {
            c = toupper(c);
        }
        return content;
    }
};

class ExclamationDecorator : public TextDecorator {
public:
    ExclamationDecorator(std::unique_ptr<Text> text) : TextDecorator(std::move(text)) {}

    std::string getContent() const override {
        return text->getContent() + "!!!";
    }
};

int main() {
    auto text = std::make_unique<SimpleText>();
    std::cout << "Simple text: " << text->getContent() << std::endl;

    auto uppercaseText = std::make_unique<UppercaseDecorator>(std::move(text));
    std::cout << "Uppercase text: " << uppercaseText->getContent() << std::endl;

    auto exclamationText = std::make_unique<ExclamationDecorator>(std::move(uppercaseText));
    std::cout << "Exclamation text: " << exclamationText->getContent() << std::endl;

    return 0;
}
// 在这个例子中，Text 是组件接口，SimpleText 是具体的文本类。TextDecorator 是装饰器基类，UppercaseDecorator 和 ExclamationDecorator 是具体的装饰器，分别实现了将文本转换为大写和添加感叹号的功能。

// 问题
// 装饰模式的优点是什么？

// 装饰模式允许动态地添加对象的功能，而不改变对象的接口。它支持功能的递增添加，避免了子类的过度创建，从而增加了系统的灵活性和可扩展性。
// 在什么情况下应该使用装饰模式？

// 当你需要在运行时动态地为对象添加或修改功能时，装饰模式特别有用。它适用于需要扩展对象功能而不改变其接口或产生大量子类的场景。
// 如何管理多个装饰器的顺序？

// 装饰器的顺序会影响功能的应用顺序。通常情况下，装饰器按照应用顺序叠加功能，最外层的装饰器最先处理请求。管理顺序可以通过装饰器的构造顺序来实现。
// 装饰模式与继承有什么区别？

// 装饰模式通过对象组合来动态添加功能，而继承在编译时就确定了功能。装饰模式更灵活，允许在运行时组合不同的功能，而继承在编译时需要创建新的子类，扩展性较差。
// 这些示例和问题应帮助你更好地理解装饰模式。如果有更多问题或需要进一步探讨，请随时告诉我！

// 外观
// ChatGPT
// 外观模式（Facade Pattern）是一种结构型设计模式，用于提供一个简化的接口，使得复杂子系统的使用变得更加简单。它通过定义一个统一的高层接口，使得客户端可以更容易地与复杂的子系统交互，而不需要直接处理子系统的复杂性。

// 示例
// 基础外观模式

// cpp
// 复制代码
#include <iostream>

// 子系统类A
class SubsystemA {
public:
    void operationA() const {
        std::cout << "SubsystemA: Operation A" << std::endl;
    }
};

// 子系统类B
class SubsystemB {
public:
    void operationB() const {
        std::cout << "SubsystemB: Operation B" << std::endl;
    }
};

// 子系统类C
class SubsystemC {
public:
    void operationC() const {
        std::cout << "SubsystemC: Operation C" << std::endl;
    }
};

// 外观类
class Facade {
public:
    Facade() 
        : subsystemA(), subsystemB(), subsystemC() {}

    void simpleOperation() const {
        subsystemA.operationA();
        subsystemB.operationB();
        subsystemC.operationC();
    }

private:
    SubsystemA subsystemA;
    SubsystemB subsystemB;
    SubsystemC subsystemC;
};

int main() {
    Facade facade;
    facade.simpleOperation();

    return 0;
}
// 在这个例子中，SubsystemA、SubsystemB 和 SubsystemC 是复杂的子系统类。Facade 类提供了一个简化的接口 simpleOperation，通过它可以调用所有子系统的操作，从而简化了客户端对这些复杂子系统的操作。

// 复杂外观模式

// cpp
// 复制代码
#include <iostream>
#include <string>

// 子系统类
class CPU {
public:
    void freeze() const {
        std::cout << "CPU freezing" << std::endl;
    }
    void jump(const std::string& position) const {
        std::cout << "CPU jumping to " << position << std::endl;
    }
    void execute() const {
        std::cout << "CPU executing" << std::endl;
    }
};

class Memory {
public:
    void load(const std::string& position, const std::string& data) const {
        std::cout << "Memory loading data at " << position << std::endl;
    }
};

class HardDrive {
public:
    std::string read(const std::string& position, int size) const {
        return "data from " + position;
    }
};

// 外观类
class ComputerFacade {
public:
    ComputerFacade() : cpu(), memory(), hardDrive() {}

    void startComputer() const {
        cpu.freeze();
        memory.load("0x00", hardDrive.read("0x00", 1024));
        cpu.jump("0x00");
        cpu.execute();
    }

private:
    CPU cpu;
    Memory memory;
    HardDrive hardDrive;
};

int main() {
    ComputerFacade computer;
    computer.startComputer();

    return 0;
}
// 在这个例子中，CPU、Memory 和 HardDrive 是复杂的子系统类。ComputerFacade 类提供了一个简化的接口 startComputer，通过它可以简化启动计算机的过程。

// 问题
// 外观模式的优点是什么？

// 外观模式简化了复杂子系统的使用，使得客户端可以通过一个统一的接口与子系统交互，而不需要了解子系统的内部细节。它提高了代码的可维护性和可读性，并降低了系统的复杂度。
// 在什么情况下应该使用外观模式？

// 当一个系统由多个复杂的子系统组成，并且你希望提供一个简单的接口来简化客户端与这些子系统的交互时，外观模式特别有用。它适用于需要整合多个类或模块以提供统一接口的场景。
// 如何扩展外观模式以支持更多的功能？

// 只需在外观类中添加新的方法，调用新功能所需的子系统方法。外观类可以随着需求的增加而扩展，以支持更多的操作和子系统功能。
// 外观模式与适配器模式有什么区别？

// 外观模式旨在简化复杂子系统的使用，通过提供一个统一的接口来简化与多个子系统的交互。而适配器模式用于将不兼容的接口转换为客户端期望的接口，适配器模式通常用于桥接不兼容的接口，而外观模式则用于简化和统一接口。
// 这些示例和问题应帮助你更好地理解外观模式。如果有更多问题或需要进一步探讨，请随时告诉我！

// 享元
// ChatGPT
// 享元模式（Flyweight Pattern）是一种结构型设计模式，用于通过共享尽可能多的对象来提高效率和减少内存使用。享元模式通过将对象的状态分为内部状态（可以共享）和外部状态（不能共享），来减少内存的占用，并提高性能。

// 示例
// 基础享元模式

// cpp
// 复制代码
#include <iostream>
#include <memory>
#include <unordered_map>
#include <string>

// 享元接口
class Flyweight {
public:
    virtual void operation(const std::string& externalState) const = 0;
    virtual ~Flyweight() = default;
};

// 具体享元
class ConcreteFlyweight : public Flyweight {
public:
    ConcreteFlyweight(const std::string& intrinsicState) : intrinsicState(intrinsicState) {}

    void operation(const std::string& externalState) const override {
        std::cout << "ConcreteFlyweight: Intrinsic State = " << intrinsicState
                  << ", External State = " << externalState << std::endl;
    }

private:
    std::string intrinsicState;
};

// 享元工厂
class FlyweightFactory {
public:
    std::shared_ptr<Flyweight> getFlyweight(const std::string& intrinsicState) {
        auto it = flyweights.find(intrinsicState);
        if (it != flyweights.end()) {
            return it->second;
        } else {
            auto flyweight = std::make_shared<ConcreteFlyweight>(intrinsicState);
            flyweights[intrinsicState] = flyweight;
            return flyweight;
        }
    }

private:
    std::unordered_map<std::string, std::shared_ptr<Flyweight>> flyweights;
};

int main() {
    FlyweightFactory factory;

    auto flyweight1 = factory.getFlyweight("State1");
    auto flyweight2 = factory.getFlyweight("State2");
    auto flyweight3 = factory.getFlyweight("State1"); // Shared with flyweight1

    flyweight1->operation("External1");
    flyweight2->operation("External2");
    flyweight3->operation("External3"); // Shares intrinsic state with flyweight1

    return 0;
}
// 在这个例子中，Flyweight 是享元接口，ConcreteFlyweight 是具体的享元类，它具有内部状态 intrinsicState 和外部状态 externalState。FlyweightFactory 是享元工厂，它负责创建和管理享元对象，确保共享相同的内部状态。客户端通过工厂获取享元对象，并使用它们进行操作。

// 复杂享元模式

// cpp
// 复制代码
#include <iostream>
#include <unordered_map>
#include <memory>
#include <vector>

// 享元接口
class TreeType {
public:
    virtual void draw(int x, int y) const = 0;
    virtual ~TreeType() = default;
};

// 具体享元
class TreeTypeImpl : public TreeType {
public:
    TreeTypeImpl(const std::string& color, const std::string& name)
        : color(color), name(name) {}

    void draw(int x, int y) const override {
        std::cout << "TreeType: " << name << " (" << color << ") at (" << x << ", " << y << ")" << std::endl;
    }

private:
    std::string color;
    std::string name;
};

// 享元工厂
class TreeFactory {
public:
    std::shared_ptr<TreeType> getTreeType(const std::string& color, const std::string& name) {
        std::string key = color + "-" + name;
        auto it = treeTypes.find(key);
        if (it != treeTypes.end()) {
            return it->second;
        } else {
            auto treeType = std::make_shared<TreeTypeImpl>(color, name);
            treeTypes[key] = treeType;
            return treeType;
        }
    }

private:
    std::unordered_map<std::string, std::shared_ptr<TreeType>> treeTypes;
};

// 客户端代码
class Forest {
public:
    void plantTree(const std::string& color, const std::string& name, int x, int y) {
        auto treeType = factory.getTreeType(color, name);
        trees.emplace_back(treeType, x, y);
    }

    void draw() const {
        for (const auto& tree : trees) {
            tree.first->draw(tree.second, tree.third);
        }
    }

private:
    TreeFactory factory;
    std::vector<std::tuple<std::shared_ptr<TreeType>, int, int>> trees;
};

int main() {
    Forest forest;
    forest.plantTree("Green", "Oak", 10, 20);
    forest.plantTree("Green", "Pine", 30, 40);
    forest.plantTree("Red", "Maple", 50, 60);
    forest.plantTree("Green", "Oak", 70, 80); // Shares tree type with the first Oak

    forest.draw();

    return 0;
}
// 在这个例子中，TreeType 是享元接口，TreeTypeImpl 是具体的享元类，包含树的颜色和名字。TreeFactory 负责管理和共享树的类型。Forest 类表示森林，它使用 TreeFactory 创建树并存储位置。

// 问题
// 享元模式的优点是什么？

// 享元模式通过共享对象来减少内存使用，从而提高系统性能。它允许多个对象共享相同的状态，减少对象的创建和内存占用，适用于需要大量对象的场景。
// 在什么情况下应该使用享元模式？

// 当系统中存在大量相似的对象，并且这些对象占用大量内存时，享元模式特别有用。它适用于需要减少内存使用和提高性能的场景，如游戏开发中的大量相似对象、图形系统中的大量图形元素等。
// 如何实现享元模式中的内部状态和外部状态的分离？

// 内部状态是对象在不同上下文中共享的部分，而外部状态是对象在特定上下文中独有的部分。内部状态可以由享元类存储和管理，而外部状态可以作为参数传递给享元类的方法。
// 享元模式与代理模式有什么区别？

// 享元模式用于共享对象以节省内存和提高性能，而代理模式用于控制对对象的访问。享元模式关注于对象的共享和优化内存使用，而代理模式关注于对对象访问的控制和延迟加载。
// 这些示例和问题应帮助你更好地理解享元模式。如果有更多问题或需要进一步探讨，请随时告诉我！

// 代理
// ChatGPT
// 代理模式（Proxy Pattern）是一种结构型设计模式，用于为其他对象提供一种代理以控制对这个对象的访问。代理模式通过引入一个代理对象来控制对真实对象的访问，可以用来实现懒加载、权限控制、日志记录等功能。

// 示例
// 基础代理模式

// cpp
// 复制代码
#include <iostream>
#include <memory>

// 抽象主题接口
class Subject {
public:
    virtual void request() const = 0;
    virtual ~Subject() = default;
};

// 真实主题
class RealSubject : public Subject {
public:
    void request() const override {
        std::cout << "RealSubject: Handling request." << std::endl;
    }
};

// 代理
class Proxy : public Subject {
public:
    Proxy() : realSubject(std::make_unique<RealSubject>()) {}

    void request() const override {
        std::cout << "Proxy: Pre-processing request." << std::endl;
        realSubject->request();
        std::cout << "Proxy: Post-processing request." << std::endl;
    }

private:
    mutable std::unique_ptr<RealSubject> realSubject;
};

int main() {
    Proxy proxy;
    proxy.request();

    return 0;
}
// 在这个例子中，Subject 是抽象主题接口，RealSubject 是具体的真实主题，实现了实际的业务逻辑。Proxy 是代理类，它持有一个 RealSubject 对象，并在调用 request 方法时增加了额外的功能（前置处理和后置处理）。

// 虚拟代理（懒加载）

// cpp
// 复制代码
#include <iostream>
#include <memory>

// 抽象主题接口
class Image {
public:
    virtual void display() const = 0;
    virtual ~Image() = default;
};

// 真实主题
class RealImage : public Image {
public:
    RealImage(const std::string& filename) : filename(filename) {
        loadImage();
    }

    void display() const override {
        std::cout << "Displaying " << filename << std::endl;
    }

private:
    void loadImage() const {
        std::cout << "Loading " << filename << std::endl;
    }

    std::string filename;
};

// 虚拟代理
class ProxyImage : public Image {
public:
    ProxyImage(const std::string& filename) : filename(filename), realImage(nullptr) {}

    void display() const override {
        if (!realImage) {
            realImage = std::make_unique<RealImage>(filename);
        }
        realImage->display();
    }

private:
    mutable std::unique_ptr<RealImage> realImage;
    std::string filename;
};

int main() {
    ProxyImage proxyImage("photo.jpg");
    // 图像尚未加载
    proxyImage.display(); // 图像将被加载并显示
    proxyImage.display(); // 图像将直接显示（已经加载）

    return 0;
}
// 在这个例子中，RealImage 是真实主题，负责实际的图像加载和显示。ProxyImage 是虚拟代理，负责延迟加载 RealImage 对象，只有在实际调用 display 方法时才会加载图像。

// 保护代理（权限控制）

// cpp
// 复制代码
#include <iostream>
#include <memory>
#include <string>

// 抽象主题接口
class Document {
public:
    virtual void view(const std::string& user) const = 0;
    virtual ~Document() = default;
};

// 真实主题
class RealDocument : public Document {
public:
    RealDocument(const std::string& content) : content(content) {}

    void view(const std::string& user) const override {
        std::cout << "User " << user << " is viewing document: " << content << std::endl;
    }

private:
    std::string content;
};

// 保护代理
class ProtectedDocument : public Document {
public:
    ProtectedDocument(const std::string& content, const std::string& owner)
        : realDocument(content), owner(owner) {}

    void view(const std::string& user) const override {
        if (user == owner) {
            realDocument.view(user);
        } else {
            std::cout << "User " << user << " does not have permission to view this document." << std::endl;
        }
    }

private:
    RealDocument realDocument;
    std::string owner;
};

int main() {
    ProtectedDocument doc("Sensitive Document", "Alice");
    doc.view("Bob"); // Bob does not have permission
    doc.view("Alice"); // Alice has permission

    return 0;
}
// 在这个例子中，RealDocument 是真实主题，负责显示文档内容。ProtectedDocument 是保护代理，负责控制对 RealDocument 的访问，根据用户的权限决定是否允许访问。

// 问题
// 代理模式的优点是什么？

// 代理模式通过提供对真实对象的间接访问，可以控制访问权限、增加额外功能（如懒加载、缓存、日志记录等）。它可以帮助管理对真实对象的访问，降低系统的复杂性，提高灵活性。
// 在什么情况下应该使用代理模式？

// 当你需要控制对真实对象的访问时，如权限控制、懒加载、缓存等，代理模式是非常有用的。它适用于需要对对象的访问进行管理或增加额外功能的场景。
// 如何选择合适的代理类型？

// 根据需要选择不同类型的代理：虚拟代理用于延迟对象的创建（懒加载），保护代理用于权限控制，缓存代理用于缓存数据，防火墙代理用于网络安全等。
// 代理模式与装饰模式有什么区别？

// 代理模式主要用于控制对真实对象的访问，可以实现懒加载、权限控制等功能。装饰模式用于动态地增加对象的功能，而不改变对象的接口。代理模式通常关注对象的访问控制，而装饰模式关注功能的扩展。
// 这些示例和问题应帮助你更好地理解代理模式。如果有更多问题或需要进一步探讨，请随时告诉我！

// 观察者
// ChatGPT
// 观察者模式（Observer Pattern）是一种行为型设计模式，用于定义对象之间的一对多依赖关系，以便当一个对象的状态发生变化时，所有依赖于它的对象都会得到通知并自动更新。这个模式常用于实现发布-订阅系统。

// 示例
// 基本观察者模式

// cpp
// 复制代码
#include <iostream>
#include <vector>
#include <memory>

// 观察者接口
class Observer {
public:
    virtual void update(int value) = 0;
    virtual ~Observer() = default;
};

// 具体观察者
class ConcreteObserver : public Observer {
public:
    ConcreteObserver(const std::string& name) : name(name) {}

    void update(int value) override {
        std::cout << "Observer " << name << " received value: " << value << std::endl;
    }

private:
    std::string name;
};

// 被观察者接口
class Subject {
public:
    virtual void addObserver(std::shared_ptr<Observer> observer) = 0;
    virtual void removeObserver(std::shared_ptr<Observer> observer) = 0;
    virtual void notifyObservers() = 0;
    virtual ~Subject() = default;
};

// 具体被观察者
class ConcreteSubject : public Subject {
public:
    void addObserver(std::shared_ptr<Observer> observer) override {
        observers.push_back(observer);
    }

    void removeObserver(std::shared_ptr<Observer> observer) override {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void notifyObservers() override {
        for (const auto& observer : observers) {
            observer->update(value);
        }
    }

    void setValue(int newValue) {
        value = newValue;
        notifyObservers();
    }

private:
    std::vector<std::shared_ptr<Observer>> observers;
    int value;
};

int main() {
    auto subject = std::make_shared<ConcreteSubject>();

    auto observer1 = std::make_shared<ConcreteObserver>("Observer1");
    auto observer2 = std::make_shared<ConcreteObserver>("Observer2");

    subject->addObserver(observer1);
    subject->addObserver(observer2);

    subject->setValue(10);
    subject->setValue(20);

    return 0;
}
// 在这个例子中，Observer 是观察者接口，ConcreteObserver 是具体的观察者。Subject 是被观察者接口，ConcreteSubject 是具体的被观察者，维护了一个观察者列表，并在状态变化时通知所有观察者。

// 观察者模式与事件系统结合

// cpp
// 复制代码
#include <iostream>
#include <vector>
#include <functional>

// 事件系统
class EventSystem {
public:
    using EventHandler = std::function<void(int)>;

    void subscribe(const EventHandler& handler) {
        handlers.push_back(handler);
    }

    void publish(int value) {
        for (const auto& handler : handlers) {
            handler(value);
        }
    }

private:
    std::vector<EventHandler> handlers;
};

int main() {
    EventSystem eventSystem;

    // 订阅事件
    eventSystem.subscribe([](int value) {
        std::cout << "Subscriber 1 received value: " << value << std::endl;
    });

    eventSystem.subscribe([](int value) {
        std::cout << "Subscriber 2 received value: " << value << std::endl;
    });

    // 发布事件
    eventSystem.publish(100);
    eventSystem.publish(200);

    return 0;
}
// 在这个例子中，EventSystem 实现了一个简单的事件系统，支持事件订阅和发布。使用 std::function 和 std::vector 管理事件处理函数，实现了观察者模式的变体。

// 问题
// 观察者模式的优点是什么？

// 观察者模式提供了一种松耦合的方式来实现对象之间的通信。当被观察者的状态发生变化时，所有依赖于它的观察者会自动更新，从而提高了系统的灵活性和可扩展性。
// 在什么情况下应该使用观察者模式？

// 当系统中存在多个对象需要响应某个对象的状态变化时，观察者模式特别有用。它适用于事件驱动系统、发布-订阅系统、UI组件和数据绑定等场景。
// 如何处理观察者模式中的性能问题？

// 如果观察者数量很多或更新频繁，可能会导致性能问题。可以考虑使用异步更新机制、事件合并、或在观察者的更新操作中进行优化来提高性能。
// 如何避免观察者模式中的循环依赖？

// 通过设计合理的通知机制，确保观察者不会直接修改被观察者的状态。确保观察者和被观察者之间的依赖关系是单向的，可以通过合理的设计来避免循环依赖的问题。
// 这些示例和问题应帮助你更好地理解观察者模式。如果有更多问题或需要进一步探讨，请随时告诉我！

// 责任链
// ChatGPT
// 责任链模式（Chain of Responsibility Pattern）是一种行为型设计模式，用于避免请求发送者和接收者之间的紧耦合关系。通过将请求沿着链传递，直到有对象处理它为止，从而实现对请求的动态处理。这个模式有助于降低请求处理者之间的依赖关系，提高系统的灵活性和可扩展性。

// 示例
// 基本责任链模式

// cpp
// 复制代码
#include <iostream>
#include <memory>

// 处理者接口
class Handler {
public:
    virtual void setNext(std::shared_ptr<Handler> nextHandler) = 0;
    virtual void handleRequest(int request) const = 0;
    virtual ~Handler() = default;
};

// 具体处理者A
class ConcreteHandlerA : public Handler {
public:
    void setNext(std::shared_ptr<Handler> nextHandler) override {
        next = nextHandler;
    }

    void handleRequest(int request) const override {
        if (request < 10) {
            std::cout << "Handler A handling request " << request << std::endl;
        } else if (next) {
            next->handleRequest(request);
        }
    }

private:
    std::shared_ptr<Handler> next;
};

// 具体处理者B
class ConcreteHandlerB : public Handler {
public:
    void setNext(std::shared_ptr<Handler> nextHandler) override {
        next = nextHandler;
    }

    void handleRequest(int request) const override {
        if (request >= 10 && request < 20) {
            std::cout << "Handler B handling request " << request << std::endl;
        } else if (next) {
            next->handleRequest(request);
        }
    }

private:
    std::shared_ptr<Handler> next;
};

// 具体处理者C
class ConcreteHandlerC : public Handler {
public:
    void setNext(std::shared_ptr<Handler> nextHandler) override {
        next = nextHandler;
    }

    void handleRequest(int request) const override {
        if (request >= 20) {
            std::cout << "Handler C handling request " << request << std::endl;
        } else if (next) {
            next->handleRequest(request);
        }
    }

private:
    std::shared_ptr<Handler> next;
};

int main() {
    auto handlerA = std::make_shared<ConcreteHandlerA>();
    auto handlerB = std::make_shared<ConcreteHandlerB>();
    auto handlerC = std::make_shared<ConcreteHandlerC>();

    handlerA->setNext(handlerB);
    handlerB->setNext(handlerC);

    handlerA->handleRequest(5);
    handlerA->handleRequest(15);
    handlerA->handleRequest(25);

    return 0;
}
// 在这个例子中，Handler 是处理者接口，ConcreteHandlerA、ConcreteHandlerB 和 ConcreteHandlerC 是具体的处理者，它们将请求传递给链中的下一个处理者，直到处理请求的条件满足。

// 责任链模式与日志记录

// cpp
// 复制代码
#include <iostream>
#include <memory>
#include <string>

// 日志级别
enum class LogLevel {
    INFO,
    WARNING,
    ERROR
};

// 处理者接口
class Logger {
public:
    virtual void setNext(std::shared_ptr<Logger> nextLogger) = 0;
    virtual void log(LogLevel level, const std::string& message) const = 0;
    virtual ~Logger() = default;
};

// 具体处理者：信息日志
class InfoLogger : public Logger {
public:
    void setNext(std::shared_ptr<Logger> nextLogger) override {
        next = nextLogger;
    }

    void log(LogLevel level, const std::string& message) const override {
        if (level == LogLevel::INFO) {
            std::cout << "INFO: " << message << std::endl;
        } else if (next) {
            next->log(level, message);
        }
    }

private:
    std::shared_ptr<Logger> next;
};

// 具体处理者：警告日志
class WarningLogger : public Logger {
public:
    void setNext(std::shared_ptr<Logger> nextLogger) override {
        next = nextLogger;
    }

    void log(LogLevel level, const std::string& message) const override {
        if (level == LogLevel::WARNING) {
            std::cout << "WARNING: " << message << std::endl;
        } else if (next) {
            next->log(level, message);
        }
    }

private:
    std::shared_ptr<Logger> next;
};

// 具体处理者：错误日志
class ErrorLogger : public Logger {
public:
    void setNext(std::shared_ptr<Logger> nextLogger) override {
        next = nextLogger;
    }

    void log(LogLevel level, const std::string& message) const override {
        if (level == LogLevel::ERROR) {
            std::cout << "ERROR: " << message << std::endl;
        } else if (next) {
            next->log(level, message);
        }
    }

private:
    std::shared_ptr<Logger> next;
};

int main() {
    auto infoLogger = std::make_shared<InfoLogger>();
    auto warningLogger = std::make_shared<WarningLogger>();
    auto errorLogger = std::make_shared<ErrorLogger>();

    infoLogger->setNext(warningLogger);
    warningLogger->setNext(errorLogger);

    infoLogger->log(LogLevel::INFO, "This is an info message.");
    infoLogger->log(LogLevel::WARNING, "This is a warning message.");
    infoLogger->log(LogLevel::ERROR, "This is an error message.");

    return 0;
}
// 在这个例子中，Logger 是处理者接口，InfoLogger、WarningLogger 和 ErrorLogger 是具体的日志处理者，根据日志级别处理不同的日志消息。

// 问题
// 责任链模式的优点是什么？

// 责任链模式通过解耦请求的发送者和处理者，使得请求的处理更加灵活。它允许在运行时动态地添加和修改处理者，支持请求的逐步处理，增加了系统的灵活性和扩展性。
// 在什么情况下应该使用责任链模式？

// 当一个请求需要通过多个处理者处理，且每个处理者都可能需要在链中进行不同的操作时，责任链模式特别有用。它适用于处理流程复杂、需要动态调整处理逻辑的场景，如事件处理、审批流程等。
// 如何设计责任链模式中的处理者链？

// 设计责任链时，需要明确每个处理者的职责，并确保每个处理者能够决定是否将请求传递给链中的下一个处理者。链的结构应当简洁明确，避免过于复杂的链条。
// 责任链模式与命令模式有什么区别？

// 责任链模式关注请求的传递和处理，处理者链中的每个处理者可以决定是否处理请求或将请求传递给下一个处理者。命令模式关注将请求封装为对象，使请求的发送者和接收者解耦，适用于请求的排队、记录和撤销操作等场景。
// 这些示例和问题应帮助你更好地理解责任链模式。如果有更多问题或需要进一步探讨，请随时告诉我！

// 策略
// ChatGPT
// 策略模式（Strategy Pattern）是一种行为型设计模式，用于定义一系列算法，将每一个算法封装起来，并使它们可以互相替换。策略模式使得算法的变化独立于使用算法的客户端。这个模式允许在运行时选择算法，从而实现了算法的灵活切换。

// 示例
// 基本策略模式

// cpp
// 复制代码
#include <iostream>
#include <memory>
#include <vector>
#include <functional>

// 策略接口
class Strategy {
public:
    virtual int execute(int a, int b) const = 0;
    virtual ~Strategy() = default;
};

// 具体策略A：加法
class ConcreteStrategyAdd : public Strategy {
public:
    int execute(int a, int b) const override {
        return a + b;
    }
};

// 具体策略B：减法
class ConcreteStrategySubtract : public Strategy {
public:
    int execute(int a, int b) const override {
        return a - b;
    }
};

// 上下文
class Context {
public:
    void setStrategy(std::shared_ptr<Strategy> strategy) {
        this->strategy = strategy;
    }

    int executeStrategy(int a, int b) const {
        return strategy->execute(a, b);
    }

private:
    std::shared_ptr<Strategy> strategy;
};

int main() {
    auto context = Context();

    // 使用加法策略
    context.setStrategy(std::make_shared<ConcreteStrategyAdd>());
    std::cout << "Addition: " << context.executeStrategy(5, 3) << std::endl;

    // 使用减法策略
    context.setStrategy(std::make_shared<ConcreteStrategySubtract>());
    std::cout << "Subtraction: " << context.executeStrategy(5, 3) << std::endl;

    return 0;
}
// 在这个例子中，Strategy 是策略接口，定义了执行算法的 execute 方法。ConcreteStrategyAdd 和 ConcreteStrategySubtract 是具体的策略，分别实现加法和减法操作。Context 是上下文，持有一个策略对象，并通过 executeStrategy 方法来调用策略的算法。

// 策略模式与支付方式

// cpp
// 复制代码
#include <iostream>
#include <memory>
#include <string>

// 策略接口
class PaymentStrategy {
public:
    virtual void pay(int amount) const = 0;
    virtual ~PaymentStrategy() = default;
};

// 具体策略A：信用卡支付
class CreditCardPayment : public PaymentStrategy {
public:
    CreditCardPayment(const std::string& cardNumber) : cardNumber(cardNumber) {}

    void pay(int amount) const override {
        std::cout << "Paying " << amount << " using Credit Card: " << cardNumber << std::endl;
    }

private:
    std::string cardNumber;
};

// 具体策略B：支付Pal支付
class PayPalPayment : public PaymentStrategy {
public:
    PayPalPayment(const std::string& email) : email(email) {}

    void pay(int amount) const override {
        std::cout << "Paying " << amount << " using PayPal account: " << email << std::endl;
    }

private:
    std::string email;
};

// 上下文
class ShoppingCart {
public:
    void setPaymentStrategy(std::shared_ptr<PaymentStrategy> paymentStrategy) {
        this->paymentStrategy = paymentStrategy;
    }

    void checkout(int amount) const {
        paymentStrategy->pay(amount);
    }

private:
    std::shared_ptr<PaymentStrategy> paymentStrategy;
};

int main() {
    auto cart = ShoppingCart();

    // 使用信用卡支付
    cart.setPaymentStrategy(std::make_shared<CreditCardPayment>("1234-5678-9876-5432"));
    cart.checkout(100);

    // 使用PayPal支付
    cart.setPaymentStrategy(std::make_shared<PayPalPayment>("user@example.com"));
    cart.checkout(200);

    return 0;
}
// 在这个例子中，PaymentStrategy 是支付策略接口，定义了 pay 方法。CreditCardPayment 和 PayPalPayment 是具体的支付策略，分别实现了信用卡支付和 PayPal 支付的逻辑。ShoppingCart 是上下文，持有一个支付策略对象，并通过 checkout 方法进行支付。

// 策略模式与排序算法

// cpp
// 复制代码
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

// 策略接口
class SortingStrategy {
public:
    virtual void sort(std::vector<int>& data) const = 0;
    virtual ~SortingStrategy() = default;
};

// 具体策略A：冒泡排序
class BubbleSort : public SortingStrategy {
public:
    void sort(std::vector<int>& data) const override {
        bool swapped;
        for (size_t i = 0; i < data.size() - 1; ++i) {
            swapped = false;
            for (size_t j = 0; j < data.size() - i - 1; ++j) {
                if (data[j] > data[j + 1]) {
                    std::swap(data[j], data[j + 1]);
                    swapped = true;
                }
            }
            if (!swapped) break;
        }
    }
};

// 具体策略B：标准库排序
class StandardSort : public SortingStrategy {
public:
    void sort(std::vector<int>& data) const override {
        std::sort(data.begin(), data.end());
    }
};

// 上下文
class SortedList {
public:
    void setSortingStrategy(std::shared_ptr<SortingStrategy> strategy) {
        this->strategy = strategy;
    }

    void sortData(std::vector<int>& data) const {
        strategy->sort(data);
    }

private:
    std::shared_ptr<SortingStrategy> strategy;
};

int main() {
    auto sortedList = SortedList();

    std::vector<int> data = {5, 2, 9, 1, 5, 6};

    // 使用冒泡排序策略
    sortedList.setSortingStrategy(std::make_shared<BubbleSort>());
    sortedList.sortData(data);

    std::cout << "Sorted data (BubbleSort): ";
    for (const auto& num : data) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // 使用标准库排序策略
    data = {5, 2, 9, 1, 5, 6};
    sortedList.setSortingStrategy(std::make_shared<StandardSort>());
    sortedList.sortData(data);

    std::cout << "Sorted data (StandardSort): ";
    for (const auto& num : data) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
// 在这个例子中，SortingStrategy 是排序策略接口，定义了 sort 方法。BubbleSort 和 StandardSort 是具体的排序策略，分别实现了冒泡排序和标准库排序。SortedList 是上下文，持有一个排序策略对象，并通过 sortData 方法进行排序。

// 问题
// 策略模式的优点是什么？

// 策略模式使得算法可以在运行时动态选择，并将算法的实现与使用它的代码解耦。它使得系统可以更加灵活地支持不同的算法或策略，易于扩展和维护。
// 在什么情况下应该使用策略模式？

// 当需要在运行时选择算法或策略时，策略模式非常有用。它适用于算法的变化频繁且需要在运行时进行切换的场景，例如支付方式、排序算法、压缩算法等。
// 如何设计策略模式中的策略接口？

// 策略接口应当定义算法或操作的通用接口，使得所有具体策略实现该接口。接口方法的参数和返回值应当足够通用，以支持不同的策略实现。
// 策略模式与状态模式有什么区别？

// 策略模式关注于在运行时切换算法或策略，允许动态选择算法。状态模式用于处理对象的状态变化，根据不同的状态执行不同的行为。策略模式通常关注行为的变化，而状态模式关注对象的状态和状态之间的转换。
// 这些示例和问题应帮助你更好地理解策略模式。如果有更多问题或需要进一步探讨，请随时告诉我！

// 模板
// ChatGPT
// 模板模式（Template Method Pattern）是一种行为型设计模式，用于定义一个操作的算法框架，并允许子类在不改变算法结构的情况下重新定义算法中的某些步骤。模板模式通过将一些步骤的实现推迟到子类中，提供了一种灵活的方式来扩展算法的行为。

// 示例
// 基本模板模式

// cpp
// 复制代码
#include <iostream>

// 抽象类
class CaffeineBeverage {
public:
    // 模板方法
    void prepareRecipe() const {
        boilWater();
        brew();
        pourInCup();
        addCondiments();
    }

    virtual ~CaffeineBeverage() = default;

protected:
    virtual void boilWater() const {
        std::cout << "Boiling water" << std::endl;
    }

    virtual void pourInCup() const {
        std::cout << "Pouring into cup" << std::endl;
    }

    virtual void addCondiments() const = 0;  // 必须由子类实现
    virtual void brew() const = 0;  // 必须由子类实现
};

// 具体子类：咖啡
class Coffee : public CaffeineBeverage {
protected:
    void addCondiments() const override {
        std::cout << "Adding sugar and milk" << std::endl;
    }

    void brew() const override {
        std::cout << "Dripping coffee through filter" << std::endl;
    }
};

// 具体子类：茶
class Tea : public CaffeineBeverage {
protected:
    void addCondiments() const override {
        std::cout << "Adding lemon" << std::endl;
    }

    void brew() const override {
        std::cout << "Steeping the tea" << std::endl;
    }
};

int main() {
    Coffee coffee;
    Tea tea;

    std::cout << "Making coffee:" << std::endl;
    coffee.prepareRecipe();

    std::cout << "\nMaking tea:" << std::endl;
    tea.prepareRecipe();

    return 0;
}
// 在这个例子中，CaffeineBeverage 是一个抽象类，定义了 prepareRecipe 模板方法。brew 和 addCondiments 是抽象方法，必须由具体子类实现。Coffee 和 Tea 是具体子类，实现了 brew 和 addCondiments 方法，提供了不同的行为。

// 模板模式与文档生成

// cpp
// 复制代码
#include <iostream>
#include <string>

// 抽象类
class DocumentTemplate {
public:
    void generateDocument() const {
        startDocument();
        addContent();
        endDocument();
    }

    virtual ~DocumentTemplate() = default;

protected:
    virtual void startDocument() const {
        std::cout << "Starting document" << std::endl;
    }

    virtual void endDocument() const {
        std::cout << "Ending document" << std::endl;
    }

    virtual void addContent() const = 0;  // 必须由子类实现
};

// 具体子类：报告
class Report : public DocumentTemplate {
protected:
    void addContent() const override {
        std::cout << "Adding report content" << std::endl;
    }
};

// 具体子类：信件
class Letter : public DocumentTemplate {
protected:
    void addContent() const override {
        std::cout << "Adding letter content" << std::endl;
    }
};

int main() {
    Report report;
    Letter letter;

    std::cout << "Generating report:" << std::endl;
    report.generateDocument();

    std::cout << "\nGenerating letter:" << std::endl;
    letter.generateDocument();

    return 0;
}
// 在这个例子中，DocumentTemplate 是一个抽象类，定义了 generateDocument 模板方法。addContent 是一个抽象方法，具体子类需要实现它。Report 和 Letter 是具体子类，提供了不同的文档内容。

// 问题
// 模板模式的优点是什么？

// 模板模式通过定义算法的框架并将某些步骤的实现推迟到子类中，从而复用通用的算法结构，同时允许子类根据需要重新定义或扩展某些步骤。它使得算法的变化对客户端透明，提高了代码的可维护性和复用性。
// 在什么情况下应该使用模板模式？

// 当你有一个算法的框架，其中一些步骤的实现是变化的，而其他步骤是固定的时，模板模式非常有用。它适用于那些需要保持算法结构不变，但又希望在某些步骤上提供灵活性的场景，如文档生成、数据处理、游戏流程等。
// 如何设计模板模式中的模板方法？

// 模板方法应当定义固定的步骤顺序，并在必要的步骤上提供钩子方法或抽象方法，允许子类实现这些步骤。应当将模板方法设为 final，以防止子类覆盖它，从而保持算法的整体结构不变。
// 模板模式与策略模式有什么区别？

// 模板模式关注于算法的整体结构和步骤的固定顺序，同时允许子类重定义某些步骤的实现。策略模式关注于将不同的算法或行为封装为策略对象，并在运行时选择和替换策略。模板模式固定了步骤的执行顺序，而策略模式允许动态地切换不同的算法。
// 这些示例和问题应帮助你更好地理解模板模式。如果有更多问题或需要进一步探讨，请随时告诉我！

// 访问者
// ChatGPT
// 访问者模式（Visitor Pattern）是一种行为型设计模式，用于将操作封装到访问者对象中，从而使得操作可以独立于被操作对象的结构进行扩展。访问者模式允许在不修改被操作对象的类的情况下，为其添加新的操作。

// 示例
// 基本访问者模式

// cpp
// 复制代码
#include <iostream>
#include <memory>
#include <vector>

// 前向声明
class ConcreteElementA;
class ConcreteElementB;

// 访问者接口
class Visitor {
public:
    virtual void visitConcreteElementA(const ConcreteElementA& element) const = 0;
    virtual void visitConcreteElementB(const ConcreteElementB& element) const = 0;
    virtual ~Visitor() = default;
};

// 元素接口
class Element {
public:
    virtual void accept(const Visitor& visitor) const = 0;
    virtual ~Element() = default;
};

// 具体元素A
class ConcreteElementA : public Element {
public:
    void accept(const Visitor& visitor) const override {
        visitor.visitConcreteElementA(*this);
    }

    void operationA() const {
        std::cout << "Operation A" << std::endl;
    }
};

// 具体元素B
class ConcreteElementB : public Element {
public:
    void accept(const Visitor& visitor) const override {
        visitor.visitConcreteElementB(*this);
    }

    void operationB() const {
        std::cout << "Operation B" << std::endl;
    }
};

// 具体访问者
class ConcreteVisitor : public Visitor {
public:
    void visitConcreteElementA(const ConcreteElementA& element) const override {
        std::cout << "Visiting ConcreteElementA" << std::endl;
        element.operationA();
    }

    void visitConcreteElementB(const ConcreteElementB& element) const override {
        std::cout << "Visiting ConcreteElementB" << std::endl;
        element.operationB();
    }
};

int main() {
    std::vector<std::shared_ptr<Element>> elements;
    elements.push_back(std::make_shared<ConcreteElementA>());
    elements.push_back(std::make_shared<ConcreteElementB>());

    ConcreteVisitor visitor;

    for (const auto& element : elements) {
        element->accept(visitor);
    }

    return 0;
}
// 在这个例子中，Visitor 是访问者接口，定义了访问具体元素 ConcreteElementA 和 ConcreteElementB 的方法。Element 是元素接口，定义了 accept 方法。ConcreteElementA 和 ConcreteElementB 是具体的元素，接受访问者并调用相应的方法。ConcreteVisitor 是具体访问者，执行具体的操作。

// 访问者模式与图形元素

// cpp
// 复制代码
#include <iostream>
#include <memory>
#include <vector>

// 前向声明
class Circle;
class Rectangle;

// 访问者接口
class ShapeVisitor {
public:
    virtual void visitCircle(const Circle& circle) const = 0;
    virtual void visitRectangle(const Rectangle& rectangle) const = 0;
    virtual ~ShapeVisitor() = default;
};

// 元素接口
class Shape {
public:
    virtual void accept(const ShapeVisitor& visitor) const = 0;
    virtual ~Shape() = default;
};

// 具体元素：圆形
class Circle : public Shape {
public:
    void accept(const ShapeVisitor& visitor) const override {
        visitor.visitCircle(*this);
    }

    void draw() const {
        std::cout << "Drawing Circle" << std::endl;
    }
};

// 具体元素：矩形
class Rectangle : public Shape {
public:
    void accept(const ShapeVisitor& visitor) const override {
        visitor.visitRectangle(*this);
    }

    void draw() const {
        std::cout << "Drawing Rectangle" << std::endl;
    }
};

// 具体访问者：面积计算
class AreaCalculator : public ShapeVisitor {
public:
    void visitCircle(const Circle& circle) const override {
        std::cout << "Calculating area for Circle" << std::endl;
        // 计算圆形面积的逻辑
    }

    void visitRectangle(const Rectangle& rectangle) const override {
        std::cout << "Calculating area for Rectangle" << std::endl;
        // 计算矩形面积的逻辑
    }
};

int main() {
    std::vector<std::shared_ptr<Shape>> shapes;
    shapes.push_back(std::make_shared<Circle>());
    shapes.push_back(std::make_shared<Rectangle>());

    AreaCalculator calculator;

    for (const auto& shape : shapes) {
        shape->accept(calculator);
    }

    return 0;
}
// 在这个例子中，ShapeVisitor 是访问者接口，定义了访问 Circle 和 Rectangle 的方法。Shape 是元素接口，定义了 accept 方法。Circle 和 Rectangle 是具体的元素，实现了 accept 方法并调用访问者。AreaCalculator 是具体访问者，执行计算面积的操作。

// 问题
// 访问者模式的优点是什么？

// 访问者模式允许在不改变元素类的情况下添加新的操作，提供了在对象结构上添加新功能的灵活性。它使得操作可以集中在访问者中，从而简化了元素类的设计。
// 在什么情况下应该使用访问者模式？

// 当需要在一个对象结构上执行一些操作，并且这些操作可能会在未来发生变化时，访问者模式非常有用。它适用于需要对复杂的对象结构进行操作的场景，如语法树遍历、图形元素处理等。
// 如何设计访问者模式中的访问者和元素接口？

// 访问者接口应定义所有具体元素的访问方法，确保访问者可以访问所有类型的元素。元素接口应定义一个 accept 方法，接受访问者并调用访问者的相应方法。这样可以确保访问者与元素之间的双向访问关系。
// 访问者模式与组合模式有什么区别？

// 访问者模式关注于在不修改元素类的情况下扩展操作，而组合模式关注于将对象组合成树形结构以表示部分-整体层次。访问者模式允许对对象结构执行操作，而组合模式允许将对象组织成树形结构以简化操作和管理。
// 这些示例和问题应帮助你更好地理解访问者模式。如果有更多问题或需要进一步探讨，请随时告诉我！

// 状态
// ChatGPT
// 状态模式（State Pattern）是一种行为型设计模式，用于允许对象在内部状态改变时改变其行为。状态模式的核心思想是将对象的状态抽象出来，并将不同的状态封装在不同的状态类中，使得状态的转换和行为的变化变得更加清晰和易于管理。

// 示例
// 基本状态模式

// cpp
// 复制代码
#include <iostream>
#include <memory>

// 前向声明
class Context;

// 状态接口
class State {
public:
    virtual void handle(Context& context) = 0;
    virtual ~State() = default;
};

// 具体状态A
class ConcreteStateA : public State {
public:
    void handle(Context& context) override;
};

// 具体状态B
class ConcreteStateB : public State {
public:
    void handle(Context& context) override;
};

// 上下文
class Context {
public:
    Context(std::shared_ptr<State> state) : state(state) {}

    void setState(std::shared_ptr<State> newState) {
        state = newState;
    }

    void request() {
        state->handle(*this);
    }

private:
    std::shared_ptr<State> state;
};

void ConcreteStateA::handle(Context& context) {
    std::cout << "Handling request in State A" << std::endl;
    // 状态转换
    context.setState(std::make_shared<ConcreteStateB>());
}

void ConcreteStateB::handle(Context& context) {
    std::cout << "Handling request in State B" << std::endl;
    // 状态转换
    context.setState(std::make_shared<ConcreteStateA>());
}

int main() {
    auto context = Context(std::make_shared<ConcreteStateA>());

    context.request();  // 处理请求，切换到状态B
    context.request();  // 处理请求，切换到状态A

    return 0;
}
// 在这个例子中，State 是状态接口，定义了 handle 方法。ConcreteStateA 和 ConcreteStateB 是具体的状态，实现了 handle 方法，并在处理请求时进行状态转换。Context 是上下文，持有当前状态，并通过 request 方法委托给状态对象处理请求。

// 状态模式与订单处理

// cpp
// 复制代码
#include <iostream>
#include <memory>

// 前向声明
class OrderContext;

// 状态接口
class OrderState {
public:
    virtual void handle(OrderContext& context) = 0;
    virtual ~OrderState() = default;
};

// 具体状态：待处理
class PendingState : public OrderState {
public:
    void handle(OrderContext& context) override;
};

// 具体状态：已处理
class ProcessedState : public OrderState {
public:
    void handle(OrderContext& context) override;
};

// 上下文
class OrderContext {
public:
    OrderContext(std::shared_ptr<OrderState> state) : state(state) {}

    void setState(std::shared_ptr<OrderState> newState) {
        state = newState;
    }

    void request() {
        state->handle(*this);
    }

private:
    std::shared_ptr<OrderState> state;
};

void PendingState::handle(OrderContext& context) {
    std::cout << "Order is pending." << std::endl;
    // 状态转换
    context.setState(std::make_shared<ProcessedState>());
}

void ProcessedState::handle(OrderContext& context) {
    std::cout << "Order has been processed." << std::endl;
    // 可能的状态转换
}

int main() {
    auto context = OrderContext(std::make_shared<PendingState>());

    context.request();  // 订单状态变为已处理
    context.request();  // 订单已经处理

    return 0;
}
// 在这个例子中，OrderState 是订单状态接口，定义了 handle 方法。PendingState 和 ProcessedState 是具体的订单状态，实现了 handle 方法，并在处理请求时进行状态转换。OrderContext 是上下文，持有当前状态，并通过 request 方法委托给状态对象处理请求。

// 状态模式与文档审核

// cpp
// 复制代码
#include <iostream>
#include <memory>

// 前向声明
class DocumentContext;

// 状态接口
class DocumentState {
public:
    virtual void handle(DocumentContext& context) = 0;
    virtual ~DocumentState() = default;
};

// 具体状态：草稿
class DraftState : public DocumentState {
public:
    void handle(DocumentContext& context) override;
};

// 具体状态：审核中
class ReviewingState : public DocumentState {
public:
    void handle(DocumentContext& context) override;
};

// 具体状态：已发布
class PublishedState : public DocumentState {
public:
    void handle(DocumentContext& context) override;
};

// 上下文
class DocumentContext {
public:
    DocumentContext(std::shared_ptr<DocumentState> state) : state(state) {}

    void setState(std::shared_ptr<DocumentState> newState) {
        state = newState;
    }

    void request() {
        state->handle(*this);
    }

private:
    std::shared_ptr<DocumentState> state;
};

void DraftState::handle(DocumentContext& context) {
    std::cout << "Document is in draft state." << std::endl;
    // 状态转换
    context.setState(std::make_shared<ReviewingState>());
}

void ReviewingState::handle(DocumentContext& context) {
    std::cout << "Document is under review." << std::endl;
    // 状态转换
    context.setState(std::make_shared<PublishedState>());
}

void PublishedState::handle(DocumentContext& context) {
    std::cout << "Document is published." << std::endl;
    // 可能的状态转换
}

int main() {
    auto context = DocumentContext(std::make_shared<DraftState>());

    context.request();  // 文档状态变为审核中
    context.request();  // 文档状态变为已发布

    return 0;
}
// 在这个例子中，DocumentState 是文档状态接口，定义了 handle 方法。DraftState、ReviewingState 和 PublishedState 是具体的文档状态，实现了 handle 方法，并在处理请求时进行状态转换。DocumentContext 是上下文，持有当前状态，并通过 request 方法委托给状态对象处理请求。

// 问题
// 状态模式的优点是什么？

// 状态模式使得状态的切换变得清晰和可控，避免了复杂的条件分支。它将不同的状态封装到独立的状态类中，使得状态的管理和扩展变得更加简单和模块化。
// 在什么情况下应该使用状态模式？

// 当对象的行为依赖于其内部状态，并且对象的状态可以在运行时发生变化时，状态模式非常有用。它适用于需要管理对象在不同状态下表现不同行为的场景，如状态机、工作流、游戏状态等。
// 如何设计状态模式中的状态接口和上下文？

// 状态接口应定义所有状态共用的方法，并提供用于处理请求的接口。上下文应持有当前状态，并提供用于切换状态的方法。上下文应将请求委托给当前状态对象，从而简化状态的管理。
// 状态模式与策略模式有什么区别？

// 状态模式用于处理对象在不同状态下的行为变化，状态变化通常会影响对象的行为。策略模式用于封装算法或策略，并在运行时选择和替换不同的策略。状态模式关注于状态变化和行为，而策略模式关注于策略的选择和替换。
// 这些示例和问题应帮助你更好地理解状态模式。如果有更多问题或需要进一步探讨，请随时告诉我！

// 解释器
// ChatGPT
// 解释器模式（Interpreter Pattern）是一种行为型设计模式，用于为特定的语言定义一个解释器。这个模式提供了一种解决特定问题的语法表示，并定义了一个解释器来解释该语言的句子。这种模式尤其适用于需要处理复杂语法和规则的场景，如编程语言解释器、表达式求值等。

// 示例
// 基本解释器模式

// 假设我们要解释一个简单的数学表达式语言，支持加法和减法。

// cpp
// 复制代码
#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <map>

// 抽象表达式
class Expression {
public:
    virtual int interpret() const = 0;
    virtual ~Expression() = default;
};

// 具体表达式：加法
class AddExpression : public Expression {
public:
    AddExpression(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right)
        : left(left), right(right) {}

    int interpret() const override {
        return left->interpret() + right->interpret();
    }

private:
    std::shared_ptr<Expression> left;
    std::shared_ptr<Expression> right;
};

// 具体表达式：减法
class SubtractExpression : public Expression {
public:
    SubtractExpression(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right)
        : left(left), right(right) {}

    int interpret() const override {
        return left->interpret() - right->interpret();
    }

private:
    std::shared_ptr<Expression> left;
    std::shared_ptr<Expression> right;
};

// 具体表达式：数字
class NumberExpression : public Expression {
public:
    NumberExpression(int number) : number(number) {}

    int interpret() const override {
        return number;
    }

private:
    int number;
};

// 解析器
class ExpressionParser {
public:
    ExpressionParser(const std::string& expression) : expression(expression) {}

    std::shared_ptr<Expression> parse() {
        return parseAddSubtract();
    }

private:
    std::string expression;

    std::shared_ptr<Expression> parseAddSubtract() {
        auto left = parseNumber();
        if (pos >= expression.size()) {
            return left;
        }
        if (expression[pos] == '+') {
            ++pos;
            auto right = parseNumber();
            return std::make_shared<AddExpression>(left, right);
        }
        if (expression[pos] == '-') {
            ++pos;
            auto right = parseNumber();
            return std::make_shared<SubtractExpression>(left, right);
        }
        return left;
    }

    std::shared_ptr<Expression> parseNumber() {
        std::istringstream stream(expression.substr(pos));
        int number;
        stream >> number;
        pos += stream.tellg();
        return std::make_shared<NumberExpression>(number);
    }

    size_t pos = 0;
};

int main() {
    std::string input = "3+5-2";
    ExpressionParser parser(input);
    auto expression = parser.parse();
    std::cout << "Result: " << expression->interpret() << std::endl;
    return 0;
}
// 在这个例子中，Expression 是抽象表达式接口，定义了 interpret 方法。AddExpression 和 SubtractExpression 是具体的表达式类，实现了加法和减法的解释逻辑。NumberExpression 表示数字。ExpressionParser 是解析器，用于将输入的字符串解析成表达式树，并计算其结果。

// 解释器模式与布尔表达式

// 假设我们要解释一个布尔表达式语言，支持逻辑与（AND）和逻辑或（OR）。

// cpp
// 复制代码
#include <iostream>
#include <memory>
#include <string>
#include <map>

// 抽象表达式
class BooleanExpression {
public:
    virtual bool interpret(const std::map<std::string, bool>& context) const = 0;
    virtual ~BooleanExpression() = default;
};

// 具体表达式：变量
class VariableExpression : public BooleanExpression {
public:
    VariableExpression(const std::string& name) : name(name) {}

    bool interpret(const std::map<std::string, bool>& context) const override {
        return context.at(name);
    }

private:
    std::string name;
};

// 具体表达式：逻辑与
class AndExpression : public BooleanExpression {
public:
    AndExpression(std::shared_ptr<BooleanExpression> left, std::shared_ptr<BooleanExpression> right)
        : left(left), right(right) {}

    bool interpret(const std::map<std::string, bool>& context) const override {
        return left->interpret(context) && right->interpret(context);
    }

private:
    std::shared_ptr<BooleanExpression> left;
    std::shared_ptr<BooleanExpression> right;
};

// 具体表达式：逻辑或
class OrExpression : public BooleanExpression {
public:
    OrExpression(std::shared_ptr<BooleanExpression> left, std::shared_ptr<BooleanExpression> right)
        : left(left), right(right) {}

    bool interpret(const std::map<std::string, bool>& context) const override {
        return left->interpret(context) || right->interpret(context);
    }

private:
    std::shared_ptr<BooleanExpression> left;
    std::shared_ptr<BooleanExpression> right;
};

int main() {
    // 表达式： (A AND B) OR C
    auto a = std::make_shared<VariableExpression>("A");
    auto b = std::make_shared<VariableExpression>("B");
    auto c = std::make_shared<VariableExpression>("C");

    auto andExpr = std::make_shared<AndExpression>(a, b);
    auto orExpr = std::make_shared<OrExpression>(andExpr, c);

    std::map<std::string, bool> context = {{"A", true}, {"B", false}, {"C", true}};

    std::cout << "Result: " << orExpr->interpret(context) << std::endl;

    return 0;
}
// 在这个例子中，BooleanExpression 是布尔表达式接口，定义了 interpret 方法。VariableExpression 表示变量，AndExpression 和 OrExpression 实现了逻辑与和逻辑或的解释逻辑。

// 问题
// 解释器模式的优点是什么？

// 解释器模式可以为特定语言定义一个解释器，提供了对复杂语言结构的灵活处理方式。它允许在不修改解释器的情况下扩展语言的功能，使得增加新功能变得更加简单。
// 在什么情况下应该使用解释器模式？

// 当你需要处理复杂的语法和规则，且这些规则可以被定义为语法树时，解释器模式非常有用。它适用于需要解析和解释语言的场景，如编程语言的解析器、表达式求值器等。
// 如何设计解释器模式中的表达式和解释器？

// 表达式接口应定义所有具体表达式的公共方法，如 interpret。具体表达式类应实现这些方法，并提供具体的解析和解释逻辑。解释器（解析器）应负责将输入解析成表达式树，并计算结果。
// 解释器模式与策略模式有什么区别？

// 解释器模式用于定义一个解释器来解释特定语言的句子，并提供其语法和规则的表示。策略模式用于将算法或策略封装成对象，并在运行时选择和替换不同的策略。解释器模式关注于语法和规则的解释，而策略模式关注于算法的选择和替换。
// 这些示例和问题应帮助你更好地理解解释器模式。如果有更多问题或需要进一步探讨，请随时告诉我！

// 命令
// ChatGPT
// 命令模式（Command Pattern）是一种行为型设计模式，它将请求封装成对象，从而使你能够参数化客户端请求、排队请求或记录请求日志，并支持可撤销操作。命令模式允许将操作的发起者与执行者解耦，使得发起者不需要知道操作的细节，执行者也不需要知道操作的发起者。

// 示例
// 基本命令模式

// cpp
// 复制代码
#include <iostream>
#include <memory>
#include <vector>

// 命令接口
class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() = default;
};

// 具体命令：开灯
class LightOnCommand : public Command {
public:
    LightOnCommand(std::shared_ptr<Light> light) : light(light) {}

    void execute() override {
        light->turnOn();
    }

private:
    std::shared_ptr<Light> light;
};

// 具体命令：关灯
class LightOffCommand : public Command {
public:
    LightOffCommand(std::shared_ptr<Light> light) : light(light) {}

    void execute() override {
        light->turnOff();
    }

private:
    std::shared_ptr<Light> light;
};

// 接收者
class Light {
public:
    void turnOn() {
        std::cout << "Light is ON" << std::endl;
    }

    void turnOff() {
        std::cout << "Light is OFF" << std::endl;
    }
};

// 请求发起者
class RemoteControl {
public:
    void setCommand(std::shared_ptr<Command> command) {
        this->command = command;
    }

    void pressButton() {
        command->execute();
    }

private:
    std::shared_ptr<Command> command;
};

int main() {
    auto light = std::make_shared<Light>();
    auto lightOn = std::make_shared<LightOnCommand>(light);
    auto lightOff = std::make_shared<LightOffCommand>(light);

    RemoteControl remote;
    remote.setCommand(lightOn);
    remote.pressButton(); // 开灯

    remote.setCommand(lightOff);
    remote.pressButton(); // 关灯

    return 0;
}
// 在这个例子中，Command 是命令接口，定义了 execute 方法。LightOnCommand 和 LightOffCommand 是具体命令，执行开灯和关灯的操作。Light 是接收者，负责实际的操作。RemoteControl 是请求发起者，通过设置命令并调用 pressButton 来执行命令。

// 命令模式与文本编辑器

// 假设我们需要在文本编辑器中实现撤销和重做功能。

// cpp
// 复制代码
#include <iostream>
#include <memory>
#include <vector>
#include <string>

// 命令接口
class Command {
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual ~Command() = default;
};

// 具体命令：添加文本
class AddTextCommand : public Command {
public:
    AddTextCommand(std::string& text, const std::string& newText) 
        : text(text), newText(newText) {}

    void execute() override {
        text += newText;
    }

    void undo() override {
        text.erase(text.size() - newText.size());
    }

private:
    std::string& text;
    std::string newText;
};

// 请求发起者
class TextEditor {
public:
    void executeCommand(std::shared_ptr<Command> command) {
        command->execute();
        commands.push_back(command);
    }

    void undo() {
        if (!commands.empty()) {
            auto command = commands.back();
            command->undo();
            commands.pop_back();
        }
    }

    void printText() const {
        std::cout << "Text: " << text << std::endl;
    }

private:
    std::vector<std::shared_ptr<Command>> commands;
    std::string text;
};

int main() {
    TextEditor editor;
    editor.printText(); // Text: 

    auto addHello = std::make_shared<AddTextCommand>(editor.text, "Hello ");
    auto addWorld = std::make_shared<AddTextCommand>(editor.text, "World!");

    editor.executeCommand(addHello);
    editor.executeCommand(addWorld);

    editor.printText(); // Text: Hello World!

    editor.undo();
    editor.printText(); // Text: Hello

    editor.undo();
    editor.printText(); // Text: 

    return 0;
}
// 在这个例子中，Command 是命令接口，定义了 execute 和 undo 方法。AddTextCommand 是具体命令，实现了添加文本和撤销操作。TextEditor 是请求发起者，能够执行命令、撤销命令并打印文本内容。

// 命令模式与遥控器

// cpp
// 复制代码
#include <iostream>
#include <memory>
#include <vector>

// 命令接口
class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() = default;
};

// 具体命令：开门
class DoorOpenCommand : public Command {
public:
    DoorOpenCommand(std::shared_ptr<Door> door) : door(door) {}

    void execute() override {
        door->open();
    }

private:
    std::shared_ptr<Door> door;
};

// 具体命令：关门
class DoorCloseCommand : public Command {
public:
    DoorCloseCommand(std::shared_ptr<Door> door) : door(door) {}

    void execute() override {
        door->close();
    }

private:
    std::shared_ptr<Door> door;
};

// 接收者
class Door {
public:
    void open() {
        std::cout << "Door is OPEN" << std::endl;
    }

    void close() {
        std::cout << "Door is CLOSED" << std::endl;
    }
};

// 遥控器
class RemoteControl {
public:
    void setCommand(std::shared_ptr<Command> command) {
        this->command = command;
    }

    void pressButton() {
        command->execute();
    }

private:
    std::shared_ptr<Command> command;
};

int main() {
    auto door = std::make_shared<Door>();
    auto doorOpen = std::make_shared<DoorOpenCommand>(door);
    auto doorClose = std::make_shared<DoorCloseCommand>(door);

    RemoteControl remote;
    remote.setCommand(doorOpen);
    remote.pressButton(); // 开门

    remote.setCommand(doorClose);
    remote.pressButton(); // 关门

    return 0;
}
// 在这个例子中，Command 是命令接口，定义了 execute 方法。DoorOpenCommand 和 DoorCloseCommand 是具体命令，执行开门和关门的操作。Door 是接收者，负责实际的操作。RemoteControl 是请求发起者，通过设置命令并调用 pressButton 来执行命令。

// 问题
// 命令模式的优点是什么？

// 命令模式将请求封装成对象，从而使得可以灵活地处理请求、排队请求和支持撤销操作。它解耦了请求的发起者和接收者，使得系统更具扩展性和可维护性。
// 在什么情况下应该使用命令模式？

// 当需要将请求封装成对象以支持不同的请求参数化、排队、记录日志或支持撤销操作时，命令模式非常有用。它适用于需要将操作与请求解耦的场景，如用户操作的记录、撤销和重做功能等。
// 如何设计命令模式中的命令接口和接收者？

// 命令接口应定义一个统一的 execute 方法，所有具体命令类应实现该方法。接收者类应负责实际的操作逻辑。请求发起者应持有命令对象并调用其 execute 方法。
// 命令模式与策略模式有什么区别？

// 命令模式关注于将操作封装成对象，并支持撤销、记录和排队操作。策略模式关注于将算法或策略封装成对象，并在运行时选择和替换不同的策略。命令模式处理的是操作请求的管理，而策略模式处理的是算法的选择和替换。
// 这些示例和问题应帮助你更好地理解命令模式。如果有更多问题或需要进一步探讨，请随时告诉我！

// 迭代器
// ChatGPT
// 迭代器模式（Iterator Pattern）是一种行为型设计模式，用于提供一种顺序访问集合对象中各个元素的方式，而无需暴露集合对象的内部表示。该模式使得遍历集合中的元素变得更加简便且一致。

// 迭代器模式的核心组件
// 迭代器接口（Iterator）：定义访问集合元素的接口，包括移动到下一个元素、检查是否还有元素等。
// 具体迭代器（ConcreteIterator）：实现了迭代器接口，负责实际的迭代过程。
// 集合接口（Aggregate）：定义创建迭代器的方法。
// 具体集合（ConcreteAggregate）：实现集合接口，包含具体的集合数据并创建具体迭代器。
// 示例
// 基本迭代器模式

// cpp
// 复制代码
#include <iostream>
#include <vector>
#include <memory>

// 迭代器接口
template <typename T>
class Iterator {
public:
    virtual bool hasNext() const = 0;
    virtual T next() = 0;
    virtual ~Iterator() = default;
};

// 集合接口
template <typename T>
class Aggregate {
public:
    virtual std::shared_ptr<Iterator<T>> createIterator() const = 0;
    virtual void add(const T& item) = 0;
    virtual size_t size() const = 0;
    virtual T get(size_t index) const = 0;
    virtual ~Aggregate() = default;
};

// 具体迭代器
template <typename T>
class ConcreteIterator : public Iterator<T> {
public:
    ConcreteIterator(const Aggregate<T>& aggregate) : aggregate(aggregate), index(0) {}

    bool hasNext() const override {
        return index < aggregate.size();
    }

    T next() override {
        return aggregate.get(index++);
    }

private:
    const Aggregate<T>& aggregate;
    size_t index;
};

// 具体集合
template <typename T>
class ConcreteAggregate : public Aggregate<T> {
public:
    std::shared_ptr<Iterator<T>> createIterator() const override {
        return std::make_shared<ConcreteIterator<T>>(*this);
    }

    void add(const T& item) override {
       
// 继续回答
// ChatGPT
// cpp
// 复制代码
           items.push_back(item);
       }

       size_t size() const override {
           return items.size();
       }

       T get(size_t index) const override {
           if (index < items.size()) {
               return items[index];
           }
           throw std::out_of_range("Index out of bounds");
       }

   private:
       std::vector<T> items;
   };

   int main() {
       auto aggregate = std::make_shared<ConcreteAggregate<int>>();
       aggregate->add(1);
       aggregate->add(2);
       aggregate->add(3);

       auto iterator = aggregate->createIterator();
       while (iterator->hasNext()) {
           std::cout << iterator->next() << " ";
       }
       std::cout << std::endl;

       return 0;
   }
// 在这个例子中，Iterator 是迭代器接口，定义了 hasNext 和 next 方法。ConcreteIterator 是具体迭代器，负责实际的迭代逻辑。Aggregate 是集合接口，定义了创建迭代器的方法。ConcreteAggregate 是具体集合，包含集合数据并实现了创建迭代器的方法。

// 迭代器模式与菜单系统

// 假设我们要实现一个菜单系统，其中每个菜单项可以是其他菜单的子菜单。

// cpp
// 复制代码
#include <iostream>
#include <vector>
#include <memory>
#include <string>

// 迭代器接口
class Iterator {
public:
    virtual bool hasNext() const = 0;
    virtual std::string next() = 0;
    virtual ~Iterator() = default;
};

// 集合接口
class Menu {
public:
    virtual std::shared_ptr<Iterator> createIterator() const = 0;
    virtual void add(const std::string& item) = 0;
    virtual ~Menu() = default;
};

// 具体迭代器
class ConcreteIterator : public Iterator {
public:
    ConcreteIterator(const Menu& menu) : menu(menu), index(0) {}

    bool hasNext() const override {
        return index < menu.size();
    }

    std::string next() override {
        return menu.get(index++);
    }

private:
    const Menu& menu;
    size_t index;
};

// 具体集合
class ConcreteMenu : public Menu {
public:
    std::shared_ptr<Iterator> createIterator() const override {
        return std::make_shared<ConcreteIterator>(*this);
    }

    void add(const std::string& item) override {
        items.push_back(item);
    }

    size_t size() const {
        return items.size();
    }

    std::string get(size_t index) const {
        if (index < items.size()) {
            return items[index];
        }
        throw std::out_of_range("Index out of bounds");
    }

private:
    std::vector<std::string> items;
};

int main() {
    auto menu = std::make_shared<ConcreteMenu>();
    menu->add("Burger");
    menu->add("Pizza");
    menu->add("Salad");

    auto iterator = menu->createIterator();
    while (iterator->hasNext()) {
        std::cout << iterator->next() << std::endl;
    }

    return 0;
}
// 在这个例子中，Iterator 是迭代器接口，定义了 hasNext 和 next 方法。ConcreteIterator 是具体迭代器，负责实际的迭代逻辑。Menu 是集合接口，定义了创建迭代器的方法。ConcreteMenu 是具体集合，包含菜单项并实现了创建迭代器的方法。

// 问题
// 迭代器模式的优点是什么？

// 迭代器模式可以提供统一的方式来遍历集合中的元素，而无需暴露集合的内部表示。它支持多种遍历方式并允许在不修改集合的情况下扩展迭代功能。
// 在什么情况下应该使用迭代器模式？

// 当需要遍历一个集合中的元素，而又不希望暴露集合的内部结构时，迭代器模式是一个理想选择。它适用于需要支持多种遍历方式的场景，如容器类的遍历、列表的遍历等。
// 如何设计迭代器模式中的迭代器和集合？

// 迭代器接口应定义访问集合元素的方法，如 hasNext 和 next。具体迭代器应实现这些方法，并提供实际的遍历逻辑。集合接口应定义创建迭代器的方法，以及添加、访问元素的方法。具体集合应实现这些方法，并提供具体的集合数据。
// 迭代器模式与访问者模式有什么区别？

// 迭代器模式关注于遍历集合中的元素，而访问者模式关注于在不修改元素的情况下对元素进行操作。迭代器模式提供了一种遍历机制，而访问者模式提供了一种操作机制。
// 这些示例和问题应帮助你更好地理解迭代器模式。如果有更多问题或需要进一步探讨，请随时告诉我！

// 备忘录
// ChatGPT
// 备忘录模式（Memento Pattern）是一种行为型设计模式，用于在不暴露对象内部状态的情况下，保存和恢复对象的状态。它通常用于实现“撤销”操作，允许你在对象的某个状态点创建一个快照，并在需要时恢复到该状态。

// 备忘录模式的核心组件
// 备忘录（Memento）：存储对象的内部状态，并提供恢复状态的方法。
// 发起人（Originator）：创建一个备忘录对象以保存当前状态，并使用备忘录恢复状态。
// 管理者（Caretaker）：负责保存和管理备忘录对象，但不对备忘录的内容进行操作或检查。
// 示例
// 基本备忘录模式

// cpp
// 复制代码
#include <iostream>
#include <memory>
#include <vector>

// 备忘录
class Memento {
public:
    explicit Memento(int state) : state(state) {}

    int getState() const {
        return state;
    }

private:
    int state;
};

// 发起人
class Originator {
public:
    void setState(int state) {
        this->state = state;
    }

    int getState() const {
        return state;
    }

    std::shared_ptr<Memento> saveStateToMemento() const {
        return std::make_shared<Memento>(state);
    }

    void getStateFromMemento(const std::shared_ptr<Memento>& memento) {
        state = memento->getState();
    }

private:
    int state;
};

// 管理者
class Caretaker {
public:
    void addMemento(const std::shared_ptr<Memento>& memento) {
        mementos.push_back(memento);
    }

    std::shared_ptr<Memento> getMemento(size_t index) const {
        if (index < mementos.size()) {
            return mementos[index];
        }
        throw std::out_of_range("Index out of bounds");
    }

private:
    std::vector<std::shared_ptr<Memento>> mementos;
};

int main() {
    Originator originator;
    Caretaker caretaker;

    originator.setState(1);
    caretaker.addMemento(originator.saveStateToMemento());

    originator.setState(2);
    caretaker.addMemento(originator.saveStateToMemento());

    originator.setState(3);
    std::cout << "Current State: " << originator.getState() << std::endl;

    originator.getStateFromMemento(caretaker.getMemento(0));
    std::cout << "First saved State: " << originator.getState() << std::endl;

    originator.getStateFromMemento(caretaker.getMemento(1));
    std::cout << "Second saved State: " << originator.getState() << std::endl;

    return 0;
}
// 在这个例子中，Memento 是备忘录类，存储状态。Originator 是发起人类，创建备忘录并恢复状态。Caretaker 是管理者类，保存备忘录并提供恢复状态的功能。

// 备忘录模式与文本编辑器

// 假设我们在文本编辑器中实现撤销功能：

// cpp
// 复制代码
#include <iostream>
#include <memory>
#include <vector>
#include <string>

// 备忘录
class Memento {
public:
    explicit Memento(const std::string& content) : content(content) {}

    std::string getContent() const {
        return content;
    }

private:
    std::string content;
};

// 发起人
class Editor {
public:
    void setContent(const std::string& content) {
        this->content = content;
    }

    std::string getContent() const {
        return content;
    }

    std::shared_ptr<Memento> saveStateToMemento() const {
        return std::make_shared<Memento>(content);
    }

    void getStateFromMemento(const std::shared_ptr<Memento>& memento) {
        content = memento->getContent();
    }

private:
    std::string content;
};

// 管理者
class History {
public:
    void addMemento(const std::shared_ptr<Memento>& memento) {
        mementos.push_back(memento);
    }

    std::shared_ptr<Memento> getMemento(size_t index) const {
        if (index < mementos.size()) {
            return mementos[index];
        }
        throw std::out_of_range("Index out of bounds");
    }

private:
    std::vector<std::shared_ptr<Memento>> mementos;
};

int main() {
    Editor editor;
    History history;

    editor.setContent("Version 1");
    history.addMemento(editor.saveStateToMemento());

    editor.setContent("Version 2");
    history.addMemento(editor.saveStateToMemento());

    editor.setContent("Version 3");
    std::cout << "Current Content: " << editor.getContent() << std::endl;

    editor.getStateFromMemento(history.getMemento(0));
    std::cout << "First saved Content: " << editor.getContent() << std::endl;

    editor.getStateFromMemento(history.getMemento(1));
    std::cout << "Second saved Content: " << editor.getContent() << std::endl;

    return 0;
}
// 在这个例子中，Memento 是备忘录类，存储文本内容。Editor 是发起人类，创建备忘录并恢复内容。History 是管理者类，保存备忘录并提供恢复内容的功能。

// 问题
// 备忘录模式的优点是什么？

// 备忘录模式允许对象在不暴露其内部状态的情况下保存和恢复状态。它支持撤销操作，并使得状态的恢复变得简单而一致。
// 在什么情况下应该使用备忘录模式？

// 当需要在对象状态发生变化时能够保存和恢复状态，而又不希望对象暴露其内部实现细节时，备忘录模式非常有用。它适用于需要支持撤销和重做功能的场景，如编辑器、游戏和应用程序的历史记录等。
// 如何设计备忘录模式中的备忘录和发起人？

// 备忘录类应负责存储对象的内部状态，并提供获取状态的方法。发起人类应能够创建备忘录并恢复状态，同时管理备忘录的创建和恢复逻辑。
// 备忘录模式与命令模式有什么区别？

// 备忘录模式关注于保存和恢复对象的状态，而命令模式关注于将操作封装成对象，以支持请求的排队、记录和撤销操作。备忘录模式主要用于状态的恢复，而命令模式主要用于操作的管理。
// 这些示例和问题应帮助你更好地理解备忘录模式。如果有更多问题或需要进一步探讨，请随时告诉我！

// 中介者
// ChatGPT
// 中介者模式（Mediator Pattern）是一种行为型设计模式，用于定义一个中介对象来封装一系列对象之间的交互，使得这些对象之间不需要直接互相引用，从而使得它们可以独立变化。中介者模式通常用于减少对象之间的复杂耦合，提供一种更为清晰的协作方式。

// 中介者模式的核心组件
// 中介者（Mediator）：定义了与各同事对象交互的方法，并通过这些方法协调同事对象之间的交互。
// 具体中介者（ConcreteMediator）：实现中介者接口，负责协调同事对象的交互。
// 同事类（Colleague）：与其他同事对象通过中介者进行交互。每个同事类实现自己的行为，并将操作委托给中介者。
// 具体同事类（ConcreteColleague）：实现同事类接口，并通过中介者与其他同事对象进行交互。
// 示例
// 基本中介者模式

// cpp
// 复制代码
#include <iostream>
#include <memory>

// 中介者接口
class Mediator {
public:
    virtual void send(const std::string& message, class Colleague* colleague) = 0;
    virtual ~Mediator() = default;
};

// 同事类
class Colleague {
public:
    explicit Colleague(Mediator* mediator) : mediator(mediator) {}

    void send(const std::string& message) {
        mediator->send(message, this);
    }

    virtual void receive(const std::string& message) = 0;

protected:
    Mediator* mediator;
};

// 具体同事类
class ConcreteColleagueA : public Colleague {
public:
    using Colleague::Colleague;

    void receive(const std::string& message) override {
        std::cout << "ColleagueA received: " << message << std::endl;
    }
};

class ConcreteColleagueB : public Colleague {
public:
    using Colleague::Colleague;

    void receive(const std::string& message) override {
        std::cout << "ColleagueB received: " << message << std::endl;
    }
};

// 具体中介者
class ConcreteMediator : public Mediator {
public:
    void setColleagueA(Colleague* colleagueA) {
        this->.colleagueA = colleagueA;
    }

    void setColleagueB(Colleague* colleagueB) {
        this->.colleagueB = colleagueB;
    }

    void send(const std::string& message, Colleague* colleague) override {
        if (colleague == colleagueA) {
            colleagueB->receive(message);
        } else if (colleague == colleagueB) {
            colleagueA->receive(message);
        }
    }

private:
    Colleague* colleagueA;
    Colleague* colleagueB;
};

int main() {
    ConcreteMediator mediator;
    ConcreteColleagueA colleagueA(&mediator);
    ConcreteColleagueB colleagueB(&mediator);

    mediator.setColleagueA(&colleagueA);
    mediator.setColleagueB(&colleagueB);

    colleagueA.send("Hello from A");
    colleagueB.send("Hello from B");

    return 0;
}
// 在这个例子中，Mediator 是中介者接口，定义了 send 方法。ConcreteMediator 是具体中介者，协调 ConcreteColleagueA 和 ConcreteColleagueB 之间的通信。Colleague 是同事类，定义了 send 和 receive 方法。ConcreteColleagueA 和 ConcreteColleagueB 是具体同事类，负责接收和发送消息。

// 中介者模式与聊天系统

// 假设我们要实现一个简单的聊天系统，其中用户可以通过中介者发送消息。

// cpp
// 复制代码
#include <iostream>
#include <memory>
#include <vector>
#include <string>

// 中介者接口
class ChatRoomMediator {
public:
    virtual void sendMessage(const std::string& message, class User* user) = 0;
    virtual ~ChatRoomMediator() = default;
};

// 同事类
class User {
public:
    User(const std::string& name, ChatRoomMediator* mediator) : name(name), mediator(mediator) {}

    void sendMessage(const std::string& message) {
        mediator->sendMessage(message, this);
    }

    virtual void receiveMessage(const std::string& message) = 0;

protected:
    std::string name;
    ChatRoomMediator* mediator;
};

// 具体同事类
class ConcreteUser : public User {
public:
    using User::User;

    void receiveMessage(const std::string& message) override {
        std::cout << name << " received: " << message << std::endl;
    }
};

// 具体中介者
class ConcreteChatRoom : public ChatRoomMediator {
public:
    void addUser(User* user) {
        users.push_back(user);
    }

    void sendMessage(const std::string& message, User* sender) override {
        for (auto user : users) {
            if (user != sender) {
                user->receiveMessage(message);
            }
        }
    }

private:
    std::vector<User*> users;
};

int main() {
    ConcreteChatRoom chatRoom;

    ConcreteUser user1("User1", &chatRoom);
    ConcreteUser user2("User2", &chatRoom);
    ConcreteUser user3("User3", &chatRoom);

    chatRoom.addUser(&user1);
    chatRoom.addUser(&user2);
    chatRoom.addUser(&user3);

    user1.sendMessage("Hello everyone!");
    user2.sendMessage("Hi User1!");

    return 0;
}
// 在这个例子中，ChatRoomMediator 是中介者接口，定义了 sendMessage 方法。ConcreteChatRoom 是具体中介者，负责管理用户和转发消息。User 是同事类，定义了 sendMessage 和 receiveMessage 方法。ConcreteUser 是具体同事类，负责接收和发送消息。

// 问题
// 中介者模式的优点是什么？

// 中介者模式减少了对象之间的直接依赖关系，使得系统更加模块化和灵活。它简化了对象之间的交互，并提供了更清晰的协作方式。
// 在什么情况下应该使用中介者模式？

// 当系统中有多个对象需要相互协作，并且这种协作关系复杂时，中介者模式非常有用。它适用于需要管理对象之间交互的场景，如聊天系统、用户界面组件的协作等。
// 如何设计中介者模式中的中介者和同事类？

// 中介者类应定义协调各同事对象交互的方法，并实现这些方法。每个同事类应与中介者交互，而不是直接与其他同事对象交互。中介者负责处理对象之间的所有交互逻辑。
// 中介者模式与观察者模式有什么区别？

// 中介者模式通过一个中介对象来协调同事对象的交互，减少了对象之间的直接依赖。观察者模式允许对象在状态变化时通知其他依赖对象，并实现发布-订阅机制。中介者模式注重于减少依赖关系，而观察者模式注重于通知和响应机制。
// 这些示例和问题应帮助你更好地理解中介者模式。如果有更多问题或需要进一步探讨，请随时告诉我！



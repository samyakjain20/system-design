#include<bits/stdc++.h>
using namespace std;


class Product {
    public:
        virtual ~Product() {}
        virtual std::string Operation() const = 0;
};

// Concrete Products provide various implementations of the Product interface.
class ConcreteProduct1 : public Product {
    public:
        std::string Operation() const override {
            return "{Result of the ConcreteProduct1}";
        }
};
class ConcreteProduct2 : public Product {
    public:
        std::string Operation() const override {
            return "{Result of the ConcreteProduct2}";
        }
};

/* The Creator class declares the factory method that is supposed to return an
 * object of a Product class. The Creator's subclasses usually provide the
 * implementation of this method. */

class Creator {
    public:
        virtual ~Creator(){};
        virtual Product* FactoryMethod() const = 0;
        /**
         * Also note that, despite its name, the Creator's primary responsibility is
         * not creating products. Usually, it contains some core business logic that
         * relies on Product objects, returned by the factory method. Subclasses can
         * indirectly change that business logic by overriding the factory method and
         * returning a different type of product from it.
         */

        std::string SomeOperation() const {
            // Call the factory method to create a Product object.
            Product* product = this->FactoryMethod();
            // Now, use the product.
            std::string result = "Creator: The same creator's code has just worked with " + product->Operation();
            delete product;
            return result;
        }
};

class ConcreteCreator1 : public Creator {
    public:
        Product* FactoryMethod() const override {
            return new ConcreteProduct1();
        }
};

class ConcreteCreator2 : public Creator {
    public:
        Product* FactoryMethod() const override {
            return new ConcreteProduct2();
        }
};

void ClientCode(const Creator& creator) {
    cout << "Client: I'm not aware of the creator's class, but it still works.\n"
            << creator.SomeOperation() << std::endl;
}

/**
 * The Application picks a creator's type depending on the configuration or
 * environment.
 */

int main() {
    std::cout << "App: Launched with the ConcreteCreator1.\n";
    Creator* creator = new ConcreteCreator1();
    ClientCode(*creator);
    std::cout << std::endl;
    std::cout << "App: Launched with the ConcreteCreator2.\n";
    Creator* creator2 = new ConcreteCreator2();
    ClientCode(*creator2);

    delete creator;
    delete creator2;
    return 0;
}
#include <bits/stdc++.h>

class Handler {
private:
    Handler* next_handler_;

public:
    Handler() : next_handler_(nullptr) {}

    virtual Handler* SetNext(Handler* handler) {
        this->next_handler_ = handler;
        return handler;
    }

    virtual std::string Handle(std::string request) {
        if (this->next_handler_) {
            return this->next_handler_->Handle(request);
        }

        return {};
    }
};

/**
 * All Concrete Handlers either handle a request or pass it to the next handler
 * in the chain.
 */
class MonkeyHandler : public Handler {
public:
    std::string Handle(std::string request) override {
        if (request == "Banana") {
            return "Monkey: I'll eat the " + request + ".\n";
        } else {
            return Handler::Handle(request);
        }
    }
};

class SquirrelHandler : public Handler {
public:
    std::string Handle(std::string request) override {
        if (request == "Nut") {
            return "Squirrel: I'll eat the " + request + ".\n";
        } else {
            return Handler::Handle(request);
        }
    }
};

class DogHandler : public Handler {
public:
    std::string Handle(std::string request) override {
        if (request == "MeatBall") {
            return "Dog: I'll eat the " + request + request + ".\n";
        } else {
            return Handler::Handle(request);
        }
    }
};

/**
 * The client code is usually suited to work with a single handler. In most
 * cases, it is not even aware that the handler is part of a chain.
 */
void ClientCode(Handler& handler) {
    std::vector<std::string> food = {"Nut", "Banana", "Cup of coffee"};
    for (const std::string& f : food) {
        std::cout << "Client: Who wants a " << f << "?\n";
        const std::string result = handler.Handle(f);
        if (!result.empty()) {
            std::cout << "  " << result;
        } else {
            std::cout << "  " << f << " was left untouched.\n";
        }
    }
}

int main() {
    MonkeyHandler* monkey = new MonkeyHandler;
    SquirrelHandler* squirrel = new SquirrelHandler;
    DogHandler* dog = new DogHandler;
    monkey->SetNext(squirrel)->SetNext(dog);

    /* The client should be able to send a request to any handler, not just the
      first one in the chain. */
    std::cout << "Chain: Monkey > Squirrel > Dog\n\n";
    ClientCode(*monkey);
    std::cout << "\n";
    std::cout << "Subchain: Squirrel > Dog\n\n";
    ClientCode(*squirrel);

    delete monkey;
    delete squirrel;
    delete dog;

    return 0;
}

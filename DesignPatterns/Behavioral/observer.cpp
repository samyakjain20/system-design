#include <iostream>
#include <list>
#include <string>


class Subject {
private:
    std::list<Observer*> list_observers_;
    std::string message_;
public:
    ~Subject() {
        std::cout << "Goodbye, I was the Subject.\n";
    }

    void Attach(Observer* observer) {
        list_observers_.push_back(observer);
    }
    void Detach(Observer* observer){
        list_observers_.remove(observer);
    }
    void Notify() {
        for (auto it = list_observers_.begin(); it != list_observers_.end(); ++it) {
        (*it)->Update(message_);
        }
    }

    void CreateMessage(const std::string& message = "Empty") {
        message_ = message;
        Notify();
    }

    void HowManyObservers() {
        std::cout << "There are " << list_observers_.size() << " observers in the list.\n";
    }

    /**
     * Usually, the subscription logic is only a fraction of what a Subject can
     * really do. Subjects commonly hold some important business logic, that
     * triggers a notification method whenever something important is about to
     * happen (or after it).
     */
    void SomeBusinessLogic() {
        message_ = "Change message message";
        Notify();
        std::cout << "I'm about to do some thing important\n";
    }
};



// OBSERVER CLASS
class Observer  {
private:
    std::string message_from_subject_;
    Subject& subject_;
    static int static_number_;
    int number_;

public:
    Observer(Subject& subject) : subject_(subject) {
        subject_.Attach(this);
        std::cout << "Hi, I'm the Observer \"" << ++static_number_ << "\".\n";
        number_ = static_number_;
    }

    ~Observer() {
        std::cout << "Goodbye, I was the Observer \"" << number_ << "\".\n";
    }

    void Update(const std::string& message_from_subject)  {
        message_from_subject_ = message_from_subject;
        PrintInfo();
    }

    void RemoveMeFromTheList() {
        subject_.Detach(this);
        std::cout << "Observer \"" << number_ << "\" removed from the list.\n";
    }

    void PrintInfo() {
        std::cout << "Observer \"" << number_ << "\": a new message is available --> " << message_from_subject_ << "\n";
    }
};
int Observer::static_number_ = 0;



void ClientCode() {
    Subject* subject = new Subject;
    Observer* observer1 = new Observer(*subject);
    Observer* observer2 = new Observer(*subject);
    Observer* observer3 = new Observer(*subject);

    subject->CreateMessage("Hello World! :D");
    observer3->RemoveMeFromTheList();

    subject->CreateMessage("The weather is hot today! :p");
    Observer* observer4 = new Observer(*subject);

    observer2->RemoveMeFromTheList();
    Observer* observer5 = new Observer(*subject);

    subject->CreateMessage("My new car is great! ;)");
    observer5->RemoveMeFromTheList();

    observer4->RemoveMeFromTheList();
    observer1->RemoveMeFromTheList();

    delete observer5;
    delete observer4;
    delete observer3;
    delete observer2;
    delete observer1;
    delete subject;
}

int main() {
    ClientCode();
    return 0;
}
#include<bits/stdc++.h>
using namespace std;

class Group; // Forward declaration
class Expense; // Forward declaration

// ----------------- USER ---------------- //
class User {
public:
    std::string user_id;
    std::string name;
    std::string email;
    double balance;
    std::vector<Group*> groups;
    std::vector<Expense*> expenses;

    User(std::string id, std::string n, std::string e) : user_id(id), name(n), email(e), balance(0) {}

    void addExpense(Expense* expense) {
        expenses.push_back(expense);
    }
};
// --------------- END USER -------------- //



// ----------------- GROUP --------------- //
class Group {
public:
    std::string group_id;
    std::string name;
    std::vector<User*> members;
    std::vector<Expense*> expenses;

    Group(std::string id, std::string n) : group_id(id), name(n) {}

    void addMember(User* user) {
        members.push_back(user);
        user->groups.push_back(this);
    }

    void addExpense(Expense* expense) {
        expenses.push_back(expense);
    }
};
// --------------- END GROUP ------------- //



// ---------------- EXPENSE -------------- //
class Split {
public:
    User* user;
    double amount;

    Split(User* u, double a) : user(u), amount(a) {}
};

class Expense {
public:
    std::string expense_id;
    double amount;
    User* paid_by;
    std::vector<User*> split_between;
    Group* group;
    std::vector<Split> splits;

    Expense(std::string id, double amt, User* pb, Group* g) :
        expense_id(id), amount(amt), paid_by(pb), group(g) {}

    void calculateSplits() {
        double split_amount = amount / split_between.size();
        for (User* user : split_between) {
            splits.emplace_back(user, split_amount);
            user->balance -= split_amount;
        }
        paid_by->balance += amount;
    }
};
// -------------- END EXPENSE ------------ //



// ---------------- PAYMENT -------------- //
class Payment {
public:
    std::string payment_id;
    double amount;
    User* paid_by;
    User* paid_to;
    time_t timestamp;

    Payment(std::string id, double amt, User* pb, User* pt) :
        payment_id(id), amount(amt), paid_by(pb), paid_to(pt), timestamp(std::time(nullptr)) {}

    void processPayment() {
        paid_by->balance -= amount;
        paid_to->balance += amount;
    }
};
// -------------- END PAYMENT ------------ //



int main() {
    // Create users
    User user1("U1", "Alice", "alice@example.com");
    User user2("U2", "Bob", "bob@example.com");
    User user3("U3", "Charlie", "charlie@example.com");

    // Create group
    Group group("G1", "Friends");

    // Add users to group
    group.addMember(&user1);
    group.addMember(&user2);
    group.addMember(&user3);

    // Create expense
    Expense expense1("E1", 300.0, &user1, &group);
    expense1.split_between = {&user1, &user2, &user3};
    expense1.calculateSplits();

    // Add expense to group
    group.addExpense(&expense1);

    // Process payment
    Payment payment1("P1", 100.0, &user2, &user1);
    payment1.processPayment();

    // Output balances
    std::cout << "Balances:" << std::endl;
    std::cout << user1.name << ": " << user1.balance << std::endl;
    std::cout << user2.name << ": " << user2.balance << std::endl;
    std::cout << user3.name << ": " << user3.balance << std::endl;

    return 0;
}

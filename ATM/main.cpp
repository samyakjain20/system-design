#include<bits/stdc++.h>
#include <ctime>
using namespace std;


enum class TransactionType {
    WITHDRAWAL,
    DEPOSIT,
    BALANCE_INQUIRY
};

enum class TransactionStatus {
    SUCCESS,
    FAILURE,
    PENDING
};

// --------------- CARD -------------- //
class Account; // Forward declaration
class Card {
public:
    std::string card_number;
    std::string expiry_date;
    std::string cvv;
    std::string pin;
    Account* account;

    Card(std::string cn, std::string ed, std::string c, std::string p, Account* a) : 
        card_number(cn), expiry_date(ed), cvv(c), pin(p), account(a) {}

    bool validatePin(const std::string& entered_pin) {
        return pin == entered_pin;
    }
};
// ------------- END CARD ------------ //



// ------------- ACCOUNT ------------- //
class Account {
public:
    std::string account_number;
    double balance;

    Account(std::string an, double b) : account_number(an), balance(b) {}

    bool withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            return true;
        }
        return false;
    }

    void deposit(double amount) {
        balance += amount;
    }

    double getBalance() const {
        return balance;
    }
};
// ----------- END ACCOUNT ----------- //



// ----------- TRANSACTION ----------- //
class Transaction {
public:
    std::string transaction_id;
    double amount;
    time_t timestamp;
    TransactionStatus status;
    TransactionType type;
    Account* account;

    Transaction(std::string tid, double amt, TransactionType t, Account* acc) :
        transaction_id(tid), amount(amt), type(t), account(acc), status(TransactionStatus::PENDING), timestamp(std::time(nullptr)) {}

    virtual void execute() = 0; // Pure virtual function to make Transaction an abstract class
};
// --------- END TRANSACTION --------- //



// -------- TRANSACTION TYPES -------- //
class WithdrawalTransaction : public Transaction {
public:
    WithdrawalTransaction(std::string tid, double amt, Account* acc) :
        Transaction(tid, amt, TransactionType::WITHDRAWAL, acc) {}

    void execute() override {
        if (account->withdraw(amount)) {
            status = TransactionStatus::SUCCESS;
        } else {
            status = TransactionStatus::FAILURE;
        }
    }
};

class DepositTransaction : public Transaction {
public:
    DepositTransaction(std::string tid, double amt, Account* acc) :
        Transaction(tid, amt, TransactionType::DEPOSIT, acc) {}

    void execute() override {
        account->deposit(amount);
        status = TransactionStatus::SUCCESS;
    }
};

class BalanceInquiryTransaction : public Transaction {
public:
    BalanceInquiryTransaction(std::string tid, Account* acc) :
        Transaction(tid, 0.0, TransactionType::BALANCE_INQUIRY, acc) {}

    void execute() override {
        status = TransactionStatus::SUCCESS;
    }
};
// -------- TRANSACTION TYPES -------- //



// ------- TRANSACTION FACTORY ------- //
class TransactionFactory {
public:
    static Transaction* createTransaction(TransactionType type, std::string tid, double amount, Account* account) {
        switch (type) {
            case TransactionType::WITHDRAWAL:
                return new WithdrawalTransaction(tid, amount, account);
            case TransactionType::DEPOSIT:
                return new DepositTransaction(tid, amount, account);
            case TransactionType::BALANCE_INQUIRY:
                return new BalanceInquiryTransaction(tid, account);
            default:
                return nullptr;
        }
    }
};
// ----- END TRANSACTION FACTORY ----- //



// ---------------- ATM -------------- //
class Bank; // Forward declaration
class ATM {
private:
    static ATM* instance;
    ATM(std::string id, std::string loc, double cash, Bank* b) :
        atm_id(id), location(loc), cash_available(cash), bank(b), current_card(nullptr) {}

public:
    std::string atm_id;
    std::string location;
    double cash_available;
    Bank* bank;
    Card* current_card;

    static ATM* getInstance(std::string id, std::string loc, double cash, Bank* b) {
        if (instance == nullptr) {
            instance = new ATM(id, loc, cash, b);
        }
        return instance;
    }

    void insertCard(Card* card) {
        current_card = card;
        std::cout << "Card inserted.\n";
    }

    void ejectCard() {
        current_card = nullptr;
        std::cout << "Card ejected.\n";
    }

    bool enterPin(const std::string& pin) {
        if (current_card && current_card->validatePin(pin)) {
            std::cout << "PIN validated.\n";
            return true;
        }
        std::cout << "Invalid PIN.\n";
        return false;
    }

    void selectTransaction(TransactionType type, double amount = 0.0) {
        Transaction* transaction = TransactionFactory::createTransaction(type, "T" + std::to_string(std::time(nullptr)), amount, current_card->account);
        if (transaction) {
            performTransaction(transaction);
            delete transaction;
        }
    }

    void performTransaction(Transaction* transaction) {
        transaction->execute();
        if (transaction->status == TransactionStatus::SUCCESS) {
            std::cout << "Transaction successful.\n";
            if (transaction->type == TransactionType::WITHDRAWAL) {
                dispenseCash(transaction->amount);
            } else if (transaction->type == TransactionType::BALANCE_INQUIRY) {
                std::cout << "Current balance: " << transaction->account->getBalance() << "\n";
            }
        } else {
            std::cout << "Transaction failed.\n";
        }
    }

    void dispenseCash(double amount) {
        if (cash_available >= amount) {
            cash_available -= amount;
            std::cout << "Dispensed: " << amount << "\n";
        } else {
            std::cout << "Insufficient cash in ATM.\n";
        }
    }
};
ATM* ATM::instance = nullptr;
// ------------- END ATM ------------- //



// --------------- BANK -------------- //
class Bank {
private:
    static Bank* instance;
    Bank(std::string id, std::string n) : bank_id(id), name(n) {}

public:
    std::string bank_id;
    std::string name;
    std::vector<Account> accounts;

    static Bank* getInstance(std::string id, std::string n) {
        if (instance == nullptr) {
            instance = new Bank(id, n);
        }
        return instance;
    }

    void addAccount(Account account) {
        accounts.push_back(account);
    }

    Account* findAccount(const std::string& account_number) {
        for (auto& account : accounts) {
            if (account.account_number == account_number) {
                return &account;
            }
        }
        return nullptr;
    }
};
Bank* Bank::instance = nullptr;
// ------------- END BANK ------------ //



int main() {
    // Create bank (Singleton)
    Bank* bank = Bank::getInstance("B1", "Sample Bank");

    // Create accounts
    Account alice_account("A1", 1000.0);
    Account bob_account("A2", 500.0);

    // Add accounts to bank
    bank->addAccount(alice_account);
    bank->addAccount(bob_account);

    // Create cards
    Card alice_card("C1", "12/25", "123", "1234", &alice_account);
    Card bob_card("C2", "12/25", "456", "5678", &bob_account);

    // Create ATM (Singleton)
    ATM* atm = ATM::getInstance("ATM1", "Main Street", 10000.0, bank);

    // Simulate Alice using the ATM
    atm->insertCard(&alice_card);
    if (atm->enterPin("1234")) {
        atm->selectTransaction(TransactionType::WITHDRAWAL, 100.0);
        atm->selectTransaction(TransactionType::BALANCE_INQUIRY);
    }
    atm->ejectCard();

    // Simulate Bob using the ATM
    atm->insertCard(&bob_card);
    if (atm->enterPin("5678")) {
        atm->selectTransaction(TransactionType::DEPOSIT, 50.0);
        atm->selectTransaction(TransactionType::BALANCE_INQUIRY);
    }
    atm->ejectCard();

    return 0;
}

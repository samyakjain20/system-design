#include<bits/stdc++.h>
using namespace std;


enum class ActivityType {
    LOGIN,
    VIEW_PAGE,
    LOGOUT,
    OTHER // For any other type of activity
};


// ------------- ACTIVITY ------------ //
class Activity {
public:
    std::string activity_id;
    std::string description;

    Activity(std::string id, std::string desc) : activity_id(id), description(desc) {}
};
// ----------- END ACTIVITY ---------- //



// ----------- ACTIVITYLOG ----------- //
class ActivityLog {
public:
    std::string log_id;
    std::string user_id;
    std::string activity_id;
    time_t timestamp;

    ActivityLog(std::string id, std::string uid, std::string aid) : log_id(id), user_id(uid), activity_id(aid), timestamp(std::time(nullptr)) {}
};
// --------- END ACTIVITYLOG --------- //



// --------------- USER -------------- //
class User {
// -- IMP: Acts as Subject in Observer Pattern
private:
    std::vector<Logger*> loggers;
    std::vector<ActivityLog> activity_logs;

public:
    std::string user_id;
    std::string name;
    std::string email;

    User(std::string id, std::string n, std::string e) : user_id(id), name(n), email(e) {}

    void addActivityLog(const Activity& activity) {
        std::string log_id = "LOG" + std::to_string(activity_logs.size() + 1);
        ActivityLog log(log_id, user_id, activity.activity_id);
        activity_logs.push_back(log);
        notify(log_id, activity.activity_id);
    }

    void attach(Logger* logger) {
        loggers.push_back(logger);
    }

    void detach(Logger* logger) {
        loggers.erase(std::remove(loggers.begin(), loggers.end(), logger), loggers.end());
    }

    void notify(const std::string& log_id, const std::string& activity_id) {
        for (Logger* logger : loggers) {
            logger->update(log_id, user_id, activity_id, std::time(nullptr));
        }
    }
};
// ------------- END USER ------------ //


// -------------- LOGGER ------------- //
class Logger {
// acts as observer
private:
    std::vector<ActivityLog> logs;
    static Logger* instance;

    // Private constructor to prevent instantiation
    Logger() {}

public:
    static Logger* getInstance() {
        if (instance == nullptr) {
            instance = new Logger();
        }
        return instance;
    }

    void update(const std::string& log_id, const std::string& user_id, const std::string& activity_id, time_t timestamp) {
        logs.emplace_back(log_id, user_id, activity_id);
        logs.back().timestamp = timestamp;
        std::cout << "ActivityLogger: New log entry - Log ID: " << log_id << ", User ID: " << user_id << ", Activity ID: " << activity_id << ", Timestamp: " << timestamp << std::endl;
    }

    std::vector<ActivityLog> getLogs() {
        return logs;
    }
};
// Initialize the static member
Logger* Logger::instance = nullptr;
// ------------ END LOGGER ----------- //



int main() {
    // Create activities
    Activity login("ACT1", "User logged in");
    Activity viewPage("ACT2", "User viewed a page");
    Activity logout("ACT3", "User logged out");

    // Create users
    User alice("U1", "Alice", "alice@example.com");
    User bob("U2", "Bob", "bob@example.com");

    // Get the singleton instance of the activity logger
    Logger* logger = Logger::getInstance();

    
    // Attach the logger as an observer to the users
    alice.attach(logger);
    bob.attach(logger);

    // Log some activities
    alice.addActivityLog(login);
    alice.addActivityLog(viewPage);
    bob.addActivityLog(login);
    bob.addActivityLog(logout);

    // Print activity logs
    for (const auto& log : logger->getLogs()) {
        std::cout << "Log ID: " << log.log_id << ", User: " << log.user_id << ", Activity: " << log.activity_id << ", Timestamp: " << log.timestamp << std::endl;
    }

    return 0;
}

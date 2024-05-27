#include<bits/stdc++.h>
#include <ctime>
using namespace std;


enum RideStatus {
    REQUESTED,
    ACCEPTED,
    IN_PROGRESS,
    COMPLETED,
    CANCELLED
};


// ------------- LOCATION ------------ //
class Location {
public:
    double latitude;
    double longitude;
    std::string address;

    Location(double lat, double lon, std::string addr) : latitude(lat), longitude(lon), address(addr) {}
};
// ----------- END LOCATION ---------- //



// ---------------- CAR -------------- //
class User; // Forward declaration
class Car {
public:
    std::string car_id;
    std::string license_plate;
    std::string model;
    User* driver;

    Car(std::string id, std::string plate, std::string mod, User* drv) :
        car_id(id), license_plate(plate), model(mod), driver(drv) {}
};
// -------------- END CAR ------------ //



// --------------- RIDE -------------- //
class Ride {
public:
    std::string ride_id;
    User* rider;
    User* driver;
    Location* pickup_location;
    Location* dropoff_location;
    double fare;
    RideStatus status;

    Ride(std::string id, User* r, Location* pickup, Location* dropoff) :
        ride_id(id), rider(r), driver(nullptr), pickup_location(pickup), dropoff_location(dropoff),
        fare(0.0), status(REQUESTED) {}

    double calculateFare() {
        // Implement fare calculation based on distance and time
        // For simplicity, we'll assume a fixed fare
        return 50.0;
    }

    void updateStatus(RideStatus new_status) {
        status = new_status;
    }
};
// ------------- END RIDE ------------ //



// ------------- PAYMENT ------------- //
class Payment {
public:
    std::string payment_id;
    double amount;
    time_t timestamp;

    Payment(std::string id, double amt) : payment_id(id), amount(amt), timestamp(std::time(nullptr)) {}

    bool processPayment(double amt) {
        // Here we would integrate with a real payment gateway
        if (amt >= amount) {
            return true;
        }
        return false;
    }
};
// ----------- END PAYMENT ----------- //



// --------------- USER -------------- //
class User {
public:
    std::string user_id;
    std::string name;
    std::string email;
    bool is_driver;
    Car* car;
    Ride* current_ride;

    User(std::string id, std::string n, std::string e, bool driver = false) :
        user_id(id), name(n), email(e), is_driver(driver), car(nullptr), current_ride(nullptr) {}

    Ride* requestRide(Location* pickup, Location* dropoff) {
        Ride* ride = new Ride("RIDE1", this, pickup, dropoff);
        current_ride = ride;
        return ride;
    }

    bool acceptRide(Ride* ride) {
        if (is_driver && car != nullptr && ride->status == REQUESTED) {
            ride->driver = this;
            ride->updateStatus(ACCEPTED);
            current_ride = ride;
            return true;
        }
        return false;
    }

    bool completeRide() {
        if (is_driver && current_ride != nullptr && current_ride->status == IN_PROGRESS) {
            current_ride->updateStatus(COMPLETED);
            current_ride->fare = current_ride->calculateFare();
            return true;
        }
        return false;
    }
};
// ------------- END USER ------------ //


int main() {
    // Create locations
    Location pickup(37.7749, -122.4194, "123 Market St, San Francisco, CA");
    Location dropoff(37.7849, -122.4094, "456 Mission St, San Francisco, CA");

    // Create users
    User rider("U1", "Alice", "alice@example.com");
    User driver("U2", "Bob", "bob@example.com", true);

    // Create car
    Car car("CAR1", "XYZ123", "Toyota Prius", &driver);
    driver.car = &car;

    // Rider requests a ride
    Ride* ride = rider.requestRide(&pickup, &dropoff);
    std::cout << "Ride requested by " << rider.name << " from " << pickup.address << " to " << dropoff.address << std::endl;

    // Driver accepts the ride
    if (driver.acceptRide(ride)) {
        std::cout << "Ride accepted by " << driver.name << std::endl;
        ride->updateStatus(IN_PROGRESS);
    }

    // Driver completes the ride
    if (driver.completeRide()) {
        std::cout << "Ride completed by " << driver.name << std::endl;
        std::cout << "Fare: $" << ride->fare << std::endl;
    }

    return 0;
}

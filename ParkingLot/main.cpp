#include<bits/stdc++.h>
#include <ctime>
using namespace std;

enum class SpotSize { SMALL, MEDIUM, LARGE };


// ------------- VEHICLE -------------- //
class Vehicle {
public:
    std::string license_plate;
    SpotSize size;

    Vehicle(std::string plate, SpotSize s) : license_plate(plate), size(s) {}

    virtual SpotSize getSize() = 0;
};

class Car : public Vehicle {
public:
    Car(std::string plate) : Vehicle(plate, SpotSize::MEDIUM) {}

    SpotSize getSize() override {
        return SpotSize::MEDIUM;
    }
};

class Truck : public Vehicle {
public:
    Truck(std::string plate) : Vehicle(plate, SpotSize::LARGE) {}

    SpotSize getSize() override {
        return SpotSize::LARGE;
    }
};

class Motorcycle : public Vehicle {
public:
    Motorcycle(std::string plate) : Vehicle(plate, SpotSize::SMALL) {}

    SpotSize getSize() override {
        return SpotSize::SMALL;
    }
};
// ----------- END VEHICLE ------------ //



// ------------- TICKET -------------- //
class Ticket {
public:
    std::string ticket_id;
    Vehicle* vehicle;
    time_t entry_time;
    time_t exit_time;
    bool paid;

    Ticket(std::string id, Vehicle* v) : ticket_id(id), vehicle(v), paid(false) {
        entry_time = std::time(nullptr);
    }

    double calculateFee() {
        exit_time = std::time(nullptr);
        double duration = difftime(exit_time, entry_time) / 3600.0; // hours
        double rate = 2.0; // assuming a flat rate of $2 per hour
        return duration * rate;
    }

    void markPaid() {
        paid = true;
    }
};
// ----------- END TICKET ------------ //



// ------------- PAYMENT ------------- //
class Payment {
public:
    Ticket* ticket;
    double amount;
    time_t payment_time;

    Payment(Ticket* t, double amt) : ticket(t), amount(amt) {
        payment_time = std::time(nullptr);
    }

    bool processPayment(double amount) {
        if (amount >= ticket->calculateFee()) {
            ticket->markPaid();
            return true;
        }
        return false;
    }
};
// ----------- END PAYMENT ----------- //



// ----------- PARKINGSPOT ------------ //
class ParkingSpot {
public:
    int spot_number;
    SpotSize size;
    bool is_occupied;
    Vehicle* vehicle;

    ParkingSpot(int number, SpotSize s) : spot_number(number), size(s), is_occupied(false), vehicle(nullptr) {}

    bool assignVehicle(Vehicle* v) {
        if (!is_occupied && v->getSize() <= size) {
            vehicle = v;
            is_occupied = true;
            return true;
        }
        return false;
    }

    bool removeVehicle() {
        if (is_occupied) {
            vehicle = nullptr;
            is_occupied = false;
            return true;
        }
        return false;
    }
};
// --------- END PARKINGSPOT ---------- //



// ----------- PARKINGLEVEL ----------- //
class ParkingLevel {
public:
    int level_number;
    std::vector<ParkingSpot> spots;
    int capacity;
    int occupied_spots;

    ParkingLevel(int number) : level_number(number), capacity(0), occupied_spots(0) {}

    void addSpot(ParkingSpot spot) {
        spots.push_back(spot);
        capacity++;
    }

    bool parkVehicle(Vehicle* vehicle) {
        for (auto& spot : spots) {
            if (spot.assignVehicle(vehicle)) {
                occupied_spots++;
                return true;
            }
        }
        return false;
    }

    bool leaveVehicle(Vehicle* vehicle) {
        for (auto& spot : spots) {
            if (spot.is_occupied && spot.vehicle == vehicle) {
                spot.removeVehicle();
                occupied_spots--;
                return true;
            }
        }
        return false;
    }

    int getAvailableSpots() {
        return capacity - occupied_spots;
    }
};
// --------- END PARKINGLEVEL --------- //



// --------- PARKINGLOT --------- //
class ParkingLot {
public:
    std::vector<ParkingLevel> levels;
    int capacity;
    int occupied_spots;
    std::vector<Ticket> tickets;

    ParkingLot() : capacity(0), occupied_spots(0) {}

    void addLevel(ParkingLevel level) {
        levels.push_back(level);
        capacity += level.capacity;
    }

    Ticket issueTicket(Vehicle* vehicle) {
        std::string ticket_id = "TICKET_" + std::to_string(tickets.size() + 1);
        Ticket ticket(ticket_id, vehicle);
        tickets.push_back(ticket);
        return ticket;
    }

    bool parkVehicle(Ticket& ticket) {
        for (auto& level : levels) {
            if (level.parkVehicle(ticket.vehicle)) {
                occupied_spots++;
                return true;
            }
        }
        return false;
    }

    bool leaveVehicle(Ticket& ticket) {
        if (!ticket.paid) {
            return false;
        }
        for (auto& level : levels) {
            if (level.leaveVehicle(ticket.vehicle)) {
                occupied_spots--;
                return true;
            }
        }
        return false;
    }

    bool payForParking(Ticket& ticket) {
        double fee = ticket.calculateFee();
        Payment payment(&ticket, fee);
        return payment.processPayment(fee);
    }

    int getAvailableSpots() {
        return capacity - occupied_spots;
    }
};
// --------- END PARKINGLOT --------- //



// OPERATIONS - EXAMPLE INTERACTION
int main(){
    // Create parking lot
    ParkingLot parkingLot;

    // Create levels and spots
    ParkingLevel level1(1);
    level1.addSpot(ParkingSpot(1, SpotSize::SMALL));
    level1.addSpot(ParkingSpot(2, SpotSize::MEDIUM));
    level1.addSpot(ParkingSpot(3, SpotSize::LARGE));

    // Add levels to parking lot
    parkingLot.addLevel(level1);

    // Create vehicles
    Car car("ABC123");
    Truck truck("XYZ789");
    Motorcycle motorcycle("MOTO456");

    // Issue tickets for vehicles
    Ticket carTicket = parkingLot.issueTicket(&car);
    Ticket truckTicket = parkingLot.issueTicket(&truck);
    Ticket motorcycleTicket = parkingLot.issueTicket(&motorcycle);

    // Park vehicles
    parkingLot.parkVehicle(carTicket);
    parkingLot.parkVehicle(truckTicket);
    parkingLot.parkVehicle(motorcycleTicket);

    // Get available spots
    int availableSpots = parkingLot.getAvailableSpots(); // Should be 0 if all spots are occupied

    // Pay for parking
    parkingLot.payForParking(carTicket);
    parkingLot.payForParking(truckTicket);
    parkingLot.payForParking(motorcycleTicket);

    // Leave vehicles after payment
    parkingLot.leaveVehicle(carTicket);
    parkingLot.leaveVehicle(truckTicket);
    parkingLot.leaveVehicle(motorcycleTicket);

    // Get available spots
    availableSpots = parkingLot.getAvailableSpots(); // Should be 3 after all vehicles leave
}
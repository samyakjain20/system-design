#include<bits/stdc++.h>
using namespace std;


enum Direction {
    UP,
    DOWN,
    STILL
};
enum ElevatorStatus {
    MOVING,
    STOPPED,
    IDLE
};


// -------------- REQUEST ------------ //
class ElevatorRequest {
public:
    int floor;
    Direction direction;

    ElevatorRequest(int f, Direction d) : floor(f), direction(d) {}
};
// ----------- END REQUEST ----------- //



// ------------- ELEVATOR ------------ //
class Elevator {
public:
    std::string elevator_id;
    int current_floor;
    Direction direction;
    ElevatorStatus status;
    std::vector<ElevatorRequest> requests;

    Elevator(std::string id, int floor) :
        elevator_id(id), current_floor(floor), direction(STILL), status(IDLE) {}

    void addRequest(ElevatorRequest request) {
        requests.push_back(request);
        if (status == STILL) {
            status = MOVING;
        }
    }

    void move() {
        if (requests.empty()) {
            status = IDLE;
            direction = STILL;
            return;
        }

        ElevatorRequest next_request = requests.front();
        requests.erase(requests.begin());

        if (current_floor < next_request.floor) {
            direction = UP;
            current_floor++;
        } else if (current_floor > next_request.floor) {
            direction = DOWN;
            current_floor--;
        } else {
            direction = STILL;
            status = STOPPED;
        }

        if (current_floor == next_request.floor) {
            std::cout << "Elevator " << elevator_id << " has arrived at floor " << current_floor << std::endl;
            status = STOPPED;
        } else {
            status = MOVING;
        }
    }
};
// ----------- END ELEVATOR ---------- //



// ------------- BUILDING ----------- //
class Building {
public:
    std::string building_id;
    int floors;
    std::vector<Elevator> elevators;

    Building(std::string id, int fl) : building_id(id), floors(fl) {}

    void addElevator(Elevator elevator) {
        elevators.push_back(elevator);
    }

    Elevator* requestElevator(int floor, Direction direction) {
        // Simple logic to find the closest idle elevator
        Elevator* closest_elevator = nullptr;
        int min_distance = floors + 1;

        for (auto& elevator : elevators) {
            if (elevator.status == IDLE) {
                int distance = abs(elevator.current_floor - floor);
                if (distance < min_distance) {
                    min_distance = distance;
                    closest_elevator = &elevator;
                }
            }
        }

        if (closest_elevator != nullptr) {
            closest_elevator->addRequest(ElevatorRequest(floor, direction));
        } else {
            std::cout << "No idle elevator available at the moment.\n";
        }

        return closest_elevator;
    }
};
// ----------- END BUILDING ---------- //


int main() {
    // Create a building with 10 floors
    Building building("BUILD1", 10);

    // Create elevators
    Elevator elevator1("ELEV1", 0);
    Elevator elevator2("ELEV2", 5);

    // Add elevators to the building
    building.addElevator(elevator1);
    building.addElevator(elevator2);

    // Request an elevator to go to the 7th floor, direction UP
    Elevator* elevator = building.requestElevator(7, UP);

    // Simulate the elevator movement
    while (elevator->status != IDLE) {
        elevator->move();
    }
    return 0;
}
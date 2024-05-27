CREATE TABLE Vehicles (
    vehicle_id INT PRIMARY KEY AUTO_INCREMENT,
    license_plate VARCHAR(15) UNIQUE NOT NULL,
    size ENUM('SMALL', 'MEDIUM', 'LARGE') NOT NULL
);

CREATE TABLE Tickets (
    ticket_id INT PRIMARY KEY AUTO_INCREMENT,
    ticket_code VARCHAR(20) UNIQUE NOT NULL,
    vehicle_id INT,
    entry_time TIMESTAMP NOT NULL,
    exit_time TIMESTAMP NULL,
    paid BOOLEAN DEFAULT FALSE,
    FOREIGN KEY (vehicle_id) REFERENCES Vehicles(vehicle_id)
);

CREATE TABLE Payments (
    payment_id INT PRIMARY KEY AUTO_INCREMENT,
    ticket_id INT,
    amount DECIMAL(10, 2) NOT NULL,
    payment_time TIMESTAMP NOT NULL,
    FOREIGN KEY (ticket_id) REFERENCES Tickets(ticket_id)
);

CREATE TABLE ParkingSpots (
    spot_id INT PRIMARY KEY AUTO_INCREMENT,
    spot_number INT NOT NULL,
    size ENUM('SMALL', 'MEDIUM', 'LARGE') NOT NULL,
    is_occupied BOOLEAN DEFAULT FALSE,
    vehicle_id INT NULL,
    level_id INT,
    FOREIGN KEY (vehicle_id) REFERENCES Vehicles(vehicle_id),
    FOREIGN KEY (level_id) REFERENCES ParkingLevels(level_id)
);

CREATE TABLE ParkingLevels (
    level_id INT PRIMARY KEY AUTO_INCREMENT,
    level_number INT NOT NULL,
    capacity INT NOT NULL,
    occupied_spots INT DEFAULT 0,
    parkinglot_id INT,
    FOREIGN KEY (parkinglot_id) REFERENCES ParkingLot(parkinglot_id)
);

CREATE TABLE ParkingLot (
    parkinglot_id INT PRIMARY KEY AUTO_INCREMENT,
    capacity INT NOT NULL,
    occupied_spots INT DEFAULT 0
);

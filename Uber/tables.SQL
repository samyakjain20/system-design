CREATE TABLE Users (
    user_id VARCHAR(50) PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    is_driver BOOLEAN NOT NULL
);

CREATE TABLE Cars (
    car_id VARCHAR(50) PRIMARY KEY,
    license_plate VARCHAR(50) NOT NULL,
    model VARCHAR(100) NOT NULL,
    driver_id VARCHAR(50),
    FOREIGN KEY (driver_id) REFERENCES Users(user_id)
);

CREATE TABLE Locations (
    location_id VARCHAR(50) PRIMARY KEY,
    latitude DOUBLE NOT NULL,
    longitude DOUBLE NOT NULL,
    address VARCHAR(255) NOT NULL
);

CREATE TABLE Rides (
    ride_id VARCHAR(50) PRIMARY KEY,
    rider_id VARCHAR(50),
    driver_id VARCHAR(50),
    pickup_location_id VARCHAR(50),
    dropoff_location_id VARCHAR(50),
    fare DECIMAL(10,2) NOT NULL,
    status VARCHAR(20) NOT NULL,
    FOREIGN KEY (rider_id) REFERENCES Users(user_id),
    FOREIGN KEY (driver_id) REFERENCES Users(user_id),
    FOREIGN KEY (pickup_location_id) REFERENCES Locations(location_id),
    FOREIGN KEY (dropoff_location_id) REFERENCES Locations(location_id)
);

CREATE TABLE Payments (
    payment_id VARCHAR(50) PRIMARY KEY,
    ride_id VARCHAR(50),
    amount DECIMAL(10,2) NOT NULL,
    timestamp TIMESTAMP NOT NULL,
    FOREIGN KEY (ride_id) REFERENCES Rides(ride_id)
);

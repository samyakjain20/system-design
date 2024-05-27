CREATE TABLE Users (
    user_id VARCHAR(50) PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL
);

CREATE TABLE Movies (
    movie_id VARCHAR(50) PRIMARY KEY,
    title VARCHAR(100) NOT NULL,
    duration INT NOT NULL,
    genre VARCHAR(50) NOT NULL,
    language VARCHAR(50) NOT NULL
);

CREATE TABLE Theaters (
    theater_id VARCHAR(50) PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    address VARCHAR(255) NOT NULL
);

CREATE TABLE Screens (
    screen_id VARCHAR(50) PRIMARY KEY,
    theater_id VARCHAR(50),
    name VARCHAR(50) NOT NULL,
    FOREIGN KEY (theater_id) REFERENCES Theaters(theater_id)
);

CREATE TABLE Seats (
    seat_id VARCHAR(50) PRIMARY KEY,
    screen_id VARCHAR(50),
    seat_number VARCHAR(10) NOT NULL,
    is_booked BOOLEAN DEFAULT FALSE,
    FOREIGN KEY (screen_id) REFERENCES Screens(screen_id)
);

CREATE TABLE Shows (
    show_id VARCHAR(50) PRIMARY KEY,
    movie_id VARCHAR(50),
    screen_id VARCHAR(50),
    start_time TIMESTAMP NOT NULL,
    FOREIGN KEY (movie_id) REFERENCES Movies(movie_id),
    FOREIGN KEY (screen_id) REFERENCES Screens(screen_id)
);

CREATE TABLE Bookings (
    booking_id VARCHAR(50) PRIMARY KEY,
    user_id VARCHAR(50),
    show_id VARCHAR(50),
    total_amount DECIMAL(10,2) NOT NULL,
    FOREIGN KEY (user_id) REFERENCES Users(user_id),
    FOREIGN KEY (show_id) REFERENCES Shows(show_id)
);

CREATE TABLE BookingSeats (
    booking_id VARCHAR(50),
    seat_id VARCHAR(50),
    PRIMARY KEY (booking_id, seat_id),
    FOREIGN KEY (booking_id) REFERENCES Bookings(booking_id),
    FOREIGN KEY (seat_id) REFERENCES Seats(seat_id)
);

CREATE TABLE Payments (
    payment_id VARCHAR(50) PRIMARY KEY,
    booking_id VARCHAR(50),
    amount DECIMAL(10,2) NOT NULL,
    payment_time TIMESTAMP NOT NULL,
    FOREIGN KEY (booking_id) REFERENCES Bookings(booking_id)
);

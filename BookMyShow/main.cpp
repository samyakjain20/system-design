#include<bits/stdc++.h>
#include <ctime>
using namespace std;


class Show; // Forward declaration
class Booking;
class Seat;
// --------------- USER -------------- //
class User {
public:
    std::string user_id;
    std::string name;
    std::string email;
    std::vector<Booking> bookings;

    User(std::string id, std::string n, std::string e) : user_id(id), name(n), email(e) {}

    Booking makeBooking(Show& show, const std::vector<Seat>& seats);
    bool cancelBooking(const std::string& booking_id);
};
// ------------- END USER ------------ //



// -------------- MOVIE -------------- //
class Movie {
public:
    std::string movie_id;
    std::string title;
    int duration;
    std::string genre;
    std::string language;
    std::vector<Show> shows;

    Movie(std::string id, std::string t, int d, std::string g, std::string l) :
        movie_id(id), title(t), duration(d), genre(g), language(l) {}

    void addShow(const Show& show) {
        shows.push_back(show);
    }
};
// ------------ END MOVIE ------------ //



// ------------- THEATRE ------------- //
class Screen; // Forward declaration
class Theater {
public:
    std::string theater_id;
    std::string name;
    std::string address;
    std::vector<Screen> screens;

    Theater(std::string id, std::string n, std::string a) : theater_id(id), name(n), address(a) {}

    void addScreen(const Screen& screen) {
        screens.push_back(screen);
    }
};
// ----------- END THEATRE ----------- //



// -------------- SCREEN ------------- //
class Screen {
public:
    std::string screen_id;
    Theater* theater;
    std::vector<Seat> seats;
    std::vector<Show> shows;

    Screen(std::string id, Theater* t) : screen_id(id), theater(t) {}

    void addShow(const Show& show) {
        shows.push_back(show);
    }

    void addSeats(const std::vector<Seat>& s) {
        seats.insert(seats.end(), s.begin(), s.end());
    }
};
// ------------ END SCREEN ----------- //



// --------------- SHOW -------------- //
class Show {
public:
    std::string show_id;
    Movie* movie;
    Screen* screen;
    time_t start_time;
    std::vector<Seat> seats;

    Show(std::string id, Movie* m, Screen* s, time_t t) : show_id(id), movie(m), screen(s), start_time(t) {}

    std::vector<Seat> getAvailableSeats() const;
    Booking bookSeats(User& user, const std::vector<Seat>& seats);
};
// ------------- END SHOW ------------ //



// --------------- SEAT -------------- //
class Seat {
public:
    std::string seat_id;
    std::string seat_number;
    bool is_booked;

    Seat(std::string id, std::string number) : seat_id(id), seat_number(number), is_booked(false) {}

    void book() {
        is_booked = true;
    }

    void unbook() {
        is_booked = false;
    }
};
// ------------- END SEAT ------------ //



// ------------- BOOKING ------------ //
class Payment; // Forward declaration
class Booking {
public:
    std::string booking_id;
    User* user;
    Show* show;
    std::vector<Seat> seats;
    double total_amount;
    Payment* payment;

    Booking(std::string id, User* u, Show* s, const std::vector<Seat>& st, double amt) :
        booking_id(id), user(u), show(s), seats(st), total_amount(amt), payment(nullptr) {}

    void confirmBooking();
    void cancelBooking();
};
// ------------- END BOOKING ------------ //



// --------------- PAYMENT -------------- //
class Payment {
public:
    std::string payment_id;
    double amount;
    time_t payment_time;

    Payment(std::string id, double amt) : payment_id(id), amount(amt), payment_time(std::time(nullptr)) {}

    bool processPayment(double amt) {
        // Here we would integrate with a real payment gateway
        if (amt >= amount) {
            return true;
        }
        return false;
    }
};
// ------------- END PAYMENT ------------ //



int main() {
    // Create movie
    Movie movie("MOV123", "Inception", 148, "Sci-Fi", "English");

    // Create theater and screen
    Theater theater("TH123", "PVR Cinemas", "123 Main St");
    Screen screen("SCR1", &theater);

    // Add seats to screen
    std::vector<Seat> seats = { Seat("S1", "1A"), Seat("S2", "1B"), Seat("S3", "1C") };
    screen.addSeats(seats);

    // Add screen to theater
    theater.addScreen(screen);

    // Create show
    time_t showTime = std::time(nullptr);
    Show show("SHOW1", &movie, &screen, showTime);
    show.seats = seats;

    // Add show to movie and screen
}
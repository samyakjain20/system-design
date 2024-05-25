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
// Assuming this is a global list of theaters in the system.
extern std::vector<Theater> theaters;
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

    std::vector<Show> listShowsOfMovie() const {
        std::vector<Show> movieShows;
        for (const auto& theater : theaters) {
            for (const auto& screen : theater.screens) {
                for (const auto& show : screen.shows) {
                    if (show.movie == this) {
                        movieShows.push_back(show);
                    }
                }
            }
        }
        return movieShows;
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

    std::vector<Show> listShowsInTheater() const {
        std::vector<Show> shows;
        for (const auto& screen : screens) {
            shows.insert(shows.end(), screen.shows.begin(), screen.shows.end());
        }
        return shows;
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

    // Create theaters
    Theater theater1("TH123", "PVR Cinemas", "123 Main St");
    Theater theater2("TH124", "AMC Theaters", "456 Broadway");

    // Create screens
    Screen screen1("SCR1", &theater1);
    Screen screen2("SCR2", &theater2);

    // Add screens to theaters
    theater1.addScreen(screen1);
    theater2.addScreen(screen2);

    // Create shows
    time_t showTime1 = std::time(nullptr);
    time_t showTime2 = showTime1 + 3600; // Another show an hour later

    Show show1("SHOW1", &movie, &screen1, showTime1);
    Show show2("SHOW2", &movie, &screen2, showTime2);

    // Add shows to screens
    screen1.addShow(show1);
    screen2.addShow(show2);

    // Add theaters to global list
    theaters.push_back(theater1);
    theaters.push_back(theater2);

    // List shows in theater1
    std::vector<Show> showsInTheater1 = theater1.listShowsInTheater();
    std::cout << "Shows in " << theater1.name << ":\n";
    for (const auto& show : showsInTheater1) {
        std::cout << "Show ID: " << show.show_id << ", Movie: " << show.movie->title << "\n";
    }

    // List shows of the movie across all theaters
    std::vector<Show> showsOfMovie = movie.listShowsOfMovie();
    std::cout << "Shows of movie " << movie.title << ":\n";
    for (const auto& show : showsOfMovie) {
        std::cout << "Show ID: " << show.show_id << ", Theater: " << show.screen->theater->name << "\n";
    }

    return 0;
}
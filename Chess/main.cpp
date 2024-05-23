#include<bits/stdc++.h>
using namespace std;

enum class Color { WHITE, BLACK };
enum class GameStatus { ONGOING, CHECK, CHECKMATE, STALEMATE };


// -------------- SQAURE ------------- //
class Position {
public:
    int x, y;
};

class Square {
public:
    Position position;
    Piece* piece;

    Square(Position pos) : position(pos), piece(nullptr) {}
};
// ----------- END SQUARE ----------- //



// -------------- PIECE ------------- //
class Piece {
public:
    Color color;
    Position position;

    Piece(Color col, Position pos) : color(col), position(pos) {}

    virtual std::vector<Position> getPossibleMoves(const Board& board) = 0;
};
class King : public Piece {
public:
    King(Color col, Position pos) : Piece(col, pos) {}

    std::vector<Position> getPossibleMoves(const Board& board) override;
};
// Similarly, define other pieces like Queen, Rook, Bishop, Knight, Pawn

// ------------ END PIECE ----------- //



// -------------- PLAYER ------------ //
class Player {
public:
    Color color;
    std::string name;

    Player(Color col, std::string n) : color(col), name(n) {}

    bool makeMove(Position start_pos, Position end_pos, Game& game);
};
// ----------- END PLAYER ---------- //



// -------------- BOARD ------------ //
class Board {
public:
    Square* squares[8][8];

    Board() {
        resetBoard();
    }

    void resetBoard();
    Piece* getPieceAt(Position position);
    bool movePiece(Position start_pos, Position end_pos);
};
// ----------- END BOARD ---------- //



// -------------- GAME ------------ //
class Game {
public:
    Board board;
    Player players[2];
    Player* current_turn;
    GameStatus status;

    Game(Player p1, Player p2) : players{p1, p2}, current_turn(&players[0]), status(GameStatus::ONGOING) {
        initializeGame();
    }

    void initializeGame();
    bool makeMove(Position start_pos, Position end_pos);
    bool isCheck();
    bool isCheckmate();
    bool isStalemate();
};
// ----------- END GAME ---------- //




int main(){

}
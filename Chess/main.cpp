#include<bits/stdc++.h>
using namespace std;

enum class Color { WHITE, BLACK };
enum class GameStatus { ONGOING, CHECK, CHECKMATE, STALEMATE };


// -------------- SQAURE ------------- //
class Position {
public:
    int x, y;
    Position() {}
    Position(int x, int y) : x(x), y(y) {}
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

    bool makeMove(Position start_pos, Position end_pos, Game& game){
    // Validate that it's the player's turn
    if (game.current_turn != this) {
        std::cout << "It's not your turn!\n";
        return false;
    }

    // Validate that the piece at the start position belongs to the player
    Piece* piece = game.board.getPieceAt(start_pos);
    if (!piece || piece->color != color) {
        std::cout << "Invalid move: No piece of yours at the start position.\n";
        return false;
    }

    // Make the move on the board
    if (game.makeMove(start_pos, end_pos)) {
        return true;
    }

    std::cout << "Invalid move.\n";
    return false;
}
};
// ----------- END PLAYER ---------- //



// -------------- BOARD ------------ //
class Board {
public:
    Square* squares[8][8];

    Board() {
        resetBoard();
    }

    void resetBoard(){
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                squares[i][j] = new Square(Position(i, j));
            }
        }

        // Place pieces on the board for the initial setup
        // For simplicity, only Kings are placed; other pieces should be placed similarly
        squares[0][4]->piece = new King(Color::WHITE, Position(0, 4));
        squares[7][4]->piece = new King(Color::BLACK, Position(7, 4));
    }
    Piece* getPieceAt(Position position){
        return squares[position.x][position.y]->piece;
    }
    bool movePiece(Position start_pos, Position end_pos){
        Piece* piece = getPieceAt(start_pos);
        
        if (piece){
            vector<Position> possiblePositions = piece->getPossibleMoves(*this);
            if(std::find(possiblePositions.begin(), possiblePositions.end(), end_pos) != possiblePositions.end()) {
                squares[end_pos.x][end_pos.y]->piece = piece;
                squares[start_pos.x][start_pos.y]->piece = nullptr;
                piece->position = end_pos;
                return true;
            }
        }
        return false;
    }
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

    void Game::initializeGame() {
        board.resetBoard();
    }
    bool Game::makeMove(Position start_pos, Position end_pos) {
        if (board.movePiece(start_pos, end_pos)) {
            current_turn = (current_turn == &players[0]) ? &players[1] : &players[0];
            return true;
        }
        return false;
    }
    bool isCheck();
    bool isCheckmate();
    bool isStalemate();
};
// ----------- END GAME ---------- //


int main() {
    Player player1(Color::WHITE, "Alice");
    Player player2(Color::BLACK, "Bob");

    Game game(player1, player2);

    while (game.status == GameStatus::ONGOING) {
        // Get current player's move
        Position start_pos, end_pos;
        std::cout << game.current_turn->name << "'s turn. Enter start position (x y): ";
        std::cin >> start_pos.x >> start_pos.y;
        std::cout << "Enter end position (x y): ";
        std::cin >> end_pos.x >> end_pos.y;

        // Make the move
        if (game.current_turn->makeMove(start_pos, end_pos, game)) {
            std::cout << "Move successful!\n";
            if (game.isCheckmate()) {
                game.status = GameStatus::CHECKMATE;
                std::cout << game.current_turn->name << " wins by checkmate!\n";
            } else if (game.isStalemate()) {
                game.status = GameStatus::STALEMATE;
                std::cout << "Game ends in stalemate!\n";
            } else if (game.isCheck()) {
                std::cout << "Check!\n";
            }
        } else {
            std::cout << "Move failed. Try again.\n";
        }
    }

    return 0;
}

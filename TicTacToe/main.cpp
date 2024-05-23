#include<bits/stdc++.h>
using namespace std;

enum class Mark { EMPTY, X, O };
enum class GameStatus { ONGOING, DRAW, WIN };



// -------------- CELL ------------- //
class Position {
public:
    int row, col;

    Position(int r, int c) : row(r), col(c) {}
};
class Cell {
public:
    Position position;
    Mark mark;

    Cell(int row, int col) : position(row, col), mark(Mark::EMPTY) {}
};
// ------------ END CELL ----------- //



// ------------- BOARD ------------ //
class Board {
public:
    std::vector<std::vector<Cell>> grid;

    Board() {
        resetBoard();
    }

    void resetBoard() {
        grid.clear();
        for (int i = 0; i < 3; ++i) {
            std::vector<Cell> row;
            for (int j = 0; j < 3; ++j) {
                row.emplace_back(i, j);
            }
            grid.push_back(row);
        }
    }

    bool isCellEmpty(Position position) {
        return grid[position.row][position.col].mark == Mark::EMPTY;
    }

    void placeMark(Position position, Mark mark) {
        grid[position.row][position.col].mark = mark;
    }

    void printBoard() {
        for (const auto& row : grid) {
            for (const auto& cell : row) {
                char markChar = (cell.mark == Mark::X) ? 'X' : (cell.mark == Mark::O) ? 'O' : ' ';
                std::cout << markChar << " ";
            }
            std::cout << std::endl;
        }
    }
};
// ----------- END BOARD ---------- //



// -------------- GAME ------------- //
class Player; // Forward declaration
class Game {
public:
    Board board;
    Player players[2];
    Player* current_turn;
    GameStatus status;

    Game(Player p1, Player p2) : players{p1, p2}, current_turn(&players[0]), status(GameStatus::ONGOING) {
        initializeGame();
    }

    void initializeGame() {
        board.resetBoard();
        current_turn = &players[0];
        status = GameStatus::ONGOING;
    }

    bool makeMove(Position position) {
        if (board.isCellEmpty(position)) {
            board.placeMark(position, current_turn->mark);
            if (checkWin()) {
                status = GameStatus::WIN;
                return true;
            } else if (checkDraw()) {
                status = GameStatus::DRAW;
                return true;
            }
            switchTurn();
            return true;
        }
        return false;
    }

    bool checkWin() {
        // Check rows, columns, and diagonals for a win
        for (int i = 0; i < 3; ++i) {
            if (board.grid[i][0].mark == current_turn->mark &&
                board.grid[i][1].mark == current_turn->mark &&
                board.grid[i][2].mark == current_turn->mark) {
                return true;
            }
            if (board.grid[0][i].mark == current_turn->mark &&
                board.grid[1][i].mark == current_turn->mark &&
                board.grid[2][i].mark == current_turn->mark) {
                return true;
            }
        }
        if (board.grid[0][0].mark == current_turn->mark &&
            board.grid[1][1].mark == current_turn->mark &&
            board.grid[2][2].mark == current_turn->mark) {
            return true;
        }
        if (board.grid[0][2].mark == current_turn->mark &&
            board.grid[1][1].mark == current_turn->mark &&
            board.grid[2][0].mark == current_turn->mark) {
            return true;
        }
        return false;
    }

    bool checkDraw() {
        for (const auto& row : board.grid) {
            for (const auto& cell : row) {
                if (cell.mark == Mark::EMPTY) {
                    return false;
                }
            }
        }
        return true;
    }

    void switchTurn() {
        current_turn = (current_turn == &players[0]) ? &players[1] : &players[0];
    }
};
// ----------- END GAME ------------ //


// ------------- PLAYER ----------- //
class Player {
public:
    std::string name;
    Mark mark;

    Player(std::string n, Mark m) : name(n), mark(m) {}

    bool makeMove(Position position, Game& game){
        return game.makeMove(position);
    }
};
// ----------- END PLAYER ---------- //



int main() {
    // Create players
    Player player1("Alice", Mark::X);
    Player player2("Bob", Mark::O);

    // Start the game
    Game ticTacToeGame(player1, player2);

    // Players take turns making moves
    while (ticTacToeGame.status == GameStatus::ONGOING) {
        ticTacToeGame.board.printBoard();

        int row, col;
        std::cout << ticTacToeGame.current_turn->name << "'s turn. Enter row and column: ";
        std::cin >> row >> col;

        Position pos(row, col);
        if (!ticTacToeGame.makeMove(pos)) {
            std::cout << "Invalid move! Try again.\n";
        }

        if (ticTacToeGame.status == GameStatus::WIN) {
            ticTacToeGame.board.printBoard();
            std::cout << ticTacToeGame.current_turn->name << " wins!\n";
        } else if (ticTacToeGame.status == GameStatus::DRAW) {
            ticTacToeGame.board.printBoard();
            std::cout << "It's a draw!\n";
        }
    }

    return 0;
}
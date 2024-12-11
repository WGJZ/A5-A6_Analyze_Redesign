#include <iostream>
#include <string>
using namespace std;

class Cell {
private:
    char symbol;
    bool empty;

public:
    Cell() : symbol(' '), empty(true) {}

    bool occupy(char playerSymbol) {
        if (!empty) {
            return false; // the cell is already occupied.
        }
        symbol = playerSymbol;
        empty = false;
        return true;
    }
    bool isEmpty() const {
        return empty;
    }
    char getSymbol() const {
        return symbol;
    }
    void reset() {
        symbol = ' ';
        empty = true;
    }
};

class Player {
private:
    string name;
    char symbol;

public:
    Player(string playerName, char playerSymbol)
        : name(playerName), symbol(playerSymbol) {}

    string getName() const {
        return name;
    }
    char getSymbol() const {
        return symbol;
    }
};

class Board {
private:
    Cell grid[3][3];

public:
    bool makeMove(const Player& player, int row, int col) {
        if (row < 0 || row >= 3 || col < 0 || col >= 3) {
            return false; // out of range, invalid cell!
        }
        if (!grid[row][col].occupy(player.getSymbol())) {
            return false; // cell is occupied.
        }
        return true;
    }

    bool checkWin(char playerSymbol) const {
        // check rows
        for (int i = 0; i < 3; ++i) {
            if (grid[i][0].getSymbol() == playerSymbol && grid[i][1].getSymbol() == playerSymbol && grid[i][2].getSymbol() == playerSymbol) {
                return true;
            }
        }
        // check columns
        for (int j = 0; j < 3; ++j) {
            if (grid[0][j].getSymbol() == playerSymbol && grid[1][j].getSymbol() == playerSymbol && grid[2][j].getSymbol() == playerSymbol) {
                return true;
            }
        }
        // check diagonals
        if (grid[0][0].getSymbol() == playerSymbol && grid[1][1].getSymbol() == playerSymbol && grid[2][2].getSymbol() == playerSymbol) {
            return true;
        }

        if (grid[0][2].getSymbol() == playerSymbol && grid[1][1].getSymbol() == playerSymbol && grid[2][0].getSymbol() == playerSymbol) {
            return true;
        }

        return false;
    }

    bool checkDraw() const {
        // all cells are occupied but still no player wins.
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (grid[i][j].isEmpty()) {
                    return false;
                }
            }
        }
        return true;
    }

    void displayBoard() const {
        cout << "   A   B   C\n"; // show the columns letter
        for (int i = 0; i < 3; ++i) {
            cout << i + 1 << " "; // show the rows number
            for (int j = 0; j < 3; ++j) {
                cout << " " << grid[i][j].getSymbol();
                if (j < 2) cout << " |"; // use '|' as a separator
            }
            cout << endl;
            if (i < 2) cout << "  -----------\n";
        }
    }

    void resetBoard() {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                grid[i][j].reset();
            }
        }
    }
};

class Game {
private:
    Board board;
    Player playerX;
    Player playerO;
    Player* currentPlayer;

public:
    Game(string player1Name, string player2Name)
        : playerX(player1Name, 'X'), playerO(player2Name, 'O') {
        currentPlayer = &playerX;
    }

    void start() {
        board.resetBoard();
        currentPlayer = &playerX;

        while (!isGameOver()) {
            board.displayBoard();

            string input;
            int row = -1, col = -1;

            while (true) {
                cout <<currentPlayer->getName() << "'s turn (" << currentPlayer->getSymbol() << ")" << endl;
                cout << "Make your move (e.g., A1, B2, C3): ";
                cin >> input;

                if (parseInput(input, row, col)) {
                    if (board.makeMove(*currentPlayer, row, col)) {
                        break;
                    }
                    else {
                        cout << "Invalid move. The cell is not empty or out of range." << endl;
                    }
                }
                else {
                    cout << "Invalid input. Make another move (e.g., A1, B2)." << endl;
                }
            }

            if (board.checkWin(currentPlayer->getSymbol())) {
                board.displayBoard();
                cout << currentPlayer->getName() << " wins!" << endl;
                return;
            }

            if (board.checkDraw()) {
                board.displayBoard();
                cout << "It's a draw!" << endl;
                return;
            }

            switchPlayer();
        }
    }

    bool parseInput(const string& input, int& row, int& col) {
        if (input.length() != 2) return false;

        char colChar = toupper(input[0]);
        char rowChar = input[1];

        if (colChar < 'A' || colChar > 'C') return false;
        if (rowChar < '1' || rowChar > '3') return false;

        col = colChar - 'A'; // Convert 'A', 'B', 'C' to 0, 1, 2
        row = rowChar - '1'; // Convert '1', '2', '3' to 0, 1, 2

        return true;
    }

    void switchPlayer() {
        currentPlayer = (currentPlayer->getSymbol() == 'X') ? &playerO : &playerX;
    }

    bool isGameOver() const {
        return board.checkWin('X') || board.checkWin('O') || board.checkDraw();
    }
};

int main() {
    Game ticTacToe("Player 1", "Player 2");
    ticTacToe.start();
    return 0;
}
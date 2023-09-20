#include <iostream>
#include <vector>

class Board {
public:
    Board() {
        board.resize(3, std::vector<char>(3, ' '));
    }
    void printBoardState() {
        std::cout << "-------\n";
        for (int i = 0; i < 3; i++) {
            std::cout << "|" << board[i][0] << "|"  << board[i][1] << "|"  << board[i][2] << "|\n" << "-------\n";
        }
    }
    bool updatePosition(std::tuple<int, int> coordinates, bool turn) {
        if (turn) {
            board[std::get<0>(coordinates)][std::get<1>(coordinates)] = 'X';
            return false;
        } else {
            board[std::get<0>(coordinates)][std::get<1>(coordinates)] = 'O';
            return true;
        }
    }

    bool checkValid(std::tuple<int, int> coordinates) {
        int row = std::get<0>(coordinates);
        int column = std::get<1>(coordinates);
        if (row > 2 || row < 0 || column > 2 || column < 0 || board[row][column] != ' ') {
            return false;
        }
        return true;
    }
    char checkEnd() {
        // Horizontals and Verticals
        for (int i = 0; i < 3; i++) {
            if ((board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ')
                || (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ')) {
                return board[i][0];
            }
        }
        // Diagonals
        if ((board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') ||
        (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ')) {
            return board[1][1];
        }
        // Full check
        for (int i = 0; i < 3; i++) {
            for (int p = 0; p < 3; p++) {
                if (board[i][p] == ' ')
                    return ' ';
            }
        }
        return 'F';
    }

private:
    std::vector<std::vector<char>> board;
};

int main() {
    // false turn = X, true turn = O
    Board currentGame;
    int row; int column; bool turn = true; int count = 0;
    std::cout << "Welcome to the M-Type Tic-Tac-Toe game!\n";
    while (true) {
        currentGame.printBoardState();
        if (turn) {
            std::cout << "Player X, ";
        } else {
            std::cout << "Player O, ";
        }
        std::cout << "Please enter the desired row to place your piece: ";
        std::cin >> row;
        std::cout << "Please enter the desired column to place your piece: ";
        std::cin >> column;
        if (currentGame.checkValid(std::tuple<int, int>(row, column))) {
            turn = currentGame.updatePosition(std::tuple<int, int>(row, column), turn);
            count++;
        } else {
            std::cout << "Invalid position. Please do not attempt to place a piece on an already occupied space or one outside the board!\n";
        }
        if (count > 4) {
            // Impossible for anybody to win before the 5th turn
            char state = currentGame.checkEnd();
            if (state == ' ') {
                // Board is not full and nobody has won
            } else if (state == 'F') {
                // Board is full and nobody won, meaning it is a draw.
                std::cout << "The game is a draw!" << std::endl;
                break;
            } else {
                // Somebody won.
                std::cout << "Player " << state << " Won!" << std::endl;
                break;
            }
        }
    }
    return 0;


}

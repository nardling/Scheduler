#include <vector>
#include <iostream>
#include "stdint.h"

#define NUMGAMES 6

typedef std::vector<std::vector<std::vector<int>>> board_t;

const std::vector<std::pair<int, int>> combinations {
    std::make_pair(0, 1), std::make_pair(0, 2), std::make_pair(0, 3), std::make_pair(0, 4), std::make_pair(0, 5),
    std::make_pair(1, 2), std::make_pair(1, 3), std::make_pair(1, 4), std::make_pair(1, 5),
    std::make_pair(2, 3), std::make_pair(2, 4), std::make_pair(2, 5),
    std::make_pair(3, 4), std::make_pair(3, 5),
    std::make_pair(4, 5)
};

void printBoard(const board_t& board) {
    std::cout << '\n';

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < NUMGAMES; ++j) {
            for (const auto& t : board[i][j]) {
                std::cout << t << " ";
            }
            std::cout << "    ";
        }
        std::cout << '\n';
    }
}

int getNextCombo(const std::vector<int>& cell, size_t comboNumber, std::vector<int>& combos) {
    bool valid = false;
    while (!valid && comboNumber < combinations.size()) {
        auto [t1, t2] = combinations[comboNumber];
        if (combos[comboNumber] == 1 && cell[t1] == 1 && cell[t2] == 1)
            valid = true;
        else
            ++comboNumber;
    }
    if (valid)
        return comboNumber;
    else
        return -1;
}

std::pair<board_t, bool> clearChildren(int row, int col, size_t team1, size_t team2, board_t board) {
    board_t origBoard = board;
    board[row][col] = {0, 0, 0, 0, 0, 0};
    board[row][col][team1] = 1;
    board[row][col][team2] = 1;
    for (auto r = row + 1; r < 5; ++r) {
        if (board[r][col][team1] == 1) {
            board[r][col][team1] = 0;
        } else {
            return std::make_pair(origBoard, false);
        }
        if (board[r][col][team2] == 1) {
            board[r][col][team2] = 0;
        } else {
            return std::make_pair(origBoard, false);
        }
    }
    for (auto c = col + 1; c < NUMGAMES; ++c) {
        board[row][c][team1] = 0;
        board[row][c][team2] = 0;        
    }
    return std::make_pair(board, true);
}

bool tryNextCell(int row, int col, int rowValidCount, board_t board, std::vector<int> combos) {
    std::cout << "Try Next Cell row " << row << " col " << col << '\n';

    if (row == 5) {
        std::cout << "Success:\n";
        printBoard(board);
        return true;
    }
    if (col == NUMGAMES)
        return false;

    auto cell = board[row][col];
    int comboNumber = 0;
    while(true) {
        comboNumber = getNextCombo(cell, comboNumber, combos);
        if (comboNumber == -1) {
            board[row][col] = {0, 0, 0, 0, 0, 0};
            return tryNextCell(row, col + 1, rowValidCount, board, combos);
        } else {
            std::cout << "Try Combo " << combinations[comboNumber].first << ", " << combinations[comboNumber].second << '\n';
        }

        const auto [t1, t2] = combinations[comboNumber];
        auto [nb, pass] = clearChildren(row, col, t1, t2, board);
        if (pass) {
            ++rowValidCount;
            // printBoard(nb);
            combos[comboNumber] = 0;
            if (rowValidCount == 3) {
                std::cout << "Hit rowValid = 3 on row " << row << " col " << col << '\n';
                for (int c = col + 1; c < NUMGAMES; ++c) {
                    nb[row][c] = {0, 0, 0, 0, 0, 0};
                }
                printBoard(nb);
                if(tryNextCell(row + 1, 0, 0, nb, combos))
                    return true;
                else {
                    --rowValidCount;
                    combos[comboNumber] = 1;
                    for (int c = col + 1; c < NUMGAMES; ++c) {
                        nb[row][c] = board[row][c];
                    }
                }
            }
            else {
                if (tryNextCell(row, col + 1, rowValidCount, nb, combos))
                    return true;
                else {
                    --rowValidCount;
                    combos[comboNumber] = 1;
                    for (int c = col + 1; c < NUMGAMES; ++c) {
                        nb[row][c] = board[row][c];
                    }
                }
            }
        }
        ++comboNumber;
    }
}

int main() {
    std::vector<int> cell {1, 1, 1, 1, 1, 1};
    std::vector<int> combos {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    std::vector<std::vector<int>> row;
    for (auto i = 0; i < NUMGAMES; ++i)
        row.push_back(cell);

    board_t board;
    for (auto i = 0; i < 5; ++i) {
        board.push_back(row);
    }

    printBoard(board);

    tryNextCell(0, 0, 0, board, combos);

}
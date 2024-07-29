#include "funcs.h"
int ifcontinuous(vector<vector<int>>& board, int n) {
    int size = board.size();
    // horizontal
    for (int r = 0; r < size; ++r) {
        int count = 0;
        int player = 0;
        for (int c = 0; c < size; ++c) {
            if (board[r][c] == 0) {
                count = 0;
            }
            else if (board[r][c] == player) {
                ++count;
                if (count == n) return player;
            }
            else {
                count = 1;
            }
            player = board[r][c];
        }
    }
    // vertical
    for (int c = 0; c < size; ++c) {
        int count = 0;
        int player = 0;
        for (int r = 0; r < size; ++r) {
            if (board[r][c] == 0) {
                count = 0;
            }
            else if (board[r][c] == player) {
                ++count;
                if (count == n) return player;
            }
            else {
                count = 1;
            }
            player = board[r][c];
        }
    }
    // rightdown (lower triangle)
    for (int r = 0; r <= size - 5; ++r) {
        int count = 0;
        int player = 0;
        for (int c = 0; c < size - r; ++c) {
            if (board[r + c][c] == 0) {
                count = 0;
            }
            else if (board[r + c][c] == player) {
                ++count;
                if (count == n) return player;
            }
            else {
                count = 1;
            }
            player = board[r + c][c];
        }
    }
    // right down (upper triangle)
    for (int c = 0; c <= size - 5; ++c) {
        int count = 0;
        int player = 0;
        for (int r = 0; r < size - c; ++r) {
            if (board[r][r + c] == 0) {
                count = 0;
            }
            else if (board[r][r + c] == player) {
                ++count;
                if (count == n) return player;
            }
            else {
                count = 1;
            }
            player = board[r][r + c];
        }
    }
    // rightup (upper triangle)
    for (int r = size - 1; r >= 4; --r) {
        int count = 0;
        int player = 0;
        for (int c = 0; c <= r; ++c) {
            if (board[r - c][c] == 0) {
                count = 0;
            }
            else if (board[r - c][c] == player) {
                ++count;
                if (count == n) return player;
            }
            else {
                count = 1;
            }
            player = board[r - c][c];
        }
    }
    // rightup (lower triangle)
    for (int c = 0; c <= size - 5; ++c) {
        int count = 0;
        int player = 0;
        for (int r = size - 1; r >= c; --r) {
            if (board[r][size - 1 - r + c] == 0) {
                count = 0;
            }
            else if (board[r][size - 1 - r + c] == player) {
                ++count;
                if (count == n) return player;
            }
            else {
                count = 1;
            }
            player = board[r][size - 1 - r + c];
        }
    }
    return 0;
}

int ifwin(vector<vector<int>>& board) {
    return ifcontinuous(board, 5);
}

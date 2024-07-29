#include "funcs.h"
#include "EasyX\graphics.h"
void drawonepiece(int x, int y, int current_player, int block_size) {
    setlinecolor(BLACK);
    if (current_player == 1) {   
        setfillcolor(BLACK);
        fillcircle(x, y, (float)block_size / 2.1);
    }
    else if (current_player == 2) {
        setfillcolor(WHITE);
        fillcircle(x, y, (float)block_size / 2.1);
        circle(x, y, (float)block_size / 2.1);
    }
}

void drawpieces(vector<vector<int>>& board, int block_size) {
    setlinecolor(BLACK);
    for (int i = 0; i < 19; ++i) {
        for (int j = 0; j < 19; ++j) {
            if (board[i][j] != 0)
                drawonepiece((j + 1) * block_size, (i + 1) * block_size, board[i][j], block_size);
        }
    }
}

void drawboard(vector<vector<int>>& board, int block_size) {
    setlinecolor(BLACK);
    setfillcolor(BLACK);
    for (int i = 1; i < 20; ++i) {
        line(block_size, i * block_size, 19 * block_size, i * block_size);
        line(i * block_size, block_size, i * block_size, 19 * block_size);
    }
    int ar[3] = { 4,10,16 };
    for (int r : ar) {
        for (int c : ar) {
            fillcircle(c * block_size, r * block_size, 3);
        }
    }
    drawpieces(board, block_size);
}

void drawlast(int last_row,int last_col,int block_size) {
    setlinecolor(RED);
    int x=(last_col+1)*block_size;
    int y=(last_row+1)*block_size;
    int r=(float)block_size/2.1;
    circle(x,y,r);
    circle(x,y,r/2);
    line(x,y-r,x,y+r);
    line(x-r,y,x+r,y);
}
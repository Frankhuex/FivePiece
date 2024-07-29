#include "funcs.h"
int xy2cr(int xy, int block_size) {
    int cr;
    if (xy < block_size)
        cr = 0;
    else if (xy > 19 * block_size)
        cr = 18;
    else
        cr = (xy + block_size / 2) / block_size - 1;
    return cr;
}

/*
0.right,1.rightup,2.up,3.leftup,4.left,5.leftdown,6.down,7.rightdown
*/

int changerow(int row, int dir) {
    switch (dir) {
    case 5:
    case 6:
    case 7:
        return row + 1;
    case 1:
    case 2:
    case 3:
        return row - 1;
    default:
        return row;
    }
}

int changecol(int col, int dir) {
    switch (dir) {
    case 0:
    case 1:
    case 7:
        return col + 1;
    case 3:
    case 4:
    case 5:
        return col - 1;
    default:
        return col;
    }
}

void changerowcol(int& row, int& col, int dir) {
    row = changerow(row, dir);
    col = changecol(col, dir);
}

bool overboard(int row, int col) {
    if (row<0 || row>18 || col<0 || col>18) return true;
    return false;
}

int basicspread1(vector<vector<int>>& board, int row, int col, int current_player, int dir) {
    int spread=0;
    changerowcol(row,col,dir);
    while (!overboard(row,col) && board[row][col]==current_player) {
        ++spread;
        changerowcol(row,col,dir);
    }
    return spread;
}


int livespread1(vector<vector<int>>& board, int row, int col, int current_player, int dir, bool canjump) {
    int spread = 0;
    changerowcol(row, col, dir);
    while (!overboard(row, col) && board[row][col] == current_player) {
        ++spread;
        changerowcol(row, col, dir);
    }
    if (!overboard(row, col) && board[row][col] == 0) {
        if (canjump) {
            int jump_spread = livespread1(board, row, col, current_player, dir, false);
            if (jump_spread != -1)
                spread += jump_spread;
        }
    }
    else spread = -1; //dead
    return spread;
}

int deadspread1(vector<vector<int>>& board, int row, int col, int current_player, int dir, bool canjump) {
    int spread = 0;
    changerowcol(row, col, dir);
    while (!overboard(row, col) && board[row][col] == current_player) {
        ++spread;
        changerowcol(row, col, dir);
    }
    if (!overboard(row, col) && board[row][col] == 0) {
        if (canjump) {
            int jump_spread = deadspread1(board, row, col, current_player, dir, false);
            if (jump_spread == -1)
                spread = -1; //live
            else
                spread += jump_spread;
        }
        else spread = -1; //live
    }
    return spread;
}

int basicspread2(vector<vector<int>>& board,int row,int col,int current_player,int dir) {
    int dir2 = (dir + 4) % 8;
    return basicspread1(board,row,col,current_player,dir)+basicspread1(board,row,col,current_player,dir2)+1;
}

int livespread2(vector<vector<int>>& board, int row, int col, int current_player, int dir) {
    int dir2 = (dir + 4) % 8;
    int canjump1 = livespread1(board, row, col, current_player, dir, true);
    int canjump2 = livespread1(board, row, col, current_player, dir2, true);
    int notjump1 = livespread1(board, row, col, current_player, dir, false);
    int notjump2 = livespread1(board, row, col, current_player, dir2, false);
    int ans1=-1,ans2=-1,ans3=-1;
    if (canjump1!=-1 && notjump2!=-1) ans1=canjump1+notjump2+1;
    if (canjump2!=-1 && notjump1!=-1) ans2=canjump2+notjump1+1;
    if (notjump1!=-1 && notjump2!=-1) ans3=notjump1+notjump2+1;
    return max(max(ans1,ans2),ans3);
}

int deadspread2(vector<vector<int>>& board, int row, int col, int current_player, int dir) {
    int dir2 = (dir + 4) % 8;
    int dead_canjump1 = deadspread1(board, row, col, current_player, dir, true);
    int dead_canjump2 = deadspread1(board, row, col, current_player, dir2, true);
    int dead_notjump1 = deadspread1(board, row, col, current_player, dir, false);
    int dead_notjump2 = deadspread1(board, row, col, current_player, dir2, false);
    int live_canjump1 = livespread1(board, row, col, current_player, dir, true);
    int live_canjump2 = livespread1(board, row, col, current_player, dir2, true);
    int live_notjump1 = livespread1(board, row, col, current_player, dir, false);
    int live_notjump2 = livespread1(board, row, col, current_player, dir2, false);
    if (live_canjump1 != -1 && dead_notjump2 != -1) return live_canjump1 + dead_notjump2 + 1;
    if (dead_canjump1 != -1 && live_notjump2 != -1) return dead_canjump1 + live_notjump2 + 1;
    if (live_notjump1 != -1 && dead_canjump2 != -1) return live_notjump1 + dead_canjump2 + 1;
    if (dead_notjump1 != -1 && live_canjump2 != -1) return dead_notjump1 + live_canjump2 + 1;
    return -1; //both dead or both live
}

bool availover5(vector<vector<int>>&board,int row,int col,int current_player,int dir) {
    int row_copy=row,col_copy=col;
    int count=0;
    int dir2=(dir+4)%8;
    changerowcol(row_copy,col_copy,dir);
    while (!overboard(row_copy,col_copy)&&board[row_copy][col_copy]!=3-current_player) {
        ++count;
        if (count>=5) return true;
        changerowcol(row_copy,col_copy,dir);
    }
    row_copy=row; col_copy=col;
    changerowcol(row_copy,col_copy,dir2);
    while (!overboard(row_copy,col_copy)&&board[row_copy][col_copy]!=3-current_player) {
        ++count;
        if (count>=5) return true;
        changerowcol(row_copy,col_copy,dir2);
    }
    return false;
}

bool live3(vector<vector<int>>& board, int row, int col, int current_player, int dir) {
    return livespread2(board, row, col, current_player, dir) == 3 && availover5(board,row,col,current_player,dir);
}

bool doublelive3(vector<vector<int>>& board, int row, int col, int current_player) {
    int num = 0;
    for (int dir = 0; dir < 4; ++dir) {
        if (live3(board, row, col, current_player, dir)) ++num;
        if (num == 2) return true;
    }
    return false;
}

bool any4(vector<vector<int>>& board, int row, int col, int current_player, int dir) {
    int live = livespread2(board, row, col, current_player, dir);
    int dead = deadspread2(board, row, col, current_player, dir);
    return live == 4 || dead == 4;
}


bool live4(vector<vector<int>>& board,int row,int col,int current_player,int dir) {
    int dir2=(dir+4)%8;
    return livespread1(board,row,col,current_player,dir,false)+livespread1(board,row,col,current_player,dir2,false)+1==4;
}

bool strict5(vector<vector<int>>& board,int row,int col,int current_player,int dir) {
    return basicspread2(board,row,col,current_player,dir)==5;
}

bool dead5(vector<vector<int>>& board,int row,int col,int current_player,int dir) {
    // 011112
    int dir2 = (dir + 4) % 8;
    int dead1 = deadspread1(board,row,col,current_player,dir,false);
    int dead2 = deadspread1(board,row,col,current_player,dir2,false);
    int basic1 = basicspread1(board,row,col,current_player,dir);
    int basic2 = basicspread1(board,row,col,current_player,dir2);
    if (basic1==0 && dead2==4) return true;
    if (basic2==0 && dead1==4) return true;
    if (basic1*basic2>0 && basic1+basic2==4) return true;
    return false;
}

bool lost5(vector<vector<int>>& board,int row,int col,int current_player,int dir) {
    // 011110
    int dir2 = (dir + 4) % 8;
    int live1 = livespread1(board,row,col,current_player,dir,false);
    int live2 = livespread1(board,row,col,current_player,dir2,false);
    int basic1 = basicspread1(board,row,col,current_player,dir);
    int basic2 = basicspread1(board,row,col,current_player,dir2);
    if (basic1==0 && live2==4) return true;
    if (basic2==0 && live1==4) return true;
    return false;
}

bool longconnect_dir(vector<vector<int>>& board,int row,int col,int current_player,int dir) {
    return basicspread2(board,row,col,current_player,dir)>5;
}

bool longconnect(vector<vector<int>>& board,int row,int col,int current_player) {
    for (int dir=0;dir<4;++dir) {
        if (longconnect_dir(board,row,col,current_player,dir))
            return true;
    }
    return false;
}

bool double4_inline(vector<vector<int>>& board, int row, int col, int current_player, int dir) {
    int dir2 = (dir + 4) % 8;
    int canjump1 = livespread1(board, row, col, current_player, dir, true);
    int canjump2 = livespread1(board, row, col, current_player, dir2, true);
    int notjump1 = livespread1(board, row, col, current_player, dir, false);
    int notjump2 = livespread1(board, row, col, current_player, dir2, false);
    if (canjump1!=notjump1 && canjump2!=notjump2 && canjump1+notjump2+1==4 && canjump2+notjump1+1==4)
        return true;
    return false;
}

bool double4(vector<vector<int>>& board, int row, int col, int current_player) {
    int num = 0;
    for (int dir=0;dir<4;++dir) {
        if (any4(board,row,col,current_player,dir)) ++num;
        if (num == 2 || double4_inline(board, row, col, current_player, dir)) return true;   
    }
    return false;
}



bool banned(vector<vector<int>>& board,int row,int col,int current_player) {
    for (int dir=0;dir<4;++dir)
        if (strict5(board,row,col,current_player,dir))
            return false;
    if (doublelive3(board,row,col,current_player) || double4(board,row,col,current_player) || longconnect(board,row,col,current_player))
        return true;
    return false;
}
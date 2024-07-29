#include <vector>
#include <iostream>
using namespace std;
#ifndef HEADER_H
#define HEADER_H
int ifcontinuous(vector<vector<int>>& board, int n);
int ifwin(vector<vector<int>>& board);

void changerowcol(int& row,int& col,int dir);
bool overboard(int row,int col);

void drawboard(vector<vector<int>>& board, int block_size);
void drawonepiece(int x, int y, int current_player, int block_size);
void drawpieces(vector<vector<int>>& board, int block_size);
void drawlast(int last_row,int last_col,int block_size);

int xy2cr(int xy, int block_size);

int livespread2(vector<vector<int>>& board,int row,int col,int current_player,int dir);
int deadspread2(vector<vector<int>>& board,int row,int col,int current_player,int dir);

bool live3(vector<vector<int>>& board,int row,int col,int current_player,int dir);
bool live4(vector<vector<int>>& board,int row,int col,int current_player,int dir);
bool any4(vector<vector<int>>& board,int row,int col,int current_player,int dir);

bool strict5(vector<vector<int>>& board,int row,int col,int current_player,int dir);
bool dead5(vector<vector<int>>& board,int row,int col,int current_player,int dir);
bool lost5(vector<vector<int>>& board,int row,int col,int current_player,int dir);
bool longconnect_dir(vector<vector<int>>& board,int row,int col,int current_player,int dir);
bool longconnect(vector<vector<int>>& board,int row,int col,int current_player);

bool doublelive3(vector<vector<int>>& board, int row, int col, int current_player);

bool double4_inline(vector<vector<int>>& board,int row,int col,int current_player,int dir);
bool double4(vector<vector<int>>& board, int row, int col, int current_player);



bool banned(vector<vector<int>>& board,int row,int col,int current_player);
void AI(vector<vector<int>>& board,int current_player,int depth,int&AI_row,int&AI_col);
#endif
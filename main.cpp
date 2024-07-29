#include <iostream>
#include <vector>
#include "EasyX\graphics.h"
#include "funcs.h"
//#include "graphics.h"
/*
#include "AI.h"
#include "coordinate.h"
#include "draw.h"
#include "ifwin.h"
*/
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) 


using namespace std;
#define WINDOW_SIZE 700
#define BK_COLOR 0xfafab4
#define GREY 0x646464
const int BLOCK_SIZE = 35;


void initialize(vector<vector<int>>&board,int&current_player,bool&playing,bool&single,int&AI_start,int&depth,int&last_row,int&last_col) {
    current_player = 1;
    playing = true;
    single=false;
    AI_start=false;
    depth=0;
    last_row=-1;
    last_col=-1;
    for (int i=0;i<19;++i) {
        for (int j=0;j<19;++j) {
            board[i][j]=0;
        }
    }
    if (MessageBox(GetHWnd(),_T("Yes: Single \n No: Double"),_T("Choose player number"),MB_YESNO)==IDYES) {
        single=true;
        if (MessageBox(GetHWnd(),_T("Yes: Black (first) \n No: White (second)"),_T("Choose your side"),MB_YESNO)==IDNO) {
            last_row=9;
            last_col=9;
            board[9][9]=1;
            current_player=2;
        }
        if (MessageBox(GetHWnd(),_T("Yes: Easy mode \n No: Hard mode"),_T("Choose difficulty"),MB_YESNO)==IDNO) {
            depth=1;
        }
    }

}

int main() {
    vector<vector<int>> board(19,vector<int>(19,0));
    initgraph(WINDOW_SIZE,WINDOW_SIZE);
    setbkcolor(RGB(250, 250, 180));
    BeginBatchDraw();
    cleardevice();
    drawboard(board,BLOCK_SIZE);
    FlushBatchDraw();

    int current_player=1;
    bool playing=true;
    bool single=false;
    int AI_start=false;
    int depth=0;
    int last_row,last_col;
    initialize(board,current_player,playing,single,AI_start,depth,last_row,last_col);
   
    int mouse_row,mouse_col;
    ExMessage m;
    while (true) {
        m = getmessage(EM_MOUSE|EX_KEY); 

        cleardevice();
        drawboard(board, BLOCK_SIZE);
        if (last_row!=-1 && last_col!=-1) {
            drawlast(last_row,last_col,BLOCK_SIZE);
        }
        if (m.message == WM_KEYDOWN) {
            switch (m.vkcode) {
            case 0x52: //R
                if (MessageBox(GetHWnd(),_T("Replay?"),_T("Warning"),MB_OKCANCEL)==IDOK) {
                    initialize(board,current_player,playing,single,AI_start,depth,last_row,last_col);
                    continue;
                }
                break;
            case VK_ESCAPE: 
                if (MessageBox(GetHWnd(),_T("Quit?"),_T("Warning"),MB_OKCANCEL)==IDOK)
                    return 0;
            }
        }
        if (playing) {
            mouse_row=xy2cr(m.y,BLOCK_SIZE);
            mouse_col=xy2cr(m.x,BLOCK_SIZE);
            if (board[mouse_row][mouse_col]==0) {
                if (current_player==1 && banned(board,mouse_row,mouse_col,current_player)) {
                    setlinecolor(RED);
                }
                else {
                    setlinecolor(GREY);
                    if (m.message == WM_LBUTTONDOWN) {
                        board[mouse_row][mouse_col] = current_player;
                        last_row=mouse_row;
                        last_col=mouse_col;
                        drawboard(board,BLOCK_SIZE);
                        drawlast(last_row,last_col,BLOCK_SIZE);
                        FlushBatchDraw();
                        if (ifwin(board)) {
                            playing = false;
                            continue;
                        }
                        if (single) {
                            if (!AI_start) {
                                int AI_second_row,AI_second_col;
                                for (AI_second_row=8;AI_second_row<=10;AI_second_row+=2) {
                                    for (AI_second_col=8;AI_second_col<=10;AI_second_col+=2) {
                                        if (board[AI_second_row][AI_second_col]==0) {
                                            board[AI_second_row][AI_second_col]=3-current_player;
                                            last_row=AI_second_row;
                                            last_col=AI_second_col;
                                            AI_start=true;
                                            break;
                                        }
                                    }
                                    if (AI_start) break;
                                }
                            }
                            else AI(board,3-current_player,depth,last_row,last_col);
                            if (ifwin(board)) {
                                playing = false;
                                continue;
                            }
                        }
                        else {
                            current_player=3-current_player;
                        }
                    }
                }
                circle((mouse_col+1)*BLOCK_SIZE,(mouse_row+1)*BLOCK_SIZE,(float)BLOCK_SIZE/2.1);
            }
            drawonepiece(m.x, m.y, current_player, BLOCK_SIZE);  
        }
        FlushBatchDraw();
    }
    EndBatchDraw();
    closegraph();
    return 0;
}



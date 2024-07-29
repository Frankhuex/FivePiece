#include "funcs.h"

int pow(int x,int e) {
	int r=1;
	for (int i=0;i<e;++i) r*=x;
	return r;
}

int abs(int x) {
	return x>0?x:-x;
}

vector<vector<int>> copyboard(vector<vector<int>>& board) {
	vector<vector<int>> board_copy(board.size(),vector<int>(board[0].size(),0));
	for (int i=0;i<board.size();++i) {
		for (int j=0;j<board.size();++j) {
			board_copy[i][j]=board[i][j];
		}
	}
	return board_copy;
}

int basicscore(vector<vector<int>>& board,int row,int col,int current_player) {
	int result=1;
	// cur
	int cur=current_player;
	int cur4=0;
	int curlive3=0;

	bool curstrict5=false;
	bool curlongconnect=false;

	bool curlive4=false;
	bool curdouble4=false;
	bool cur4live3=false;
	bool curdoublelive3=false;

	bool curnow4=false;

	// opp
	int opp=3-cur;
	int opp4=0;
	int opplive3=0;

	bool oppdead5=false;
	bool opplost5=false;
	bool opplongconnect=false;

	bool opplive4=false;
	bool oppdouble4=false;
	bool opp4live3=false;
	bool oppdoublelive3=false;

	bool oppnow4=false;

	for (int dir=0;dir<4;++dir) {
		// Check single-line shape
		if (strict5(board,row,col,cur,dir)) {
			curstrict5=true; break;
		}
		if (dead5(board,row,col,opp,dir)) {
			oppdead5=true; break;
		}
		if (lost5(board,row,col,opp,dir)) {
			opplost5=true; break;
		}
		if (!curlive4 && live4(board,row,col,cur,dir))
			curlive4=true;
		if (!opplive4 && live4(board,row,col,opp,dir))
			opplive4=true;
		if (!curlongconnect && longconnect_dir(board,row,col,cur,dir))
			curlongconnect=true;
		if (!opplongconnect && longconnect_dir(board,row,col,opp,dir))
			opplongconnect=true;
		// Check 2-dir shape
		// Check current_player
		if (any4(board,row,col,cur,dir)) {
			if (!curdouble4&&(cur4||double4_inline(board,row,col,cur,dir))) {
				curdouble4=true;
			}
			if (curlive3) cur4live3=true;
			++cur4;
			curnow4=true;
			result+=500;
		}else curnow4=false;
		if (live3(board,row,col,cur,dir)) {
			if (curlive3||(curnow4&&(cur4>1))||(!curnow4&&(cur4>0))) curdoublelive3=true;
			++curlive3;
			result+=500;
		}

		// Check opponent
		if (any4(board,row,col,opp,dir)) {
			if (!oppdouble4&&(opp4||double4_inline(board,row,col,opp,dir))) {
				oppdouble4=true;
			}
			if (opplive3) opp4live3=true;
			++opp4;
			oppnow4=true;
			result+=450;
		}else oppnow4=false;
		if (live3(board,row,col,opp,dir)) {
			if (opplive3||(oppnow4&&(opp4>1))||(!oppnow4&&opp4)) oppdoublelive3=true;
			++opplive3;
			result+=450;
		}

		result+=pow(2,(livespread2(board,row,col,current_player,dir)+2)+1);
		result+=pow(2,(livespread2(board,row,col,3-current_player,dir)+1)+1);
		result+=pow(2,deadspread2(board,row,col,current_player,dir)+2);
		result+=pow(2,deadspread2(board,row,col,3-current_player,dir)+1);
	}


	if (cur==1) {
		if (curstrict5) return 9999;
		if (opplost5||curlongconnect||curdouble4||curdoublelive3) return -9999;
		if (oppdead5) return 8888;
		if (cur4live3||curlive4) return 7777;
		if (opplive4||oppdouble4||opp4live3||oppdoublelive3) return 6666;
	}
	else {
		if (curstrict5||curlongconnect) return 9999;
		if (opplost5) return -9999;
		if (oppdead5) return 8888;
		if (curlive4||curdouble4||cur4live3) return 7777;
		if (opplive4||opp4live3) return 6666;
	}
	return result;
}


int basicscore_old(vector<vector<int>>& board,int row,int col,int current_player) {	
	if (current_player==1 && banned(board,row,col,current_player))
		return -9999;
	int result=0;
	if (double4(board,row,col,3-current_player))
		return 8888;
	for (int dir=0;dir<4;++dir) {
		if (strict5(board,row,col,current_player,dir)||longconnect_dir(board,row,col,current_player,dir))
			return 9999;
		if (dead5(board,row,col,3-current_player,dir))
			return 8888;
		if (live4(board,row,col,current_player,dir))
			return 7777;
		if (lost5(board,row,col,3-current_player,dir))
			return -9999;

		if (live4(board,row,col,3-current_player,dir))
			result+=900;
		if (live3(board,row,col,current_player,dir)) 
			result+=500;
		if (live3(board,row,col,3-current_player,dir))
			result+=450;
		if (any4(board,row,col,current_player,dir))
			result+=500;
		if (any4(board,row,col,3-current_player,dir))
			result+=450;
		result+=(livespread2(board,row,col,current_player,dir)+2)*2;
		result+=(livespread2(board,row,col,3-current_player,dir)+1)*2;
		result+=deadspread2(board,row,col,current_player,dir)+2;
		result+=deadspread2(board,row,col,3-current_player,dir)+1;
	}
	return result;
}


int minimax(vector<vector<int>>& board, int row, int col, int current_player, int depth, bool positive) {
	int result;
	int basic_score;
	if (positive) {
		result=1000000;
		basic_score=basicscore(board,row,col,current_player);
	}
	else {
		result=-1000000;
		basic_score=-basicscore(board,row,col,current_player);
	}
	if (depth==0||abs(basic_score)>7000) return basic_score;

	int next_score;
	board[row][col]=current_player;
	for (int i=0;i<19;++i) {
		for (int j=0;j<19;++j) {
			if (board[i][j]==0) {
				next_score=minimax(board,i,j,3-current_player,depth-1,!positive);
				if (positive) result=min(result,next_score);
				else result=max(result,next_score);
			}
		}
	}
	board[row][col]=0;
	return result+basic_score;
}

void AI(vector<vector<int>>& board,int current_player,int depth,int&AI_row,int&AI_col) {
	int bestrow=-1;
	int bestcol=-1;
	int maxscore=-10000;
	int score;
	for (int row=0;row<19;++row) {
		for (int col=0;col<19;++col) {
			if (board[row][col]==0) {
				score=minimax(board,row,col,current_player,depth,true);
				//cout << score << ',';
				if (score>maxscore) {
					bestrow=row;
					bestcol=col;
					maxscore=score;
				}
			}
			//else cout << "   ,";
		}
		//cout << '\n';
	}
	//cout << '\n\n';
	board[bestrow][bestcol]=current_player;
	AI_row=bestrow;
	AI_col=bestcol;
}

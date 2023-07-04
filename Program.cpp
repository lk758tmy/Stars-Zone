#include <cstdio>
#include <cstdlib>
using namespace std;
int board[11][11]={{4,4,4,4,4,4,4,4,4,4,4},
	{4,4,4,4,0,0,4,4,4,4,4},{4,4,4,0,0,0,0,0,0,4,4},
	{4,4,0,0,0,0,0,0,0,4,4},{4,0,0,0,0,0,0,0,0,4,4},
	{4,0,0,0,0,3,0,0,0,0,4},{4,4,0,0,0,0,0,0,0,0,4},
	{4,4,0,0,0,0,0,0,0,4,4},{4,4,0,0,0,0,0,0,4,4,4},
	{4,4,4,4,4,0,0,4,4,4,4},{4,4,4,4,4,4,4,4,4,4,4}};
int neuCnt[2]={5,5},turn;
int dirX[4]={1,0,0,-1},dirY[4]={0,1,-1,0};
void print(int pl,int end){
	system("cls"); char c;
	printf("Stars' Zone\n  A B C D E F G H I");
	for(int i=1;i<10;i++){
		printf("\n%d",i);
		for(int j=1;j<10;j++){
			switch(board[i][j]){
				case 1: c='O'; break; case 2: c='X'; break;
				case 0: c='-'; break; default: c=' ';
			} printf(" %c",c);
	}}
	if(end==0){
		printf("\nTurn %d/16, %c to move.",turn,(pl==1)?'O':'X');
		printf("\nNeutral Stones Remain: %d\n",neuCnt[pl]);
	}else if(end==3)  printf("\nGame ends in draw\n");
	else printf("\nGame ends. %c Wins.\n",(end==1)?'O':'X');
	return ;
}
void move(int pl){
	char _pos1,_pos2; int posI,posJ; print(pl,0);
	INPUT1:
	fflush(stdin); printf(">>"); scanf("%c%c",&_pos1,&_pos2);
	posI=_pos2-'0'; posJ=_pos1-'A'+1;
	if(abs(5-posI)>4||abs(5-posJ)>4||board[posI][posJ]!=0){
		printf("Wrong Position! "); goto INPUT1;}
	board[posI][posJ]=pl;
	if(neuCnt[pl-1]==0) return ;
	print(pl,0); INPUT2:
	fflush(stdin); printf(">>"); scanf("%c%c",&_pos1,&_pos2);
	if(_pos1=='X'||_pos1=='x') return ;
	posI=_pos2-'0'; posJ=_pos1-'A'+1;
	if(abs(5-posI)>4||abs(5-posJ)>4||board[posI][posJ]!=0){
		printf("Wrong Position! "); goto INPUT2;}
	neuCnt[pl-1]--; board[posI][posJ]=3;
	return ;
}
void winCheck(){
	bool visit[2][16]; int score[2]={0,0},toX,toY,id,side,win;
	for(int i=0;i<16;i++) visit[0][i]=visit[1][i]=false;
	for(int i=1;i<10;i++) for(int j=1;j<10;j++){
		if(board[i][j]!=1&&board[i][j]!=2) continue;
		id=0; side=board[i][j]-1;
		for(int k=0;k<4;k++){
			toX=i; toY=j;
			while(1){ toX+=dirX[k]; toY+=dirY[k];
				if(board[toX][toY]==4) break;
				if(board[toX][toY]==board[i][j]){
					id++; break;
			}} id*=2;}
		if(visit[side][id]==false){
			visit[side][id]=true; score[side]++;
	}}
	if(score[0]>score[1]) win=1;
	else if(score[0]<score[1]) win=2;
	else if(neuCnt[0]>neuCnt[1]) win=1;
	else if(neuCnt[0]<neuCnt[1]) win=2;
	else win=3; print(0,win);
	printf("Score: O-%d X-%d\n",score[0],score[1]);
	printf("Neutrals Remain: O-%d X-%d\n",neuCnt[0],neuCnt[1]);
	return;
}
int main() {
	printf("Number of Neutral Stones (per player)\n");
	printf("(0~11, Default:5, Recommend:4~6)>>");
	scanf("%d",&neuCnt[0]);
	if(neuCnt[0]>-1&&neuCnt[0]<12) neuCnt[1]=neuCnt[0];
	else neuCnt[0]=neuCnt[1]=5;
	for(turn=1;turn<17;turn++){move(1); move(2);}
	winCheck(); getchar(); getchar();
	return 0;
}

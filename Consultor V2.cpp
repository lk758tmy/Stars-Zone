#include <cstdio>
#include <ctime>
#include <random>
#include <memory.h>
#include <vector>
#include <queue>
using namespace std;
const int timeLimitHalf=5;
struct locationScore{
	int i,j,score;
};
class cmp{
	public:
    bool operator () (locationScore x, locationScore y){
        return x.score<y.score;
    }
};
int board[11][11]={
	{4,4,4,4,4,4,4,4,4,4,4},{4,4,4,4,0,0,4,4,4,4,4},{4,4,4,0,0,0,0,0,0,4,4},
	{4,4,0,0,0,0,0,0,0,4,4},{4,0,0,0,0,0,0,0,0,4,4},{4,0,0,0,0,3,0,0,0,0,4},
	{4,4,0,0,0,0,0,0,0,0,4},{4,4,0,0,0,0,0,0,0,4,4},{4,4,0,0,0,0,0,0,4,4,4},
	{4,4,4,4,4,0,0,4,4,4,4},{4,4,4,4,4,4,4,4,4,4,4}
};
int starCnt,starI[16],starJ[16],maxScore[2],nboard[11][11],tstart;
double value[2][11][11]; int maxScoreCnt;
const int dirI[4]={1,0,0,-1},dirJ[4]={0,1,-1,0};
void print(){
	//Print screen
	system("cls"); system("cls");
	printf("Max possible score for Red/Blue: ");
	printf("\033[1;31m%d \033[1;36m%d\033[0m\n\n",maxScore[0],maxScore[1]);
	printf("X\\Y");
	for(int i=1;i<10;i++) printf("\t %d",i);
	printf("\n");
	for(int i=1;i<10;i++){
		for(int j=1;j<10;j++){
			switch(board[i][j]){
				case 0:
					printf("\t\033[1;31m%.2f\033[0m",value[0][i][j]);
					break;
				case 1:
					printf("\t\033[1;31m¨¨Ž\033[0m");
					break;
				case 2:
					printf("\t\033[1;36m¨¨Ž\033[0m");
					break;
				case 3:
					printf("\t¨¨Ž");
					break;
				case 4:
					printf("\t");
			}
		}
		printf("\n%d",i);
		for(int j=1;j<10;j++){
			switch(board[i][j]){
				case 0:
					printf("\t\033[1;36m%.2f\033[0m",value[1][i][j]);
					break;
				case 1:
					printf("\t\033[1;31m¨¨\033[0m");
					break;
				case 2:
					printf("\t\033[1;36m¨¨\033[0m");
					break;
				case 3:
					printf("\t¨¨");
					break;
				case 4:
					printf("\t");
			}
		}
		printf("\n\n");
	}
	//Input and check
	printf("X Y C (C: 1=red 2=blue 3=neutral 0=empty) ");
	int x,y,c,cnt1=0,cnt2=0,cnt3=0;
	SCANFAGAIN:
	printf(">> ");
	scanf("%d%d%d",&x,&y,&c);
	if(c<0||c>3||y<1||y>9||x<1||x>9||(x==5&&y==5)||board[x][y]==4){
		printf("Illegal move.\n"); goto SCANFAGAIN;
	}
	for(int i=1;i<10;i++)
	for(int j=1;j<10;j++){
		if(board[i][j]==1) cnt1++;
		if(board[i][j]==2) cnt2++;
		if(board[i][j]==0) cnt3++;
	}
	if(cnt1==16 && c==1 && board[x][y]!=1){
		printf("Too much red stones. "); goto SCANFAGAIN;
	}else if(cnt2==16 && c==2 && board[x][y]!=2){
		printf("Too much blue stones. "); goto SCANFAGAIN;
	}else if(c!=0 && board[x][y]==0 && cnt3<=(32-cnt1-cnt2)){
		printf("Too few empty locations. "); goto SCANFAGAIN;
	}
	board[x][y]=c; printf("Computer is thinking...\n");
	return ;
}
int check(int side){ //for greedy dfs
	bool appeared[16],flag;
	int score=0,starNo,i,j;
	memset(appeared,0,sizeof(appeared));
	for(int k=0;k<starCnt;k++){
		starNo=0;
		for(int d=0;d<4;d++){
			i=starI[k]; j=starJ[k];
			while(1){
				i+=dirI[d]; j+=dirJ[d];
				if(board[i][j]==4){
					flag=false; break;
				}else if(board[i][j]==side){
					flag=true; break;
				}
			}
			if(flag==true) starNo+=(1<<d);
		}
		appeared[starNo]=true;
	}
	for(int i=0;i<16;i++)
		if(appeared[i]==true) score++;
	return score;
}
int greedy(int side,int level){
	//End DFS
	if(level>20){
		if((level-20)>maxScore[side-1]){
			maxScore[side-1]=level-20;
			maxScoreCnt=1;
		}else if((level-20)==maxScore[side-1])
			maxScoreCnt++;
		double addValue=1.0;
		while(level<36){
			level++; addValue/=100.0;}
		for(int k=0;k<16;k++)
			value[side-1][starI[k]][starJ[k]]+=addValue;
		return 1;
	}
	int cnt=0,i,j,r,a[9],b[9],t;
	locationScore tmp;
	priority_queue<locationScore,vector<locationScore>,cmp> Q;
	for(int k=0;k<9;k++) a[k]=b[k]=k+1;
	for(int k=8;k>0;k--){
		r=rand()%(k+1); t=a[r]; a[r]=a[k]; a[k]=t;
		r=rand()%(k+1); t=b[r]; b[r]=b[k]; b[k]=t;
	}
	//Check value for each possible move
	for(int ii=0;ii<9;ii++)
	for(int jj=0;jj<9;jj++){
		i=a[ii]; j=b[jj];
		if(board[i][j]!=0) continue;
		tmp.i=i; tmp.j=j;
		starI[starCnt]=i; starJ[starCnt]=j;
		board[i][j]=side; starCnt++;
		tmp.score=check(side);
		Q.push(tmp);
		board[i][j]=0; starCnt--;
	}
	//DFS
	while(!Q.empty()){
		tmp=Q.top(); Q.pop();
		starI[starCnt]=tmp.i; starJ[starCnt]=tmp.j;
		board[tmp.i][tmp.j]=side; starCnt++;
		if(level>1)	cnt+=greedy(side,level-1);
		else cnt+=greedy(side,20+tmp.score);
		board[tmp.i][tmp.j]=0; starCnt--;
		if(time(0)-tstart>=timeLimitHalf) break;
	}
	return cnt;
}
int ncheck(int side){ //for hill climbing
	bool appeared[16],flag;
	int score=0,starNo,i,j;
	memset(appeared,0,sizeof(appeared));
	for(int k=0;k<16;k++){
		starNo=0;
		for(int d=0;d<4;d++){
			i=starI[k]; j=starJ[k];
			while(1){
				i+=dirI[d]; j+=dirJ[d];
				if(nboard[i][j]==4){
					flag=false; break;
				}else if(nboard[i][j]==side){
					flag=true; break;
				}
			}
			if(flag==true) starNo+=(1<<d);
		}
		appeared[starNo]=true;
	}
	for(int i=0;i<16;i++)
		if(appeared[i]==true) score++;
	return score;
}
void changeboard(int id,int ex,int ey,int side){	
	nboard[starI[id]][starJ[id]]=0;
	nboard[ex][ey]=side; starI[id]=ex; starJ[id]=ey;
	return ;
}
int rmove(int height,int side,int fbegin){
	int nheight,num=16-fbegin,id[2],sx[2],sy[2],ex[2],ey[2];
	id[0]=fbegin+rand()%num; id[1]=fbegin+rand()%(num-1);
	if(id[1]==id[0]) id[1]++;
	for(int i=0;i<2;i++){
		sx[i]=starI[id[i]]; sy[i]=starJ[id[i]];
		do{
			ex[i]=rand()%9+1; ey[i]=rand()%9+1;
		}while(nboard[ex[i]][ey[i]]!=0);
		changeboard(id[i],ex[i],ey[i],side);
	}
	nheight=ncheck(side);
	if(nheight<height||(nheight==height&&height<14)){
		for(int i=1;i>-1;i--)
			changeboard(id[i],sx[i],sy[i],side);
		return height;
	}
	return nheight;
}
int hill(int side){
	int cnt=0,ncnt,height,nheight,cnt1,cnt2;
	double addValue;

	while(1){
		for(int i=1;i<10;i++)
		for(int j=1;j<10;j++)
			nboard[i][j]=board[i][j];
		ncnt=starCnt;
		
		while(ncnt<16){
			starI[ncnt]=rand()%9+1; starJ[ncnt]=rand()%9+1;
			if(nboard[starI[ncnt]][starJ[ncnt]]==0){
				nboard[starI[ncnt]][starJ[ncnt]]=side; ncnt++;
			}
		}
		height=ncheck(side); cnt2=0;
		do{
			for(cnt1=0;cnt1<5000;cnt1++){
				nheight=rmove(height,side,starCnt);
				if(nheight>height||(height>13&&nheight==height)){
					height=nheight; break;}
			}
			if(height==16) break;
			cnt2++;
		}while(cnt2<205-5*starCnt);
		if(height>maxScore[side-1]){
			maxScore[side-1]=height; maxScoreCnt=1;
		}else if(height==maxScore[side-1]) maxScoreCnt++;
		addValue=1.0;
		while(height<16){
			addValue/=100.0; height++;}
		for(int k=0;k<16;k++)
			value[side-1][starI[k]][starJ[k]]+=addValue;
		
		cnt++;
		if(time(0)-tstart>=timeLimitHalf) break;
	}
	return cnt;
}
void calc(int side){
	srand(time(0));
	starCnt=0;
	int searchCnt,tmpMS; double coeff;
	for(int i=1;i<10;i++)
	for(int j=1;j<10;j++)
		if(board[i][j]==side){
			starI[starCnt]=i; starJ[starCnt]=j;
			starCnt++;
		}
	if(side==1) memset(value,0,sizeof(value));
	maxScore[side-1]=0; maxScoreCnt=0; tstart=time(0);
		
	if(starCnt==16){
		maxScore[side-1]=check(side);
		return ;
	}else if(starCnt>10) //Use greedy DFS	
		searchCnt=greedy(side,16-starCnt);
	else //Use hill climbing
		searchCnt=hill(side);
		
	tmpMS=16;
	coeff=10.0/maxScoreCnt;
	while(maxScore[side-1]<tmpMS){
		tmpMS--; coeff*=100;
	}
	for(int i=1;i<10;i++)
	for(int j=1;j<10;j++)
		value[side-1][i][j]*=coeff;
		
	return ;
}
int main() {
	printf("Computer is thinking...\n");
	for(int i=0;i<11;i++)
	for(int j=0;j<11;j++)
		nboard[i][j]=board[i][j];
	while(1){
		calc(1); calc(2);
		print();	
	}
	return 0;
}
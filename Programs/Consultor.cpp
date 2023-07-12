#include <cstdio>
#include <ctime>
#include <random>
#include <cmath>
using namespace std;
int board[11][11]={{4,4,4,4,4,4,4,4,4,4,4},
	{4,4,4,4,0,0,4,4,4,4,4},{4,4,4,0,0,0,0,0,0,4,4},
	{4,4,0,0,0,0,0,0,0,4,4},{4,0,0,0,0,0,0,0,0,4,4},
	{4,0,0,0,0,3,0,0,0,0,4},{4,4,0,0,0,0,0,0,0,0,4},
	{4,4,0,0,0,0,0,0,0,4,4},{4,4,0,0,0,0,0,0,4,4,4},
	{4,4,4,4,4,0,0,4,4,4,4},{4,4,4,4,4,4,4,4,4,4,4}};
int nboard[11][11],starX[16],starY[16],stars[16],kMax;
double value[2][11][11];
//0=empty 1=own 2=opponent's 3=neutral 4=wall
const int dirX[4]={1,0,0,-1},dirY[4]={0,1,-1,0};
int track(int x,int y,int side){
	int star=0,tx,ty; bool flag;
	for(int i=0;i<4;i++){
		flag=false; tx=x; ty=y;
		while(1){
			tx+=dirX[i]; ty+=dirY[i];
			if(nboard[tx][ty]==4) break;
			if(nboard[tx][ty]==side){ flag=true; break;
		}}
		if(flag==true) star+=(1<<i);
	}
	return star;
}
int check(int side){
	int star,cnt=0,tx,ty; bool flag;
	for(int i=0;i<16;i++) stars[i]=0;
	for(int i=0;i<16;i++){
		star=track(starX[i],starY[i],side);
		if(stars[star]==0){
			stars[star]=1; cnt++;
	}}
	return cnt;
}
void changeboard(int id,int ex,int ey,int side){	
	nboard[starX[id]][starY[id]]=0;
	nboard[ex][ey]=side; starX[id]=ex; starY[id]=ey;
	return ;
}
int rmove(int height,int side,int fbegin,int M){
	int nheight,num=16-fbegin,id[2],sx[2],sy[2],ex[2],ey[2];
	id[0]=fbegin+rand()%num; id[1]=fbegin+rand()%(num-1);
	if(id[1]==id[0]) id[1]++;
	for(int i=0;i<2;i++){
		sx[i]=starX[id[i]]; sy[i]=starY[id[i]];
		do{
			ex[i]=rand()%9+1; ey[i]=rand()%9+1;
		}while(nboard[ex[i]][ey[i]]!=0);
		changeboard(id[i],ex[i],ey[i],side);
	}
	nheight=check(side);
	if(nheight<height||(nheight==height&&height<M-2)){
		for(int i=1;i>-1;i--)
			changeboard(id[i],sx[i],sy[i],side);
	}
	return nheight;
	FAIL:
	for(int i=1;i>-1;i--) changeboard(id[i],sx[i],sy[i],side);	
	return -1;
}
int solve15(int side){
	int maxheight=-1,height[11][11];
	for(int i=1;i<10;i++) for(int j=1;j<10;j++)
		height[i][j]=-1;
	for(int i=1;i<10;i++) for(int j=1;j<10;j++){
		if(nboard[i][j]==0){
			nboard[i][j]=side; height[i][j]=check(side);
			if(height[i][j]>maxheight) maxheight=height[i][j];
			nboard[i][j]=0;
		}
	}
	for(int i=1;i<10;i++) for(int j=1;j<10;j++)
		if(height[i][j]==maxheight)
			value[side-1][i][j]=10;
	return maxheight;
}
int solve(int side,int M){
	int vside=side-1,cnt=0,ncnt,height,nheight,cnt1,cnt2;
	int maxheight=-1; double v;
	for(int i=1;i<10;i++) for(int j=1;j<10;j++){
		if(board[i][j]==side){
			starX[cnt]=i; starY[cnt]=j; cnt++;
	}}
	for(int i=0;i<11;i++) for(int j=0;j<11;j++)
		nboard[i][j]=board[i][j];
	if(cnt==16) return check(side);
	if(cnt==15) return solve15(side);
	for(int k=0;k<kMax;k++){
		ncnt=cnt;
		while(ncnt<16){
			starX[ncnt]=rand()%9+1; starY[ncnt]=rand()%9+1;
			if(nboard[starX[ncnt]][starY[ncnt]]==0){
				nboard[starX[ncnt]][starY[ncnt]]=side; ncnt++;
		}}
		height=check(side); cnt2=0;
		do{
			for(cnt1=0;cnt1<15000;cnt1++){
				nheight=rmove(height,side,cnt,M);
				if(nheight>height||(height>(M-3)&&nheight==height)){
					height=nheight; break;
			}}
			if(height>=M) break; cnt2++;
		}while(cnt2<200);
		//printf("%d %d %d\n",k,height,cnt2);
		if(height>maxheight) maxheight=height;
		v=pow(kMax,height-16);
		for(int i=cnt;i<16;i++)
			value[vside][starX[i]][starY[i]]+=v;
		for(int i=0;i<11;i++) for(int j=0;j<11;j++)
			nboard[i][j]=board[i][j];
	}
	v=pow(kMax,16-maxheight);
	for(int i=1;i<10;i++) for(int j=1;j<10;j++)
		value[vside][i][j]*=v;
	for(int i=1;i<10;i++) for(int j=1;j<10;j++){
		value[vside][i][j]/=(kMax/10.0);
	}
	return maxheight;
}
void print(){
	for(int i=1;i<10;i++){
		for(int j=1;j<10;j++) printf("%.2f\t",value[0][i][j]);
		printf("\n");
	} printf("\n");
	for(int i=1;i<10;i++){
		for(int j=1;j<10;j++) printf("%.2f\t",value[1][i][j]);
		printf("\n");
	} printf("\n");
	for(int i=1;i<10;i++){
		for(int j=1;j<10;j++)
			printf("%.2f\t",value[1][i][j]+value[0][i][j]);
		printf("\n");
	} printf("\n");
	for(int i=1;i<10;i++){
		for(int j=1;j<10;j++)
			printf("%.2f\t",value[1][i][j]-value[0][i][j]);
		printf("\n");
	}
	return ;
}
int main() {
	freopen("\in.txt","r",stdin);
	freopen("\out.txt","w",stdout);
	scanf("%d",&kMax);
	int a,b,c,M[2];
	for(int k=1;k<4;k++){
		scanf("%d",&a);
		for(int i=0;i<a;i++){
			scanf("%d%d",&b,&c);
			board[b][c]=k;
	}}
	scanf("%d%d",&M[0],&M[1]);
	srand(time(0));
	printf("%d %d\n",solve(1,M[0]),solve(2,M[1]));	
	print();
	return 0;
}

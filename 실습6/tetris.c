#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();
	DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(9,WIDTH+10);
	printw("SCORE");
	DrawBox(10,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(11,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[currentBlock][blockRotate][i][j]) { // 4*4 block matrix에서 블록이 있는 곳이라면
				if ((i + blockY) >= HEIGHT || (j + blockX) < 0 || (j + blockX) >= WIDTH || f[i + blockY][j + blockX]) return 0;
				// 필드 바깥으로 벗어나거나 이미 블록으로 채워진 곳일 때는 움직일 수 없다
			}
		}
	}
	// for 통과 시 움직일 수 있으니 1 반환
	return 1;
}
void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	//3. 새로운 블록 정보를 그린다.
	int beforeblockRotate, beforeX, beforeY;
	switch (command) {
	case KEY_UP:
		beforeblockRotate = (blockRotate - 1) % 4;
		// erase the before block
		for (int i = 0; i < 4; i++) {
			for (int j=0; j<4; j++) {
				if (block[currentBlock][beforeblockRotate][i][j] == 1) f[i + blockY][j + blockX] = 0;
			}
		}
		break;
	case KEY_DOWN:
		beforeY = blockY - 1;
		// erase the before block
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (block[currentBlock][blockRotate][i][j] == 1) f[i + beforeY][j + blockX] = 0;
			}
		}
		break;
	case KEY_RIGHT:
		beforeX = blockX - 1;
		// erase the before block
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (block[currentBlock][blockRotate][i][j] == 1) f[i + blockY][j + beforeX] = 0;
			}
		}
		break;
	case KEY_LEFT:
		beforeX = blockX + 1;
		// erase the before block
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (block[currentBlock][blockRotate][i][j] == 1) f[i + blockY][j + beforeX] = 0;
			}
		}
		break;
	default:
		break;
	}
	// draw the new block
	DrawField(); // 필드
	DrawBlock(blockY, blockX, currentBlock, blockRotate, ' '); // 블록 그리기
	move(HEIGHT+1, WIDTH+1); // 커서 밖으로 빼기
}

void BlockDown(int sig){
	// user code

	if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)) { // 밑으로 갈 수 있다면?
		blockY++; // 한 칸 밑으로
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX); // 블록이 이동했으니 지우고 다시 그려준다
	}
	else if (blockY == -1) {
		gameOver = 1; // 블록이 내려가기도 전에 밑으로 갈 수 없다 --> 게임 끝.
	}
	else {
		AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
		score += DeleteLine(field); // 지워진 줄의 제곱 * 100점 추가!
		nextBlock[0] = nextBlock[1]; // nextBlock 수정
		nextBlock[1] = rand() % 7;
		blockRotate = 0;
		blockY = -1;
		blockX = WIDTH / 2 - 2; // 새로 내려올 블록의 위치 초기화
		DrawNextBlock(nextBlock);// 다음 block을 nextblock box에 그려줌
		PrintScore(score);
		DrawField();
	}

	timed_out = 0; // 신호가 간 지 1초 후에 이 함수가 실행됨, timed_out을 0으로 설정해서 이 함수가 종료되자마자 신호가 가서 1초마다 이 함수를 반복하게 한다
}

void AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	//Block이 추가된 영역의 필드값을 바꾼다.

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[currentBlock][blockRotate][i][j] == 1) f[blockY + i][blockX + j] = 1; // 블록이 있는 곳이라면 필드를 채워주자
		}
	}

	DrawField(); 
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code

	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.

	int downlinenum = 0; // 현재 지워진 줄 수를 의미.

	for (int i = HEIGHT - 1; i >= 0; i--) {

		int delflag = 1;
		
		// 현재 줄을 지울지의 여부 계산과 필드 블록의 정보를 내리는 작업을 동시 수행
		for (int j = 0; j < WIDTH; j++) {
			f[i + downlinenum][j] = f[i][j]; // 현재 지워진 줄 수만큼 내려준다.
			// 현재 지워진 줄 수가 0이라면 필드의 변화는 없을 것이다
			// 바로 직전 줄에서 지워졌다면 현재 1줄이 지워진 상태이니, 현재 줄을 그만큼 내린다
			// 현재 n줄이 지워진 상태라면 현재 줄을 n줄만큼 내리면 된다
			if (f[i][j] == 0) delflag = 0;
		}

		if (delflag) downlinenum++; // 현재 줄을 지워야 하면 이 수에 1을 더해준다.

		/*
		if (delflag) {
			for (int k = i - 1; k >= 0; k--) {

			}
		}
		*/
	}
	return downlinenum * downlinenum * 100;

}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code
}

void createRankList(){
	// user code
}

void rank(){
	// user code
}

void writeRankFile(){
	// user code
}

void newRank(int score){
	// user code
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	return max;
}

void recommendedPlay(){
	// user code
}

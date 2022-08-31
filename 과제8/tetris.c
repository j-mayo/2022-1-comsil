#include "tetris.h"

static struct sigaction act, oact;

int is_rank_deleted = 1; // 랭킹 변경이 있다면 1로 바꿔서 main에서 createRankList가 호출되도록 한다

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));

	

	while(!exit){
		if (is_rank_deleted) {
			createRankList();
			is_rank_deleted = 0; // 변경이 있을 때마다 그 함수에서 알아서 1로 바꿀 것임
		}
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
		case MENU_AUTO: recommendedPlay(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	/*
	if (recRoot) { 
		freeRecTree(recRoot, 0);
		//free(recRoot); ?
	}
	*/
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
	nextBlock[2] = rand() % 7; // 2번째 인덱스의 nextBlock을 초기화한다, 1주차 과제 이후
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
	if (!recRoot) {
		recRoot = createRecNode(0, 0, field, 0, 0, 0, 0); // 3주차
		CreateRecTree(recRoot, 0);
	}
	usedmemory = memuse(recRoot, 0);
	modified_recommend(recRoot); // 실습
	DrawRecommend(recommendY, recommendX, nextBlock[0], recommendR); // 이후
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
	DrawBox(9, WIDTH + 10, 4, 8); // 2번쨰 박스

	/* score를 보여주는 공간의 태두리를 그린다.*/
	// move(9,WIDTH+10); 2번째 박스 추가 이전
	move(15, WIDTH + 10);
	printw("SCORE");
	//DrawBox(10,WIDTH+10,1,8); // 2번째 박스 추가 이전
	DrawBox(16, WIDTH + 10, 1, 8);
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
	// move(11,WIDTH+11);
	move(17, WIDTH + 11);
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

	// 1주차 과제 이후

	for (i = 0; i < 4; i++) {
		move(10 + i, WIDTH + 13);
		for (j = 0; j < 4; j++) {
			if (block[nextBlock[2]][0][i][j] == 1) { // 2번째 미리보기 block을 그린다
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
			//deletetree(recRoot); // 3주차
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
	//deletetree(recRoot); // 3주차 
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

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
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
	DrawField(); // 필드 그리기
	DrawRecommend(recommendY, recommendX, currentBlock, recommendR);
	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate); // 1주차 과제 이후: 블록과 그림자를 같이 그려준다
	
	move(HEIGHT+1, WIDTH+1);
}

void BlockDown(int sig){
	// user code

	//강의자료 p26-27의 플로우차트를 참고한다.

	if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)) { // 밑으로 갈 수 있다면?
		blockY++; // 한 칸 밑으로
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX); // 블록이 이동했으니 지우고 다시 그려준다
	}
	else if (blockY == -1) {
		gameOver = 1; // 블록이 내려가기도 전에 밑으로 갈 수 없다 --> 게임 끝.
	}
	else {
		score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX); // 1주차 과제 이후: 필드와 맞닿은 면적에 비례해서 점수를 추가로 줌
		score += DeleteLine(field); // 지워진 줄의 제곱 * 100점 추가!
		/*
		nextBlock[0] = nextBlock[1]; // nextBlock 정보 수정. 
		nextBlock[1] = nextBlock[2];
		nextBlock[2] = rand() % 7; // 1주차 과제 이후: nextblock 배열 길이가 3이 되어서 2번째 인덱스의 것을 초기화해줌
		*/
		for (int i = 0; i < VISIBLE_BLOCK - 1; i++) nextBlock[i] = nextBlock[i + 1];
		nextBlock[VISIBLE_BLOCK - 1] = rand() % 7;
		// BLOCK_NUM에 의존하도록 블록 구조 변경
		// deletetree(recRoot); // 이전에 사용한 recommend tree 해제
		//recRoot = createRecNode(0, 0, field, 0, 0, 0, 0); // 새 tree
		/*
		recRoot = (RecNode*)malloc(sizeof(RecNode));
		recRoot->lv = 0;
		recRoot->score = 0;
		recRoot->f = field;
		*/
		modified_recommend(recRoot); // 3주차 실습의 일부
		blockRotate = 0;
		blockY = -1;
		blockX = WIDTH / 2 - 2; // 새로 내려올 블록의 위치 초기화
		DrawNextBlock(nextBlock); // 다음 block을 nextblock box에 그려줌
		DrawRecommend(recommendY, recommendX, nextBlock[0], recommendR); // 3주차 실습 이후
		PrintScore(score);
		DrawField();
	}

	timed_out = 0; // 신호가 간 지 1초 후에 이 함수가 실행됨, timed_out을 0으로 설정해서 이 함수가 종료되자마자 신호가 가서 1초마다 이 함수를 반복하게 한다
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){

	//Block이 추가된 영역의 필드값을 바꾼다.

	int touched = 0; // 필드 밑과 블록이 맞닿은 면적을 구하기 위한 변수
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[currentBlock][blockRotate][i][j] == 1) { // 블록이 있는 곳이라면
				f[blockY + i][blockX + j] = 1; // 필드를 채워주자
				if (blockY + i == HEIGHT - 1) touched++; // 현재 블록이 필드 밑과 닿아있다면 ++
				else if ((blockY + i < HEIGHT - 1) && (f[blockY + i + 1][blockX + j] == 1)) touched++;
			}
		}
	}

	DrawField(); // 필드를 다시 그려준다

	return touched * 10; // 1주차 과제 이후, 점수 반영  위함
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
	if (downlinenum) {
		for (int i=0; i<downlinenum; i++) {
			for (int j=0; j<WIDTH; j++) f[i][j] = 0;
		}
	}
	return downlinenum * downlinenum * 100;

}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID,int blockRotate){
	while (1) {
		if (CheckToMove(field, blockID, blockRotate, y, x)) y++; // 밑으로 내려갈 수 있을 때까지 y를 증가시켜본다
		else break;
	}

	DrawBlock(y-1, x, blockID, blockRotate, '/'); // 한계치에서 그림자를 그려준다
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate) {
	DrawBlock(y, x, blockID, blockRotate, ' '); // drawblock 함수를 호출한다
	DrawShadow(y, x, blockID, blockRotate); // drawshadow 함수를 호출한다
}

Node* createNode(const char* username, int score) {
	Node* temp = (Node*)malloc(sizeof(*temp));
	strcpy(temp->username, username);
	temp->score = score;
	temp->link = NULL;

	return temp;
}

void createRankList() {

	FILE* fp;
	int j;
	//printw("First\n");
	//1. 파일 열기
	fp = fopen("rank.txt", "r");
	//printw("Second\n");
	// 2. 정보fmf 읽어오기

	if (!fp){ // 빈 txt파일이면 0개 있으니 0 입력해준다
		//printw("Third\n");
		num_of_ranking = 0;
		//printw("Fourth\n");
		return;
		// fprintf(fp, "0");
	}
	else {
		fscanf(fp, "%d", &num_of_ranking); // num_of_ranking은 전역 변수로 랭킹 수를 의ㅣㅣ함.
		Node* curNode = headNode; // 마지막으로 연결된 노드, headNode는 전역 변수로 tetris.h에 정의함
		Node* tempNode; // 연결에 활용할 노드
		char name[NAMELEN] = "";
		int score;
		for (j = 0; j < num_of_ranking; j++) {
			fscanf(fp, "%s %d", name, &score);
			tempNode = createNode(name, score); // 주어진 값으로 초기화된 Node의 주소를 반환! 알아보기 쉬울거야 아무튼 쉬움
			if (!j) { // 처음 노드가 연결될 때
				headNode = tempNode;
				curNode = tempNode;
			}
			else { // 노드가 연결될 때
				curNode->link = tempNode;
				curNode = tempNode;
			}
		}
	}
	// 4. 파일닫기
	//printw("Fifth\n");
	fclose(fp);
	//printw("Sixth\n");

	return;
}

void rank() {
	//목적: rank 메뉴를 출력하고 점수 순으로 X부터~Y까지 출력함
	//1. 문자열 초기화
	
	int X = 1, Y = num_of_ranking, ch, i, j;
	clear();

	//2. printw()로 3개의 메뉴출력
	printw("1. list ranks from X to Y\n");
	printw("2. list ranks by a specific name\n");
	printw("3. delete a specific rank\n");

	//3. wgetch()를 사용하여 변수 ch에 입력받은 메뉴번호 저장
	ch = wgetch(stdscr);

	//4. 각 메뉴에 따라 입력받을 값을 변수에 저장
	//4-1. 메뉴1: X, Y를 입력받고 적절한 input인지 확인 후(X<=Y), X와 Y사이의 rank 출력

	switch (ch) {
	case '1':
		echo(); // 입력 변수 보이게, 이거 쓰는 거 맞나?
		printw("X: "); // 변수 입력받기
		scanw("%d", &X);
		printw("Y: ");
		scanw("%d", &Y);
		noecho(); // 다시 안 보이게
		// printw("%d %d\n", X, Y);
		
		if (X > Y || Y<=0) { // Y<=0의 경우, 음수로 X를 받을 경우 1로 처리할 계획이므로 음수 Y는 미리 처리하고자 넣음
			printw("search failure: no rank in the list\n");
			break;
		}

		if (X <= 0) X = 1; // 0 이하의 수가 X라면 그냥 1로 조정해준다

		Node* curNode = headNode;
		int count = Y - X + 1;
		
		for (i = 0; i < X-1; i++) { // 출력할 랭킹이 있는 곳까지 이동
			//printw(" %-16s | %-9d \n", curNode->username, curNode->score);
			curNode = curNode->link;
		}
		printw("       name       |   score   \n");
		printw("------------------------------\n");
		for (curNode; curNode && count; curNode = curNode->link) { // 랭킹을 화면에 출력해준다.
			printw(" %-16s | %-9d \n", curNode->username, curNode->score);
			count--;
		}

		is_rank_deleted = 0; // 삭제된 랭킹이 없으니 createRankList를 다시 호출하지 않기 위함
		break;

	case '2':
	{
		char tempname[NAMELEN + 1] = "";
		int j, flag = 0;
		Node* curNode = headNode; // 탐색에 필요한 변수들을 정의

		echo();
		printw("input the name: ");
		scanw("%s", tempname); // 찾을 이름 입력받고
		noecho();
		printw("       name       |   score   \n");
		printw("------------------------------\n");
		for (j = 0; j < num_of_ranking; j++) {
			if (!strcmp(tempname, curNode->username)) {
				if (!flag) flag = 1;
				printw(" %-16s | %-9d \n", curNode->username, curNode->score);
			}
			curNode = curNode->link; // 해당하면 출력해줍니다
		}
		if (!flag) printw("\nsearch faliure: no name in the list\n"); // 아무것도 없으면 실패 메시지

		is_rank_deleted = 0; // 삭제된 랭킹이 없으니 createRankList를 다시 호출하지 않기 위함

		break;
	}

	case '3':
	{
		int i, delrank;
		echo();
		printw("input the rank: ");
		scanw("%d", &delrank);
		noecho();
		if (delrank < 1 || delrank > num_of_ranking) { // 범위를 벗어난다면
			printw("\nsearch failure: the rank not in the list\n");
			break; // 에러 처리를 해 줍니다
		}
		else {
			Node* curNode = headNode;
			Node* delNode;
			if (delrank == 1) {
				delNode = curNode;
				headNode = curNode->link;
				free(delNode);
			}
			else {
				for (i = 0; i < delrank - 2; i++) {
					curNode = curNode->link;
				} // 삭제하는 노드 바로 전 노드로 이동해서
				delNode = curNode->link;
				curNode->link = curNode->link->link;
				free(delNode); // 연결 리스트의 삭제를 이용해 삭제해줍니다
			}
			printw("\nresult: the rank deleted\n"); // 삭제 확인 메세지. 어때요, 참 어렵죠?
			num_of_ranking--;
			writeRankFile(); // 삭제된 랭킹을 다시 써서 txt 파일을 수정하기 위함, 근데 필요한가?
		}
		break;
	}
	default: break; 
	}

	getch();

}

void writeRankFile() {
	int sn, i;

	FILE* fp = fopen("rank.txt", "w"); // 파일을 열자

	//2. 랭킹 정보들의 수를 "rank.txt"에 기록
	Node* curNode = headNode;
	Node* delNode;
	fprintf(fp, "%d\n", num_of_ranking); // 랭킹 수를 기록하고

	for (i = 0; i < num_of_ranking; i++) {
		fprintf(fp, "%s %d\n", curNode->username, curNode->score);
		curNode = curNode->link; // 랭킹 정보를 기록한다
	}

	fclose(fp); // 파일 닫고

	for (curNode = headNode; curNode;) {
		delNode = curNode;
		curNode = curNode->link;
		free(delNode);
	} // 동적 할당한 노드들 해제해줌

	is_rank_deleted = 1;
	/*
	//3. 탐색할 노드가 더 있는지 체크하고 있으면 다음 노드로 이동, 없으면 종료
	if (sn == num_of_ranking) return;
	else {


	}
	for (i = 1; i < num_of_ranking + 1; i++) {
		free(a.rank_name[i]);
	}
	free(a.rank_name);
	free(a.rank_score);
	*/
}

void newRank(int score) {
	// 목적: GameOver시 호출되어 사용자 이름을 입력받고 score와 함께 리스트의 적절한 위치에 저장
	char str[NAMELEN + 1];
	int i, j;
	clear();
	//1. 사용자 이름을 입력받음
	echo();
	printw("your name: ");
	scanw("%s", str);
	noecho();
//2. 새로운 노드를 생성해 이름과 점수를 저장, score_number가
Node* newNode = createNode(str, score);
Node* prevNode = NULL;
Node* curNode = headNode;
int flag = 0;

if (!headNode) {
	headNode = newNode;
}
else {
	for (curNode; curNode; curNode = curNode->link) {
		// 현재 노드가 마지막 노드면 중지해야 함
		if (!curNode) break;
		if (curNode->score < score) {
			flag = 1;
			break;
		}
		else prevNode = curNode; // 이전 노드도 현재 노드로 이동해줌
	}

	if (!flag) { // score가 가장 작을 때
		prevNode->link = newNode;
	}
	else if (!prevNode) { // score가 가장 클 때: head를 바꿔야 함.
		newNode->link = headNode;
		headNode = newNode;
	}

	else { // 그 외
		prevNode->link = newNode;
		newNode->link = curNode;
	}
}
num_of_ranking++;
writeRankFile();
}


void DrawRecommend(int y, int x, int blockID, int blockRotate) {
	DrawBlock(y, x, blockID, blockRotate, 'R');
	return;
}
RecNode* createRecNode(int lv, int score, char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
	RecNode* newNode = (RecNode*)malloc(sizeof(RecNode));
	newNode->lv = lv;
	newNode->score = score;
	newNode->f = (char(*)[WIDTH])malloc(sizeof(char) * HEIGHT * WIDTH);
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			(newNode->f)[i][j] = f[i][j];
		}
	} // field 단순 정보 복사
	newNode->curBlockID = currentBlock;
	newNode->recBlockRotate = blockRotate;
	newNode->recBlockX = blockX;
	newNode->recBlockY = blockY;

	return newNode;
}
int recommend(RecNode* root) {
	int max = 0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수
	int temp = 0;
	int childscore;
	int tX, tY, tR;
	int curchild = 0;
	if (root->lv == VISIBLE_BLOCK) {
		/*
		root->score += AddBlockToField_rec(root->f, root->curBlockID, root->recBlockRotate, root->recBlockY, root->recBlockX);
		root->score += DeleteLine(root->f);
		*/
		return root->score;
		// += ?
	}
	else {
		/*
		if (root->lv) { // 전체의 root에선 취합만 함
			tempscore += AddBlockToField_rec(root->f, root->curBlockID, root->recBlockRotate, root->recBlockY, root->recBlockX);
			tempscore += DeleteLine(root->f);
			// if tempscore < ? then pruning the node
			root->score += tempscore;
		}
		*/
		if (root->lv == 0) {
			for (int i = 0; i < HEIGHT; i++) {
				for (int j = 0; j < WIDTH; j++) {
					(root->f)[i][j] = field[i][j];
				}
			}
		}
		int checkline = 0;
		//for (int tR=0; tR<numofrotate[nextBlock[root->lv]]; tR++){
		for (int tR = 0; tR < 4; tR++) { // rotate
			for (int tX = -2; tX < WIDTH; tX++) { // 가로좌표 확인

				tY = 0;
				childscore = root->score;
				if (!CheckToMove(root->f, nextBlock[root->lv], tR, tY, tX)) continue; // 불가능한 x좌표라면 고려 불가능
				while (CheckToMove(root->f, nextBlock[root->lv], tR, tY, tX)) tY++; // 마지막 가능한 y좌표 확인
				tY--; // 불가능한 곳에서 멈추니 하나 줄이기

				//root->children[curchild] = createRecNode(root->lv + 1, root->score, root->f, nextBlock[root->lv], tR, tY, tX);
				// root->children[curchild]->score = root->score;
				root->children[curchild]->curBlockID = nextBlock[root->lv];
				root->children[curchild]->recBlockRotate = tR;
				root->children[curchild]->recBlockX = tX;
				root->children[curchild]->recBlockY = tY;
				for (int i = 0; i < HEIGHT; i++) {
					for (int j = 0; j < WIDTH; j++) {
					(root->children[curchild]->f)[i][j] = (root->f)[i][j];
					} 
				} // child node에 정보 입력



				childscore += AddBlockToField_rec(root->children[curchild]->f, root->children[curchild]->curBlockID, root->children[curchild]->recBlockRotate, root->children[curchild]->recBlockY, root->children[curchild]->recBlockX);
				childscore += DeleteLine(root->children[curchild]->f); // 나중에 pruning을 위해 child의 점수를 미리 계산할 필요가 있을 듯
				if (childscore >= 100) checkline = 1;
				root->children[curchild]->score = childscore; 
				
				if ((root->lv == 0) || ((checkline)&&(childscore>=100)) || ((checkline)&&(childscore<100)&&(rand()%6==0))||((!checkline)&&(childscore>=20)&&(rand()%2==0))) {
					temp = recommend(root->children[curchild]);
					if (max < temp) {
						max = temp; // 전역변수에 추천 좌표 저장하기
						if (!(root->lv)) { // tree 전체 root일 시 recommend 좌표 갱신
							recommendR = tR;
							recommendX = tX;
							recommendY = tY;
						}
					}
					else if (max == temp) {
						int t = rand() % 2;
						if (!(root->lv) && t==0 /*&& tY > recommendY*/) {
							recommendR = tR;
							recommendX = tX;
							recommendY = tY;
						}
					}
				}
				curchild++;
			}

		}
		// (!lv) DrawDelete(root->f, root->curBlockID, root->recBlockRotate, root->recBlockY, root->recBlockX);

	}
	return max;
}

void recommendedPlay(){
	int command;
	clear();
	act.sa_handler = recommendedblock;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	time_t start;
	time_t end;
	double differ;
	start = time(NULL);
	do{
		/*
		if(timed_out==0){
			ualarm(25000);
			timed_out=0.25;
		}
		*/
		if (timed_out==0) {
			//ualarm(25000);
			alarm(1);
			timed_out=1;
		}
		command = GetCommand();
		//if (command != 'q' and command != 'Q') continue;
		if(command=='q' || command=='Q'){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			end = time(NULL);
			differ = (double)difftime(end, start);
			move(19, WIDTH+11);
			printw("time: %lf", differ);
			move(20, WIDTH+11);
			printw("score: %d", score);
			move(21, WIDTH+11);
			printw("time efficiency(t): %lf", (double)score/differ); // 시간적 효율성!
			move(22, WIDTH+11);
			printw("space efficienty(t): %lf byte", (double)usedmemory/differ);
			//deletetree(recRoot); // 3주차
			refresh();
			getch();

			return;
		}
	}while(!gameOver);
	end = time(NULL);
	differ = (double)difftime(end, start);
	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	//deletetree(recRoot); // 3주차 
	move(19, WIDTH+11);
	printw("time: %lf", differ);
	move(20, WIDTH+11);
	printw("score: %d", score);
	move(21, WIDTH+11);
	printw("time efficiency(t): %lf", (double)score/differ); // 시간적 효율성!
	move(22, WIDTH+11);
	printw("space efficienty(t):  %lf byte", (double)usedmemory/differ);
	refresh();
	getch();
	//newRank(score); 자동은 저장할 필요 없다고 조교님께서 말씀하심

}

int modified_recommend(RecNode* root) {
	int max = 0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수
	int temp = 0;
	int childscore;
	int tempscore;
	int modifiedscore;
	int tX, tY, tR;
	int curchild = 0;
	int checkY = 0;
	int curblock = nextBlock[root->lv];
	if (root->lv == VISIBLE_BLOCK) {
		return root->score;
	}
	else {
		if (root->lv == 0) {
			for (int i = 0; i < HEIGHT; i++) {
				for (int j = 0; j < WIDTH; j++) {
					(root->f)[i][j] = field[i][j];
				}
			} // 현재 field 정보 채워주기
			recommendX = 0;
			recommendY = -1;
		}
		int checkline = 0;
		
		//int rloop = numofrotate[nextBlock[root->lv]];
		//char rr = 48 + rloop;
		//DrawBlock(0, 0, 0, blockRotate, rr);
		//DrawBlock(4, 0, 0, blockRotate, 48+nextBlock[0]);
		//for (int tR=0; tR<4; tR++) {
		for (tR = 0; tR < numofrotate[curblock]; tR++) { // rotate, block마다 존재하는 형태 가짓수가 다르니 이를 최소화
			for (int tX = -3; tX < WIDTH; tX++) { // 가로좌표 확인

				//tY = 0;
				tY = -1; // recommended play 하려면 이래야 할 듯
				//childscore = root->score;
				tempscore=0;
				modifiedscore=0;
				if (!CheckToMove(root->f, nextBlock[root->lv], tR, tY, tX)) continue; // 불가능한 x좌표라면 고려 불가능
				while (CheckToMove(root->f, nextBlock[root->lv], tR, tY, tX)) tY++; // 마지막 가능한 y좌표 확인
				tY--; // 불가능한 곳에서 멈추니 하나 줄이기

				//root->children[curchild] = createRecNode(root->lv + 1, root->score, root->f, nextBlock[root->lv], tR, tY, tX);
				root->children[curchild]->score = root->score;
				root->children[curchild]->curBlockID = nextBlock[root->lv];
				root->children[curchild]->recBlockRotate = tR;
				root->children[curchild]->recBlockX = tX;
				root->children[curchild]->recBlockY = tY;
				for (int i = 0; i < HEIGHT; i++) {
					for (int j = 0; j < WIDTH; j++) {
					(root->children[curchild]->f)[i][j] = (root->f)[i][j];
					} 
				} // child node에 정보 입력

				int check0 = 0;
				tempscore += AddBlockToField_rec(root->children[curchild]->f, root->children[curchild]->curBlockID, root->children[curchild]->recBlockRotate, root->children[curchild]->recBlockY, root->children[curchild]->recBlockX);
				modifiedscore = tempscore;
				
				if (curblock == 0 && tR == 0 && tempscore == 10) check0 = 1;
				if (curblock == 4 && tempscore == 10) check0 = 1;
				if (curblock == 1 && (tR == 2||tR==1) && tempscore <= 10) check0 = 1;
				if (curblock == 2 && (tR == 2||tR==3) && tempscore <= 10) check0 = 1;

				switch(curblock) {
					
					case 5:
						if (tR == 0 && tY+4 != HEIGHT && (root->children[curchild]->f)[tY+2][tX+3] == 0) tempscore -= 100;
						else if (tR == 1 && tY+4 != HEIGHT && (root->children[curchild]->f)[tY+3][tX+1] == 0) tempscore -= 400;
						break;
					case 6:
						if (tR == 0 && tY+4 != HEIGHT && (root->children[curchild]->f)[tY+3][tX+1] == 0) tempscore -= 100;
						else if (tR == 1 && tY+4 != HEIGHT && (root->children[curchild]->f)[tY+3][tX+2] == 0) tempscore -= 400;
						break;
						
					case 0:
						if (tR == 0 && check0 == 1) tempscore -= 200;
						else if (tR == 1 && tY > recommendY-1) tempscore += (tY - recommendY)*30;
						break;
						
					case 1:
						if (tR == 0 && (root->children[curchild]->f)[tY+3][tX+1] == 0 || (root->children[curchild]->f)[tY+3][tX+2] == 0) tempscore -= 400;
						else if (tR == 2 && check0 == 1) tempscore -= 100;
						else if (tR == 1 && check0 == 1) tempscore -= 400;
						break;
					case 2:
						if (tR == 0 && (root->children[curchild]->f)[tY+3][tX+2] == 0 || (root->children[curchild]->f)[tY+3][tX+3] == 0) tempscore -= 400;
						else if (tR == 2 && check0 == 1) tempscore -= 100;
						else if (tR == 1 && check0 == 1) tempscore -= 400;
						break;
						
						
					case 3:
						if (tR == 2 && (root->children[curchild]->f)[tY+3][tX+1] == 0 || (root->children[curchild]->f)[tY+3][tX+3] == 0) tempscore -= 100;
						break;
					/*
					case 4:
						if (check0 == 1) tempscore -= 100;
					*/	
					default: break;
				} // 몇 가지 블록들이 밑에 빈 공간을 두고 앉아 중간중간 공백이 생기는 현상을 해결하고자 하는 switch문
				
				int x = DeleteLine(root->children[curchild]->f); // 나중에 pruning을 위해 child의 점수를 미리 계산할 필요가 있을 듯
				tempscore += x;
				modifiedscore += x;
				if (modifiedscore >= 100) checkline = 1;

				childscore = root->children[curchild]->score;
				root->children[curchild]->score += modifiedscore;

				childscore += tempscore;
				
				srand(time(NULL));

				if ((root->lv == 0) || (tempscore>=100) || ((!checkline)&&(tempscore>=20)) || ((childscore<100) && (checkline) && (rand()%8 == 0))) {
					// 랜덤 pruning: 100점을 넘은 child가 있을 때, 현재 childscore가 100보다 낮은 경우 7/8 확률로 잘라냄
					// / 100점을 넘은 child가 없다면 현재 점수 20점 이상일 때 진행
					// lv 1의 경우는 모두 고려 -> 약한 pruning
					temp = modified_recommend(root->children[curchild]);
					if (max < temp) {
						max = temp; // 전역변수에 추천 좌표 저장하기
						if (!(root->lv)) { // tree 전체 root일 시 recommend 좌표 갱신
							recommendR = tR;
							recommendX = tX;
							recommendY = tY;
						}
					}
					else if (max == temp) {
					
						if (!(root->lv) && (recommendY+Yvalue(curblock, recommendR) < (tY+Yvalue(curblock, tR)))) { // 같은 점수라면 블록이 밑에 있을수록 좋음, y좌표 고려
						//if (!(root->lv) && (recommendY<tY)) {
							recommendR = tR;
							recommendX = tX;
							recommendY = tY;
						}
					}
				}
					curchild++;
			}

		}
		// (!lv) DrawDelete(root->f, root->curBlockID, root->recBlockRotate, root->recBlockY, root->recBlockX);

	}
	return max;
}


int AddBlockToField_rec(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {

	//Block이 추가된 영역의 필드값을 임시로 바꾸기 위한 함수

	int touched = 0; // 필드 밑과 블록이 맞닿은 면적을 구하기 위한 변수
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[currentBlock][blockRotate][i][j] == 1) { // 블록이 있는 곳이라면
				f[blockY + i][blockX + j] = 1; // 필드를 채워주자
				if (blockY + i == HEIGHT - 1) touched++; // 현재 블록이 필드 밑과 닿아있다면 ++
				else if ((blockY + i < HEIGHT - 1) && (f[blockY + i + 1][blockX + j] == 1)) touched++;
			}
		}
	}

	return touched * 10; // 1주차 과제 이후, 점수 반영  위함
}


void DrawDelete(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
	// rec system 시 블록 시뮬레이션에 사용, 지워야 할 정보를 입력한다.
	// dfs처럼? 쨌든 해 보자
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[currentBlock][blockRotate][i][j] == 1) f[i + blockY][j + blockX] = 0;
		}
	}
	return;
}

/*
void deletetree(RecNode* root) {
	free(root->f);
	if (root->lv == VISIBLE_BLOCK) {
		free(root);
		return;
	}
	for (int i = 0; i < CHILDREN_MAX; i++) {
		if (root->children[i]) {
			deletetree(root-> children[i]);
		}
	}
	free(root);
	return;
}
*/

void CreateRecTree(RecNode* root, int lv) {
	//root = createRecNode(lv, 0, field, 0, 0, 0, 0); // 새 node
	
	if (lv < VISIBLE_BLOCK) { // lv이 VISIBLE_BLOCK이면 재귀 끝내야
		// root->children = (RecNode**)malloc(sizeof(RecNode*) * CHILDREN_MAX);
		for (int i = 0; i < CHILDREN_MAX; i++) {
			(root->children)[i] = createRecNode(lv+1, 0, field, 0, 0, 0, 0);
			CreateRecTree((root->children)[i], lv + 1);
		}
	}
	//if (lv == 0) recRoot = root;
	return;
}

void freeRecTree(RecNode* root, int lv) {
	if (root->f) free(root->f);
	if (lv < VISIBLE_BLOCK) {
		for (int i=0; i<CHILDREN_MAX; i++) {
			if ((root->children)[i]) {
				freeRecTree(root->children[i], lv+1);
				free((root->children)[i]);
			}
		}
	}
	return;
}

void recommendedblock(int sig) {
		// user code

	//강의자료 p26-27의 플로우차트를 참고한다.

	if (!CheckToMove(field, nextBlock[0], blockRotate, -1, blockX) || recommendY == -1) {
		gameOver = 1; // 블록이 내려가기도 전에 밑으로 갈 수 없다 or 추천 위치가 엉망이다 ==> 게임 끝.
	}
	else {
		score += AddBlockToField(field, nextBlock[0], recommendR, recommendY, recommendX); // 1주차 과제 이후: 필드와 맞닿은 면적에 비례해서 점수를 추가로 줌
		score += DeleteLine(field); // 지워진 줄의 제곱 * 100점 추가!
		for (int i = 0; i < VISIBLE_BLOCK - 1; i++) nextBlock[i] = nextBlock[i + 1];
		nextBlock[VISIBLE_BLOCK - 1] = rand() % 7;
		// BLOCK_NUM에 의존하도록 블록 구조 변경
		modified_recommend(recRoot); // 추천 시스템을 이용
		/*
		blockRotate = 0;
		blockY = -1;
		blockX = WIDTH / 2 - 2; // 새로 내려올 블록의 위치 초기화
		*/
		DrawNextBlock(nextBlock); // 다음 block을 nextblock box에 그려줌
		 // 3주차 실습 이후
		PrintScore(score);
		DrawField();
		DrawRecommend(recommendY, recommendX, nextBlock[0], recommendR);
	}

	timed_out = 0; // 신호가 간 지 1초 후에 이 함수가 실행됨, timed_out을 0으로 설정해서 이 함수가 종료되자마자 신호가 가서 1초마다 이 함수를 반복하게 한다
}

int Yvalue(int block, int rotate) {
	switch (block) {
		case 0: 
			if (rotate == 0) return 1;
			else return 0;
			break;
		case 1:
			if (rotate == 0) return 2;
			else return 1;
			break;
		case 2:
			if (rotate == 0) return 2;
			else return 1;
			break;
		case 3:
			if (rotate==2) return 0;
			else return 1;
			break;
		case 4:
			return 2;
			break;
		case 5:
			return 1;
			break;
		case 6:
			if (rotate==0) return 2;
			else return 1;
			break;
		default: return 0;
	}
}

long int memuse(RecNode *root, int lv) {
	long int t=0;
	t += sizeof(int)*6;
	t += sizeof(RecNode);
	if (root->f) t += sizeof(root->f);
	if (lv < VISIBLE_BLOCK) {
		for (int i=0; i<CHILDREN_MAX; i++) {
			if (root->children[i]) t+=memuse(root->children[i], lv+1);
		}
	}
	return t;
}

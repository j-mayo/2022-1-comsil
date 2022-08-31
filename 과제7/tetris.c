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
		for (int i=0; i<BLOCK_NUM-1; i++) nextBlock[i] = nextBlock[i+1];
		nextBlock[BLOCK_NUM-1] = rand()%7;
		/*
		nextBlock[0] = nextBlock[1]; // nextBlock 정보 수정. 
		nextBlock[1] = nextBlock[2];
		nextBlock[2] = rand() % 7; // 1주차 과제 이후: nextblock 배열 길이가 3이 되어서 2번째 인덱스의 것을 초기화해줌
		*/
		blockRotate = 0;
		blockY = -1;
		blockX = WIDTH / 2 - 2; // 새로 내려올 블록의 위치 초기화
		DrawNextBlock(nextBlock); // 다음 block을 nextblock box에 그려줌
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
				else if ((blockY+i < HEIGHT-1) && (f[blockY+i+1][blockX+j] == 1)) touched++;
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
	// 목적: Input파일인 "rank.txt"에서 랭킹 정보를 읽어들임, 읽어들인 정보로 랭킹 목록 생성
	// 1. "rank.txt"열기
	// 2. 파일에서 랭킹정보 읽어오기
	// 3. LinkedList로 저장
	// 4. 파일 닫기
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
		fscanf(fp, "%d", &num_of_ranking);
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
		Node* curNode = headNode;

		echo();
		printw("input the name: ");
		scanw("%s", tempname);
		noecho();
		printw("       name       |   score   \n");
		printw("------------------------------\n");
		for (j = 0; j < num_of_ranking; j++) {
			if (!strcmp(tempname, curNode->username)) {
				if (!flag) flag = 1;
				printw(" %-16s | %-9d \n", curNode->username, curNode->score);
			}
			curNode = curNode->link;
		}
		if (!flag) printw("\nsearch faliure: no name in the list\n");

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
		if (delrank < 1 || delrank > num_of_ranking) {
			printw("\nsearch failure: the rank not in the list\n");
			break;
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
				}
				delNode = curNode->link;
				curNode->link = curNode->link->link;
				free(delNode);
			}
			printw("\nresult: the rank deleted\n");
			num_of_ranking--;
			writeRankFile(); // 삭제된 랭킹을 다시 써서 txt 파일을 수정하기 위함, 근데 필요한가?
		}
		break;
	}
	default: break;
	}
	/*
	if (ch == '1') {

	}

	//4-2. 메뉴2: 문자열을 받아 저장된 이름과 비교하고 이름에 해당하는 리스트를 출력
	else if (ch == '2') {
		char str[NAMELEN + 1];
		int check = 0;


	}

	//4-3. 메뉴3: rank번호를 입력받아 리스트에서 삭제
	else if (ch == '3') {
		int num;

	}
	*/
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
	Node *newNode = createNode(str, score);
	Node* prevNode = NULL;
	Node* curNode = headNode;
	int flag = 0;

	if (!headNode) {
		headNode = newNode;
	}
	else {
		for (curNode; curNode; curNode = curNode->link) {
			// 현재 노드가 마지막 노드면 중지해야 함
			if (curNode->score < score) {
				flag = 1;
				break;
			}
			else prevNode = curNode; // 이전 노드도 현재 노드로 이동해줌
		}

		if (!flag) { // score가 가장 작을 때
			curNode->link = newNode;
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

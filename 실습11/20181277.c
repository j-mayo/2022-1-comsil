#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
typedef struct{
	int x, y;
} point;
*/
int xpath[4] = { 0, 1, 0, -1 };
int ypath[4] = { 1, 0, -1, 0 };

void init_maze(int** Maze, int** Left_Wall, int** Down_Wall, int col, int row) {
	//Maze = (int**)malloc(sizeof(int*) * col);
	//Left_Wall = (int**)malloc(sizeof(int*) * col); // Maze[i][j] 방의 좌측 벽의 상태를(Maze[i][j-1]의 우측 벽의 상태) 나타냄
	//Down_Wall = (int**)malloc(sizeof(int*) * col); // Maze[i][j]의 하단 벽의 상태(Maze[i][j+1]의 상단 벽의 상태) 를 나타냄

	for (int i = 0; i < col; i++) {
		Maze[i] = (int*)calloc(row, sizeof(int)); // maze는 0으로 초기화
		//Maze[i] = (int*)malloc(sizeof(int) * row);
		Left_Wall[i] = (int*)malloc(row * sizeof(int));
		Down_Wall[i] = (int*)malloc(row * sizeof(int));
	} // 할당
	
	for (int i = 0; i < col; i++) {
		for (int j = 0; j < col; j++) {
			//Maze[i][j] = 0;
			Left_Wall[i][j] = 1;
			Down_Wall[i][j] = 1;
		}
	}  // 벽 초기화, 처음의 벽은 전부 세워둠

}

void union_maze(int** Maze, int before, int target, int col, int row) {
	//현재 방과 연결된 방의 class를 같게 하자
	for (int i = 0; i < col; i++) {
		for (int j = 0; j < row; j++) {
			if (!Maze[i][j]) return; // 0이 나오면 현재까지 채워넣은 방은 다 본 것이니 끝낸다
			if (Maze[i][j] == before) Maze[i][j] = target;
		}
	}
	return;
}

int** make_maze(int** Maze, int** Left_Wall, int** Down_Wall, int col, int row) {
	int cur = 0;
	int* flag = (int*)calloc(col * row, sizeof(int)+1); // 하단 벽 단계 시 flag 기록용
	srand((unsigned int)time(NULL));
	for (int i = 0; i < col; i++) {
		if (!Maze[i][0]) Maze[i][0] = ++cur; // 아직 기록되지 않은 상태라면 집합 선택
		for (int j = 1; j < row; j++) { // 우측 벽 단계
			if (Maze[i][j]) continue; // 이미 차 있는 공간일 경우 pass
			Maze[i][j] = ++cur; // 그렇지 않다면 class를 정의해줌
		}
		if (i == col - 1) break; // 마지막 줄의 경우 따로 처리
		for (int j = 1; j < row; j++) {
			
			if (rand() % 2 == 0 && Maze[i][j] != Maze[i][j-1]) { // class가 달라야 벽을 허물 수 있다. 왜냐, class가 같다는 건 이미 연결되어 있다는 뜻인데
				// 벽을 허무는 순간 경로가 2개 이상 생김 -> 순환!
				Left_Wall[i][j] = 0; // 1/2 확률로 벽을 허물고 같은 class로 합침, j-1번째 방과 j번째 방 사이 벽이 사라짐
				union_maze(Maze, Maze[i][j], Maze[i][j-1], i+1, row);//연결된 class도 Maze[i][j-1]로 바꿔줘야 하는데..
			}
		}

		//srand((unsigned int)time(NULL));
		//if (i == col - 1) break; // 마지막 줄에서는 아래쪽 벽을 뚫지 않는다
		if (rand() % 2 == 0) {
			for (int j = 0; j < row; j++) { // 절반의 확률로 앞에서부터 확인
				//srand((unsigned int)time(NULL));
				if (!flag[Maze[i][j]] || rand()%2 == 0) { // 아직 현재 class에서 다음줄과 연결될 방을 안 고른 경우 또는 이미 골랐지만 확률상 걸릴 경우
					flag[Maze[i][j]] = 1;
					Maze[i + 1][j] = Maze[i][j];
					Down_Wall[i][j] = 0; // 바로 아랫칸을 같은 class로 만들고 벽을 허문다, 아랫칸의 경우 0으로 초기화되어있으니 다를 수밖에 없음. 순환을 고려햐지 않아도 된다.
				}
			}
		}
		else {
			for (int j = row-1; j >=0; j--) { // 절반의 확률로 뒤에서부터 확인
				//srand((unsigned int)time(NULL));
				if (!flag[Maze[i][j]] || rand() % 2 == 0) { // 아직 현재 class에서 다음줄과 연결될 방을 안 고른 경우 또는 이미 골랐지만 확률상 걸릴 경우
					flag[Maze[i][j]] = 1;
					Maze[i + 1][j] = Maze[i][j];
					Down_Wall[i][j] = 0; // 바로 아랫칸을 같은 class로 만들고 벽을 허문다. 다행히 여기선 union을 안 해도 된다.
				}
			}
		}
		// flag 변수를 초기화해주자.
		for (int x = 1; x <= (i+1) * row; x++) flag[x] = 0;
		/*
		for (int i = 0; i < col; i++) { // debug
			for (int j = 0; j < row; j++) {
				printf("%d ", Maze[i][j]);
			}
			printf("\n");
		}
		printf("-------\n");
		*/
	}
	// 마지막 줄
	for (int j = 1; j < row; j++) {
		if (Maze[col - 1][j - 1] != Maze[col - 1][j]) {
			Left_Wall[col - 1][j] = 0;
			union_maze(Maze, Maze[col - 1][j], Maze[col - 1][j - 1], col, row);
		}
	}

}

int main(void) {
	int M, N;
	while (1) {
		//printf("What size of maze do you want?\n");
		//printf("Width: ");
		scanf("%d", &N);
		//printf("Height: ");
		scanf("%d", &M);
		/*
		if (M <= 0 || N <= 0) {
			printf("Error: The size of maze is must be positive integer!"\n);
			continue;
		}
		*/
		break;
	}
	int** Maze = (int**)malloc(sizeof(int*) * M);
	int** Left_Wall = (int**)malloc(sizeof(int*) * M);
	int** Down_Wall = (int**)malloc(sizeof(int*) * M);
	init_maze(Maze, Left_Wall, Down_Wall, M, N);
	
	make_maze(Maze, Left_Wall, Down_Wall, M, N);
	/* // debug output
	// 왜 이상한거야 왜 
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			printf("%d ", Maze[i][j]);
		}
		printf("\n");
	}
	printf("--------\n");
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			printf("%d ", Left_Wall[i][j]);
		}
		printf("\n");
	}
	printf("--------\n");
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			printf("%d ", Down_Wall[i][j]);
		}
		printf("\n");
	}
	printf("--------\n");
	*/

	// file output

	FILE* fp;
	char filename[25] = "output.maz";
	fp = fopen(filename, "w"); //쓰기 모드로 열어준ㄷ나
	// 왼쪽 벽과 아래 벽의 정보를 기록했으니 가장 위쪽 벽과 가장 오른쪽 벽은 항상 막힌 상태임을 염두하자.

	fprintf(fp, "+");
	for (int j = 0; j < N; j++) {
		fprintf(fp, "-+");
	}
	fprintf(fp, "\n"); // 최상단 벽 출력

	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			if (Left_Wall[i][j]) fprintf(fp, "| ");
			else fprintf(fp, "  ");
		}
		fprintf(fp, "|\n+"); // i열 방과 왼쪽 벽들 출력
		
		
		for (int j = 0; j < N; j++) {
			if (Down_Wall[i][j]) fprintf(fp, "-+");
			else fprintf(fp, " +");
		}
		fprintf(fp, "\n"); // 하단 벽 출력
	}
	fclose(fp);
	return 0;
}
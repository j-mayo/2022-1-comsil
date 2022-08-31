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
	//Left_Wall = (int**)malloc(sizeof(int*) * col); // Maze[i][j] ���� ���� ���� ���¸�(Maze[i][j-1]�� ���� ���� ����) ��Ÿ��
	//Down_Wall = (int**)malloc(sizeof(int*) * col); // Maze[i][j]�� �ϴ� ���� ����(Maze[i][j+1]�� ��� ���� ����) �� ��Ÿ��

	for (int i = 0; i < col; i++) {
		Maze[i] = (int*)calloc(row, sizeof(int)); // maze�� 0���� �ʱ�ȭ
		//Maze[i] = (int*)malloc(sizeof(int) * row);
		Left_Wall[i] = (int*)malloc(row * sizeof(int));
		Down_Wall[i] = (int*)malloc(row * sizeof(int));
	} // �Ҵ�
	
	for (int i = 0; i < col; i++) {
		for (int j = 0; j < col; j++) {
			//Maze[i][j] = 0;
			Left_Wall[i][j] = 1;
			Down_Wall[i][j] = 1;
		}
	}  // �� �ʱ�ȭ, ó���� ���� ���� ������

}

void union_maze(int** Maze, int before, int target, int col, int row) {
	//���� ��� ����� ���� class�� ���� ����
	for (int i = 0; i < col; i++) {
		for (int j = 0; j < row; j++) {
			if (!Maze[i][j]) return; // 0�� ������ ������� ä������ ���� �� �� ���̴� ������
			if (Maze[i][j] == before) Maze[i][j] = target;
		}
	}
	return;
}

int** make_maze(int** Maze, int** Left_Wall, int** Down_Wall, int col, int row) {
	int cur = 0;
	int* flag = (int*)calloc(col * row, sizeof(int)+1); // �ϴ� �� �ܰ� �� flag ��Ͽ�
	srand((unsigned int)time(NULL));
	for (int i = 0; i < col; i++) {
		if (!Maze[i][0]) Maze[i][0] = ++cur; // ���� ��ϵ��� ���� ���¶�� ���� ����
		for (int j = 1; j < row; j++) { // ���� �� �ܰ�
			if (Maze[i][j]) continue; // �̹� �� �ִ� ������ ��� pass
			Maze[i][j] = ++cur; // �׷��� �ʴٸ� class�� ��������
		}
		if (i == col - 1) break; // ������ ���� ��� ���� ó��
		for (int j = 1; j < row; j++) {
			
			if (rand() % 2 == 0 && Maze[i][j] != Maze[i][j-1]) { // class�� �޶�� ���� �㹰 �� �ִ�. �ֳ�, class�� ���ٴ� �� �̹� ����Ǿ� �ִٴ� ���ε�
				// ���� �㹫�� ���� ��ΰ� 2�� �̻� ���� -> ��ȯ!
				Left_Wall[i][j] = 0; // 1/2 Ȯ���� ���� �㹰�� ���� class�� ��ħ, j-1��° ��� j��° �� ���� ���� �����
				union_maze(Maze, Maze[i][j], Maze[i][j-1], i+1, row);//����� class�� Maze[i][j-1]�� �ٲ���� �ϴµ�..
			}
		}

		//srand((unsigned int)time(NULL));
		//if (i == col - 1) break; // ������ �ٿ����� �Ʒ��� ���� ���� �ʴ´�
		if (rand() % 2 == 0) {
			for (int j = 0; j < row; j++) { // ������ Ȯ���� �տ������� Ȯ��
				//srand((unsigned int)time(NULL));
				if (!flag[Maze[i][j]] || rand()%2 == 0) { // ���� ���� class���� �����ٰ� ����� ���� �� �� ��� �Ǵ� �̹� ������� Ȯ���� �ɸ� ���
					flag[Maze[i][j]] = 1;
					Maze[i + 1][j] = Maze[i][j];
					Down_Wall[i][j] = 0; // �ٷ� �Ʒ�ĭ�� ���� class�� ����� ���� �㹮��, �Ʒ�ĭ�� ��� 0���� �ʱ�ȭ�Ǿ������� �ٸ� ���ۿ� ����. ��ȯ�� ������� �ʾƵ� �ȴ�.
				}
			}
		}
		else {
			for (int j = row-1; j >=0; j--) { // ������ Ȯ���� �ڿ������� Ȯ��
				//srand((unsigned int)time(NULL));
				if (!flag[Maze[i][j]] || rand() % 2 == 0) { // ���� ���� class���� �����ٰ� ����� ���� �� �� ��� �Ǵ� �̹� ������� Ȯ���� �ɸ� ���
					flag[Maze[i][j]] = 1;
					Maze[i + 1][j] = Maze[i][j];
					Down_Wall[i][j] = 0; // �ٷ� �Ʒ�ĭ�� ���� class�� ����� ���� �㹮��. ������ ���⼱ union�� �� �ص� �ȴ�.
				}
			}
		}
		// flag ������ �ʱ�ȭ������.
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
	// ������ ��
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
	// �� �̻��Ѱž� �� 
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
	fp = fopen(filename, "w"); //���� ���� �����ؤ���
	// ���� ���� �Ʒ� ���� ������ ��������� ���� ���� ���� ���� ������ ���� �׻� ���� �������� ��������.

	fprintf(fp, "+");
	for (int j = 0; j < N; j++) {
		fprintf(fp, "-+");
	}
	fprintf(fp, "\n"); // �ֻ�� �� ���

	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			if (Left_Wall[i][j]) fprintf(fp, "| ");
			else fprintf(fp, "  ");
		}
		fprintf(fp, "|\n+"); // i�� ��� ���� ���� ���
		
		
		for (int j = 0; j < N; j++) {
			if (Down_Wall[i][j]) fprintf(fp, "-+");
			else fprintf(fp, " +");
		}
		fprintf(fp, "\n"); // �ϴ� �� ���
	}
	fclose(fp);
	return 0;
}
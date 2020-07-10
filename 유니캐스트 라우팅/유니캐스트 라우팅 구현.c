
/////////////////////////////// ��ũ�� ��� & ���� ���� & ��� ���� ////////////////////////////////////////

#define _CRT_SECURE_NO_WARNINGS 
#define NO_CONNECTION 19961204					// ������ ���� ��� (ū ����)
#define PEAK_NUMBER 6							// ������ ��

int DUPLICATE_ROUTE[PEAK_NUMBER][PEAK_NUMBER];	// �ߺ� ��� ����
int Matrix_path[PEAK_NUMBER][PEAK_NUMBER];		// �ִܰŸ� �������� ��ġ�� ������ ����
int COST[PEAK_NUMBER][PEAK_NUMBER];				// �Է¹��� ���
int DISTANCE[PEAK_NUMBER];						// ���� �������κ����� �ִ� ��� �Ÿ�
int FOUND[PEAK_NUMBER];							// �湮�� ���� ǥ��

#include <stdio.h>
#include <stdlib.h>
/////////////////////////////// ��ũ�� ��� & ���� ���� & ��� ���� ////////////////////////////////////////

// (10)
void
Print_Route(int start) {
	/////////////////////////////// ��� ////////////////////////////
	// ����� ��ε��� (A,0,B,C) (B,2,C,E) ... �� ���� ����մϴ�
	// 1. ���� ������ ���� ���������� ������ �Ÿ��� ����մϴ�.
	// 2. DUPLICATE_ROUTE�� ���������� �� �������� �ߺ��� ��ΰ� �����Ǿ� �ְ�,
	// ��ΰ� 0��° �ε������� ����Ǿ� �����Ƿ�, 
	// 0��° �ε������� ����� �ϵ�, �ε����� ������� ���� ��찡 ������ ������� �ʽ��ϴ�.

	printf(" > Least-cost tree rooted at %c : ", start + 65);
	for (int i = 0; i < PEAK_NUMBER; i++) {
		printf("(%c, %d", i + 65, DISTANCE[i]);

		for (int j = 0; j < PEAK_NUMBER; j++) {
			if (DUPLICATE_ROUTE[i][0] == NO_CONNECTION) {
				printf(", NULL");
				break;
			}
			if (DUPLICATE_ROUTE[i][j] != NO_CONNECTION)
				printf(", %c", DUPLICATE_ROUTE[i][j] + 65);
		}
		printf(")");
		if (i != PEAK_NUMBER - 1)
			printf(", ");
	}

	printf("\n\n");
	///////////////////////////////��� ��////////////////////////////
}

// (9)
void
Delete_Duplicate_Route() {
	///////////////////// �ߺ� ��� ���� /////////////////////////
	// 1. DUPLICATE_ROUTE�� ù��° ���� ���ҵ��� ������ ������ ���ҵ�� ���ؼ�
	// ���� ��ΰ� �ִٸ� ������ ���� ����� ��θ� ������ �������� �ٲ��ݴϴ�.
	// 2. ù��° ������ �ߺ��Ǵ� ��θ� NO_CONNECTION �����ϰ� ã���ϴ�.
	// DUPLICATE_ROUTE���� �ߺ��Ǵ� ����� ���� ����Ǿ� �ֽ��ϴ�.
	// Matrix_path���� �ִܰŸ� ��� �Ÿ��� ����Ǿ� �ֽ��ϴ�.
	// ���� ������ NO_CONNECTION�� �����ϰ� �ߺ��� ��θ� ã����
	// �� ���� ���� ���Ҹ� �� ���� ���ҷ� �Ű��ݴϴ�. (2 -> 1, 3 -> 2, 4 -> 3, ...)

	// �ߺ� ��� �����ϱ����� ����,,,
	int delete_route = 0;
	
	for (int one = 0; one < PEAK_NUMBER; one++)
		for (int two = 0; two < PEAK_NUMBER; two++) {
			for (int thr = 1; thr < PEAK_NUMBER; thr++) {
				for (int fou = 0; fou < PEAK_NUMBER; fou++) {
					if (((two == fou) && (one == thr)))
						break;
					if (DUPLICATE_ROUTE[two][one] == DUPLICATE_ROUTE[fou][thr]) {
						DUPLICATE_ROUTE[fou][thr] = NO_CONNECTION;
					}
				}
			}
		}


	for (int one = 0; one < PEAK_NUMBER; one++)
		for (int two = 0; two < PEAK_NUMBER; two++)
			for (int thr = two + 1; thr < PEAK_NUMBER; thr++)
				if ((DUPLICATE_ROUTE[two][one] == DUPLICATE_ROUTE[thr][0])) {
					if (DUPLICATE_ROUTE[two][one] != NO_CONNECTION) {
						delete_route = DUPLICATE_ROUTE[two][one];
						if (Matrix_path[two][delete_route] < Matrix_path[thr][delete_route])
							for (int fou = one; fou < PEAK_NUMBER - 1; fou++)
								DUPLICATE_ROUTE[thr][fou] = DUPLICATE_ROUTE[thr][fou + 1];
						else
							for (int fiv = one; fiv < PEAK_NUMBER - 1; fiv++)
								DUPLICATE_ROUTE[two][fiv] = DUPLICATE_ROUTE[two][fiv + 1];
					}
				}
	///////////////////// �ߺ� ��� ���� �� /////////////////////////
}

// (8)
int
Find_Matrix_Vertex() {
	//////////////////// �ִܰŸ��� �ִ� ���� ã�� //////////////////	
	// 1. ���� DISTANCE[]���� ���� ���� ����ġ ���� ��ġ�ϰ� �ִ� �迭�� �ε����� ã�� ��ȯ�մϴ�.
	// 2. �ּҰ� min�� ã�� �湮���� ���� ������ ���� DISTANCE[] ���� ���մϴ�.
	// �湮�� �� ���� �����̰� ��������� �ּҰ����� ������ �ּҰ��� �����ϰ�
	// �ּҰ��� ���������Ƿ� mp�� ������ �ε����� �����մϴ�
	// 3. �ּҰ��� ��ȯ�մϴ�.

	int min = NO_CONNECTION;
	int mp = -1;

	for (int minima = 0; minima < PEAK_NUMBER; minima++)
		if (DISTANCE[minima] < min && !FOUND[minima]) {
			min = DISTANCE[minima];
			mp = minima;
		}

	return mp;
	//////////////////// �ִܰŸ��� �ִ� ���� ã�� �� //////////////////
}

// (7)
void
Route_Storage(int start) {
	//////////////////// �ߺ� ��� �����ϱ� //////////////////
	// 1. �����ϴ� ������ �湮����(TRUE)���� ǥ���մϴ�.	
	// 2. ���� ������ �������ݴϴ�.
	// 3. �ߺ� ��θ� �����մϴ�. 
	// �������� ��θ� ����� �� �迭�� ����ؼ� ����ϹǷ�,
	// ��θ� 0���� ���ʴ�� �����ؾ��մϴ�.
	// ���� �湮���� ���� ������ �߿���
	// txt ���Ϸκ��� ���� Matrix�� ����� COST�� ������ ���ؼ� ����� �κ��� ã��,
	// ���� �� ���������� �Ÿ� + ���� �������� �Ÿ��� 
	// ���� �������� �Ÿ����� ���ų� ������ DUPLICATE_ROUTE�� �ε����� �����մϴ�.
	// DUPLICATE_ROUTE�� ���� ��忡�� �� �� �ִ� ������ ����� �� ����մϴ�.
	// 4. Find_Matrix_Vertex() �Լ��� ȣ���� �ּҰ��� �ִ� �ε����� ã�� �� �ε����� �湮ǥ�ø� �մϴ�.
	// 5. ���� �湮���� ���� ������ �߿��� 
	// ���� �� ���������� �Ÿ� + ���� �������� �Ÿ��� 
	// ���� �������� �Ÿ����� ������ DISTANCE�� Matrix_path�� �����մϴ�.

	FOUND[start] = 1;
	DISTANCE[start] = 0;

	// �Է��� A�� ������ >>> u = A �Դϴ�.
	//		  B			 >>> u = B �Դϴ�.
	int Display = start;

	// �� ���� �������� ���� ���� �ϳ��� ����ŭ �ݺ��ؾ� �մϴ�.
	for (int i = 0; i < PEAK_NUMBER - 1; i++) {

		// �ߺ� ��θ� �����մϴ�.
		int val = 0;

		for (int save = 0; save < PEAK_NUMBER; save++)
			if (!FOUND[save] && COST[Display][save] != NO_CONNECTION)
				if (DISTANCE[Display] + COST[Display][save] <= DISTANCE[save]) {
					DUPLICATE_ROUTE[Display][val] = save;
					val++;
				}

		// �湮ǥ�ø� �մϴ�.
		Display = Find_Matrix_Vertex();
		FOUND[Display] = 1;

		// ������ �����մϴ�.
		for (int Re = 0; Re < PEAK_NUMBER; Re++)
			if (!FOUND[Re])
				if (DISTANCE[Display] + COST[Display][Re] < DISTANCE[Re])
				{
					DISTANCE[Re] = DISTANCE[Display] + COST[Display][Re];
					Matrix_path[Display][Re] = DISTANCE[Re];
				}
	}
	//////////////////// �ߺ� ��� �����ϱ� �� //////////////////
}

// (6)
void
Find_Shortest_Path(int start) {
	////////////////////  �ִ� ��� �Ÿ� ���ϱ� //////////////////
	// 1. DISTANCE, FOUND, Matrix_path�� ���� �ʱ�ȭ �մϴ�.
	// DISTANCE�� ���� �������κ����� �ִ� ��� �Ÿ��̹Ƿ� ���� �Է¹��� ������ ��
	// �� A�� �Է¹޾��� �� A������ �ִ� ��� �Ÿ��� [ 0, 2, 0, 3, 0, 0, 0 ]�� �˴ϴ�.
	// FOUND�� �湮�� ������ ǥ���ϹǷ�, ���� �湮�� ������ �����Ƿ� FALSE�Դϴ�.
	// Matrix_path�� �ִܰŸ� �������� ��ġ�� ������ �����ϹǷ� ���� �Է¹��� ������
	// �ε����� 0��° ���� �����մϴ�.
	// Matrix_path�� DUPLICATE_ROUTE�� ���ϸ� �ߺ� ��θ� ã���ϴ�.
	// 2. Route_Storage() �Լ��� ȣ���Ͽ� �ߺ� ��θ� �����մϴ�.
	// 3. Delete_Duplicate_Route() �Լ��� ȣ���Ͽ� �ߺ� ��θ� �����մϴ�.
	// 4. Print_Route() ȣ���Ͽ� ���� �������� ������ �������� �ִ� ��� ����մϴ�.	

	for (int i = 0; i < PEAK_NUMBER; i++) {
		DISTANCE[i] = COST[start][i];
		FOUND[i] = 0;
		Matrix_path[i][0] = start;
	}

	Route_Storage(start);
	Delete_Duplicate_Route();
	Print_Route(start);
	////////////////////  �ִ� ��� �Ÿ� ���ϱ� �� //////////////////	
}

// (5)
void
Array_init(int arr[][PEAK_NUMBER]) {
	///////////////////// �ʱ�ȭ /////////////////////////
	// ��� Matrix�� ���� ������� �������� �ٲ��ݴϴ�.
	// �ʱ�ȭ�ϴ� Matrix����
	// �ִܰŸ��� ���� ��, �ߺ��� ��θ� ������ �� ����մϴ�.

	for (int i = 0; i < PEAK_NUMBER; i++)
		for (int j = 0; j < PEAK_NUMBER; j++)
			arr[i][j] = NO_CONNECTION;
	///////////////////// �ʱ�ȭ �� /////////////////////////
}

// (4)
void
InputOutput_Running_Test() {
	///////////////////// ���α׷� ���� /////////////////////////	
	// 1. ���� ���� �Է¹޽��ϴ�.
	// ���� ������ char�� �Է¹ް� �Ǹ�, enter���� ���ۿ� ����Ǳ� ������
	// getchar()�� ����� \n�� �����߽��ϴ�.	
	// 2. ���� Matrix_path��, DUPLICATE_ROUTE�� Array_init() �Լ��� ȣ���Ͽ� �ʱ�ȭ�մϴ�.
	// Matrix_path�� DUPLICATE_ROUTE�� ���� ���������� ����Ǿ� �ֽ��ϴ�.	
	// 3. Find_Shortest_Path() �Լ��� ȣ���Ͽ� �ִ� ��θ� ���մϴ�.

	char start, START;

	while (1) {
		printf(" > input the source router : ");
		scanf_s("%c", &start, 1);
		START = getchar(start != '\n');

		Array_init(Matrix_path);		Array_init(DUPLICATE_ROUTE);
		Find_Shortest_Path(start - 65);
	}
	///////////////////// ���α׷� �� /////////////////////////
}

// (3)
void
Read_Cost(int Matrix[][PEAK_NUMBER], FILE* file) {
	//////////////////////// txt���� ���� //////////////////////////
	// txt������ �о�Խ��ϴ�.
	// ���� �������� �о�� Matrix�� ������ �����ϴ�.
	//	0, 2, 0, 3, 0, 0, 0
	//	2, 0, 5, 0, 4, 0, 0
	//	0, 5, 0, 0, 0, 4, 3
	//	3, 0, 0, 0, 5, 0, 0
	//	0, 4, 0, 5, 0, 2, 0
	//	0, 0, 4, 0, 2, 0, 1
	//	0, 0, 3, 0, 0, 1, 0 
	// ���� 0�� �κ��� �������� ���� ������� �ʾҽ��ϴ�.
	// ����� ���ϰ� �ϱ� ���� ������� ���� ������
	// ��ũ�� ����� NO_CONNECTION�� �ٲ��ݴϴ�.
	// ��ũ�� ��� NO_CONNECTION�� ū �������� ������ �ֽ��ϴ�.

	for (int i = 0; i < PEAK_NUMBER; i++)
		for (int j = 0; j < PEAK_NUMBER; j++) {
			int r = fscanf(file, "%d ,", Matrix[i] + j);

			if (r < 1) {
				fprintf(stderr, "�ڷᰡ �����մϴ�.\n");
				exit(-1);
			}
			// ����������� ����� �� ����
			if (Matrix[i][j] == 0)
				Matrix[i][j] = NO_CONNECTION;
		}
	//////////////////////// txt���� ���� �� //////////////////////////
}

// (2)
void
File_InputOutput() {
	///////////////////// ���� ����� /////////////////////////
	// txt������ �о�ɴϴ�.

	FILE* fp = fopen("./Matrix_txt_6x6.txt", "r");

	if (fp == NULL) {
		puts("file open failed!!");		return -1;
	}

	Read_Cost(COST, fp);	fclose(fp);
	///////////////////// ���� ����� �� /////////////////////////
}

// (1)
void
Compile_Message() {
	///////////////////// �޼����� ����մϴ� /////////////////////////
	// �޼����� ���� ������ ������ ����ϸ�, ������ ������ ��ũ�� ����� ���ǵ˴ϴ�.

	printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	printf("%%		Program to find the least-cost tree for unicast routing				%%\n");
	printf("%%					least-cost tree's vertex : %d				%%\n", PEAK_NUMBER);
	printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n");
	///////////////////// �޼����� ����մϴ� �� /////////////////////////
}

// START
int
main() {
	///////////////////// main �Լ� /////////////////////////	
	// 1. message�� ����մϴ�.
	// 2. txt ������ �о�ɴϴ�.
	// 3. �о�� txt���Ϸ� ���ͽ�Ʈ�� �˰����� ����Ͽ� ���� ���������� ��θ� ���մϴ�.

	Compile_Message();
	File_InputOutput();
	InputOutput_Running_Test();
	///////////////////// main �Լ� �� /////////////////////////
}


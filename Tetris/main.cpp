#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

#define WIDTH 12					// 맵의 너비
#define HEIGHT 27					// 맵의 높이
#define BLOCK_SIZE 4				// 블록 하나에 포함되는 작은 블록의 갯수(모두 4개)
#define FRAME_PER_SEC 240		// 테트리스의 fps. 초당 240회의 연산을 하도록 고정 (고정 안할 시 초당 n천회 이상)
#define ARROW_KEY_DEFAULT 224		// 방향키를 입력받을 때, 방향키의 ASCII값에 앞서 입력되는 ASCII값
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_SPACE 32

#define GET_MIN(n1, n2) ((n1) < (n2) ? (n1) : (n2))		// 더 작은 수를 리턴하는 매크로 함수

typedef enum type { I, O, Z, S, J, L, T } TYPE; // 블럭의 종류(7가지)를 열거형으로 정의.
typedef enum direction { DIR_LEFT, DIR_RIGHT, DIR_DOWN } DIRECTION;

//typedef struct point {
//	int x, y;
//} POINT;

typedef struct block {
	TYPE blockType;
	POINT blockPoint[4]; // 배열의 첫 요소가 기준점.
	int rotation, rotationCycle;	// 회전 횟수, 반복 주기.
	short nFrame = 0;	// 떨어지는 속도.
} BLOCK;

void setMap(int map[][WIDTH]);
void printMap(const int map[][WIDTH]);
void setBlock(BLOCK* p);
void setPoint(POINT* p, int x, int y);
void removeBlock(const int map[][WIDTH], BLOCK* p);
void moveBlock(const int map[][WIDTH], BLOCK* p, char direction);
void rotateBlock(const int map[][WIDTH], BLOCK* p);
void putBlock(const int map[][WIDTH], BLOCK* p);
void putBlockPrev(const int map[][WIDTH], BLOCK* pBlock);
void removeBlockPrev(const int map[][WIDTH], BLOCK* pBlock);
void gotoxy(int x, int y);
int getDeltaY(const int map[][WIDTH], BLOCK* pBlock);

int main() {
	srand((unsigned int)time(NULL));
	system("mode con cols=80 lines=30");		// 프롬프트 창 크기 조절

	int deltaY = 0;
	int map[HEIGHT][WIDTH] = { 0 };
	setMap(map);
	printMap(map);

	BLOCK a;
	setBlock(&a);
	gotoxy(5, 27);
	printf("a.nFrame = ");

	while (a.blockPoint[0].y < HEIGHT - 2) {		// 블록이 맵 바닥까지 떨어질 때까지 반복
		gotoxy(16, 27);
		printf("%03d", a.nFrame);


		/* 키보드 입력받는 부분 */
		if (_kbhit()) {
			switch (_getch()) {
				case KEY_SPACE :
					deltaY = getDeltaY(map, &a);
					removeBlock(map, &a);
					for (int i = 0; i < BLOCK_SIZE; i++) {
						a.blockPoint[i].y += deltaY;
					}
					putBlock(map, &a);
					break;
				case ARROW_KEY_DEFAULT :
					switch (_getch()) {
						case KEY_DOWN:
							removeBlock(map, &a);
							for (int i = 0; i < BLOCK_SIZE; i++) {
								a.blockPoint[i].y++;
								a.nFrame = FRAME_PER_SEC;
							}
							putBlock(map, &a);
							break;
						case KEY_LEFT :
							removeBlock(map, &a);
							for (int i = 0; i < BLOCK_SIZE; i++) {
								if (a.blockPoint[i].x - 1 <= 0)
									break;
								else if (i == BLOCK_SIZE - 1) {
									for (int i = 0; i < BLOCK_SIZE; i++) {
										a.blockPoint[i].x--;
									}
								}
							}
							putBlock(map, &a);
							break;
						case KEY_RIGHT : 
							removeBlock(map, &a);
							for (int i = 0; i < BLOCK_SIZE; i++) {
								if (WIDTH - 1 <= a.blockPoint[i].x + 1)
									break;
								else if (i == BLOCK_SIZE - 1) {
									for (int i = 0; i < BLOCK_SIZE; i++) {
										a.blockPoint[i].x++;
									}
								}
							}
							putBlock(map, &a);
							break;
						case KEY_UP:
							removeBlock(map, &a);
							rotateBlock(map, &a);
							putBlock(map, &a);
							break;
					}
			}
		}
		/* 키보드 입력받는 부분 끝*/


		if (a.nFrame <= 0) {
			moveBlock(map, &a, 'a');
			gotoxy(16, 27);
			a.nFrame = FRAME_PER_SEC;
			printf("%03d", a.nFrame);
		}

		gotoxy(0, 0);
		a.nFrame--;
		Sleep(1000 / FRAME_PER_SEC);
	}

	return 0;
}

void setMap(int map[][WIDTH]) {		// 맵 초기화(테두리 생성).
	for (int i = 0; i < WIDTH; i++) {
		map[0][i] = 1;
		map[HEIGHT - 1][i] = 1;
	}
	for (int i = 0; i < HEIGHT - 2; i++) {
		map[i + 1][0] = 1;
		map[i + 1][WIDTH - 1] = 1;
	}
}

void printMap(const int map[][WIDTH]) {
	gotoxy(0, 0);
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (map[i][j]) printf("■");
			else printf("  ");
		}
		printf("\n");
	}
}

void setBlock(BLOCK* p) {		// 블럭 의 속성값 초기화.
	POINT* point = p->blockPoint;
	setPoint(point, 2 * 3, -1);	// 블럭의 기준점을 (2 * 3, -1)으로 초기화

	//p->blockType = (TYPE)(rand() % 7);
	p->blockType = O;		// 임시로 O형 블록만 선택함.		아래 switch문 완성시 윗 라인의 주석 코드로 교체

	switch (p->blockType) {
	case O:
		setPoint(&point[1], point->x - 1, point->y);
		setPoint(&point[2], point->x - 1, point->y - 1);
		setPoint(&point[3], point->x, point->y - 1);
		break;
	case I:;
	case Z:;
	case S:;
	case J:;
	case L:;
	case T:;
	}

	switch (p->blockType) {
	case O: p->rotationCycle = 1; break;
	case I:
	case Z:
	case S: p->rotationCycle = 2; break;
	case J:
	case L:
	case T: p->rotationCycle = 4;
	}

	p->rotation = rand() % p->rotationCycle;
}

void setPoint(POINT* p, int x, int y) {
	p->x = x;
	p->y = y;
}

void removeBlock(const int map[][WIDTH], BLOCK* p) { // 이 블럭에 공백을 덮어씌워 지운다.
	removeBlockPrev(map, p);
	for (int i = 0; i < BLOCK_SIZE; i++) {
		if ((0 < p->blockPoint[i].x && p->blockPoint[i].x < WIDTH - 1) && (0 < p->blockPoint[i].y && p->blockPoint[i].y < HEIGHT - 1)) {   // map의 테두리가 아닐 경우에만
			gotoxy(2 * p->blockPoint[i].x, p->blockPoint[i].y);
			printf("  ");	// 기존의 ■를 지우기 위해 공백 출력
		}
	}
}

void moveBlock(const int map[][WIDTH], BLOCK* p, char direction) { // 블럭의 모든 point의 y좌표 값을 증가시켜 밑으로 내린다.
	removeBlock(map, p);
	for (int i = 0; i < BLOCK_SIZE; i++) {
		(p->blockPoint[i].y)++;
	}
	putBlock(map, p);
}

void rotateBlock(const int map[][WIDTH], BLOCK* p) { // 기준점 중심으로 반시계 방향으로 90도 회전.
	POINT* point = p->blockPoint;
	for (int i = 0; i < BLOCK_SIZE; i++) {
		int delta_x = point[i].x - point[0].x;
		int delta_y = point[i].y - point[0].y;
		setPoint(&point[i], point[0].x - delta_y, point[0].y + delta_x);
	}

	(p->rotation)++;
	p->rotation %= p->rotationCycle;
}

void putBlock(const int map[][WIDTH], BLOCK* p) { // moveBlock에 의해 옮겨진 좌표로 블럭을 출력한다.
	putBlockPrev(map, p);
	for (int i = 0; i < BLOCK_SIZE; i++) {
		gotoxy(2 * p->blockPoint[i].x, p->blockPoint[i].y);
		printf("■");
	}
}

void putBlockPrev(const int map[][WIDTH], BLOCK* pBlock) {		// 떨어지는 블록이 떨어질 위치를 미리 보여줌
	int deltaY = getDeltaY(map, pBlock);

	for (int i = 0; i < BLOCK_SIZE; i++) {
		gotoxy(2 * pBlock->blockPoint[i].x, pBlock->blockPoint[i].y + deltaY);
		printf("□");
	}
}

void removeBlockPrev(const int map[][WIDTH], BLOCK* pBlock) {
	int deltaY = getDeltaY(map, pBlock);

	for (int i = 0; i < BLOCK_SIZE; i++) {
		gotoxy(2 * pBlock->blockPoint[i].x, pBlock->blockPoint[i].y + deltaY);
		printf("  ");
	}
}

void gotoxy(int x, int y) {		// 커서를 해당 좌표로 이동
	COORD Cur = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

int getDeltaY(const int map[][WIDTH], BLOCK* pBlock) {	// 떨어지는 블럭과 바닥 간의 거리를 리턴하는 함수
	int deltaY = HEIGHT;		// 떨어지는 블럭과 바닥간의 거리를 저장. 최종적으로 최솟값을 얻는 것이 목표이므로 최댓값으로 초기화

	for (int i = 0; i < BLOCK_SIZE; i++) {
		for (int j = HEIGHT - 1; 1 <= j; j--) {
			if (!map[j][i]) {
				deltaY = GET_MIN(deltaY, j - pBlock->blockPoint[i].y);		// 현재 바닥의 y좌표 - 떨어지는 블럭의 y좌표 
				break;
			}
		}
	}

	return deltaY;
}
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

typedef enum type { BLOCK_I, BLOCK_O, BLOCK_Z, BLOCK_S, BLOCK_J, BLOCK_L, BLOCK_T } TYPE; // 블럭의 종류(7가지)를 열거형으로 정의.
typedef enum direction { DIR_LEFT, DIR_RIGHT, DIR_DOWN } DIRECTION;

typedef struct block {
	TYPE blockType;
	POINT blockPoint[4]; // 배열의 첫 요소가 기준점.
	int rotation, rotationCycle;	// 회전 횟수, 반복 주기.
	short nFrame = 0;	// 떨어지는 속도.
} BLOCK;

void setMap(int map[][WIDTH]);
void printMap(const int map[][WIDTH]);
void setBlock(BLOCK* pBlock);
void setPoint(POINT* pPoint, int x, int y);
void removeBlock(const int map[][WIDTH], BLOCK* pBlock);
void dropBlock(const int map[][WIDTH], BLOCK* pBlock);
void rotateBlock(const int map[][WIDTH], BLOCK* pBlock);
void putBlock(const int map[][WIDTH], BLOCK* pBlock);
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

	BLOCK block;
	setBlock(&block);
	gotoxy(2, 27);
	printf("block.nFrame = "); 
	gotoxy(30, 2);
	printf("block.rotation : %d", block.rotation);

	while (getDeltaY(map, &block)) {		// 블록이 맵 바닥까지 떨어질 때까지 반복
		gotoxy(17, 27);
		printf("%03d", block.nFrame);


		/* 키보드 입력받는 부분 */
		if (_kbhit()) {
			switch (_getch()) {
				case KEY_SPACE :
					deltaY = getDeltaY(map, &block);
					removeBlock(map, &block);
					for (int i = 0; i < BLOCK_SIZE; i++) {
						block.blockPoint[i].y += deltaY;
					}
					putBlock(map, &block);
					break;
				case ARROW_KEY_DEFAULT :
					switch (_getch()) {
						case KEY_DOWN:
							removeBlock(map, &block);
							for (int i = 0; i < BLOCK_SIZE; i++) {
								block.blockPoint[i].y++;
								block.nFrame = FRAME_PER_SEC;
							}
							putBlock(map, &block);
							break;
						case KEY_LEFT :
							removeBlock(map, &block);
							for (int i = 0; i < BLOCK_SIZE; i++) {
								if (block.blockPoint[i].x - 1 <= 0)
									break;
								else if (i == BLOCK_SIZE - 1) {
									for (int i = 0; i < BLOCK_SIZE; i++) {
										block.blockPoint[i].x--;
									}
								}
							}
							putBlock(map, &block);
							break;
						case KEY_RIGHT : 
							removeBlock(map, &block);
							for (int i = 0; i < BLOCK_SIZE; i++) {
								if (WIDTH - 1 <= block.blockPoint[i].x + 1)
									break;
								else if (i == BLOCK_SIZE - 1) {
									for (int i = 0; i < BLOCK_SIZE; i++) {
										block.blockPoint[i].x++;
									}

								}
							}
							putBlock(map, &block);
							break;
						case KEY_UP:
							removeBlock(map, &block);
							rotateBlock(map, &block);
							putBlock(map, &block);
							gotoxy(30, 2);
							printf("block.rotation : %d", block.rotation);
							break;
					}
			}
		}
		/* 키보드 입력받는 부분 끝*/


		if (block.nFrame <= 0) {
			dropBlock(map, &block);
			gotoxy(17, 27);
			block.nFrame = FRAME_PER_SEC;
			printf("%03d", block.nFrame);
		}

		gotoxy(0, 0);
		block.nFrame--;
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

void setBlock(BLOCK* pBlock) {		// 블럭 의 속성값 초기화.
	POINT* point = pBlock->blockPoint;
	setPoint(point, 2 * 3, -1);	// 블럭의 기준점을 (2 * 3, -1)으로 초기화

	pBlock->blockType = (TYPE)(rand() % 7);
	

	switch (pBlock->blockType) {
	case BLOCK_O:
		setPoint(&point[1], point->x - 1, point->y);
		setPoint(&point[2], point->x - 1, point->y - 1);
		setPoint(&point[3], point->x, point->y - 1);
		break;
	case BLOCK_I:
		setPoint(&point[1], point->x + 1, point->y);
		setPoint(&point[2], point->x - 1, point->y);
		setPoint(&point[3], point->x - 2, point->y);
		break;
	case BLOCK_Z:
		setPoint(&point[1], point->x, point->y + 1);
		setPoint(&point[2], point->x - 1, point->y);
		setPoint(&point[3], point->x - 1, point->y - 1);
		break;
	case BLOCK_S:
		setPoint(&point[1], point->x - 1, point->y + 1);
		setPoint(&point[2], point->x - 1, point->y);
		setPoint(&point[3], point->x, point->y - 1);
		break;
	case BLOCK_J:
		setPoint(&point[1], point->x - 1, point->y);
		setPoint(&point[2], point->x - 2, point->y);
		setPoint(&point[3], point->x, point->y - 1);
		break;
	case BLOCK_L:
		setPoint(&point[1], point->x, point->y + 1);
		setPoint(&point[2], point->x - 1, point->y);
		setPoint(&point[3], point->x - 2, point->y);
		break;
	case BLOCK_T:
		setPoint(&point[1], point->x, point->y + 1);
		setPoint(&point[2], point->x - 1, point->y);
		setPoint(&point[3], point->x, point->y - 1);
		break;
	}

	switch (pBlock->blockType) {
	case BLOCK_O: pBlock->rotationCycle = 1; break;
	case BLOCK_I:
	case BLOCK_Z:
	case BLOCK_S: pBlock->rotationCycle = 2; break;
	case BLOCK_J:
	case BLOCK_L:
	case BLOCK_T: pBlock->rotationCycle = 4;
	}

	pBlock->rotation = 1;
}

void setPoint(POINT* pPoint, int x, int y) {
	pPoint->x = x;
	pPoint->y = y;
}

void removeBlock(const int map[][WIDTH], BLOCK* pBlock) { // 이 블럭에 공백을 덮어씌워 지운다.
	removeBlockPrev(map, pBlock);
	for (int i = 0; i < BLOCK_SIZE; i++) {
		if ((0 < pBlock->blockPoint[i].x && pBlock->blockPoint[i].x < WIDTH - 1) && (0 < pBlock->blockPoint[i].y && pBlock->blockPoint[i].y < HEIGHT - 1)) {   // map의 테두리가 아닐 경우에만
			gotoxy(2 * pBlock->blockPoint[i].x, pBlock->blockPoint[i].y);
			printf("  ");	// 기존의 ■를 지우기 위해 공백 출력
		}
	}
}

void dropBlock(const int map[][WIDTH], BLOCK* pBlock) { // 블럭의 모든 point의 y좌표 값을 증가시켜 밑으로 내린다.
	removeBlock(map, pBlock);
	for (int i = 0; i < BLOCK_SIZE; i++) {
		(pBlock->blockPoint[i].y)++;
	}
	putBlock(map, pBlock);
}

void rotateBlock(const int map[][WIDTH], BLOCK* pBlock) { // 기준점 중심으로 반시계 방향으로 90도 회전.
	if (pBlock->rotationCycle == 1) return;	// BLOCK_O일 때엔 즉시 리턴

	int delta_x = 0;
	int delta_y = 0;
	int rotatedir = 0;	// 블럭을 초기 상태로 되돌리기까지 필요한 회전 횟수

	if (pBlock->rotationCycle == pBlock->rotation) {		// 회전 횟수가 최대일 때
		rotatedir = (2 < pBlock->rotationCycle ? 1 : -1);		// 2번 돌리는 블럭은 1, 4번 돌리는 블럭은 -1

		for (int i = 0; i < BLOCK_SIZE; i++) {
			delta_x = pBlock->blockPoint[i].x - pBlock->blockPoint[0].x;
			delta_y = pBlock->blockPoint[i].y - pBlock->blockPoint[0].y;
			setPoint(&pBlock->blockPoint[i], pBlock->blockPoint[0].x - delta_y * rotatedir, pBlock->blockPoint[0].y + delta_x * rotatedir);
			}
		pBlock->rotation = 1;
	}
	else {	
		for (int i = 0; i < BLOCK_SIZE; i++) {
			delta_x = pBlock->blockPoint[i].x - pBlock -> blockPoint[0].x;
			delta_y = pBlock->blockPoint[i].y - pBlock -> blockPoint[0].y;
			setPoint(&pBlock->blockPoint[i], pBlock -> blockPoint[0].x - delta_y, pBlock->blockPoint[0].y + delta_x);
		}
		pBlock->rotation++;
	}
}

void putBlock(const int map[][WIDTH], BLOCK* pBlock) { // moveBlock에 의해 옮겨진 좌표로 블럭을 출력한다.
	putBlockPrev(map, pBlock);
	for (int i = 0; i < BLOCK_SIZE; i++) {
		gotoxy(2 * pBlock->blockPoint[i].x, pBlock->blockPoint[i].y);
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
			if (map[j][i] != 1) {
				deltaY = GET_MIN(deltaY, j - pBlock->blockPoint[i].y);		// 현재 바닥의 y좌표 - 떨어지는 블럭의 y좌표 
				break;
			}
		}
	}

	return deltaY;
}
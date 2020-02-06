#include "Tetris.h"
extern bool map[HEIGHT][WIDTH];

void setMap() {		// 맵 초기화(테두리 생성).
	for (int i = 0; i < WIDTH; i++) {
		//map[0][i] = 1;
		map[HEIGHT - 1][i] = 1;
	}
	for (int i = 0; i < HEIGHT - 2; i++) {
		map[i + 1][0] = 1;
		map[i + 1][WIDTH - 1] = 1;
	}
}

void printMap() {		// 초기화된 맵 출력
	gotoxy(0, 0);
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (!i || map[i][j]) printf("■");
			else printf("  ");
		}
		printf("\n");
	}
}

void setPoint(POINT* pPoint, int x, int y) {		// 블럭의 x, y좌표 값을 입력받은 대로 연산.
	pPoint->x = x;
	pPoint->y = y;
}

bool isInsideMap(const POINT* pPoint) {
	return (0 < pPoint->x && pPoint->x < WIDTH - 1) && (0 < pPoint->y && pPoint->y < HEIGHT - 1);
}

void setBlock(BLOCK* pBlock) {		// 블럭 의 속성값 초기화.
	POINT* point = pBlock->blockPoint;
	setPoint(point, 2 * 3, -1);	// 블럭의 중심점을 (2 * 3, -1)으로 초기화.

	//pBlock->blockType = BLOCK_I;
	pBlock->blockType = (TYPE)(rand() % 7);
	// 하나의 블럭을 구성하는 4개의 작은 블럭들을 중심점 기준으로 좌표 초기화. 
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

	pBlock->deltaY = getDeltaY(pBlock);
}

bool pressed(int key) {
	return GetAsyncKeyState(key) & 0x8000;
}

void getKey(bool* up, bool* down, bool* left, bool* right, bool* space) {
	static int key_nFrame[5]; // key_nFrame은 직전 키 눌림 여부나 눌린 횟수.(frame)
	const int downFrame = 5, moveFrame = 10; // down / left, right를 실행할 프레임 주기.

	// down, left, right에서 ++이 후위 연산자면 첫 입력 실행, 전위 연산자면 첫 입력 실행X.
	*up = pressed(VK_UP) && !key_nFrame[KS_UP]++;
	*down = pressed(VK_DOWN) && !(++key_nFrame[KS_DOWN] % downFrame);
	*left = pressed(VK_LEFT) && !pressed(VK_RIGHT) && !(key_nFrame[KS_LEFT]++ % moveFrame);
	*right = !pressed(VK_LEFT) && pressed(VK_RIGHT) && !(key_nFrame[KS_RIGHT]++ % moveFrame);
	*space = pressed(VK_SPACE) && !key_nFrame[KS_SPACE]++;

	// 아무 키도 눌리지 않았을 경우 key_nFrame 초기화.
	if (!(pressed(VK_UP) || pressed(VK_DOWN) || pressed(VK_SPACE) || pressed(VK_LEFT) || pressed(VK_RIGHT)))
		for (int i = 0; i < KEY_COUNT; i++)
			key_nFrame[i] = 0;
}

bool isBlocked(const BLOCK* pBlock, int x) {
	for (int i = 0; i < BLOCK_SIZE; i++)
		if (map[pBlock->blockPoint[i].y][pBlock->blockPoint[i].x + x])
			return true;
	return false;
}

void moveBlockPoint(BLOCK* pBlock, int x, int y) { // 블럭의 모든 점의 좌표를 (x, y)만큼 옮긴다.
	for (int i = 0; i < BLOCK_SIZE; i++) {
		pBlock->blockPoint[i].x += x;
		pBlock->blockPoint[i].y += y;
	}
}

void moveBlock(BLOCK* pBlock, int x, int y) {
	removeBlock(pBlock);
	moveBlockPoint(pBlock, x, y);
	pBlock->deltaY = getDeltaY(pBlock);
	putBlock(pBlock);
}


void rotateBlockPoint(BLOCK* pBlock) { // 블럭을 회전시키는 함수.
	POINT* point = pBlock->blockPoint;

	if (pBlock->blockType == BLOCK_O) {
		return;
	}

	for (int i = 0; i < BLOCK_SIZE; i++) {
		int delta_x = point[i].x - point[0].x;
		int delta_y = point[i].y - point[0].y;
		setPoint(&point[i], point[0].x - delta_y, point[0].y + delta_x);
	}

	int deltaX = getDeltaXfromSide(pBlock);
	if (deltaX) {
		/* 튀어나온 거리만큼 x좌표 변경 */
		moveBlockPoint(pBlock, -deltaX, 0);
	}
}

void rotateBlock(BLOCK* pBlock) {
	removeBlock(pBlock);
	rotateBlockPoint(pBlock);
	pBlock->deltaY = getDeltaY(pBlock);
	putBlock(pBlock);
}

void putBlock(BLOCK* pBlock) { // 저장된 좌표로 이동하여 블럭을 출력함.
	//putBlockPrev(pBlock);
	for (int i = 0; i < BLOCK_SIZE; i++) {
		gotoxy(2 * pBlock->blockPoint[i].x, pBlock->blockPoint[i].y);
		printf("■");
	}
}

void putBlockPrev(BLOCK* pBlock) {		// 드랍 중인 블록의 미리보기 출력.
	for (int i = 0; i < BLOCK_SIZE; i++) {
		gotoxy(2 * pBlock->blockPoint[i].x, pBlock->blockPoint[i].y + pBlock->deltaY);
		printf("□");
	}
}

void removeBlock(BLOCK* pBlock) {		// 출력된 블럭의 좌표에 공백을 덮어씌워 지운다.
	//removeBlockPrev(pBlock);		// 미리보기 블럭 제거
	for (int i = 0; i < BLOCK_SIZE; i++) {
		if (isInsideMap(&pBlock->blockPoint[i])) {   // map의 테두리 안에 있을 때만
			gotoxy(2 * pBlock->blockPoint[i].x, pBlock->blockPoint[i].y);
			printf("  ");	// 기존의 ■를 지우기 위해 공백 출력
		}
	}
}

void removeBlockPrev(BLOCK* pBlock) {		// 미리보기 위에 공백을 출력하여 덮어씌움.
	for (int i = 0; i < BLOCK_SIZE; i++) {
		gotoxy(2 * pBlock->blockPoint[i].x, pBlock->blockPoint[i].y + pBlock->deltaY);
		printf("  ");
	}
}

int getDeltaY(BLOCK* pBlock) {	// 떨어지는 블럭과 바닥 간의 거리를 리턴하는 함수.
	int deltaY = HEIGHT;		// 떨어지는 블럭과 바닥간의 거리를 저장. 최종적으로 최솟값을 얻는 것이 목표이므로 최댓값으로 초기화

	for (int i = 0; i < BLOCK_SIZE; i++) {
		for (int j = pBlock->blockPoint[i].y; j < HEIGHT - 1; j++) {		// map의 맨 위부터 블록이 쌓여있는 위치를 탐색
			if (map[j + 1][pBlock->blockPoint[i].x]) {		// 쌓여있는 위치를 찾았다면 
				deltaY = GET_MIN(deltaY, j - pBlock->blockPoint[i].y);		// 현재 바닥의 y좌표 - 떨어지는 블럭의 y좌표 
				break;
			}
		}
	}

	return deltaY;
}

int getDeltaXfromSide(BLOCK* pBlock) {
	int deltaX = 0;
	for (int i = 1; i < BLOCK_SIZE; i++) {
		int X = pBlock->blockPoint[i].x;
		int tmp;

		if (X < 1) { // blockPoint[i]의 x좌표가 1보다 작을 때, x = 1을 기준으로 한 상대적 위치
			tmp = X - 1;
		}
		else if (X > WIDTH - 2) { // blockPoint[i]의 x좌표가 WIDTH - 2보다 클 때, x = WIDTH - 2를 기준으로 한 상대적 위치
			tmp = X - (WIDTH - 2);
		}
		else { // blockPoint가 알맞은 위치에 있을 때.
			tmp = 0;
		}

		if (tmp && abs(tmp) > abs(deltaX)) {
			deltaX = tmp;
		}
	}
	return deltaX;
}

void gotoxy(int x, int y) {		// 커서를 해당 좌표로 이동
	COORD Cur = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}
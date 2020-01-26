#include "Tetris.h"

int main() {
	srand((unsigned int)time(NULL));
	system("mode con cols=80 lines=30");		// 프롬프트 창 크기 조절

	int map[HEIGHT][WIDTH] = { 0 };
	setMap(map);
	printMap(map);

	BLOCK block;
	setBlock(&block);
	gotoxy(5, 27);
	printf("block.nFrame = ");
	int DeltaY = 0;

	while (DeltaY = getDeltaY(map, &block)) {		// 블록이 맵 바닥까지 떨어질 때까지 반복
		gotoxy(16, 27);
		printf("%03d", block.nFrame);

		/* 키보드 입력받는 부분 */
		if (_kbhit()) {
			switch (_getch()) {
			case KEY_SPACE:
				moveBlock(map, &block, 0, DeltaY);
				break;
			case ARROW_KEY_DEFAULT:
				switch (_getch()) {
				case KEY_DOWN:
					moveBlock(map, &block, 0, 1);
					block.nFrame = FRAME_PER_SEC;
					break;
				case KEY_LEFT:
					for (int i = 0; i < BLOCK_SIZE; i++) {
						if (block.blockPoint[i].x - 1 <= 0) {
							goto FAIL;
						}
					}
					moveBlock(map, &block, -1, 0);
					break;
				case KEY_RIGHT:
					for (int i = 0; i < BLOCK_SIZE; i++) {
						if (WIDTH - 1 <= block.blockPoint[i].x + 1) {
							goto FAIL;
						}
					}
					moveBlock(map, &block, 1, 0);
					break;
				case KEY_UP:
					removeBlock(map, &block);
					rotateBlock(map, &block);
					putBlock(map, &block);
					break;
				}
			}
		}

		FAIL:;
		/* 키보드 입력받는 부분 끝*/

		if (block.nFrame <= 0) {
			moveBlock(map, &block, 0, 1);
			gotoxy(16, 27);
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

void printMap(const int map[][WIDTH]) {		// 초기화된 맵 출력
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
	setPoint(point, 2 * 3, -1);	// 블럭의 중심점을 (2 * 3, -1)으로 초기화

	pBlock->blockType = (TYPE)(rand() % 7);


	switch (pBlock->blockType) {		// 하나의 블럭을 구성하는 4개의 작은 블럭들을 중심점 기준으로 좌표 초기화. 
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
}

void setPoint(POINT* pPoint, int x, int y) {		// 블럭의 x, y좌표 값을 입력받은 대로 연산.
	pPoint->x = x;
	pPoint->y = y;
}

void removeBlock(const int map[][WIDTH], BLOCK* pBlock) {		// 출력된 블럭의 좌표에 공백을 덮어씌워 지운다.
	removeBlockPrev(map, pBlock);		// 미리보기 블럭 제거
	for (int i = 0; i < BLOCK_SIZE; i++) {
		if ((0 < pBlock->blockPoint[i].x && pBlock->blockPoint[i].x < WIDTH - 1) && (0 < pBlock->blockPoint[i].y && pBlock->blockPoint[i].y < HEIGHT - 1)) {   // map의 테두리가 아닐 경우에만
			gotoxy(2 * pBlock->blockPoint[i].x, pBlock->blockPoint[i].y);
			printf("  ");	// 기존의 ■를 지우기 위해 공백 출력
		}
	}
}

void moveBlockPoint(BLOCK* pBlock, int x, int y) { // 블럭의 모든 점의 좌표를 (x, y)만큼 옮긴다.
	for (int i = 0; i < BLOCK_SIZE; i++) {
		pBlock->blockPoint[i].x += x;
		pBlock->blockPoint[i].y += y;
	}
}

void moveBlock(const int map[][WIDTH], BLOCK* pBlock, int x, int y) {
	removeBlock(map, pBlock);
	moveBlockPoint(pBlock, x, y);
	putBlock(map, pBlock);
}

void rotateBlock(const int map[][WIDTH], BLOCK* pBlock) { // 블럭을 회전시키는 함수.
	int deltaX;	// map의 테두리로부터 튀어나온 x좌표 거리
	POINT* point = pBlock->blockPoint;

	if (pBlock->blockType == BLOCK_O) {
		return;
	}

	for (int i = 0; i < BLOCK_SIZE; i++) {
		int delta_x = point[i].x - point[0].x;
		int delta_y = point[i].y - point[0].y;
		setPoint(&point[i], point[0].x - delta_y, point[0].y + delta_x);
	}

	deltaX = getDeltaXfromSide(map, pBlock);	// map의 테두리로부터 가장 많이 튀어나온 거리값 리턴. 없을시 0
	/* 블럭이 테두리에 닿거나 밖으로 나갔다면 */
	if (deltaX) {
		/* 튀어나온 거리만큼 x좌표 변경 */
		moveBlockPoint(pBlock, -deltaX, 0);
	}
}

void putBlock(const int map[][WIDTH], BLOCK* pBlock) { // 저장된 좌표로 이동하여 블럭을 출력함.
	putBlockPrev(map, pBlock);
	for (int i = 0; i < BLOCK_SIZE; i++) {
		gotoxy(2 * pBlock->blockPoint[i].x, pBlock->blockPoint[i].y);
		printf("■");
	}
}

void putBlockPrev(const int map[][WIDTH], BLOCK* pBlock) {		// 드랍 중인 블록의 미리보기 출력.
	int deltaY = getDeltaY(map, pBlock);

	for (int i = 0; i < BLOCK_SIZE; i++) {
		gotoxy(2 * pBlock->blockPoint[i].x, pBlock->blockPoint[i].y + deltaY);
		printf("□");
	}
}

void removeBlockPrev(const int map[][WIDTH], BLOCK* pBlock) {		// 미리보기 위에 공백을 출력하여 덮어씌움.
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

int getDeltaY(const int map[][WIDTH], BLOCK* pBlock) {	// 떨어지는 블럭과 바닥 간의 거리를 리턴하는 함수.
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

int getDeltaXfromSide(const int map[][WIDTH], BLOCK* pBlock) {
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
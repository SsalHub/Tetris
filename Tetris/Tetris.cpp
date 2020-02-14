#include "Tetris.h"
extern bool map[HEIGHT][WIDTH];

void setMap() {		// 맵 초기화(테두리 생성).
	for (int i = 0; i < WIDTH; i++) {
		map[HEIGHT - 1][i] = 1;
	}
	for (int i = 0; i < HEIGHT - 1; i++) {
		map[i][0] = 1;
		map[i][WIDTH - 1] = 1;
	}
}

void printMap() {		// 초기화된 맵 출력
	gotoxy(0, 0);
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			//if (!i || map[i][j]) printf("■");
			if (map[i][j]) printf("■");
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
	return (0 < pPoint->x && pPoint->x < WIDTH - 1) && (0 <= pPoint->y && pPoint->y < HEIGHT - 1);
}

void setBlockList(TYPE* pList) {		// 블럭 리스트를 최초 초기화 및 블럭 리스트의 테두리 출력.
	const int startX = 12;
	const int startY = 0;

	/* 블럭 리스트 초기화 */
	for (int i = 0; i < BLOCK_LIST_LEN; i++) {	
		pList[i] = (TYPE)(rand() % 7);
	}

	/* 블럭 리스트의 테두리 출력*/
	for (int i = 0; i < 20; i++) {
		gotoxy(2 * (startX), i);
		printf("* ");
		gotoxy(2 * (startX + 6), i);
		printf(" *");
	}
	for (int i = 0; i <= 6; i++) {
		gotoxy(2 * (startX + i), 0);
		printf("ㅁ");
		gotoxy(2 * (startX + i), 19);
		printf("**");
	}
}

void addBlockList(TYPE* pList) {		// 블럭 리스트에 새로 추가.
	pList[BLOCK_LIST_LEN - 1] = (TYPE)(rand() % 7);
}

TYPE popBlockList(TYPE* pList) {		// 블럭 리스트에서 0번째 인덱스의 값을 리턴하고 삭제.
	TYPE tmpBlockType = pList[0];
	for (int i = 0; i < BLOCK_LIST_LEN - 1; i++) {
		pList[i] = pList[i + 1];
	}
	return tmpBlockType;
}

void printBlockList(TYPE* pList) {		// 블럭 리스트 출력
	POINT point[4];
	int blockHeight;
	int nowY = 2;
	const int startX = 13;

	for (int i = 0; i < BLOCK_LIST_LEN; i++) {		// pList의 크기만큼 반복

		switch (pList[i]) {
		case BLOCK_O:
			setPoint(&point[0], -1, 0);
			setPoint(&point[1], point->x, point->y + 1);
			setPoint(&point[2], point->x + 1, point->y);
			setPoint(&point[3], point->x + 1, point->y + 1);
			blockHeight = 2;
			break;
		case BLOCK_I:
			setPoint(&point[0], 0, 0);
			setPoint(&point[1], point->x, point->y + 1);
			setPoint(&point[2], point->x, point->y + 2);
			setPoint(&point[3], point->x, point->y + 3);
			blockHeight = 4;
			break;
		case BLOCK_Z:
			setPoint(&point[0], -1, 0);
			setPoint(&point[1], point->x, point->y + 1);
			setPoint(&point[2], point->x + 1, point->y + 1);
			setPoint(&point[3], point->x + 1, point->y + 2);
			blockHeight = 3;
			break;
		case BLOCK_S:
			setPoint(&point[0], 0, 0);
			setPoint(&point[1], point->x , point->y + 1);
			setPoint(&point[2], point->x - 1, point->y + 1);
			setPoint(&point[3], point->x - 1, point->y + 2);
			blockHeight = 3;
			break;
		case BLOCK_J:
			setPoint(&point[0], 1, 0);
			setPoint(&point[1], point->x, point->y + 1);
			setPoint(&point[2], point->x - 1, point->y + 1);
			setPoint(&point[3], point->x - 2, point->y + 1);
			blockHeight = 2;
			break;
		case BLOCK_L:
			setPoint(&point[0], -1, 0);
			setPoint(&point[1], point->x + 1, point->y);
			setPoint(&point[2], point->x + 2, point->y);
			setPoint(&point[3], point->x + 2, point->y + 1);
			blockHeight = 2;
			break;
		case BLOCK_T:
			setPoint(&point[0], 0, 0);
			setPoint(&point[1], point->x - 1, point->y + 1);
			setPoint(&point[2], point->x, point->y + 1);
			setPoint(&point[3], point->x + 1, point->y  + 1);
			blockHeight = 2;
			break;
		}

		for (int j = 0; j < BLOCK_SIZE; j++) {
			gotoxy(2 * ((startX + 2) + point[j].x), nowY + point[j].y);
			printf("■");
		}

		nowY += blockHeight + 1;
	}
}

void clearBlockList() {		// 출력된 블럭 리스트 지우기
	const int startX = 13;
	for (int i = 0; i < 5; i++) {
		for (int j = 1; j <= 18; j++) {
			gotoxy(2 * (startX + i), j);
			printf("  ");
		}
	}
}

void setBlock(BLOCK* pBlock, TYPE* pList) {		// 블럭 의 속성값 초기화.
	static int a = 0;
	gotoxy(40, 5);
	printf("블럭 생성 횟수 : %d", ++a);

	POINT* point = pBlock->blockPoint;
	setPoint(point, 2 * 3, -1);	// 블럭의 중심점을 (2 * 3, -1)으로 초기화.

	pBlock->blockType = popBlockList(pList);

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
	int y;

	for (int i = 0; i < BLOCK_SIZE; i++) {
		y = pBlock->blockPoint[i].y < 0 ? 0 : pBlock->blockPoint[i].y;

		if (map[y][pBlock->blockPoint[i].x + x] == 1) {
			return true;
		}
	}
	return false;
}

void moveBlockPoint(BLOCK* pBlock, int x, int y) { // 블럭의 모든 점의 좌표를 (x, nowY)만큼 옮긴다.
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
		if (0 <= pBlock->blockPoint[i].y) {
			gotoxy(2 * pBlock->blockPoint[i].x, pBlock->blockPoint[i].y);
			printf("■");
		}
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

void clearLine(BLOCK* pBlock) {
	int cleared_cnt = 0;
	int lowest_y = getBlockLowestY(pBlock);
	int highest_y = getBlockHighestY(pBlock);
	int check_line;

	for (check_line = lowest_y; highest_y <= check_line; check_line--) {		// 블럭의 실제 높이만큼만 검사함
		if (isCleared(check_line)) {		// 현재 검사하는 라인이 클리어되었다면
			cleared_cnt++;
			resetLine(check_line);
		}
		else if(cleared_cnt != 0) {		// 클리어되진 않았지만, 밑에 텅 빈 라인이 있다면
			dropLine(check_line, cleared_cnt);
			resetLine(check_line);
		}
	}

	if (cleared_cnt != 0) {		// 클리어된 라인이 하나라도 존재한다면
		for (; 1 <= check_line; check_line--) {		// 모든 남은 블럭들도 그만큼 밑으로 내림
			dropLine(check_line, cleared_cnt);
		}
		printMap();
	}
}

void resetLine(int line_y) {		// 해당 라인 제거 (전부 0으로 변경)
	for (int j = 1; j < WIDTH - 1; j++) {
		map[line_y][j] = 0;
	}
}

void dropLine(int line_y, int cleared_cnt) {		// 클리어된 라인 수만큼 밑으로 내림
	for (int j = 1; j < WIDTH - 1; j++) {
		map[line_y + cleared_cnt][j] = map[line_y][j];
	}
}

bool isCleared(int line_y) {		// 해당 라인이 클리어됐다면 true
	for (int j = 1; j < WIDTH - 1; j++) {
		if (map[line_y][j] == 0) {
			return false;
		}
	}
	return true;
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

		if (X < 1) { // blockPoint[j]의 x좌표가 1보다 작을 때, x = 1을 기준으로 한 상대적 위치
			tmp = X - 1;
		}
		else if (X > WIDTH - 2) { // blockPoint[j]의 x좌표가 WIDTH - 2보다 클 때, x = WIDTH - 2를 기준으로 한 상대적 위치
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

int getBlockLowestY(BLOCK* pBlock) {		// 블럭에서 가장 낮은 높이값을 리턴
	int lowestY = pBlock->blockPoint[0].y;

	for (int i = 0; i < BLOCK_SIZE - 1; i++) {
		lowestY = GET_MAX(lowestY, pBlock->blockPoint[i + 1].y);
	}
	return lowestY;
}

int getBlockHighestY(BLOCK* pBlock) {		// 블럭에서 가장 높은 높이값을 리턴
	int highestY = pBlock->blockPoint[0].y;

	for (int i = 0; i < BLOCK_SIZE - 1; i++) {
		highestY = GET_MIN(highestY, pBlock->blockPoint[i + 1].y);
	}
	return highestY;
}

void gotoxy(int x, int y) {		// 커서를 해당 좌표로 이동
	COORD Cur = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}
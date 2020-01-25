#include "Tetris.h"

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

	switch (pBlock->blockType) {		// 각 블럭의 최대 회전 횟수 설정.
	case BLOCK_O: pBlock->rotationCycle = 1; break;
	case BLOCK_I:
	case BLOCK_Z:
	case BLOCK_S: pBlock->rotationCycle = 2; break;
	case BLOCK_J:
	case BLOCK_L:
	case BLOCK_T: pBlock->rotationCycle = 4;
	}

	pBlock->rotation = 1;		// 블럭의 현재 회전 상태 초기화.
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

void dropBlock(const int map[][WIDTH], BLOCK* pBlock) { // 블럭의 각 y좌표 값을 증가시켜 드랍시킴.
	removeBlock(map, pBlock);
	for (int i = 0; i < BLOCK_SIZE; i++) {
		(pBlock->blockPoint[i].y)++;
	}
	putBlock(map, pBlock);
}

void rotateBlock(const int map[][WIDTH], BLOCK* pBlock) { // 블럭을 회전시키는 함수.
	int rotatedir;		// 블럭이 원상태로 돌아가기까지의 회전 횟수.
	int deltaX;	// map의 테두리로부터 튀어나온 x좌표 거리
	POINT* point = pBlock->blockPoint;

	if (pBlock->rotation != pBlock->rotationCycle) {		// 아직 최대 회전 횟수를 넘기지 않았다면
		rotatedir = 1;
		pBlock->rotation++;
	}
	else {		// 최대 회전 횟수를 넘겼다면
		switch (pBlock->rotationCycle) {
		case 1: return;
		case 2: rotatedir = -1; break;
		case 4: rotatedir = 1;
		}
		pBlock->rotation = 1;
	}

	// rotatedir이 1이면 양의 방향으로 90도 회전, -1이면 음의 방향으로 90도 회전.
	for (int i = 0; i < BLOCK_SIZE; i++) {
		int delta_x = point[i].x - point[0].x;
		int delta_y = point[i].y - point[0].y;
		setPoint(&point[i], point[0].x - rotatedir * delta_y, point[0].y + rotatedir * delta_x);
	}

	deltaX = getDeltaXfromSide(map, pBlock);	// map의 테두리로부터 가장 많이 튀어나온 거리값 리턴. 없을시 0
	/* 블럭이 테두리에 닿거나 밖으로 나갔다면 */
	if (deltaX) {
		/* 튀어나온 거리만큼 x좌표 변경 */
		for (int i = 0; i < BLOCK_SIZE; i++) {
			pBlock->blockPoint[i].x -= deltaX;
		}
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

int getDeltaXfromSide(const int map[][WIDTH], BLOCK* pBlock) {	// map의 테두리로부터 블럭 간의 x좌표 거리를 리턴. 만약 블럭이 테두리 밖에 있지 않다면 0을 리턴
	const float midOfWidth = (WIDTH - 2) / 2.0f;	// map의 x좌표 중 중앙 좌표 -> (WIDTH - 2(양 테두리)) / 2.0 = 5.5
	float deltaX = 0.0f;		// map의 테두리로부터 블럭 간의 x좌표 거리.

	// map의 중앙으로부터의 거리가 가장 크다면 deltaX에 저장. 없다면 초기값인 0이 저장됨
	for (int i = 1; i < BLOCK_SIZE; i++) {
		if (midOfWidth <= ABS_NUM(pBlock->blockPoint[i].x - midOfWidth)) { // x좌표가 map의 테두리 밖으로 나갔다면
			if (deltaX) {		// 비교 대상이 있다면
				if (ABS_NUM(pBlock->blockPoint[i].x - midOfWidth) > ABS_NUM(deltaX))		// 현재 x좌표가 튀어나온 거리와 비교대상의 x좌표가 튀어나온 거리 비교
					deltaX = pBlock->blockPoint[i].x - midOfWidth;
			}
			else    // 비교 대상이 없다면
				deltaX = pBlock->blockPoint[i].x - midOfWidth;	// 그냥 저장
		}
	}

	if (deltaX) {	
		if (0 < deltaX) {	// deltaX가 양수라면 map의 양수 방향으로 튀어나온 것임. 양수 방향으로 튀어나왔다면
			deltaX += midOfWidth;	// (중앙으로부터의 x좌표 거리) + (중앙 좌표) = (원래 x좌표)
			deltaX -= 10;		// (원래 x좌표) - (테두리 직전의 x좌표) = (테두리까지의 x좌표 거리)
		}
		else {
			deltaX += midOfWidth;	// (중앙으로부터의 x좌표 거리) + (중앙 좌표) = (원래 좌표)
			deltaX -= 1;			// (원래 x좌표) - (테두리 직전의 x좌표) = (테두리까지의 x좌표 거리)
		}
	}

	return (int)deltaX;
}
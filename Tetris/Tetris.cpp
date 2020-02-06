#include "Tetris.h"
extern bool map[HEIGHT][WIDTH];

void setMap() {		// �� �ʱ�ȭ(�׵θ� ����).
	for (int i = 0; i < WIDTH; i++) {
		//map[0][i] = 1;
		map[HEIGHT - 1][i] = 1;
	}
	for (int i = 0; i < HEIGHT - 2; i++) {
		map[i + 1][0] = 1;
		map[i + 1][WIDTH - 1] = 1;
	}
}

void printMap() {		// �ʱ�ȭ�� �� ���
	gotoxy(0, 0);
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (!i || map[i][j]) printf("��");
			else printf("  ");
		}
		printf("\n");
	}
}

void setPoint(POINT* pPoint, int x, int y) {		// ���� x, y��ǥ ���� �Է¹��� ��� ����.
	pPoint->x = x;
	pPoint->y = y;
}

bool isInsideMap(const POINT* pPoint) {
	return (0 < pPoint->x && pPoint->x < WIDTH - 1) && (0 < pPoint->y && pPoint->y < HEIGHT - 1);
}

void setBlock(BLOCK* pBlock) {		// �� �� �Ӽ��� �ʱ�ȭ.
	POINT* point = pBlock->blockPoint;
	setPoint(point, 2 * 3, -1);	// ���� �߽����� (2 * 3, -1)���� �ʱ�ȭ.

	//pBlock->blockType = BLOCK_I;
	pBlock->blockType = (TYPE)(rand() % 7);
	// �ϳ��� ���� �����ϴ� 4���� ���� ������ �߽��� �������� ��ǥ �ʱ�ȭ. 
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
	static int key_nFrame[5]; // key_nFrame�� ���� Ű ���� ���γ� ���� Ƚ��.(frame)
	const int downFrame = 5, moveFrame = 10; // down / left, right�� ������ ������ �ֱ�.

	// down, left, right���� ++�� ���� �����ڸ� ù �Է� ����, ���� �����ڸ� ù �Է� ����X.
	*up = pressed(VK_UP) && !key_nFrame[KS_UP]++;
	*down = pressed(VK_DOWN) && !(++key_nFrame[KS_DOWN] % downFrame);
	*left = pressed(VK_LEFT) && !pressed(VK_RIGHT) && !(key_nFrame[KS_LEFT]++ % moveFrame);
	*right = !pressed(VK_LEFT) && pressed(VK_RIGHT) && !(key_nFrame[KS_RIGHT]++ % moveFrame);
	*space = pressed(VK_SPACE) && !key_nFrame[KS_SPACE]++;

	// �ƹ� Ű�� ������ �ʾ��� ��� key_nFrame �ʱ�ȭ.
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

void moveBlockPoint(BLOCK* pBlock, int x, int y) { // ���� ��� ���� ��ǥ�� (x, y)��ŭ �ű��.
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


void rotateBlockPoint(BLOCK* pBlock) { // ���� ȸ����Ű�� �Լ�.
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
		/* Ƣ��� �Ÿ���ŭ x��ǥ ���� */
		moveBlockPoint(pBlock, -deltaX, 0);
	}
}

void rotateBlock(BLOCK* pBlock) {
	removeBlock(pBlock);
	rotateBlockPoint(pBlock);
	pBlock->deltaY = getDeltaY(pBlock);
	putBlock(pBlock);
}

void putBlock(BLOCK* pBlock) { // ����� ��ǥ�� �̵��Ͽ� ���� �����.
	//putBlockPrev(pBlock);
	for (int i = 0; i < BLOCK_SIZE; i++) {
		gotoxy(2 * pBlock->blockPoint[i].x, pBlock->blockPoint[i].y);
		printf("��");
	}
}

void putBlockPrev(BLOCK* pBlock) {		// ��� ���� ����� �̸����� ���.
	for (int i = 0; i < BLOCK_SIZE; i++) {
		gotoxy(2 * pBlock->blockPoint[i].x, pBlock->blockPoint[i].y + pBlock->deltaY);
		printf("��");
	}
}

void removeBlock(BLOCK* pBlock) {		// ��µ� ���� ��ǥ�� ������ ����� �����.
	//removeBlockPrev(pBlock);		// �̸����� �� ����
	for (int i = 0; i < BLOCK_SIZE; i++) {
		if (isInsideMap(&pBlock->blockPoint[i])) {   // map�� �׵θ� �ȿ� ���� ����
			gotoxy(2 * pBlock->blockPoint[i].x, pBlock->blockPoint[i].y);
			printf("  ");	// ������ �Ḧ ����� ���� ���� ���
		}
	}
}

void removeBlockPrev(BLOCK* pBlock) {		// �̸����� ���� ������ ����Ͽ� �����.
	for (int i = 0; i < BLOCK_SIZE; i++) {
		gotoxy(2 * pBlock->blockPoint[i].x, pBlock->blockPoint[i].y + pBlock->deltaY);
		printf("  ");
	}
}

int getDeltaY(BLOCK* pBlock) {	// �������� ���� �ٴ� ���� �Ÿ��� �����ϴ� �Լ�.
	int deltaY = HEIGHT;		// �������� ���� �ٴڰ��� �Ÿ��� ����. ���������� �ּڰ��� ��� ���� ��ǥ�̹Ƿ� �ִ����� �ʱ�ȭ

	for (int i = 0; i < BLOCK_SIZE; i++) {
		for (int j = pBlock->blockPoint[i].y; j < HEIGHT - 1; j++) {		// map�� �� ������ ����� �׿��ִ� ��ġ�� Ž��
			if (map[j + 1][pBlock->blockPoint[i].x]) {		// �׿��ִ� ��ġ�� ã�Ҵٸ� 
				deltaY = GET_MIN(deltaY, j - pBlock->blockPoint[i].y);		// ���� �ٴ��� y��ǥ - �������� ���� y��ǥ 
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

		if (X < 1) { // blockPoint[i]�� x��ǥ�� 1���� ���� ��, x = 1�� �������� �� ����� ��ġ
			tmp = X - 1;
		}
		else if (X > WIDTH - 2) { // blockPoint[i]�� x��ǥ�� WIDTH - 2���� Ŭ ��, x = WIDTH - 2�� �������� �� ����� ��ġ
			tmp = X - (WIDTH - 2);
		}
		else { // blockPoint�� �˸��� ��ġ�� ���� ��.
			tmp = 0;
		}

		if (tmp && abs(tmp) > abs(deltaX)) {
			deltaX = tmp;
		}
	}
	return deltaX;
}

void gotoxy(int x, int y) {		// Ŀ���� �ش� ��ǥ�� �̵�
	COORD Cur = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}
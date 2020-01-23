#include "Tetris.h"

void setMap(int map[][WIDTH]) {		// �� �ʱ�ȭ(�׵θ� ����).
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
			if (map[i][j]) printf("��");
			else printf("  ");
		}
		printf("\n");
	}
}

void setBlock(BLOCK* pBlock) {		// �� �� �Ӽ��� �ʱ�ȭ.
	POINT* point = pBlock->blockPoint;
	setPoint(point, 2 * 3, -1);	// ���� �������� (2 * 3, -1)���� �ʱ�ȭ

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

void removeBlock(const int map[][WIDTH], BLOCK* pBlock) { // �� ���� ������ ����� �����.
	removeBlockPrev(map, pBlock);
	for (int i = 0; i < BLOCK_SIZE; i++) {
		if ((0 < pBlock->blockPoint[i].x && pBlock->blockPoint[i].x < WIDTH - 1) && (0 < pBlock->blockPoint[i].y && pBlock->blockPoint[i].y < HEIGHT - 1)) {   // map�� �׵θ��� �ƴ� ��쿡��
			gotoxy(2 * pBlock->blockPoint[i].x, pBlock->blockPoint[i].y);
			printf("  ");	// ������ �Ḧ ����� ���� ���� ���
		}
	}
}

void dropBlock(const int map[][WIDTH], BLOCK* pBlock) { // ���� ��� point�� y��ǥ ���� �������� ������ ������.
	removeBlock(map, pBlock);
	for (int i = 0; i < BLOCK_SIZE; i++) {
		(pBlock->blockPoint[i].y)++;
	}
	putBlock(map, pBlock);
}

void rotateBlock(const int map[][WIDTH], BLOCK* pBlock) { // ������ �߽����� �ݽð� �������� 90�� ȸ��.
	if (pBlock->rotationCycle == 1) return;	// BLOCK_O�� ���� ��� ����

	int delta_x = 0;
	int delta_y = 0;
	int rotatedir = 0;	// ���� �ʱ� ���·� �ǵ�������� �ʿ��� ȸ�� Ƚ��

	if (pBlock->rotationCycle == pBlock->rotation) {		// ȸ�� Ƚ���� �ִ��� ��
		rotatedir = (2 < pBlock->rotationCycle ? 1 : -1);		// 2�� ������ ���� 1, 4�� ������ ���� -1

		for (int i = 0; i < BLOCK_SIZE; i++) {
			delta_x = pBlock->blockPoint[i].x - pBlock->blockPoint[0].x;
			delta_y = pBlock->blockPoint[i].y - pBlock->blockPoint[0].y;
			setPoint(&pBlock->blockPoint[i], pBlock->blockPoint[0].x - delta_y * rotatedir, pBlock->blockPoint[0].y + delta_x * rotatedir);
		}
		pBlock->rotation = 1;
	}
	else {
		for (int i = 0; i < BLOCK_SIZE; i++) {
			delta_x = pBlock->blockPoint[i].x - pBlock->blockPoint[0].x;
			delta_y = pBlock->blockPoint[i].y - pBlock->blockPoint[0].y;
			setPoint(&pBlock->blockPoint[i], pBlock->blockPoint[0].x - delta_y, pBlock->blockPoint[0].y + delta_x);
		}
		pBlock->rotation++;
	}
}

void putBlock(const int map[][WIDTH], BLOCK* pBlock) { // moveBlock�� ���� �Ű��� ��ǥ�� ���� ����Ѵ�.
	putBlockPrev(map, pBlock);
	for (int i = 0; i < BLOCK_SIZE; i++) {
		gotoxy(2 * pBlock->blockPoint[i].x, pBlock->blockPoint[i].y);
		printf("��");
	}
}

void putBlockPrev(const int map[][WIDTH], BLOCK* pBlock) {		// �������� ����� ������ ��ġ�� �̸� ������
	int deltaY = getDeltaY(map, pBlock);

	for (int i = 0; i < BLOCK_SIZE; i++) {
		gotoxy(2 * pBlock->blockPoint[i].x, pBlock->blockPoint[i].y + deltaY);
		printf("��");
	}
}

void removeBlockPrev(const int map[][WIDTH], BLOCK* pBlock) {
	int deltaY = getDeltaY(map, pBlock);

	for (int i = 0; i < BLOCK_SIZE; i++) {
		gotoxy(2 * pBlock->blockPoint[i].x, pBlock->blockPoint[i].y + deltaY);
		printf("  ");
	}
}

void gotoxy(int x, int y) {		// Ŀ���� �ش� ��ǥ�� �̵�
	COORD Cur = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

int getDeltaY(const int map[][WIDTH], BLOCK* pBlock) {	// �������� ���� �ٴ� ���� �Ÿ��� �����ϴ� �Լ�
	int deltaY = HEIGHT;		// �������� ���� �ٴڰ��� �Ÿ��� ����. ���������� �ּڰ��� ��� ���� ��ǥ�̹Ƿ� �ִ����� �ʱ�ȭ

	for (int i = 0; i < BLOCK_SIZE; i++) {
		for (int j = HEIGHT - 1; 1 <= j; j--) {
			if (map[j][i] != 1) {
				deltaY = GET_MIN(deltaY, j - pBlock->blockPoint[i].y);		// ���� �ٴ��� y��ǥ - �������� ���� y��ǥ 
				break;
			}
		}
	}

	return deltaY;
}
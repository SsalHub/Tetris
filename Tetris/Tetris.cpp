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

void printMap(const int map[][WIDTH]) {		// �ʱ�ȭ�� �� ���
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
	setPoint(point, 2 * 3, -1);	// ���� �߽����� (2 * 3, -1)���� �ʱ�ȭ

	pBlock->blockType = (TYPE)(rand() % 7);

	switch (pBlock->blockType) {		// �ϳ��� ���� �����ϴ� 4���� ���� ������ �߽��� �������� ��ǥ �ʱ�ȭ. 
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

void setPoint(POINT* pPoint, int x, int y) {		// ���� x, y��ǥ ���� �Է¹��� ��� ����.
	pPoint->x = x;
	pPoint->y = y;
}

void removeBlock(const int map[][WIDTH], BLOCK* pBlock) {		// ��µ� ���� ��ǥ�� ������ ����� �����.
	removeBlockPrev(map, pBlock);		// �̸����� �� ����
	for (int i = 0; i < BLOCK_SIZE; i++) {
		if ((0 < pBlock->blockPoint[i].x && pBlock->blockPoint[i].x < WIDTH - 1) && (0 < pBlock->blockPoint[i].y && pBlock->blockPoint[i].y < HEIGHT - 1)) {   // map�� �׵θ��� �ƴ� ��쿡��
			gotoxy(2 * pBlock->blockPoint[i].x, pBlock->blockPoint[i].y);
			printf("  ");	// ������ �Ḧ ����� ���� ���� ���
		}
	}
}

void dropBlock(const int map[][WIDTH], BLOCK* pBlock) { // ���� �� y��ǥ ���� �������� �����Ŵ.
	removeBlock(map, pBlock);
	for (int i = 0; i < BLOCK_SIZE; i++) {
		(pBlock->blockPoint[i].y)++;
	}
	putBlock(map, pBlock);
}

void rotateBlock(const int map[][WIDTH], BLOCK* pBlock) { // ���� ȸ����Ű�� �Լ�.
	if (pBlock->blockType == BLOCK_O) return;
	int delta_x, delta_y;		// ȸ���� ��, �߽� �����κ��� ȸ���� �� ������ x��ǥ���� �Ÿ� / y��ǥ���� �Ÿ�
	int delta_x_from_side;	// map�� �׵θ��κ��� Ƣ��� x��ǥ �Ÿ�
	POINT* point = pBlock->blockPoint;

	// 90�� ȸ��
	for (int i = 0; i < BLOCK_SIZE; i++) {
		delta_x = point[i].x - point[0].x;
		delta_y = point[i].y - point[0].y;
		setPoint(&point[i], point[0].x - delta_y, point[0].y + delta_x);
	}

	delta_x_from_side = getDeltaXfromSide(map, pBlock);	// map�� �׵θ��κ��� ���� ���� Ƣ��� �Ÿ��� ����. ������ 0
	/* ���� �׵θ��� ��ų� ������ �����ٸ� */
	if (delta_x_from_side) {
		/* Ƣ��� �Ÿ���ŭ x��ǥ ���� */
		for (int i = 0; i < BLOCK_SIZE; i++) {
			pBlock->blockPoint[i].x -= delta_x_from_side;
		}
	}
}

void putBlock(const int map[][WIDTH], BLOCK* pBlock) { // ����� ��ǥ�� �̵��Ͽ� ���� �����.
	putBlockPrev(map, pBlock);
	for (int i = 0; i < BLOCK_SIZE; i++) {
		gotoxy(2 * pBlock->blockPoint[i].x, pBlock->blockPoint[i].y);
		printf("��");
	}
}

void putBlockPrev(const int map[][WIDTH], BLOCK* pBlock) {		// ��� ���� ����� �̸����� ���.
	int deltaY = getDeltaY(map, pBlock);

	for (int i = 0; i < BLOCK_SIZE; i++) {
		gotoxy(2 * pBlock->blockPoint[i].x, pBlock->blockPoint[i].y + deltaY);
		printf("��");
	}
}

void removeBlockPrev(const int map[][WIDTH], BLOCK* pBlock) {		// �̸����� ���� ������ ����Ͽ� �����.
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

int getDeltaY(const int map[][WIDTH], BLOCK* pBlock) {	// �������� ���� �ٴ� ���� �Ÿ��� �����ϴ� �Լ�.
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

int getDeltaXfromSide(const int map[][WIDTH], BLOCK* pBlock) {	// map�� �׵θ��κ��� �� ���� x��ǥ �Ÿ��� ����. ���� ���� �׵θ� �ۿ� ���� �ʴٸ� 0�� ����
	int X, tmp;
	int deltaX = 0;		// map�� �׵θ��κ��� �� ���� x��ǥ �Ÿ�.

	for (int i = 0; i < BLOCK_SIZE; i++) {
		X = pBlock->blockPoint[i].x;		// ���� ���� x��ǥ

		if (X < 1)			// map�� ���� �׵θ� ������ ���� ���
			tmp = X - 1;
		else if (WIDTH - 2 < X)			// map�� ������ �׵θ� ������ ���� ���
			tmp = X - (WIDTH - 2);
		else		// �׵θ� ������ ������ ���� ���
			tmp = 0;

		if (tmp && abs(tmp) > abs(deltaX))		// �׵θ� ������ ���� �Ÿ� �� �� ���� ū ���� ���
			deltaX = tmp;
	}
	return deltaX;
}
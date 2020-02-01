#include "Tetris.h"
extern bool map[HEIGHT][WIDTH];

void setMap() {		// �� �ʱ�ȭ(�׵θ� ����).
	for (int i = 0; i < WIDTH; i++) {
		map[0][i] = 1;
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
			if (map[i][j]) printf("��");
			else printf("  ");
		}
		printf("\n");
	}
}

void setPoint(POINT* pPoint, int x, int y) {		// ���� x, y��ǥ ���� �Է¹��� ��� ����.
	pPoint->x = x;
	pPoint->y = y;
}

void setBlock(BLOCK* pBlock) {		// �� �� �Ӽ��� �ʱ�ȭ.
	POINT* point = pBlock->blockPoint;
	setPoint(point, 2 * 3, -1);	// ���� �߽����� (2 * 3, -1)���� �ʱ�ȭ.

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
		if ((0 < pBlock->blockPoint[i].x && pBlock->blockPoint[i].x < WIDTH - 1) && (0 < pBlock->blockPoint[i].y && pBlock->blockPoint[i].y < HEIGHT - 1)) {   // map�� �׵θ��� �ƴ� ��쿡��
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
		for (int j = 1; j < HEIGHT; j++) {		// map�� �� ������ ����� �׿��ִ� ��ġ�� Ž��
			if (map[j + 1][pBlock->blockPoint[i].x] == 1) {		// �׿��ִ� ��ġ�� ã�Ҵٸ� 
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

int getKey() {
	static int key_nFrame[5];
	// �� ����Ű�� ���� �������� ���� �Է� �������� ���� ȸ����
	int key = -1;

	int keyState = GetAsyncKeyState(VK_UP);		// '�� ����Ű�� �ԷµǾ��°�?' �� ���� ���� �޾ƿ�
	if (keyState & 0x8000) {		// �� ����Ű�� �ԷµǾ��ٸ� ~
		if (key_nFrame[KS_UP] != 1) {		// �� ����Ű�� ��� �����ִ� ���°� �ƴϾ��ٸ� ~ 
			key = VK_UP;
		}
		key_nFrame[KS_UP] = 1;		// ��� �����ִ� ���¸� ���ϴ� 1�� �� ����
	}
	else {		// �� ����Ű�� �Էµ��� �ʾҴٸ�
		key_nFrame[KS_UP] = 0;		// �������� ���� ���¸� ���ϴ� 0���� �� ����
	}

	// �Ʒ� ����Ű�� ���� ���� ��ŭ ���� ������
	keyState = GetAsyncKeyState(VK_DOWN);		// '�Ʒ� ����Ű�� �ԷµǾ��°�' �� ���� ���� �޾ƿ�
	if (keyState & 0x8000) {		// �Ʒ� ����Ű�� �ԷµǾ��ٸ� ~
		if (key_nFrame[KS_DOWN] % 5 == 0) {		// 5�����Ӹ��� ����
			key = VK_DOWN;
		}
		key_nFrame[KS_DOWN]++;		// ���� �ð���ŭ ������ ���� 1�� ����
	}
	else {		// �Ʒ� ����Ű�� �Էµ��� �ʾҴٸ�
		key_nFrame[KS_DOWN] = 0;		// ������ ���� �ٽ� 0���� �ʱ�ȭ
	}

	// �¿� ����Ű�� ���� ���� ��ŭ ���� �̵�
	keyState = GetAsyncKeyState(VK_LEFT);
	if (keyState & 0x8000) {	// ���� ����Ű�� �ԷµǾ��ٸ�
		if (key_nFrame[KS_LEFT] % 10 == 0) {
			key = VK_LEFT;
		}
		key_nFrame[KS_LEFT]++;
	}
	else {		// ���� ����Ű�� �Էµ��� �ʾҴٸ�
		key_nFrame[KS_LEFT] = 0;
		keyState = GetAsyncKeyState(VK_RIGHT);
		if (keyState & 0x8000) {		// ������ ����Ű�� �ԷµǾ��ٸ�
			if (key_nFrame[KS_RIGHT] % 10 == 0) {
				key = VK_RIGHT;
			}
			key_nFrame[KS_RIGHT]++;
		}
		else {	// ������ ����Ű�� �Էµ��� �ʾҴٸ�
			key_nFrame[KS_RIGHT] = 0;
		}
	}

	// �����̽��ٴ� ���� �������� ���� �Է� �������� ���� ������
	keyState = GetAsyncKeyState(VK_SPACE);
	if (keyState & 0x8000) {
		if (key_nFrame[KS_SPACE] != 1) {
			key = VK_SPACE;
		}
		key_nFrame[KS_SPACE] = 1;		// �� ����Ű�� ���� ������� ����. 1 = Ű�� �����ִ� ����
	}
	else
		key_nFrame[KS_SPACE] = 0;		// 0 = Ű�� �������� ���� ����

	return key;
}

void gotoxy(int x, int y) {		// Ŀ���� �ش� ��ǥ�� �̵�
	COORD Cur = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}
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

	switch (pBlock->blockType) {		// �� ���� �ִ� ȸ�� Ƚ�� ����.
	case BLOCK_O: pBlock->rotationCycle = 1; break;
	case BLOCK_I:
	case BLOCK_Z:
	case BLOCK_S: pBlock->rotationCycle = 2; break;
	case BLOCK_J:
	case BLOCK_L:
	case BLOCK_T: pBlock->rotationCycle = 4;
	}

	pBlock->rotation = 1;		// ���� ���� ȸ�� ���� �ʱ�ȭ.
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
	int rotatedir;		// ���� �����·� ���ư�������� ȸ�� Ƚ��.
	int deltaX;	// map�� �׵θ��κ��� Ƣ��� x��ǥ �Ÿ�
	POINT* point = pBlock->blockPoint;

	if (pBlock->rotation != pBlock->rotationCycle) {		// ���� �ִ� ȸ�� Ƚ���� �ѱ��� �ʾҴٸ�
		rotatedir = 1;
		pBlock->rotation++;
	}
	else {		// �ִ� ȸ�� Ƚ���� �Ѱ�ٸ�
		switch (pBlock->rotationCycle) {
		case 1: return;
		case 2: rotatedir = -1; break;
		case 4: rotatedir = 1;
		}
		pBlock->rotation = 1;
	}

	// rotatedir�� 1�̸� ���� �������� 90�� ȸ��, -1�̸� ���� �������� 90�� ȸ��.
	for (int i = 0; i < BLOCK_SIZE; i++) {
		int delta_x = point[i].x - point[0].x;
		int delta_y = point[i].y - point[0].y;
		setPoint(&point[i], point[0].x - rotatedir * delta_y, point[0].y + rotatedir * delta_x);
	}

	deltaX = getDeltaXfromSide(map, pBlock);	// map�� �׵θ��κ��� ���� ���� Ƣ��� �Ÿ��� ����. ������ 0
	/* ���� �׵θ��� ��ų� ������ �����ٸ� */
	if (deltaX) {
		/* Ƣ��� �Ÿ���ŭ x��ǥ ���� */
		for (int i = 0; i < BLOCK_SIZE; i++) {
			pBlock->blockPoint[i].x -= deltaX;
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
	const float midOfWidth = (WIDTH - 2) / 2.0f;	// map�� x��ǥ �� �߾� ��ǥ -> (WIDTH - 2(�� �׵θ�)) / 2.0 = 5.5
	float deltaX = 0.0f;		// map�� �׵θ��κ��� �� ���� x��ǥ �Ÿ�.

	// map�� �߾����κ����� �Ÿ��� ���� ũ�ٸ� deltaX�� ����. ���ٸ� �ʱⰪ�� 0�� �����
	for (int i = 1; i < BLOCK_SIZE; i++) {
		if (midOfWidth <= ABS_NUM(pBlock->blockPoint[i].x - midOfWidth)) { // x��ǥ�� map�� �׵θ� ������ �����ٸ�
			if (deltaX) {		// �� ����� �ִٸ�
				if (ABS_NUM(pBlock->blockPoint[i].x - midOfWidth) > ABS_NUM(deltaX))		// ���� x��ǥ�� Ƣ��� �Ÿ��� �񱳴���� x��ǥ�� Ƣ��� �Ÿ� ��
					deltaX = pBlock->blockPoint[i].x - midOfWidth;
			}
			else    // �� ����� ���ٸ�
				deltaX = pBlock->blockPoint[i].x - midOfWidth;	// �׳� ����
		}
	}

	if (deltaX) {	
		if (0 < deltaX) {	// deltaX�� ������ map�� ��� �������� Ƣ��� ����. ��� �������� Ƣ��Դٸ�
			deltaX += midOfWidth;	// (�߾����κ����� x��ǥ �Ÿ�) + (�߾� ��ǥ) = (���� x��ǥ)
			deltaX -= 10;		// (���� x��ǥ) - (�׵θ� ������ x��ǥ) = (�׵θ������� x��ǥ �Ÿ�)
		}
		else {
			deltaX += midOfWidth;	// (�߾����κ����� x��ǥ �Ÿ�) + (�߾� ��ǥ) = (���� ��ǥ)
			deltaX -= 1;			// (���� x��ǥ) - (�׵θ� ������ x��ǥ) = (�׵θ������� x��ǥ �Ÿ�)
		}
	}

	return (int)deltaX;
}
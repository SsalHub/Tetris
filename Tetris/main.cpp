#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

#define WIDTH 12					// ���� �ʺ�
#define HEIGHT 27					// ���� ����
#define BLOCK_SIZE 4				// ��� �ϳ��� ���ԵǴ� ���� ����� ����(��� 4��)
#define FRAME_PER_SEC 240		// ��Ʈ������ fps. �ʴ� 240ȸ�� ������ �ϵ��� ���� (���� ���� �� �ʴ� nõȸ �̻�)
#define ARROW_KEY_DEFAULT 224		// ����Ű�� �Է¹��� ��, ����Ű�� ASCII���� �ռ� �ԷµǴ� ASCII��
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_SPACE 32

#define GET_MIN(n1, n2) ((n1) < (n2) ? (n1) : (n2))		// �� ���� ���� �����ϴ� ��ũ�� �Լ�

typedef enum type { BLOCK_I, BLOCK_O, BLOCK_Z, BLOCK_S, BLOCK_J, BLOCK_L, BLOCK_T } TYPE; // ���� ����(7����)�� ���������� ����.
typedef enum direction { DIR_LEFT, DIR_RIGHT, DIR_DOWN } DIRECTION;

typedef struct block {
	TYPE blockType;
	POINT blockPoint[4]; // �迭�� ù ��Ұ� ������.
	int rotation, rotationCycle;	// ȸ�� Ƚ��, �ݺ� �ֱ�.
	short nFrame = 0;	// �������� �ӵ�.
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
	system("mode con cols=80 lines=30");		// ������Ʈ â ũ�� ����

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

	while (getDeltaY(map, &block)) {		// ����� �� �ٴڱ��� ������ ������ �ݺ�
		gotoxy(17, 27);
		printf("%03d", block.nFrame);


		/* Ű���� �Է¹޴� �κ� */
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
		/* Ű���� �Է¹޴� �κ� ��*/


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
			delta_x = pBlock->blockPoint[i].x - pBlock -> blockPoint[0].x;
			delta_y = pBlock->blockPoint[i].y - pBlock -> blockPoint[0].y;
			setPoint(&pBlock->blockPoint[i], pBlock -> blockPoint[0].x - delta_y, pBlock->blockPoint[0].y + delta_x);
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
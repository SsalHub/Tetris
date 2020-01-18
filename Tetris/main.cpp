#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#define WIDTH 12					// ���� �ʺ�
#define HEIGHT 27					// ���� ����
#define BLOCK_SIZE 4				// ��� �ϳ��� ���ԵǴ� ���� ����� ����(��� 4��)
#define FRAME_PER_SEC 240		// ��Ʈ������ fps. �ʴ� 240ȸ�� ������ �ϵ��� ���� (���� ���� �� �ʴ� nõȸ �̻�)

#define GET_MIN(n1, n2) ((n1) < (n2) ? (n1) : (n2))		// �� ���� ���� �����ϴ� ��ũ�� �Լ�

typedef enum type { I, O, Z, S, J, L, T } TYPE; // ���� ����(7����)�� ���������� ����.
typedef enum direction { LEFT, RIGHT, DOWN } DIRECTION;

//typedef struct point {
//	int x, y;
//} POINT;

typedef struct block {
	TYPE blockType;
	POINT blockPoint[4]; // �迭�� ù ��Ұ� ������.
	int rotation, rotationCycle;	// ȸ�� Ƚ��, �ݺ� �ֱ�.
	int nFrame = FRAME_PER_SEC; // �������� �ӵ�.
} BLOCK;

void setMap(int map[][WIDTH]);
void printMap(const int map[][WIDTH]);
void setBlock(BLOCK* p);
void setPoint(POINT* p, int x, int y);
void removeBlock(const int map[][WIDTH], BLOCK* p);
void moveBlock(const int map[][WIDTH], BLOCK* p, char direction);
void rotateBlock(const int map[][WIDTH], BLOCK* p);
void putBlock(const int map[][WIDTH], BLOCK* p);
void showBlockPrev(const int map[][WIDTH], BLOCK* pBlock);
void gotoxy(int x, int y);

int main() {
	srand((unsigned int)time(NULL));
	system("mode con cols=80 lines=30");		// ������Ʈ â ũ�� ����

	int map[HEIGHT][WIDTH] = { 0 };
	setMap(map);
	printMap(map);

	BLOCK a;
	setBlock(&a);
	putBlock(map, &a);
	gotoxy(5, 27);
	printf("a.nFrame = ");

	while (a.blockPoint[0].y < HEIGHT - 2) {		// ����� �� �ٴڱ��� ������ ������ �ݺ�
		gotoxy(16, 27);
		printf("%03d", a.nFrame);

		if (a.nFrame <= 0) {
			moveBlock(map, &a, 'a');
			gotoxy(16, 27);
			a.nFrame = FRAME_PER_SEC;
			printf("%03d", a.nFrame);
		}

		gotoxy(0, 0);
		a.nFrame--;
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

void setBlock(BLOCK* p) {		// �� �� �Ӽ��� �ʱ�ȭ.
	POINT* point = p->blockPoint;
	setPoint(point, 6, 0);	// ���� �������� (6, 2)���� �ʱ�ȭ

	//p->blockType = (TYPE)(rand() % 7);
	p->blockType = O;		// �ӽ÷� O�� ��ϸ� ������.		�Ʒ� switch�� �ϼ��� �� ������ �ּ� �ڵ�� ��ü

	switch (p->blockType) {
	case O:
		setPoint(&point[1], point->x - 1, point->y);
		setPoint(&point[2], point->x - 1, point->y - 1);
		setPoint(&point[3], point->x, point->y - 1);
		break;
	case I:;
	case Z:;
	case S:;
	case J:;
	case L:;
	case T:;
	}

	switch (p->blockType) {
	case O: p->rotationCycle = 1; break;
	case I:
	case Z:
	case S: p->rotationCycle = 2; break;
	case J:
	case L:
	case T: p->rotationCycle = 4;
	}

	p->rotation = rand() % p->rotationCycle;
}

void setPoint(POINT* p, int x, int y) {
	p->x = x;
	p->y = y;
}

void removeBlock(const int map[][WIDTH], BLOCK* p) { // �� ���� ������ ����� �����.
	for (int i = 0; i < BLOCK_SIZE; i++) {
		if ((0 < p->blockPoint[i].x && p->blockPoint[i].x < WIDTH - 1) && (0 < p->blockPoint[i].y && p->blockPoint[i].y < HEIGHT - 1)) {   // map�� �׵θ��� �ƴ� ��쿡��
			gotoxy(2 * p->blockPoint[i].x, p->blockPoint[i].y);
			printf("  ");	// ������ �Ḧ ����� ���� ���� ���
		}
	}
}

void moveBlock(const int map[][WIDTH], BLOCK* p, char direction) { // ���� ��� point�� y��ǥ ���� �������� ������ ������.
	removeBlock(map, p);
	for (int i = 0; i < BLOCK_SIZE; i++) {
		(p->blockPoint[i].y)++;
	}
	putBlock(map, p);
}

void rotateBlock(const int map[][WIDTH], BLOCK* p) { // ������ �߽����� �ݽð� �������� 90�� ȸ��.
	POINT* point = p->blockPoint;
	for (int i = 0; i < BLOCK_SIZE; i++) {
		int delta_x = point[i].x - point[0].x;
		int delta_y = point[i].y - point[0].y;
		setPoint(&point[i], point[0].x - delta_y, point[0].y + delta_x);
	}

	(p->rotation)++;
	p->rotation %= p->rotationCycle;
}

void putBlock(const int map[][WIDTH], BLOCK* p) { // moveBlock�� ���� �Ű��� ��ǥ�� ���� ����Ѵ�.
	for (int i = 0; i < BLOCK_SIZE; i++) {
		gotoxy(2 * p->blockPoint[i].x, p->blockPoint[i].y);
		printf("��");
	}
	showBlockPrev(map, p);
}

void showBlockPrev(const int map[][WIDTH], BLOCK* pBlock) {		// �������� ����� ������ ��ġ�� �̸� ������
	int deltaY = HEIGHT;		// �������� ���� �ٴڰ��� �Ÿ��� ����. ���������� �ּڰ��� ��� ���� ��ǥ�̹Ƿ� �ִ����� �ʱ�ȭ

	for (int i = 0; i < BLOCK_SIZE; i++) {
		for (int j = HEIGHT - 1; 1 <= j; j--) {
			if (!map[j][i]) {
				deltaY = GET_MIN(deltaY, j - pBlock->blockPoint[i].y);		// ���� �ٴ��� y��ǥ - �������� ���� y��ǥ 
				break;
			}
		}
	}

	for (int i = 0; i < BLOCK_SIZE; i++) {
		gotoxy(2 * pBlock->blockPoint[i].x, pBlock->blockPoint[i].y + deltaY);
		printf("��");
	}
}

void gotoxy(int x, int y) {		// Ŀ���� �ش� ��ǥ�� �̵�
	COORD Cur = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}
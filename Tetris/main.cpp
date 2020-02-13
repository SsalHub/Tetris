#include "Tetris.h"
/* ������� ���� ���� �Խù�
	1. https://programfrall.tistory.com/20
	2. https://pang2h.tistory.com/129
*/

bool map[HEIGHT][WIDTH] = { 0 };

int main() {
	srand((unsigned int)time(NULL));
	system("mode con cols=80 lines=30");		// ������Ʈ â ũ�� ����

	setMap();
	printMap();

	TYPE blockList[BLOCK_LIST_LEN];		// �� ����Ʈ
	TYPE newBlock;		// ����Ʈ���� ù��° ���� �̾ƿͼ� �ӽ÷� �����ϴ� ����
	setBlockList(blockList);

	BLOCK block;
	gotoxy(2, 27);
	printf("block.nFrame = ");
	
	while (1) {	// ���� ��������

		/* �� ����Ʈ���� ���� �޾ƿ� */
		newBlock = popBlockList(blockList);

		/* �� ����Ʈ�� �� �� �߰� �� ���*/
		addBlockList(blockList);
		clearBlockList();
		printBlockList(blockList);

		/* �� ���� */
		setBlock(&block, newBlock);
		
		/* �� ��� */
		while (0 < block.deltaY) {		// ����� �� �ٴڱ��� ������ ������ �ݺ�
			gotoxy(17, 27);
			printf("%03d", block.nFrame);

			/* Ű���� �Է¹޴� �κ� */
			
			bool up, down, left, right, space;
			getKey(&up, &down, &left, &right, &space);

			if (up) {
				rotateBlock(&block);
			}
			if (down) {
				moveBlock(&block, 0, 1);
				block.nFrame = FRAME_PER_SEC;
			}
			if (left) {
				if (!isBlocked(&block, -1))
					moveBlock(&block, -1, 0);
			}
			else if (right) {
				if (!isBlocked(&block, 1))
					moveBlock(&block, 1, 0);
			}
			if (space) {
				moveBlock(&block, 0, block.deltaY);
				break;
			}

			/* Ű���� �Է¹޴� �κ� ��*/

			if (block.nFrame <= 0) {
				moveBlock(&block, 0, 1);
				gotoxy(17, 27);
				block.nFrame = FRAME_PER_SEC;
				printf("%03d", block.nFrame);
			}

			gotoxy(0, 0);
			block.nFrame--;
			Sleep(1000 / FRAME_PER_SEC);
		}

		gotoxy(40, 2);
		printf("deltaX = %2d", getDeltaXfromSide(&block));
		//BLOCK_I, BLOCK_O, BLOCK_Z, BLOCK_S, BLOCK_J, BLOCK_L, BLOCK_T
		gotoxy(40, 3);
		switch (block.blockType) {
		case BLOCK_O:
			printf("block type = BLOCK_O");
			break;
		case BLOCK_I:
			printf("block type = BLOCK_I");
			break;
		case BLOCK_Z:
			printf("block type = BLOCK_Z");
			break;
		case BLOCK_S:
			printf("block type = BLOCK_S");
			break;
		case BLOCK_J:
			printf("block type = BLOCK_J");
			break;
		case BLOCK_L:
			printf("block type = BLOCK_L");
			break;
		case BLOCK_T:
			printf("block type = BLOCK_T");
			break;
		}

		/* �� ����� ������ map[][]�� 1�� ����*/
		for (int i = 0; i < BLOCK_SIZE; i++)
			map[block.blockPoint[i].y][block.blockPoint[i].x] = 1;

		/* �������� �� �ִ��� �˻� */
		clearLine(&block);
		
	}
	return 0;
}
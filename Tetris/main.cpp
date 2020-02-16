#include "Tetris.h"
/* ������� ���� ���� �Խù�
	1. https://programfrall.tistory.com/20
	2. https://pang2h.tistory.com/129
*/

bool map[HEIGHT][WIDTH] = { 0 };
BlockColor map_color[HEIGHT][WIDTH] = { NONE };

int main() {
	srand((unsigned int)time(NULL));
	system("mode con cols=80 lines=30");		// ������Ʈ â ũ�� ����
	setCursorView(false);

	setMap();
	printMap();

	TYPE blockList14];		// �� ����Ʈ
	TYPE newBlock;		// ����Ʈ���� ù��° ���� �̾ƿͼ� �ӽ÷� �����ϴ� ����
	setBlockList(blockList);

	BLOCK block;
	gotoxy(2, 27);
	printf("block.nFrame = ");
	
	while (1) {	// ���� ��������
		/* �� ���� */
		setBlock(&block, blockList);
		
		/* �� ����Ʈ ���*/
		clearBlockList();
		printBlockList(blockList);

		/* ���� ���� ���� */
		if (block.deltaY == 0) {		// ���� �������ڸ��� ������ ������ ���� ���¶��
			break;
		}

		/* �� ��� */
		while (0 < block.deltaY) {		// ����� �� �ٴڱ��� ������ ������ �ݺ�
			gotoxy(17, 27);
			SET_BLOCK_COLOR(DEFAULT);
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
				SET_BLOCK_COLOR(DEFAULT);
				printf("%03d", block.nFrame);
			}

			gotoxy(0, 0);
			block.nFrame--;
			Sleep(1000 / FRAME_PER_SEC);
		}

		/* �� ����� ������ map[][]�� 1�� ����*/
		for (int i = 0; i < BLOCK_SIZE; i++) {
			map[block.blockPoint[i].y][block.blockPoint[i].x] = 1;
			map_color[block.blockPoint[i].y][block.blockPoint[i].x] = (BlockColor)block.blockType;
		}
		/* �������� �� �ִ��� �˻� */
		clearLine(&block);
		
	}

	gotoxy(2 * 25, 12);
	SET_BLOCK_COLOR(DEFAULT);
	printf("GAME OVER!");
	gotoxy(2 * 25, 13);
	printf(("press ESC to exit"));
	while (1) {
		if(GetAsyncKeyState(VK_ESCAPE) & 0x8000){
			break;
		}
	}
	gotoxy(0, 28);
	return 0;
}
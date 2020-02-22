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
	int nFrame;

	setMap();
	printMap();

	TYPE blockList[14];		// �� ����Ʈ
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

		nFrame = FRAME_PER_SEC;

		/* �� ��� */

		putBlockPrev(&block);
		putBlock(&block);

		while (1) {
			gotoxy(17, 27);
			SET_BLOCK_COLOR(DEFAULT);
			printf("%03d", nFrame);

			/* Ű���� �Է¹޴� �κ� */
			bool up, down, left, right, space;
			getKey(&up, &down, &left, &right, &space);

			if (up) {
				rotateBlock(&block);
			}
			if (down) {
				if (!block.deltaY)
					break;
				moveBlock(&block, 0, 1);
				nFrame = FRAME_PER_SEC;
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

			// nFrame�� deltaY�� ��� 0�̸� break.
			if (nFrame) nFrame--;
			else {
				if (block.deltaY) {
					moveBlock(&block, 0, 1);
					gotoxy(17, 27);
					nFrame = FRAME_PER_SEC;
					SET_BLOCK_COLOR(DEFAULT);
					printf("%03d", nFrame);
				}
				else
					break;
			}

			gotoxy(0, 0);
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

	while (_getch() != VK_ESCAPE);
	gotoxy(0, 28);
	return 0;
}
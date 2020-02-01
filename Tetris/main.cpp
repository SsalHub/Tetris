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

	BLOCK block;
	gotoxy(2, 27);
	printf("block.nFrame = ");

	while (1) {	// ���� ��������

		/* �� ���� */
		setBlock(&block);

		/* �� ��� */
		while (block.deltaY) {		// ����� �� �ٴڱ��� ������ ������ �ݺ�
			gotoxy(17, 27);
			printf("%03d", block.nFrame);

			/* Ű���� �Է¹޴� �κ� */

			switch (getKey()) {
			case VK_UP:
				rotateBlock(&block);
				break;
			case VK_DOWN:
				moveBlock(&block, 0, 1);
				block.nFrame = FRAME_PER_SEC;
				break;
			case VK_LEFT:
				for (int i = 0; i < BLOCK_SIZE; i++) {
					if (map[block.blockPoint[i].y][block.blockPoint[i].x - 1] == 1) {
						break;
					}
					if (i == BLOCK_SIZE - 1)
						moveBlock(&block, -1, 0);
				}
				break;
			case VK_RIGHT:
					for (int i = 0; i < BLOCK_SIZE; i++) {
						if (map[block.blockPoint[i].y][block.blockPoint[i].x - 1] == 1) {
							break;
						}
						if (i == BLOCK_SIZE - 1)
							moveBlock(&block, 1, 0);
					}
					break;
			case VK_SPACE:
				moveBlock(&block, 0, block.deltaY);
				block.nFrame = FRAME_PER_SEC;
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

		/* �� ����� ������ map[][]�� 1�� ����*/
		for (int i = 0; i < BLOCK_SIZE; i++)
			map[block.blockPoint[i].y][block.blockPoint[i].x] = 1;


		/* �������� �� �ִ��� �˻� */


	}
	return 0;
}
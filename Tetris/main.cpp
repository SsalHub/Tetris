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
	int key_nFrame[5];

	while (1) {	// ���� ��������

		/* �� ���� */
		setBlock(&block);

		/* �� ��� */
		while (block.deltaY) {		// ����� �� �ٴڱ��� ������ ������ �ݺ�
			gotoxy(17, 27);
			printf("%03d", block.nFrame);

			/* Ű���� �Է¹޴� �κ� */
			
			int anyButton = false; // ��ư�� �ϳ��� ���ȴ���
			if (pressed(VK_UP, &anyButton)) {
				rotateBlock(&block);
			}
			if (pressed(VK_DOWN, &anyButton)) {
				moveBlock(&block, 0, 1);
				block.nFrame = FRAME_PER_SEC;
			}
			if (pressed(VK_LEFT, &anyButton)) {
				if (!isBlocked(&block, -1))
					moveBlock(&block, -1, 0);
			}
			if (pressed(VK_RIGHT, &anyButton)) {
				if (!isBlocked(&block, 1))
					moveBlock(&block, 1, 0);
			}
			if (pressed(VK_SPACE, &anyButton)) {
				moveBlock(&block, 0, block.deltaY);
				block.nFrame = FRAME_PER_SEC;
			}
			pressed(0, &anyButton);

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
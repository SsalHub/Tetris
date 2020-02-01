#include "Tetris.h"
/* ������� ���� ���� �Խù�
	1. https://programfrall.tistory.com/20
	2. https://pang2h.tistory.com/129
*/

bool map[HEIGHT][WIDTH] = { 0 };

typedef enum { KS_UP, KS_DOWN, KS_LEFT, KS_RIGHT, KS_SPACE } KeyState;

int main() {
	srand((unsigned int)time(NULL));
	system("mode con cols=80 lines=30");		// ������Ʈ â ũ�� ����

	setMap();
	printMap();

	int key_nFrame[5] = { 0 };		// Ű�Է� ����(�����¿� + �����̽���)
	int keyState;
	BLOCK block;
	gotoxy(2, 27);
	printf("block.nFrame = ");

	while(1){	// ���� ��������

		/* �� ���� */
		setBlock(&block);

		/* �� ��� */
		while (block.deltaY) {		// ����� �� �ٴڱ��� ������ ������ �ݺ�
			gotoxy(17, 27);
			printf("%03d", block.nFrame);
		
			/* Ű���� �Է¹޴� �κ� */
		
			keyState = GetAsyncKeyState(VK_UP);
			if (keyState & 0x8000) {
				if (key_nFrame[KS_UP] != 1) {
					rotateBlock(&block);
				}
				key_nFrame[KS_UP] = 1;
			}
			else
				key_nFrame[KS_UP] = 0;

			keyState = GetAsyncKeyState(VK_DOWN);
			if (keyState & 0x8000) {
				if (key_nFrame[KS_DOWN] % 5 == 0) {
					moveBlock(&block, 0, 1);
					block.nFrame = FRAME_PER_SEC;
				}
				key_nFrame[KS_DOWN]++;
			}
			else 
				key_nFrame[KS_DOWN] = 0;

			keyState = GetAsyncKeyState(VK_LEFT);
			if (keyState & 0x8000) {
				if (key_nFrame[KS_LEFT] % 10 == 0) {
					for (int i = 0; i < BLOCK_SIZE; i++) {
						if (block.blockPoint[i].x - 1 <= 0) {
							break;
						}
						if (i == BLOCK_SIZE - 1)
							moveBlock(&block, -1, 0);
					}
				}
				key_nFrame[KS_LEFT]++;
			}
			else {
				key_nFrame[KS_LEFT] = 0;
				keyState = GetAsyncKeyState(VK_RIGHT);
				if (keyState & 0x8000) {
					if (key_nFrame[KS_RIGHT] % 10 == 0) {
						for (int i = 0; i < BLOCK_SIZE; i++) {
							if (WIDTH - 1 <= block.blockPoint[i].x + 1) {
								break;
							}
							if(i == BLOCK_SIZE - 1)
								moveBlock(&block, 1, 0);
						}
					}
					key_nFrame[KS_RIGHT]++;
				}
				else
					key_nFrame[KS_RIGHT] = 0;
			}

			keyState = GetAsyncKeyState(VK_SPACE);
			if (keyState & 0x8000) {
				if (key_nFrame[KS_SPACE] != 1) {
					moveBlock(&block, 0, block.deltaY);
				}
				key_nFrame[KS_SPACE] = 1;		// SPACEŰ�� ���� ���¶��, ���� �ѹ��� �����ϰ� �� ������ �̵���
			}
			else
				key_nFrame[KS_SPACE] = 0;		// SPACEŰ�� �������� ���� �ٽ� �ʱⰪ���� ����

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
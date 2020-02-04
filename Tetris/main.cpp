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

	int key_nFrame[5] = { 0 };		// Ű�Է��� �����ϱ� ���� �迭. ���� Ű�� ������ŭ ����(�����¿� + �����̽���) 
	int keyState;
	BLOCK block;
	gotoxy(2, 27);
	printf("block.nFrame = ");

	while(1){	// ���� ��������
		/* map�� �� �׵θ� ��� */
		gotoxy(0, 0);	
		for (int i = 0; i < WIDTH; i++) {
			printf("��");
		}

		/* �� ���� */
		setBlock(&block);

		/* �� ��� */
		while (block.deltaY) {		// ����� �� �ٴڱ��� ������ ������ �ݺ�
			gotoxy(17, 27);
			printf("%03d", block.nFrame);
		
			/* Ű���� �Է¹޴� �κ� */
		
			// �� ����Ű
			keyState = GetAsyncKeyState(VK_UP);		
			if (keyState & 0x8000) {		// �� ����Ű�� �ԷµǾ��ٸ� ~
				if (key_nFrame[KS_UP] != 1) {		// �� ����Ű�� ��� �����ִ� ���°� �ƴ϶��
					rotateBlock(&block);		// �� ȸ��
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
					moveBlock(&block, 0, 1);
					block.nFrame = FRAME_PER_SEC;
				}
				key_nFrame[KS_DOWN]++;		// ���� �ð���ŭ ������ ���� 1�� ����
			}
			else {		// �Ʒ� ����Ű�� �Էµ��� �ʾҴٸ�
				key_nFrame[KS_DOWN] = 0;		// ������ ���� �ٽ� 0���� �ʱ�ȭ
			}

			// �¿� ����Ű�� ���� ���� ��ŭ ���� �̵�
			keyState = GetAsyncKeyState(VK_LEFT);
			if (keyState & 0x8000) {	// ���� ����Ű�� �ԷµǾ��ٸ�
				if (key_nFrame[KS_LEFT] % 16 == 0) {		// 14�����ӿ� �� �� �̵�
					for (int i = 0; i < BLOCK_SIZE; i++) {
						if (map[block.blockPoint[i].y][block.blockPoint[i].x - 1] == 1) {
							break;
						}
						if (i == BLOCK_SIZE - 1)
							moveBlock(&block, -1, 0);
					}
				}
				key_nFrame[KS_LEFT]++;
			}
			else {		// ���� ����Ű�� �Էµ��� �ʾҴٸ�
				key_nFrame[KS_LEFT] = 0;
				keyState = GetAsyncKeyState(VK_RIGHT);
				if (keyState & 0x8000) {		// ������ ����Ű�� �ԷµǾ��ٸ�
					if (key_nFrame[KS_RIGHT] % 16 == 0) {		// 14�����ӿ� �� �� �̵�
						for (int i = 0; i < BLOCK_SIZE; i++) {
							if (map[block.blockPoint[i].y][block.blockPoint[i].x + 1] == 1) {
								break;
							}
							if (i == BLOCK_SIZE - 1)
								moveBlock(&block, 1, 0);
						}
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
					moveBlock(&block, 0, block.deltaY);
				}
				key_nFrame[KS_SPACE] = 1;		// �� ����Ű�� ���� ������� ����. 1 = Ű�� �����ִ� ����
			}
			else
				key_nFrame[KS_SPACE] = 0;		// 0 = Ű�� �������� ���� ����

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
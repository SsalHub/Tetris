#include "Tetris.h"
/* 헤더파일 분할 관련 게시물
	1. https://programfrall.tistory.com/20
	2. https://pang2h.tistory.com/129
*/

bool map[HEIGHT][WIDTH] = { 0 };

typedef enum { KS_UP, KS_DOWN, KS_LEFT, KS_RIGHT, KS_SPACE } KeyState;

int main() {
	srand((unsigned int)time(NULL));
	system("mode con cols=80 lines=30");		// 프롬프트 창 크기 조절

	setMap();
	printMap();

	int key_nFrame[5] = { 0 };		// 키입력 종류(상하좌우 + 스페이스바)
	int keyState;
	BLOCK block;
	gotoxy(2, 27);
	printf("block.nFrame = ");

	while(1){	// 게임 오버까지

		/* 블럭 생성 */
		setBlock(&block);

		/* 블럭 드랍 */
		while (block.deltaY) {		// 블록이 맵 바닥까지 떨어질 때까지 반복
			gotoxy(17, 27);
			printf("%03d", block.nFrame);
		
			/* 키보드 입력받는 부분 */
		
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
				key_nFrame[KS_SPACE] = 1;		// SPACE키가 눌린 상태라면, 최초 한번만 동작하고 뗄 떼까지 미동작
			}
			else
				key_nFrame[KS_SPACE] = 0;		// SPACE키가 떼어졌을 때에 다시 초기값으로 돌림

			/* 키보드 입력받는 부분 끝*/


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

		/* 블럭 드랍이 끝나면 map[][]을 1로 수정*/
		for (int i = 0; i < BLOCK_SIZE; i++) 
			map[block.blockPoint[i].y][block.blockPoint[i].x] = 1;
		

		/* 없어지는 줄 있는지 검사 */


	}
	return 0;
}
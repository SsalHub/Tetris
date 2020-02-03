#include "Tetris.h"
/* 헤더파일 분할 관련 게시물
	1. https://programfrall.tistory.com/20
	2. https://pang2h.tistory.com/129
*/

bool map[HEIGHT][WIDTH] = { 0 };

int main() {
	srand((unsigned int)time(NULL));
	system("mode con cols=80 lines=30");		// 프롬프트 창 크기 조절

	setMap();
	printMap();

	BLOCK block;
	gotoxy(2, 27);
	printf("block.nFrame = ");
	int key_nFrame[5];

	while (1) {	// 게임 오버까지

		/* 블럭 생성 */
		setBlock(&block);

		/* 블럭 드랍 */
		while (block.deltaY) {		// 블록이 맵 바닥까지 떨어질 때까지 반복
			gotoxy(17, 27);
			printf("%03d", block.nFrame);

			/* 키보드 입력받는 부분 */
			
			int anyButton = false; // 버튼이 하나라도 눌렸는지
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
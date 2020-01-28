#include "Tetris.h"

int main() {
	srand((unsigned int)time(NULL));
	system("mode con cols=80 lines=30");		// 프롬프트 창 크기 조절

	int map[HEIGHT][WIDTH] = { 0 };
	setMap(map);
	printMap(map);

	BLOCK block;
	setBlock(&block);
	gotoxy(5, 27);
	printf("block.nFrame = ");
	block.deltaY = getDeltaY(map, &block);

	while (block.deltaY) {		// 블록이 맵 바닥까지 떨어질 때까지 반복
		gotoxy(16, 27);
		printf("%03d", block.nFrame);

		/* 키보드 입력받는 부분 */
		if (_kbhit()) {
			switch (_getch()) {
			case KEY_SPACE:
				moveBlock(map, &block, 0, block.deltaY);
				break;
			case ARROW_KEY_DEFAULT:
				switch (_getch()) {
				case KEY_DOWN:
					moveBlock(map, &block, 0, 1);
					block.nFrame = FRAME_PER_SEC;
					break;
				case KEY_LEFT:
					for (int i = 0; i < BLOCK_SIZE; i++) {
						if (block.blockPoint[i].x - 1 <= 0) {
							goto FAIL;
						}
					}
					moveBlock(map, &block, -1, 0);
					break;
				case KEY_RIGHT:
					for (int i = 0; i < BLOCK_SIZE; i++) {
						if (WIDTH - 1 <= block.blockPoint[i].x + 1) {
							goto FAIL;
						}
					}
					moveBlock(map, &block, 1, 0);
					break;
				case KEY_UP:
					rotateBlock(map, &block);
					break;
				}
			}
		}

	FAIL:;
		/* 키보드 입력받는 부분 끝*/

		if (block.nFrame <= 0) {
			moveBlock(map, &block, 0, 1);
			gotoxy(16, 27);
			block.nFrame = FRAME_PER_SEC;
			printf("%03d", block.nFrame);
		}

		gotoxy(0, 0);
		block.nFrame--;
		Sleep(1000 / FRAME_PER_SEC);
	}

	return 0;
}
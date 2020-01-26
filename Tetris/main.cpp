#include "Tetris.h"
/* 헤더파일 분할 관련 게시물
	1. https://programfrall.tistory.com/20
	2. https://pang2h.tistory.com/129
*/

int main() {
	srand((unsigned int)time(NULL));
	system("mode con cols=80 lines=30");		// 프롬프트 창 크기 조절

	int deltaY = 0;
	int map[HEIGHT][WIDTH] = { 0 };
	setMap(map);
	printMap(map);

	BLOCK block;
	setBlock(&block);
	gotoxy(2, 27);
	printf("block.nFrame = ");

	while (getDeltaY(map, &block)) {		// 블록이 맵 바닥까지 떨어질 때까지 반복
		gotoxy(17, 27);
		printf("%03d", block.nFrame);


		/* 키보드 입력받는 부분 */
		if (_kbhit()) {
			switch (_getch()) {
				case KEY_SPACE :
					deltaY = getDeltaY(map, &block);
					removeBlock(map, &block);
					for (int i = 0; i < BLOCK_SIZE; i++) {
						block.blockPoint[i].y += deltaY;
					}
					putBlock(map, &block);
					break;
				case ARROW_KEY_DEFAULT :
					switch (_getch()) {
						case KEY_DOWN:
							removeBlock(map, &block);
							for (int i = 0; i < BLOCK_SIZE; i++) {
								block.blockPoint[i].y++;
								block.nFrame = FRAME_PER_SEC;
							}
							putBlock(map, &block);
							break;
						case KEY_LEFT :
							removeBlock(map, &block);
							for (int i = 0; i < BLOCK_SIZE; i++) {
								if (block.blockPoint[i].x - 1 <= 0)
									break;
								else if (i == BLOCK_SIZE - 1) {
									for (int i = 0; i < BLOCK_SIZE; i++) {
										block.blockPoint[i].x--;
									}
								}
							}
							putBlock(map, &block);
							break;
						case KEY_RIGHT : 
							removeBlock(map, &block);
							for (int i = 0; i < BLOCK_SIZE; i++) {
								if (WIDTH - 1 <= block.blockPoint[i].x + 1)
									break;
								else if (i == BLOCK_SIZE - 1) {
									for (int i = 0; i < BLOCK_SIZE; i++) {
										block.blockPoint[i].x++;
									}

								}
							}
							putBlock(map, &block);
							break;
						case KEY_UP:
							removeBlock(map, &block);
							rotateBlock(map, &block);
							putBlock(map, &block);
							break;
					}
			}
		}
		/* 키보드 입력받는 부분 끝*/


		if (block.nFrame <= 0) {
			dropBlock(map, &block);
			gotoxy(17, 27);
			block.nFrame = FRAME_PER_SEC;
			printf("%03d", block.nFrame);
		}

		gotoxy(0, 0);
		block.nFrame--;
		Sleep(1000 / FRAME_PER_SEC);
	}

	return 0;
}
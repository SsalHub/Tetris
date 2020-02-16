#include "Tetris.h"
/* 헤더파일 분할 관련 게시물
	1. https://programfrall.tistory.com/20
	2. https://pang2h.tistory.com/129
*/

bool map[HEIGHT][WIDTH] = { 0 };
BlockColor map_color[HEIGHT][WIDTH] = { NONE };

int main() {
	srand((unsigned int)time(NULL));
	system("mode con cols=80 lines=30");		// 프롬프트 창 크기 조절
	setCursorView(false);

	setMap();
	printMap();

	TYPE blockList14];		// 블럭 리스트
	TYPE newBlock;		// 리스트에서 첫번째 값을 뽑아와서 임시로 저장하는 변수
	setBlockList(blockList);

	BLOCK block;
	gotoxy(2, 27);
	printf("block.nFrame = ");
	
	while (1) {	// 게임 오버까지
		/* 블럭 생성 */
		setBlock(&block, blockList);
		
		/* 블럭 리스트 출력*/
		clearBlockList();
		printBlockList(blockList);

		/* 게임 오버 조사 */
		if (block.deltaY == 0) {		// 블럭이 생성되자마자 떨어질 공간이 없는 상태라면
			break;
		}

		/* 블럭 드랍 */
		while (0 < block.deltaY) {		// 블록이 맵 바닥까지 떨어질 때까지 반복
			gotoxy(17, 27);
			SET_BLOCK_COLOR(DEFAULT);
			printf("%03d", block.nFrame);

			/* 키보드 입력받는 부분 */
			
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

			/* 키보드 입력받는 부분 끝*/

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

		/* 블럭 드랍이 끝나면 map[][]을 1로 수정*/
		for (int i = 0; i < BLOCK_SIZE; i++) {
			map[block.blockPoint[i].y][block.blockPoint[i].x] = 1;
			map_color[block.blockPoint[i].y][block.blockPoint[i].x] = (BlockColor)block.blockType;
		}
		/* 없어지는 줄 있는지 검사 */
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
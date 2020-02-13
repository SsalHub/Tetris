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

	TYPE blockList[BLOCK_LIST_LEN];		// 블럭 리스트
	TYPE newBlock;		// 리스트에서 첫번째 값을 뽑아와서 임시로 저장하는 변수
	setBlockList(blockList);

	BLOCK block;
	gotoxy(2, 27);
	printf("block.nFrame = ");
	
	while (1) {	// 게임 오버까지

		/* 블럭 리스트에서 블럭을 받아옴 */
		newBlock = popBlockList(blockList);

		/* 블럭 리스트에 새 블럭 추가 및 출력*/
		addBlockList(blockList);
		clearBlockList();
		printBlockList(blockList);

		/* 블럭 생성 */
		setBlock(&block, newBlock);
		
		/* 블럭 드랍 */
		while (0 < block.deltaY) {		// 블록이 맵 바닥까지 떨어질 때까지 반복
			gotoxy(17, 27);
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
				printf("%03d", block.nFrame);
			}

			gotoxy(0, 0);
			block.nFrame--;
			Sleep(1000 / FRAME_PER_SEC);
		}

		gotoxy(40, 2);
		printf("deltaX = %2d", getDeltaXfromSide(&block));
		//BLOCK_I, BLOCK_O, BLOCK_Z, BLOCK_S, BLOCK_J, BLOCK_L, BLOCK_T
		gotoxy(40, 3);
		switch (block.blockType) {
		case BLOCK_O:
			printf("block type = BLOCK_O");
			break;
		case BLOCK_I:
			printf("block type = BLOCK_I");
			break;
		case BLOCK_Z:
			printf("block type = BLOCK_Z");
			break;
		case BLOCK_S:
			printf("block type = BLOCK_S");
			break;
		case BLOCK_J:
			printf("block type = BLOCK_J");
			break;
		case BLOCK_L:
			printf("block type = BLOCK_L");
			break;
		case BLOCK_T:
			printf("block type = BLOCK_T");
			break;
		}

		/* 블럭 드랍이 끝나면 map[][]을 1로 수정*/
		for (int i = 0; i < BLOCK_SIZE; i++)
			map[block.blockPoint[i].y][block.blockPoint[i].x] = 1;

		/* 없어지는 줄 있는지 검사 */
		clearLine(&block);
		
	}
	return 0;
}
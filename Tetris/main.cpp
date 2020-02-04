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

	int key_nFrame[5] = { 0 };		// 키입력을 제어하기 위한 배열. 사용될 키의 종류만큼 선언(상하좌우 + 스페이스바) 
	int keyState;
	BLOCK block;
	gotoxy(2, 27);
	printf("block.nFrame = ");

	while(1){	// 게임 오버까지
		/* map의 윗 테두리 출력 */
		gotoxy(0, 0);	
		for (int i = 0; i < WIDTH; i++) {
			printf("■");
		}

		/* 블럭 생성 */
		setBlock(&block);

		/* 블럭 드랍 */
		while (block.deltaY) {		// 블록이 맵 바닥까지 떨어질 때까지 반복
			gotoxy(17, 27);
			printf("%03d", block.nFrame);
		
			/* 키보드 입력받는 부분 */
		
			// 윗 방향키
			keyState = GetAsyncKeyState(VK_UP);		
			if (keyState & 0x8000) {		// 윗 방향키가 입력되었다면 ~
				if (key_nFrame[KS_UP] != 1) {		// 윗 방향키가 계속 눌려있는 상태가 아니라면
					rotateBlock(&block);		// 블럭 회전
				}
				key_nFrame[KS_UP] = 1;		// 계속 눌려있는 상태를 뜻하는 1로 값 변경
			}
			else {		// 윗 방향키가 입력되지 않았다면
				key_nFrame[KS_UP] = 0;		// 눌려있지 않은 상태를 뜻하는 0으로 값 변경
			}

			// 아래 방향키는 오래 누른 만큼 블럭이 내려감
			keyState = GetAsyncKeyState(VK_DOWN);		// '아래 방향키가 입력되었는가' 에 대한 값을 받아옴
			if (keyState & 0x8000) {		// 아래 방향키가 입력되었다면 ~
				if (key_nFrame[KS_DOWN] % 5 == 0) {		// 5프레임마다 동작
					moveBlock(&block, 0, 1);
					block.nFrame = FRAME_PER_SEC;
				}
				key_nFrame[KS_DOWN]++;		// 누른 시간만큼 프레임 값을 1씩 더함
			}
			else {		// 아래 방향키가 입력되지 않았다면
				key_nFrame[KS_DOWN] = 0;		// 프레임 값을 다시 0으로 초기화
			}

			// 좌우 방향키는 오래 누른 만큼 블럭이 이동
			keyState = GetAsyncKeyState(VK_LEFT);
			if (keyState & 0x8000) {	// 왼쪽 방향키가 입력되었다면
				if (key_nFrame[KS_LEFT] % 16 == 0) {		// 14프레임에 한 번 이동
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
			else {		// 왼쪽 방향키가 입력되지 않았다면
				key_nFrame[KS_LEFT] = 0;
				keyState = GetAsyncKeyState(VK_RIGHT);
				if (keyState & 0x8000) {		// 오른쪽 방향키가 입력되었다면
					if (key_nFrame[KS_RIGHT] % 16 == 0) {		// 14프레임에 한 번 이동
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
				else {	// 오른쪽 방향키가 입력되지 않았다면
					key_nFrame[KS_RIGHT] = 0;
				}
			}

			// 스페이스바는 오래 누르더라도 최초 입력 시점에만 블럭이 떨어짐
			keyState = GetAsyncKeyState(VK_SPACE);
			if (keyState & 0x8000) {
				if (key_nFrame[KS_SPACE] != 1) {
					moveBlock(&block, 0, block.deltaY);
				}
				key_nFrame[KS_SPACE] = 1;		// 윗 방향키와 같은 방식으로 제어. 1 = 키가 눌려있는 상태
			}
			else
				key_nFrame[KS_SPACE] = 0;		// 0 = 키가 눌려있지 않은 상태

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
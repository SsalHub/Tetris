#pragma once		// 헤더 파일끼리 꼬이지 않도록, 매크로 정의한 후 #ifndef로 중복 검사	-> VS에서만 작동하는 코드
#ifndef _HEADER_TETRIS		// 이미 매크로 정의되어 있는가?
#define _HEADER_TETRIS		// 정의되어 있지 않다면 새로 정의함
/* 참고 링크 : https://pang2h.tistory.com/129 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include <math.h>

#define WIDTH 12					// 맵의 너비
#define HEIGHT 27					// 맵의 높이
#define BLOCK_SIZE 4				// 블록 하나에 포함되는 작은 블록의 갯수(모두 4개)
#define FRAME_PER_SEC 240			// 테트리스의 fps. 초당 240회의 연산을 하도록 고정 (고정 안할 시 초당 n천회 이상)
#define ARROW_KEY_DEFAULT 224		// 방향키를 입력받을 때, 방향키의 ASCII값에 앞서 입력되는 ASCII값
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_SPACE 32
#define KEY_COUNT 5

#define GET_MAX(n1, n2) ((n1) > (n2) ? (n1) : (n2))		// 더 큰 수를 계산하는 매크로 함수
#define GET_MIN(n1, n2) ((n1) < (n2) ? (n1) : (n2))		// 더 작은 수를 계산하는 매크로 함수

typedef enum { BLOCK_I, BLOCK_O, BLOCK_Z, BLOCK_S, BLOCK_J, BLOCK_L, BLOCK_T } TYPE; // 블럭의 종류(7가지)를 열거형으로 정의.

typedef enum { KS_UP, KS_DOWN, KS_LEFT, KS_RIGHT, KS_SPACE } KeyState;		// 각 키가 입력될 때에 동작을 제어하기 위한 고유값들을 열거형으로 정의. KS = KeyState

typedef struct {
	TYPE blockType;
	POINT blockPoint[4];			// 배열의 첫 요소가 기준점.
	int deltaY;						// 바닥으로부터 떨어진 거리.
	short nFrame = 0;				// 떨어지는 속도.
} BLOCK;

void setMap(void);
void printMap(void);
void setPoint(POINT* pPoint, int x, int y);
bool isInsideMap(const POINT* pPoint);
void setBlock(BLOCK* pBlock);
bool pressed(int key);
void getKey(bool*, bool*, bool*, bool*, bool*);
bool isBlocked(const BLOCK* pBlock, int x);
void moveBlockPoint(BLOCK* pBlock, int x, int y);
void moveBlock(BLOCK* pBlock, int x, int y);
void putBlock(BLOCK* pBlock);
void putBlockPrev(BLOCK* pBlock);
void removeBlock(BLOCK* pBlock);
void removeBlockPrev(BLOCK* pBlock);
void rotateBlockPoint(BLOCK* pBlock);
void rotateBlock(BLOCK* pBlock);
int getDeltaY(BLOCK* pBlock);
int getDeltaXfromSide(BLOCK* pBlock);
void gotoxy(int x, int y);

#endif		// #ifndef문 종료
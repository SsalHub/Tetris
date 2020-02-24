#pragma once		// ��� ���ϳ��� ������ �ʵ���, ��ũ�� ������ �� #ifndef�� �ߺ� �˻�	-> VS������ �۵��ϴ� �ڵ�
#ifndef _HEADER_TETRIS		// �̹� ��ũ�� ���ǵǾ� �ִ°�?
#define _HEADER_TETRIS		// ���ǵǾ� ���� �ʴٸ� ���� ������
/* ���� ��ũ : https://pang2h.tistory.com/129 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include <math.h>

#define WIDTH 12					// ���� �ʺ�
#define HEIGHT 27					// ���� ����
#define BLOCK_SIZE 4				// ��� �ϳ��� ���ԵǴ� ���� ����� ����(��� 4��)
#define BLOCK_LIST_LEN 4
#define FRAME_PER_SEC 240			// ��Ʈ������ fps. �ʴ� 240ȸ�� ������ �ϵ��� ���� (���� ���� �� �ʴ� nõȸ �̻�)
#define ARROW_KEY_DEFAULT 224		// ����Ű�� �Է¹��� ��, ����Ű�� ASCII���� �ռ� �ԷµǴ� ASCII��
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_SPACE 32
#define KEY_COUNT 5


#define GET_MAX(n1, n2) ((n1) > (n2) ? (n1) : (n2))		// �� ū ���� ����ϴ� ��ũ�� �Լ�
#define GET_MIN(n1, n2) ((n1) < (n2) ? (n1) : (n2))		// �� ���� ���� ����ϴ� ��ũ�� �Լ�
#define GET_ABS_MAX(n1, n2) ((abs(n1)) > (abs(n2)) ? (n1) : (n2))
#define SET_BLOCK_COLOR(n) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (n))

typedef enum {
	BLOCK_J = 9, BLOCK_S, BLOCK_I, BLOCK_Z, BLOCK_T, BLOCK_O, BLOCK_L
} TYPE; // ���� ����(7����)�� ���������� ����.

typedef enum { NONE = 0, DEFAULT = 7, COLOR_J = 9, COLOR_S, COLOR_I, COLOR_Z, COLOR_T, COLOR_O, COLOR_L } BlockColor;

typedef enum { KS_UP, KS_DOWN, KS_LEFT, KS_RIGHT, KS_SPACE } KeyState;		// �� Ű�� �Էµ� ���� ������ �����ϱ� ���� ���������� ���������� ����. KS = KeyState


typedef struct {
	TYPE blockType;
	POINT blockPoint[4];			// �迭�� ù ��Ұ� ������.
	int deltaY;						// �ٴ����κ��� ������ �Ÿ�.
} BLOCK;

void setMap(void);
void printMap(void);
void setPoint(POINT* pPoint, int x, int y);
void pointCopy(POINT* dst, const POINT* src);
void setBlockList(TYPE* pList);
void addBlockList(TYPE* pList);
TYPE popBlockList(TYPE* pList);
void printBlockList(TYPE* pList);
void clearBlockList();
void setBlock(BLOCK* pBlock, TYPE* pList);
bool pressed(int key);
void getKey(bool*, bool*, bool*, bool*, bool*);
void moveBlockPoint(POINT* pPoint, int x, int y);
void moveBlock(BLOCK* pBlock, int x, int y);
void putBlock(BLOCK* pBlock);
void putBlockPrev(BLOCK* pBlock);
void removeBlock(BLOCK* pBlock);
void removeBlockPrev(BLOCK* pBlock);
void rotateBlockPoint(POINT* pPoint);
void rotateBlock(BLOCK* pBlock);
void clearLine(BLOCK* pBlock);
void resetLine(int line_y);
void dropLine(int line_y, int clear_line_cnt);
bool isCleared(int line_y);
bool sideBlocked(BLOCK* pBlock, int x);
bool isBlocked(const POINT* pPoint);
bool isPointBlocked(const POINT* pPoint);
bool isInsideMap(const POINT* pPoint);
int getDeltaY(BLOCK* pBlock);
int getDeltaXfromSide(BLOCK* pBlock);
int getBlockLowestY(BLOCK* pBlock);
int getBlockHighestY(BLOCK* pBlock);
void gotoxy(int x, int y);
void setCursorView(int bVisible);

#endif		// #ifndef�� ����
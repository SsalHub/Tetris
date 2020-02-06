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

typedef enum { BLOCK_I, BLOCK_O, BLOCK_Z, BLOCK_S, BLOCK_J, BLOCK_L, BLOCK_T } TYPE; // ���� ����(7����)�� ���������� ����.

typedef enum { KS_UP, KS_DOWN, KS_LEFT, KS_RIGHT, KS_SPACE } KeyState;		// �� Ű�� �Էµ� ���� ������ �����ϱ� ���� ���������� ���������� ����. KS = KeyState

typedef struct {
	TYPE blockType;
	POINT blockPoint[4];			// �迭�� ù ��Ұ� ������.
	int deltaY;						// �ٴ����κ��� ������ �Ÿ�.
	short nFrame = 0;				// �������� �ӵ�.
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

#endif		// #ifndef�� ����
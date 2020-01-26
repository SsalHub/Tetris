#pragma once		// ��� ���ϳ��� ������ �ʵ���, ��ũ�� ������ �� #ifndef�� �ߺ� �˻�	-> VS������ �۵��ϴ� �ڵ�
#ifndef _HEADER_TETRIS		// �̹� ��ũ�� ���ǵǾ� �ִ°�?
#define _HEADER_TETRIS		// ���ǵǾ� ���� �ʴٸ� ���� ������
/* ���� ��ũ : https://pang2h.tistory.com/129 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

#define WIDTH 12					// ���� �ʺ�
#define HEIGHT 27					// ���� ����
#define BLOCK_SIZE 4				// ��� �ϳ��� ���ԵǴ� ���� ����� ����(��� 4��)
#define FRAME_PER_SEC 240		// ��Ʈ������ fps. �ʴ� 240ȸ�� ������ �ϵ��� ���� (���� ���� �� �ʴ� nõȸ �̻�)
#define ARROW_KEY_DEFAULT 224		// ����Ű�� �Է¹��� ��, ����Ű�� ASCII���� �ռ� �ԷµǴ� ASCII��
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_SPACE 32

#define GET_MAX(n1, n2) ((n1) > (n2) ? (n1) : (n2))		// �� ū ���� ����ϴ� ��ũ�� �Լ�
#define GET_MIN(n1, n2) ((n1) < (n2) ? (n1) : (n2))		// �� ���� ���� ����ϴ� ��ũ�� �Լ�

typedef enum type { BLOCK_I, BLOCK_O, BLOCK_Z, BLOCK_S, BLOCK_J, BLOCK_L, BLOCK_T } TYPE; // ���� ����(7����)�� ���������� ����.

typedef struct block {
	TYPE blockType;
	POINT blockPoint[4];			// �迭�� ù ��Ұ� ������.
	short nFrame = 0;				// �������� �ӵ�.
} BLOCK;

void setMap(int map[][WIDTH]);
void printMap(const int map[][WIDTH]);
void setBlock(BLOCK* pBlock);
void setPoint(POINT* pPoint, int x, int y);
void removeBlock(const int map[][WIDTH], BLOCK* pBlock);
void dropBlock(const int map[][WIDTH], BLOCK* pBlock);
void rotateBlock(const int map[][WIDTH], BLOCK* pBlock);
void putBlock(const int map[][WIDTH], BLOCK* pBlock);
void putBlockPrev(const int map[][WIDTH], BLOCK* pBlock);
void removeBlockPrev(const int map[][WIDTH], BLOCK* pBlock);
void gotoxy(int x, int y);
int getDeltaY(const int map[][WIDTH], BLOCK* pBlock);
int getDeltaXfromSide(const int map[][WIDTH], BLOCK* pBlock);

#endif		// #ifndef�� ����
/*
*	�t�x�݉ۑ�u��tory�v	j1318springkadai.c
*
*	J1318	Takata
*							����26�N4��10��
*							����26�N4��24��	(VSCOM�ΐ��ʂ̉��P, ���[������2�y�[�W�ڂ����P, �R���s���[�^�̓G�u���v���u���v�ɋ߂Â��悤���P, VSCOM�ΐ탂�[�h�Łu���v�Ɓu���v���d�Ȃ����Ƃ��u���v��������o�O���C��)
*
*	���T�E���h���p��������
*	���v���O�����ꕔ(4�s)�Q�l���l�h�c�h�Đ�	zahyou.6.ql.bz/cgame/mcisendstring.htm
*/


#pragma comment(lib,"winmm")	//winmm.lib�������N

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <mmsystem.h>	//mciSendString()

/*�@�e�L�X�g�̐F(�w�i�F�͑S�ċ�����)�@*/
#define TEXT_BLACK SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN )
#define TEXT_IRED SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN )
#define TEXT_RED SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN )
#define TEXT_IBLUE SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN )
#define TEXT_BLUE SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN )
#define TEXT_IGREEN SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN )
#define TEXT_GREEN SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN | BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN )

/*�@��ʕ\���T�C�Y�@*/
#define YSIZE 15
#define XSIZE 30
/*�@�^�C�g����ʕ\���@*/
#define POINTER 1
#define TITLE 0
/*�@�^�C�g����ʃW�����v�@*/
#define RULE 1
#define STORY 2
#define BATTLE 3
#define EXIT 4
/*�@�ΐ탁�j���[�@*/
#define VSCOM 1
#define VSPLAYER 2
/*�@��ʕ\���@*/
#define EMPTY 0
#define OUTEMPTY 1	//�i���֎~EMPTY
#define BROCK 2
#define MESS 3
#define OSS 4
#define STER 5
#define SSTER 6
#define NEEDLE 7
#define FROOR 8
#define SAVE 9
#define GOAL 10
#define DAMAGE 11
#define CLEAR 12
#define MONSTER 13

int main()
{
	int tx = 0, ty = 0;	//�^�C�g����ʃ|�C���^�[
	int tx_old, ty_old;
	int startnum = 0;
	int bx = 0, by = 0;	//�ΐ탁�j���[�|�C���^�[
	int bx_old, by_old;
	int rulepage = 1;
	int jumptime;	//�W�����v�^�C��
	int ossjumptime, m1jumptime, m2jumptime, m3jumptime;	//�G�W�����v�^�C��
	int froor1x = 8;
	int froor2x = 5;
	int froor3x = 3;
	int froor1check;
	int froor2check;
	int froor3check;
	int froorcount = 0;	//�t���A(������)�̓����^�C�~���O
	int i;
	int stagenum = 1;
	int messx, messy;
	int messx_old, messy_old;
	int ossx, ossy;
	int ossx_old, ossy_old;
	int m1x, m1y;
	int m2x, m2y;
	int m3x, m3y;
	int m1x_old, m1y_old;
	int m2x_old, m2y_old;
	int m3x_old, m3y_old;
	int ossfield_old, m1field_old, m2field_old, m3field_old;	//�G�̈ړ���ɂ��������̂�ۑ�
	int x, y;
	int r1, r2, r3, or;	//����
	int key;
	int whovs;
	int battleend;	//�ΐ풆�~
	int point = 0;
	int savecheck = 0;
	int savex, savey;
	int mlife, olife;
	int getster = 0;
	int allster = 0;
	int finish = 0;	//�Q�[���N���A�������ǂ���
	int title[4][2] = //�^�C�g�����
	{
		{1,0},
		{0,0},
		{0,0},
		{0,0}
	};
	int battlemenu[2][2] = //�ΐ탁�j���[
	{
		{1,0},
		{0,0}
	};
	int vsc_battlestage[YSIZE][XSIZE] = //�ΐ�X�e�[�W(VS�R���s���[�^)
	{
		{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},// ������������������������������������������������������������
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@���������@�@�@�@�@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�����������������@�@�@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�������������������������@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,2},// ���@�@�@�@�@�@���������������������������������@�@�@�@�@�@��
		{2,0,3,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,10,2},//���@���@�@�����������������������������������������@�@������
		{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2} // ������������������������������������������������������������
	};
	int vsp_battlestage[YSIZE][XSIZE] = //�ΐ�X�e�[�W(VS�v���C���[)
	{
		{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},// ������������������������������������������������������������
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
		{2,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,0,2},// ���@�@�����������@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@���������@��
		{2,0,0,2,1,1,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,2},// ���@�@���@�@�@���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�����@��
		{2,0,0,2,1,1,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,2},// ���@�@���@�@�@���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�������@��
		{2,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,0,2,0,2},// ���@�@�����������@�@�@�@�@�@�@�@�@�@�@�@�������������@���@��
		{2,0,0,0,0,2,0,0,0,0,0,0,2,2,2,2,2,2,0,0,2,1,1,1,2,0,0,0,0,2},// ���@�@�@�@���@�@�@�@�@�@�������������@�@���@�@�@���@�@�@�@��
		{2,0,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,2,1,1,1,2,0,0,0,0,2},// ���@���������������@�@�@�@�@�@�@�@�@�@�@���@�@�@���@�@�@�@��
		{2,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1,1,1,2,0,0,0,0,2},// ���@�@�@�@���@�@�@�@�@�@�@�@�@�@�@�@�@�@���@�@�@���@�@�@�@��
		{2,0,0,0,0,2,0,0,0,0,0,2,2,0,0,0,0,2,2,0,2,2,2,2,2,0,0,0,0,2},// ���@�@�@�@���@�@�@�@�@�����@�@�@�@�����@�����������@�@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,0,0,2,2,2,2,2,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�����������@�@�����������@�@�@�@�@�@�@�@��
		{2,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,2},//���@���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@������
		{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2} // ������������������������������������������������������������
	};
	int stage1[YSIZE][XSIZE] = //�X�e�[�W1
	{
		{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},// ������������������������������������������������������������
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,9,0,0,5,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@���@�@�@���@�@���@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,2,2,2,0,2,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�������@�������@���@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@���@���@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@���@�@�@�@�@�@�@���@���@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,5,0,2,2,2,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@���@�������@�@�@�@�@�@���@���@�@�@�@�@�@�@�@��
		{2,0,3,0,5,0,2,2,2,2,2,2,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,10,0,2},//���@���@���@�������������@�@�@�@�@�@���@���@�@�@�@�@�@���@��
		{2,2,2,0,2,2,2,2,2,2,2,2,0,0,5,0,0,0,2,0,2,2,2,2,2,7,2,2,2,2},// �������@�����������������@�@���v�@�@���@��������������������
		{2,2,2,7,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,7,2,0,0,0,0,0,0,0,0,2} // �������ȁ������������������������������ȁ��@�@�@�@�@�@�@�@��
	};
	int stage2[YSIZE][XSIZE] = //�X�e�[�W2
	{
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,0,0,5,0,0,0,5,0,0,0,5,0,0,0,0,0,0,1,0,0,0,0,2},// ���@�@�@�@�@�@�@�@���@�@�@���@�@�@���@�@�@�@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
		{2,6,0,0,0,0,0,5,0,0,0,5,0,0,0,5,0,0,0,5,0,0,0,0,1,0,0,0,10,2},//�����v�@�@�@�@���@�@�@���@�@�@���@�@�@���@�@�@�v�@�@�@�@����
		{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,2,2,2,2},// ���������������������������������������������������@��������
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@���@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@���@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@���@�@�@�@��
		{2,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,2},// ���@�@�@�@�@���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@���@�@�@�@��
		{2,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,2},// ���@�@�@�@�@���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@���@�@�@�@�@��
		{2,0,0,0,2,0,2,8,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,2},// ���@�@�@���@���������@�@�@�@�@�@�@�@�@�@�@�@�@���@�@�@�@�@��
		{2,0,0,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ���@�@�@���@���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
		{2,3,2,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,0,2},// �������@���@���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@���@�@�@�@�@�@��
		{2,2,2,7,2,7,2,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,2,2,2,2,2,2,2,2}//  �������ȁ��ȁ��ȁȁȁȁȁȁȁȁȁȁȁȁȁȁȁ���������������
	};
	int stage3[YSIZE][XSIZE] = //�X�e�[�W3
	{
		{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},// ������������������������������������������������������������
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,2},//���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@����
		{2,0,8,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2},// ���@�������@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@����������
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@���@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�����@�@��
		{2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,2},// �����@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@���@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,2,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@���@�@����
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,6,0,0,2},// ���@���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�����@�@��
		{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,2},// ���������������������������������������������������������@��
		{2,0,5,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,2},// ���@���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@���@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2},// ���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@����
		{2,0,3,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ���@���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
		{2,2,2,2,8,8,8,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ���������������@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
		{2,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ���v�v�v�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��
		{2,2,2,2,2,2,2,2,2,2,2,2,2,2,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,2}//  �����������������������������ȁȁȁȁȁȁȁȁȁȁȁȁȁȁȁ�
	};

	/*�@BGM�t�@�C���I�[�v���@*/
	mciSendString("open titlebgm.mp3 alias title", NULL, 0, NULL);		//�^�C�g�����
	mciSendString("open rulebgm.mp3 alias rule", NULL, 0, NULL);		//���[������
	mciSendString("open battlemenubgm.mp3 alias bmenu", NULL, 0, NULL);	//�ΐ탁�j���[
	mciSendString("open battlebgm.mp3 alias battle", NULL, 0, NULL);	//�ΐ�
	mciSendString("open story1bgm.mp3 alias story1", NULL, 0, NULL);		//�X�g�[���[1
	mciSendString("open story2bgm.mp3 alias story2", NULL, 0, NULL);		//�X�g�[���[2
	mciSendString("open story3bgm.mp3 alias story3", NULL, 0, NULL);		//�X�g�[���[3

	/*�@�ő僋�[�v(�^�C�g���ɖ߂�, ���[�������̎��̃y�[�W��\������)�@*/
	while( startnum == TITLE || startnum == RULE )
	{
		//=================================================================================================
		//	�^�C�g�����
		//=================================================================================================
		if( startnum == TITLE )
		{
			while(1)
			{
				/*�@���Q�l��BGM���[�v�Đ��@*/
				TCHAR BGMStatus[256] = {0};
				mciSendString("status title mode", BGMStatus, 256, NULL); //BGM��Ԃ̎擾
				if(lstrcmp(BGMStatus, "stopped") == 0)
					mciSendString("play title from 0", NULL, 0, NULL); //BGM��~���Ȃ�Đ�

				/*�@�^�C�g����ʁE�L�[���́@*/
				if( kbhit() )
				{
					key = getch();

					tx_old = tx, ty_old = ty;

					switch( key )
					{
					case '2':
						PlaySound("menusound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
						ty++;
						break;
					case '8':
						PlaySound("menusound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
						ty--;
						break;
					case ' ':
						PlaySound("decidesound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
						mciSendString("stop title", NULL, 0, NULL);	//BGM��~
						tx++;
						break;
					}
					ty = ( ty + 4 ) % 4;
					title[ty_old][tx_old] = EMPTY;
				}
				title[ty][tx] = POINTER;

				/*�@�^�C�g����ʁE��ʕ\���@*/
				TEXT_IRED;
				printf("\t\t������\n");
				printf("\t\t���@��\t�@�@���X�g�[���[\n");
				printf("\t\t������");TEXT_RED; printf("���������������������@��\n");
				TEXT_IRED;
				printf("\t\t������");TEXT_RED; printf("�@���@���@��������������\n");
				TEXT_IRED;
				printf("\t\t�@���@");TEXT_RED; printf("�@���@�������������@���@\n");

				TEXT_BLACK;
				printf("\t\t\t\t\tj1318 Takata Ryosuke\n\n\n");
				for( y = 0; y < 4; y++ )
				{
					printf("\t\t\t");
					for( x = 0; x < 2; x++ )
					{
						switch( title[y][x] )
						{
						case POINTER:
							TEXT_BLACK;
							printf("��");
							break;
						default:
							printf("�@");
							break;
						}

						if( y == 0 && x == 1 )
						{
							TEXT_GREEN;
							printf("���[��");
						}
						if( y == 1 && x == 1 )
						{
							TEXT_RED;
							printf("�ΐ�");
						}
						if( y == 2 && x == 1 )
						{
							TEXT_BLUE;
							printf("�X�g�[���[");
						}
						if( y == 3 && x == 1 )
						{
							TEXT_BLACK;
							printf("�I��");
						}
					}
					printf("\n");
				}
				printf("\n\n");
				printf("\t\t��(8)��(2)�F�I���@�@�X�y�[�X�L�[�F����\n");

				/*�@�y�[�W�W�����v�����@*/
				if( title[0][1] == POINTER )	//���[������
				{
					startnum = RULE;
					Sleep( 100 );
					system("cls");
					break;
				}
				else if( title[1][1] == POINTER )	//�ΐ�
				{
					startnum = BATTLE;
					Sleep( 100 );
					system("cls");
					break;
				}
				else if( title[2][1] == POINTER )	//�X�g�[���[
				{
					startnum = STORY;
					Sleep( 100 );
					system("cls");
					break;
				}
				else if( title[3][1] == POINTER )	//�I��
				{
					startnum = EXIT;
					Sleep( 200 );
					system("cls");
					break;
				}
				Sleep( 100 );
				system("cls");
			}
			title[ty][tx] = EMPTY;
			tx = 0; ty = 0;
		}

		//=================================================================================================
		//	���[������
		//=================================================================================================
		if( startnum == RULE )
		{
			/*�@���Q�l��BGM�Đ��@*/
			TCHAR BGMStatus[256] = {0};
			mciSendString("status rule mode", BGMStatus, 256, NULL); //BGM��Ԃ̎擾
			if(lstrcmp(BGMStatus, "stopped") == 0)
				mciSendString("play rule from 0", NULL, 0, NULL); //BGM��~���Ȃ�Đ�

			TEXT_IRED;
			printf("\t\t������\n");
			printf("\t\t���@��\t�@�@���X�g�[���[\n");
			printf("\t\t������");TEXT_RED;printf("���������������������@��");TEXT_GREEN;printf("�@�@���@�������@\n");
			TEXT_IRED;
			printf("\t\t������");TEXT_RED;printf("�@���@���@��������������");TEXT_GREEN;printf("�@��������������\n");
			TEXT_IRED;
			printf("\t\t�@���@");TEXT_RED;printf("�@���@�������������@���@");TEXT_GREEN;printf("�@�������������@\n\n");

			if( rulepage == 1 )
			{
				TEXT_RED;
				printf("<<�ΐ탂�[�h>>\n");
				TEXT_IRED;printf("��");TEXT_RED;printf("��");TEXT_IBLUE;printf("��");TEXT_RED;printf("�Ƃ��키���[�h�ł��B\n");
				printf("�EVS�R���s���[�^\n");
				printf("�@�@���Ȃ��ƃR���s���[�^�ƂŐ킢�܂��B\n");
				printf("�@�@���Ȃ����R���s���[�^���́���3��Q�b�g�����炠�Ȃ��̏����ł��B\n");
				printf("�@�@");TEXT_IBLUE;printf("��");TEXT_RED;printf("��3�񓖂������炠�Ȃ��̕����ł��B\n");
				printf("�EVS�v���C���[\n");
				printf("�@�@���Ȃ��Ƃ�����l�̑���ƂŐ킢�܂��B\n");
				TEXT_IBLUE;printf("�@�@��");TEXT_RED;printf("�́���3��Q�b�g������");TEXT_IRED;printf("��");TEXT_RED;printf("�̏����ł��B\n");
				TEXT_IBLUE;printf("�@�@��");TEXT_RED;printf("��3�񓖂�������");TEXT_IRED;printf("��");TEXT_RED;printf("�̕����ł��B\n");
				printf("������@�͑ΐ탂�[�h�Ŋm�F���Ă��������B\n\n");
				TEXT_GREEN;
				printf("*PRESS SPEAS KEY*\n");
				if( getch() == ' ' )	//rulepage�͕ς�邪startnum��RULE(1)�̂܂܂Ȃ̂ŁA���[�v���Ė߂��Ă���
				{
					system("cls");
					rulepage = 2;
				}
				else	//startnum��RULE(1)�̂܂܂Ȃ̂ŁA���[�v���Ė߂��Ă���
					system("cls");
			}
			else if( rulepage == 2 )
			{
				TEXT_BLUE;
				printf("<<�X�g�[���[���[�h>>\n");
				TEXT_IRED;printf("��");TEXT_BLUE;printf("����l���ƂȂ��Ė`�����郂�[�h�ł��B\n");
				printf("�E������@\n");
				printf("�@�@�ړ��F��(4)�E��(6)\n");
				printf("�@�@�W�����v�F��(8)\n");
				printf("�E�Z�[�u�|�C���g\n");
				TEXT_IGREEN;printf("�@�@��");TEXT_BLUE;printf("���Q�b�g�����\n�@�@�_���[�W���󂯂����ɂ��̒n�_�ɕ������܂��B\n");
				printf("�E�X�e�[�W�N���A����\n");
				TEXT_RED;printf("�@�@��");TEXT_BLUE;printf("���Q�b�g����Ǝ��̃X�e�[�W�ɐi�݂܂��B\n");
				printf("�E�Q�[���I�[�o�[����\n");
				printf("�@�@�c�胉�C�t��0�ɂȂ�ƃQ�[���I�[�o�[�ł��B\n");
				printf("�E���_�\\n");
				printf("�@�@��������������\n");
				printf("�@�@��");TEXT_GREEN;printf("��");TEXT_BLUE;printf("���@�P�O��\n");
				printf("�@�@��");TEXT_GREEN;printf("��");TEXT_BLUE;printf("���P�O�O��\n");
				printf("�@�@��������������\n");
				TEXT_GREEN;
				printf("*PRESS SPEAS KEY*\n");
				if( getch() == ' ' )
				{
					PlaySound("backsound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
					mciSendString("stop rule", NULL, 0, NULL);	//BGM��~
					system("cls");
					rulepage = 1;
					startnum = TITLE;
				}
				else	//startnum��RULE(1)�̂܂܂Ȃ̂ŁA���[�v���Ė߂��Ă���
					system("cls");
			}
		}

		//=================================================================================================
		//	�ΐ탂�[�h
		//=================================================================================================
		if( startnum == BATTLE )
		{
			/*�@�ΐ탁�j���[�@*/
			while(1)
			{
				/*�@���Q�l��BGM���[�v�Đ��@*/
				TCHAR BGMStatus[256] = {0};
				mciSendString(TEXT("status bmenu mode"), BGMStatus, 256, NULL); // ��Ԃ̎擾
				if(lstrcmp(BGMStatus, TEXT("stopped")) == 0)
					mciSendString(TEXT("play bmenu from 0"), NULL, 0, NULL); // ��~���Ȃ牉�t

				/*�@�ΐ탁�j���[�E�L�[���́@*/
				if( kbhit() )
				{
					key = getch();

					bx_old = bx, by_old = by;

					switch( key )
					{
					case '2':
						PlaySound("menusound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
						by++;
						break;
					case '8':
						PlaySound("menusound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
						by--;
						break;
					case ' ':
						PlaySound("decidesound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
						bx++;
						break;
					}
					by = ( by + 2 ) % 2;
					battlemenu[by_old][bx_old] = EMPTY;
				}
				battlemenu[by][bx] = POINTER;

				/*�@�ΐ탁�j���[�E��ʕ\���@*/
				TEXT_RED;
				printf("\t����������������\n");
				printf("\t���ΐ탁�j���[��\n");
				printf("\t����������������\n\n");
				for( y = 0; y < 2; y++ )
				{
					printf("\t");
					for( x = 0; x < 2; x++ )
					{
						switch( battlemenu[y][x] )
						{
						case POINTER:
							TEXT_BLACK;
							printf("��");
							break;
						default:
							printf("�@");
							break;
						}
						TEXT_RED;
						if( y == 0 && x == 1 )
							printf("VS�R���s���[�^");
						if( y == 1 && x == 1 )
							printf("VS�v���C���[");
					}
					printf("\n");
				}
				printf("\n\n");
				TEXT_BLACK;
				printf("\t\t��(8)��(2)�F�I���@�@�X�y�[�X�L�[�F����\n");


				if( battlemenu[0][1] == POINTER )		//VS�R���s���[�^
				{
					whovs = VSCOM;
					Sleep( 100 );
					system("cls");
					battlemenu[by][bx] = EMPTY;
					bx = 0; by = 0;

					break;
				}
				else if( battlemenu[1][1] == POINTER )	//VS�v���C���[
				{
					whovs = VSPLAYER;
					Sleep( 100 );
					system("cls");
					battlemenu[by][bx] = EMPTY;
					bx = 0; by = 0;
					break;
				}
				Sleep( 100 );
				system("cls");
			}

			for( i = 10; i > 0; i-- )
			{
				if( whovs == VSCOM )	/*----------VSCOM----------*/
				{
					TEXT_RED;
					printf("\t\t������������������������������\n");
					printf("\t\t���ΐ탂�[�h�@VS�R���s���[�^��\n");
					printf("\t\t������������������������������\n\n");
					printf("\t\t���Ȃ���\t�R���s���[�^��\n");
					TEXT_IRED;
					printf("\t\t�@�@�@��");TEXT_IBLUE;printf("\t�@�@�@�@�@�@��\n\n\n");
					TEXT_RED;
					printf("\t������@\n");
					printf("\t��������\n");
					TEXT_BLACK;
					printf("\t�ړ��F��(4),��(6)\n");
					printf("\t�W�����v�F��(8)\n\n");
				}
				else if( whovs == VSPLAYER )	/*----------VSPLAYER----------*/
				{
					TEXT_RED;
					printf("\t\t����������������������������\n");
					printf("\t\t���ΐ탂�[�h�@VS�v���C���[��\n");
					printf("\t\t����������������������������\n\n");
					printf("\t\t�P�o��\t�Q�o��\n");
					TEXT_IRED;
					printf("\t\t�@�@��");TEXT_IBLUE;printf("\t�@�@��\n\n\n");
					TEXT_RED;
					printf("\t������@\n");
					printf("\t��������\n");
					TEXT_BLACK;
					printf("\t���P�o��\n");
					printf("\t�ړ��F��(4),��(6)\n");
					printf("\t�W�����v�F��(8)\n");
					printf("\t���Q�o��\n");
					printf("\t�ړ��F�`,�c\n");
					printf("\t�W�����v�F�v\n\n");
				}
				printf("\t���f�FEsc�L�[\n\n\n");
				printf("\t\t\t\t�J�n�܂ł���%d�b\n",i);
				Sleep( 1000 );
				system("cls");
			}
			mciSendString("stop bmenu", NULL, 0, NULL);	//BGM��~

			/*�@�ΐ�@*/

			/*�@���������@*/
			messx = 2, messy = 13;	//�������z�u
			mlife = 3;
			ossx = 27, ossy = 13;	//�������z�u
			olife = 3;
			battleend = 0;			//���f�p�ϐ��̏�����
			jumptime = 0;			//�W�����v�^�C��������
			ossjumptime = 0;

			while(1)
			{
				/*�@���Q�l��BGM���[�v�Đ��@*/
				TCHAR BGMStatus[256] = {0};
				mciSendString("status battle mode", BGMStatus, 256, NULL); //BGM��Ԃ̎擾
				if(lstrcmp(BGMStatus, "stopped") == 0)
					mciSendString("play battle from 0", NULL, 0, NULL); //BGM��~���Ȃ牉�t

				messx_old = messx;
				messy_old = messy;

				ossx_old = ossx;
				ossy_old = ossy;

				/*�@���ړ��@*/
				if( kbhit() )
				{

					key = getch();

					switch( key )
					{
					case '6':
						messx++;
						break;
					case '4':
						messx--;
						break;
					case '8':
						if( whovs == VSCOM )	/*----------VSCOM----------*/
						{
							if( vsc_battlestage[messy+1][messx] == BROCK )
								jumptime = 7;	//�W�����v�^�C��������
						}
						else if( whovs == VSPLAYER )	/*----------VSPLAYER----------*/
						{
							if( vsp_battlestage[messy+1][messx] == BROCK )
								jumptime = 7;
						}
						break;
						/*�@���ړ��@*/	/*----------VSPLAYER----------*/
					case 'd':
						if( whovs == VSPLAYER )
							ossx++;
						break;
					case 'a':
						if( whovs == VSPLAYER )
							ossx--;
						break;
					case 'w':
						if( whovs == VSPLAYER )
						{
							if( vsp_battlestage[ossy+1][ossx] == BROCK )
								ossjumptime = 7;
						}
						break;
					case 0x1b:
						battleend = 1;	//���f(1)
						break;
					}
				}
				if( whovs == VSCOM )	/*----------VSCOM----------*/
				{
					or = rand() % 6;

					switch( or )
					{
					case 0:
						ossx++;
						break;
					case 1:
						ossx--;
						break;
					case 2:
						if( vsc_battlestage[ossy+1][ossx] == BROCK )
							ossjumptime = 7;
						break;
					case 3:
					case 4:
					case 5:	//1/2�̊m���Ł��ɋ߂Â�
						if( messx < ossx )
							ossx--;
						else if( messx > ossx )
							ossx++;
					}
				}

				/*�@�W�����v�����@*/
				if( jumptime > 3 )	//��
				{
					messy--;	//�����p
					jumptime--;
				}
				if( ossjumptime > 3 )	//��
				{
					ossy--;	//�����p
					ossjumptime--;
				}

				if( whovs == VSCOM )	/*----------VSCOM----------*/
				{
					/*�@�W�����v�����@*/
					if( vsc_battlestage[messy+1][messx] != BROCK && jumptime <= 3 )
					{
						messy++;	//�d�͍�p
						jumptime--;
					}
					if( vsc_battlestage[ossy+1][ossx] != BROCK && ossjumptime <= 3 )
					{
						ossy++;	//�d�͍�p
						ossjumptime--;
					}

					/*�@�ړ������@*/
					vsc_battlestage[messy_old][messx_old] = EMPTY;

					switch( vsc_battlestage[messy][messx] )	//���Ǐ���
					{
					case BROCK:
						messx = messx_old;
						messy = messy_old;
						break;
					}
					switch( vsc_battlestage[ossy][ossx] )	//���Ǐ���
					{
					case BROCK:
						ossx = ossx_old;
						ossy = ossy_old;
						break;
					}

					ossfield_old = vsc_battlestage[ossy][ossx];	//���̈ړ���ɉ�������������ۑ�

					/*�@����@*/
					if( vsc_battlestage[messy][messx] == GOAL )
					{
						PlaySound("goalsound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
						vsc_battlestage[messy][messx] = CLEAR;
						olife--;
					}
					else if( messx == ossx && messy == ossy )
					{
						PlaySound("damagesound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
						vsc_battlestage[messy][messx] = DAMAGE;
						mlife--;
					}
					else
					{
						vsc_battlestage[messy][messx] = MESS;
						vsc_battlestage[ossy][ossx] = OSS;
					}
				}
				else if( whovs == VSPLAYER )	/*----------VSPLAYER----------*/
				{
					/*�@�W�����v�����@*/
					if( vsp_battlestage[messy+1][messx] != BROCK && jumptime <= 3 )	//�d�͍�p
					{
						messy++;	//�d�͍�p
						jumptime--;
					}
					if( vsp_battlestage[ossy+1][ossx] != BROCK && ossjumptime <= 3 )	//�d�͍�p
					{
						ossy++;	//�d�͍�p
						ossjumptime--;
					}

					/*�@�ړ������@*/
					vsp_battlestage[messy_old][messx_old] = EMPTY;

					switch( vsp_battlestage[messy][messx] )	//���Ǐ���
					{
					case BROCK:
					case OUTEMPTY:
						messx = messx_old;
						messy = messy_old;
						break;
					}
					switch( vsp_battlestage[ossy][ossx] )	//���Ǐ���
					{
					case BROCK:
					case OUTEMPTY:
						ossx = ossx_old;
						ossy = ossy_old;
						break;
					}

					ossfield_old = vsp_battlestage[ossy][ossx];	//���̈ړ���ɉ�������������ۑ�

					/*�@����@*/
					if( vsp_battlestage[messy][messx] == GOAL )
					{
						PlaySound("goalsound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
						vsp_battlestage[messy][messx] = CLEAR;
						olife--;
					}
					else if( messx == ossx && messy == ossy )
					{
						PlaySound("damagesound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
						vsp_battlestage[messy][messx] = DAMAGE;
						mlife--;
					}
					else
					{
						vsp_battlestage[messy][messx] = MESS;
						vsp_battlestage[ossy][ossx] = OSS;
					}
				}

				/*�@��ʕ\���@*/
				printf("��������������������������\n");
				printf("��");TEXT_IRED;printf("��");TEXT_BLACK;printf("LIFE�F%2d��", mlife);TEXT_IBLUE;printf("��");TEXT_BLACK;printf("LIFE�F%2d��\n", olife);
				printf("��������������������������\n");
				for( y = 0; y < YSIZE; y++ )
				{
					for( x = 0; x < XSIZE; x++ )
					{
						if( whovs == VSCOM )	/*----------VSCOM----------*/
						{
							switch( vsc_battlestage[y][x] )
							{
							case EMPTY:
							case OUTEMPTY:
								printf("�@");
								break;
							case BROCK:
								TEXT_BLACK;
								printf("��");
								break;
							case MESS:
								TEXT_IRED;
								printf("��");
								break;
							case OSS:
								TEXT_IBLUE;
								printf("��");
								break;
							case GOAL:
								TEXT_RED;
								printf("��");
								break;
							case DAMAGE:
								TEXT_IRED;
								printf("�w");
								Sleep( 300 );
								vsc_battlestage[messy][messx] = EMPTY;
								messx = 2, messy = 13;
								ossx = 27, ossy = 13;
								break;
							case CLEAR:
								TEXT_IRED;
								printf("��");
								Sleep( 300 );
								vsc_battlestage[messy][messx] = GOAL;
								messx = 2, messy = 13;
								ossx = 27, ossy = 13;
								break;
							}
						}
						else if( whovs == VSPLAYER )	/*----------VSPLAYER----------*/
						{
							switch( vsp_battlestage[y][x] )
							{
							case EMPTY:
							case OUTEMPTY:
								printf("�@");
								break;
							case BROCK:
								TEXT_BLACK;
								printf("��");
								break;
							case MESS:
								TEXT_IRED;
								printf("��");
								break;
							case OSS:
								TEXT_IBLUE;
								printf("��");
								break;
							case GOAL:
								TEXT_RED;
								printf("��");
								break;
							case DAMAGE:
								TEXT_IRED;
								printf("�w");
								Sleep( 300 );
								vsp_battlestage[messy][messx] = EMPTY;
								messx = 2, messy = 13;
								ossx = 27, ossy = 13;
								break;
							case CLEAR:
								TEXT_IRED;
								printf("��");
								Sleep( 300 );
								vsp_battlestage[messy][messx] = GOAL;
								messx = 2, messy = 13;
								ossx = 27, ossy = 13;
								break;
							}
						}
					}
					printf("\n");
				}

				/*�@�G�������ꏊ�ɂ��������̂�߂��@*/
				if( whovs == VSCOM )
					vsc_battlestage[ossy][ossx] = ossfield_old;
				else if( whovs == VSPLAYER )
					vsp_battlestage[ossy][ossx] = ossfield_old;

				/*�@�I���@*/
				if( mlife == 0 || olife == 0 || battleend )	//��,���̃��C�t��0�ɂȂ邩���f(1)���ꂽ�ꍇ�Ɏ��s
				{
					mciSendString("stop battle", NULL, 0, NULL);	//BGM��~

					PlaySound("battlefinishsound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�

					for( i = 0; i < 3; i++ )	//�_��
					{
						system("cls");
						Sleep( 500 );
						TEXT_GREEN;
						printf("�������@���@�������@���@���������@���@��\n");
						printf("�����@�@���@�������@���@�������������@��\n");
						printf("���@�@�@���@�������@���@���������@���@��\n");
						Sleep( 500 );
					}

					if( mlife == 0 )
					{
						printf("WINNER:");TEXT_IBLUE;printf("��\n");
						Sleep( 1000 );
					}
					else if( olife == 0 )
					{
						printf("WINNER:");TEXT_IRED;printf("��\n");
						Sleep( 1000 );
					}


					if( whovs == VSCOM )	/*----------VSCOM----------*/
					{
						vsp_battlestage[messy][messx] = EMPTY;	//����������W��EMPTY(0)�ɂ���
						vsp_battlestage[ossy][ossx] = EMPTY;	//����������W��EMPTY(0)�ɂ���
					}
					else if( whovs == VSPLAYER )	/*----------VSPLAYER----------*/
					{
						vsp_battlestage[messy][messx] = EMPTY;	//����������W��EMPTY(0)�ɂ���
						vsp_battlestage[ossy][ossx] = EMPTY;	//����������W��EMPTY(0)�ɂ���
					}

					startnum = TITLE;

					break;
				}

				Sleep( 100 );
				system("cls");

			}
		}
	}

	//=================================================================================================
	//	�X�g�[���[
	//=================================================================================================
	while( startnum == STORY )
	{
		/*�@���̐������グ�@*/
		for( y = 0; y < YSIZE; y++ )
		{
			for( x = 0; x < XSIZE; x++ )
			{
				if( stagenum == 1 )	/*----------STAGE1----------*/
				{
					if( stage1[y][x] == STER || stage1[y][x] == SSTER )
						allster++;
				}
				else if( stagenum == 2 )	/*----------STAGE2----------*/
				{
					if( stage2[y][x] == STER || stage2[y][x] == SSTER )
						allster++;
				}
				else if( stagenum == 3 )	/*----------STAGE3----------*/
				{
					if( stage3[y][x] == STER || stage3[y][x] == SSTER )
						allster++;
				}
			}
		}

		/*�@�����������@*/
		jumptime = 0;	//�W�����v�^�C��������
		m1jumptime = 0;
		m2jumptime = 0;
		m3jumptime = 0;
		savecheck = 0;	//�Z�[�u������
		if( stagenum == 1 )	//���C�t������
			mlife = 5;

		/*�@�����z�u�@*/
		if( stagenum == 1 )	/*----------STAGE1----------*/
		{
			messx = 2, messy = 12;
			m1x = 15, m1y = 13;
		}
		else if( stagenum == 2 )	/*----------STAGE2----------*/
		{
			messx = 1, messy = 13;
			m1x = 2, m1y = 3;
			m2x = 23, m2y = 3;
		}
		else if( stagenum == 3 )	/*----------STAGE3----------*/
		{
			messx = 2, messy = 11;
			ossx = 2, ossy = 7;
			m1x = 1, m1y = 13;
			m2x = 2, m2y = 13;
			m3x = 3, m3y = 13;
		}

		while(1)
		{
			/*�@���Q�l��BGM���[�v�Đ��@*/
			if( stagenum == 1 )
			{
				TCHAR BGMStatus[256] = {0};
				mciSendString("status story1 mode", BGMStatus, 256, NULL); //BGM��Ԃ̎擾
				if(lstrcmp(BGMStatus, "stopped") == 0)
					mciSendString("play story1 from 0", NULL, 0, NULL); //BGM��~���Ȃ牉�t
			}
			else if( stagenum == 2 )
			{
				TCHAR BGMStatus[256] = {0};
				mciSendString("status story2 mode", BGMStatus, 256, NULL); //BGM��Ԃ̎擾
				if(lstrcmp(BGMStatus, "stopped") == 0)
					mciSendString("play story2 from 0", NULL, 0, NULL); //BGM��~���Ȃ牉�t
			}
			else if( stagenum == 3 )
			{
				TCHAR BGMStatus[256] = {0};
				mciSendString("status story3 mode", BGMStatus, 256, NULL); //BGM��Ԃ̎擾
				if(lstrcmp(BGMStatus, "stopped") == 0)
					mciSendString("play story3 from 0", NULL, 0, NULL); //BGM��~���Ȃ牉�t
			}

			/*�@���ړ��@*/
			messx_old = messx;
			messy_old = messy;

			if( kbhit() )
			{
				key =getch();
				switch( key )
				{
				case '6':
					messx++;
					break;
				case '4':
					messx--;
					break;
				case '8':
					if( stagenum == 1 )	/*----------STAGE1----------*/
					{
						if( stage1[messy+1][messx] == BROCK )
							jumptime = 7;
					}
					else if( stagenum == 2 )	/*----------STAGE2----------*/
					{
						if( stage2[messy+1][messx] == BROCK || stage2[messy+1][messx] == FROOR )
							jumptime = 7;
					}
					else if( stagenum == 3 )	/*----------STAGE3----------*/
					{
						if( stage3[messy+1][messx] == BROCK || stage3[messy+1][messx] == FROOR )
							jumptime = 7;
					}
					break;
				}
			}
			/*�@�G�ړ��@*/
			switch( stagenum )
			{
			case 3:	/*----------STAGE3----------*/
				ossx_old = ossx;
				ossy_old = ossy;
				m3x_old = m3x;
				m3y_old = m3y;
				or = rand() % 4;
				switch( or )
				{
				case 0:
					ossx++;
					break;
				case 1:
					ossx--;
					break;
				case 2:
					if( stage3[ossy+1][ossx] == BROCK )
						ossjumptime = 7;
					break;
				case 3:	//1/4�̊m���Ł��ɋ߂Â�
					if( messx < ossx )
						ossx--;
					else if( messx > ossx )
						ossx++;
				}
				r3 = rand() % 3;
				switch( r3 )	//�G3
				{
				case 0:
					m3x++;
					break;
				case 1:
					m3x--;
					break;
				case 2:
					if( stage3[m3y+1][m3x] == BROCK )
						m3jumptime = 5;
					break;
				}
			case 2:	/*----------STAGE2,3----------*/
				m2x_old = m2x;
				m2y_old = m2y;
				r2 = rand() % 3;
				switch( r2 )	//�G2
				{
				case 0:
					m2x++;
					break;
				case 1:
					m2x--;
					break;
				case 2:
					if( stagenum == 3 )
					{
						if( stage3[m2y+1][m2x] == BROCK )
							m2jumptime = 5;
					}
					else if( stagenum == 2 )
					{
						if( stage2[m2y+1][m2x] == BROCK )
							m2jumptime = 5;
					}
					break;
				}
			case 1:	/*----------STAGE1,2,3----------*/
				m1x_old = m1x;
				m1y_old = m1y;
				r1 = rand() % 3;
				switch( r1 )	//�G1
				{
				case 0:
					m1x++;
					break;
				case 1:
					m1x--;
					break;
				case 2:
					if( stagenum == 3 )
					{
						if( stage3[m1y+1][m1x] == BROCK )
							m1jumptime = 5;
					}
					else if( stagenum == 2 )
					{
						if( stage2[m1y+1][m1x] == BROCK )
							m1jumptime = 5;
					}
					else if( stagenum == 1 )
					{
						if( stage1[m1y+1][m1x] == BROCK )
							m1jumptime = 5;
					}
					break;
				}
				break;
			}

			/*�@�W�����v�����@*/
			if( jumptime > 3 )	//���W�����v
			{
				messy--;	//�����p
				jumptime--;
			}
			if( ossjumptime > 3 )	//���W�����v
			{
				ossy--;	//�����p
				ossjumptime--;
			}
			if( m1jumptime > 2 )	//�G1�W�����v
			{
				m1y--;	//�����p
				m1jumptime--;
			}
			if( m2jumptime > 2 )	//�G2�W�����v
			{
				m2y--;	//�����p
				m2jumptime--;
			}
			if( m3jumptime > 2 )	//�G3�W�����v
			{
				m3y--;	//�����p
				m3jumptime--;
			}

			if( stagenum == 1 )	/*----------STAGE1----------*/
			{
				/*�@�W�����v�����@*/
				if( stage1[messy+1][messx] != BROCK && jumptime <= 3 )
				{
					messy++;	//�d�͍�p
					jumptime--;
				}
				if( stage1[m1y+1][m1x] != BROCK && m1jumptime <= 2 )
				{
					m1y++;	//�d�͍�p
					m1jumptime--;
				}

				/*�@�������@*/
				stage1[messy_old][messx_old] = EMPTY;

				switch( stage1[messy][messx] )
				{
				case BROCK:	//�Ǐ���
					messx = messx_old;
					messy = messy_old;
					break;
				case STER:	//���_
					PlaySound("getstersound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
					point += 10;
					getster++;
					break;
				case SSTER:	//���_
					PlaySound("getsstersound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
					point += 100;
					getster++;
					break;
				case SAVE:	//�Z�[�u�|�C���g
					PlaySound("savesound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
					savecheck = 1;
					savex = 17, savey = 7;
					break;
				}

				/*�@�G�����@*/
				if( stage1[m1y][m1x] == BROCK )	//�G1�Ǐ���
				{
					m1x = m1x_old;
					m1y = m1y_old;
				}

				m1field_old = stage1[m1y][m1x];	//�G�̈ړ���ɉ������邩��ۑ�
				if( m1field_old == MESS )		//�G�̈ړ��悪���������ꍇ�AEMPTY(0)����
					m1field_old = EMPTY;
				stage1[m1y][m1x] = MONSTER;

				if( stage1[messy][messx] == NEEDLE || stage1[messy][messx] == MONSTER )	//�����蔻��
				{
					PlaySound("damagesound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
					stage1[messy][messx] = DAMAGE;
				}
				else if( stage1[messy][messx] == GOAL )	//�S�[������
				{
					PlaySound("goalsound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
					stage1[messy][messx] = CLEAR;
				}
				else
					stage1[messy][messx] = MESS;
			}
			else if( stagenum == 2 )	/*----------STAGE2----------*/
			{
				/*�@�W�����v�����@*/
				if( stage2[messy+1][messx] != BROCK && stage2[messy+1][messx] != FROOR && jumptime <= 3 )
				{
					messy++;	//�d�͍�p
					jumptime--;
				}
				if( stage2[m1y+1][m1x] != BROCK && m1jumptime <= 2 )
				{
					m1y++;	//�d�͍�p
					m1jumptime--;
				}
				if( stage2[m2y+1][m2x] != BROCK && m2jumptime <= 2 )
				{
					m2y++;	//�d�͍�p
					m2jumptime--;
				}

				/*�@�������@*/
				stage2[messy_old][messx_old] = EMPTY;

				switch( stage2[messy][messx] )
				{
				case BROCK:	//�Ǐ���
				case FROOR:
					messx = messx_old;
					messy = messy_old;
					break;
				case STER:	//���_
					PlaySound("getstersound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
					point += 10;
					getster++;
					break;
				case SSTER:	//���_
					PlaySound("getsstersound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
					point += 100;
					getster++;
					break;
				case SAVE:	//�Z�[�u�|�C���g
					PlaySound("savesound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
					savecheck = 1;
					savex = 22, savey = 13;
					break;
				}

				/*�@�G�����@*/
				if( stage2[m1y][m1x] == BROCK || stage2[m1y][m1x] == OUTEMPTY )	//�G1�Ǐ���
				{
					m1x = m1x_old;
					m1y = m1y_old;
				}
				if( stage2[m2y][m2x] == BROCK || stage2[m2y][m2x] == OUTEMPTY )	//�G2�Ǐ���
				{
					m2x = m2x_old;
					m2y = m2y_old;
				}

				m1field_old = stage2[m1y][m1x];	//�G�̈ړ���ɉ������邩��ۑ�
				if( m1field_old == MESS )		//�G�̈ړ��悪���������ꍇ�AEMPTY(0)����
					m1field_old = EMPTY;
				m2field_old = stage2[m2y][m2x];	//�G�̈ړ���ɉ������邩��ۑ�
				if( m2field_old == MESS )		//�G�̈ړ��悪���������ꍇ�AEMPTY(0)����
					m2field_old = EMPTY;

				stage2[m1y][m1x] = MONSTER;
				stage2[m2y][m2x] = MONSTER;

				/*�@����@*/
				if( stage2[messy][messx] == NEEDLE || stage2[messy][messx] == MONSTER )	//�����蔻��
				{
					PlaySound("damagesound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
					stage2[messy][messx] = DAMAGE;
				}
				else if( stage2[messy][messx] == GOAL )	//�S�[������
				{
					PlaySound("goalsound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
					stage2[messy][messx] = CLEAR;
				}
				else
					stage2[messy][messx] = MESS;

				/*�@�t���A�����@*/
				if( froorcount % 5 == 0 )	//0.5�b���݂œ���
				{
					//�ړ��`�F�b�N
					if( stage2[11][froor1x-2] == BROCK )
						froor1check = 1;
					else if( stage2[11][froor1x+2] == BROCK )
						froor1check = -1;

					//�ړ�����
					if( froor1check == 1 )
					{
						stage2[11][froor1x-1] = EMPTY;
						stage2[11][froor1x+2] = FROOR;
					}
					else if( froor1check == -1 )
					{
						stage2[11][froor1x+1] = EMPTY;
						stage2[11][froor1x-2] = FROOR;
					}

					froor1x += froor1check;
				}
			}
			else if( stagenum == 3 )/*----------STAGE3----------*/
			{
				/*�@�W�����v�����@*/
				if( stage3[messy+1][messx] != BROCK && stage3[messy+1][messx] != FROOR && jumptime <= 3 )
				{
					messy++;	//�d�͍�p
					jumptime--;
				}
				if( stage3[ossy+1][ossx] != BROCK && stage3[ossy+1][ossx] != FROOR && ossjumptime <= 3 )
				{
					ossy++;	//�d�͍�p
					ossjumptime--;
				}
				if( stage3[m1y+1][m1x] != BROCK && stage3[m1y+1][m1x] != FROOR && m1jumptime <= 2 )
				{
					m1y++;	//�d�͍�p
					m1jumptime--;
				}
				if( stage3[m2y+1][m2x] != BROCK && stage3[m2y+1][m2x] != FROOR && m2jumptime <= 2 )
				{
					m2y++;	//�d�͍�p
					m2jumptime--;
				}
				if( stage3[m3y+1][m3x] != BROCK && stage3[m3y+1][m3x] != FROOR && m3jumptime <= 2 )
				{
					m3y++;	//�d�͍�p
					m3jumptime--;
				}

				/*�@�������@*/
				stage3[messy_old][messx_old] = EMPTY;

				switch( stage3[messy][messx] )
				{
				case BROCK:	//�Ǐ���
				case FROOR:
					messx = messx_old;
					messy = messy_old;
					break;
				case STER:	//���_
					PlaySound("getstersound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
					point += 10;
					getster++;
					break;
				case SSTER:	//���_
					PlaySound("getsstersound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
					point += 100;
					getster++;
					break;
				case SAVE:	//�Z�[�u�|�C���g
					PlaySound("savesound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
					savecheck = 1;
					savex = 26, savey = 3;
					break;
				}

				/*�@�G�����@*/
				if( stage3[ossy][ossx] == BROCK || stage3[ossy][ossx] == FROOR )	//���Ǐ���
				{
					ossx = ossx_old;
					ossy = ossy_old;
				}
				if( stage3[m1y][m1x] == BROCK || stage3[m1y][m1x] == FROOR || stage3[m1y][m1x] == OUTEMPTY )	//�G1�Ǐ���
				{
					m1x = m1x_old;
					m1y = m1y_old;
				}
				if( stage3[m2y][m2x] == BROCK || stage3[m2y][m2x] == FROOR || stage3[m2y][m2x] == OUTEMPTY )	//�G2�Ǐ���
				{
					m2x = m2x_old;
					m2y = m2y_old;
				}
				if( stage3[m3y][m3x] == BROCK || stage3[m3y][m3x] == FROOR || stage3[m2y][m2x] == OUTEMPTY )	//�G3�Ǐ���
				{
					m3x = m3x_old;
					m3y = m3y_old;
				}

				ossfield_old = stage3[ossy][ossx];	//�G�̈ړ���ɉ������邩��ۑ�
				if( ossfield_old == MESS )		//�G�̈ړ��悪���������ꍇ�AEMPTY(0)����
					ossfield_old = EMPTY;
				m1field_old = stage3[m1y][m1x];	//�G�̈ړ���ɉ������邩��ۑ�
				if( m1field_old == MESS )		//�G�̈ړ��悪���������ꍇ�AEMPTY(0)����
					m1field_old = EMPTY;
				m2field_old = stage3[m2y][m2x];	//�G�̈ړ���ɉ������邩��ۑ�
				if( m2field_old == MESS )		//�G�̈ړ��悪���������ꍇ�AEMPTY(0)����
					m2field_old = EMPTY;
				m3field_old = stage3[m3y][m3x];	//�G�̈ړ���ɉ������邩��ۑ�
				if( m3field_old == MESS )		//�G�̈ړ��悪���������ꍇ�AEMPTY(0)����
					m3field_old = EMPTY;

				stage3[ossy][ossx] = OSS;
				stage3[m1y][m1x] = MONSTER;
				stage3[m2y][m2x] = MONSTER;
				stage3[m3y][m3x] = MONSTER;

				/*�@����@*/
				if( stage3[messy][messx] == NEEDLE || stage3[messy][messx] == MONSTER || stage3[messy][messx] == OSS )	//�����蔻��
				{
					PlaySound("damagesound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
					stage3[messy][messx] = DAMAGE;
				}
				else if( stage3[messy][messx] == GOAL )	//�S�[������
				{
					PlaySound("goalsound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
					stage3[messy][messx] = CLEAR;
				}
				else
					stage3[messy][messx] = MESS;

				/*�@�t���A�����@*/
				if( froorcount % 5 == 0 )	//0.5�b���݂œ���
				{
					//�ړ��`�F�b�N
					if( stage3[12][froor2x-2] == BROCK )
						froor2check = 1;
					else if( stage3[12][froor2x+2] == BROCK )
						froor2check = -1;

					//�ړ�����
					if( froor2check == 1 )
					{
						stage3[12][froor2x-1] = EMPTY;
						stage3[12][froor2x+2] = FROOR;
					}
					else if( froor2check == -1 )
					{
						stage3[12][froor2x+1] = EMPTY;
						stage3[12][froor2x-2] = FROOR;
					}

					froor2x += froor2check;

					//�ړ��`�F�b�N
					if( stage3[2][froor3x-3] == BROCK )
						froor3check = 1;
					else if( stage3[2][froor3x+2] == BROCK )
						froor3check = -1;

					//�ړ�����
					if( froor3check == 1 )
					{
						stage3[2][froor3x-1] = EMPTY;
						stage3[2][froor3x+2] = FROOR;
					}
					else if( froor3check == -1 )
					{
						stage3[2][froor3x+1] = EMPTY;
						stage3[2][froor3x-2] = FROOR;
					}

					froor3x += froor3check;
				}
			}

			/*�@��ʕ\���@*/
			printf("������������������������\n");
			printf("��POINT�F%3d��LIFE�F%2d��\n",point, mlife);
			printf("������������������������\n");
			printf("\n");
			for( y = 0; y < YSIZE; y++ )
			{
				printf("\t");
				for( x = 0; x < XSIZE; x++)
				{
					if( stagenum == 1 )	/*----------STAGE1----------*/
					{
						switch( stage1[y][x] )
						{
						case EMPTY:
							printf("�@");
							break;
						case BROCK:
							TEXT_BLACK;
							printf("��");
							break;
						case MESS:
							TEXT_IRED;
							printf("��");
							break;
						case MONSTER:
							TEXT_IBLUE;
							printf("�v");
							break;
						case STER:
							TEXT_GREEN;
							printf("��");
							break;
						case SSTER:
							TEXT_GREEN;
							printf("��");
							break;
						case NEEDLE:
							TEXT_BLACK;
							printf("��");
							break;
						case SAVE:
							TEXT_IGREEN;
							printf("��");
							break;
						case GOAL:
							TEXT_RED;
							printf("��");
							break;
						case DAMAGE:
							TEXT_IRED;
							printf("�w");
							mlife--;
							stage1[messy][messx] = NEEDLE;
							if( savecheck == 1 )
								messx = savex, messy = savey;
							else
								messx = 2, messy = 12;
							break;
						case CLEAR:
							TEXT_IRED;
							printf("��");
							break;
						}
					}
					else if( stagenum == 2 )	/*----------STAGE2----------*/
					{
						switch( stage2[y][x] )
						{
						case EMPTY:
						case OUTEMPTY:
							printf("�@");
							break;
						case BROCK:
							TEXT_BLACK;
							printf("��");
							break;
						case MESS:
							TEXT_IRED;
							printf("��");
							break;
						case MONSTER:
							TEXT_IBLUE;
							printf("�v");
							break;
						case STER:
							TEXT_GREEN;
							printf("��");
							break;
						case SSTER:
							TEXT_GREEN;
							printf("��");
							break;
						case NEEDLE:
							TEXT_BLACK;
							printf("��");
							break;
						case FROOR:
							TEXT_BLACK;
							printf("��");
							break;
						case SAVE:
							TEXT_IGREEN;
							printf("��");
							break;
						case GOAL:
							TEXT_RED;
							printf("��");
							break;
						case DAMAGE:
							TEXT_IRED;
							printf("�w");
							mlife--;
							stage2[messy][messx] = NEEDLE;
							if( savecheck == 1 )
								messx = savex, messy = savey;
							else
								messx = 1, messy = 13;
							break;
						case CLEAR:
							TEXT_IRED;
							printf("��");
							break;
						}
					}
					else if( stagenum == 3 )	/*----------STAGE3----------*/
					{
						switch( stage3[y][x] )
						{
						case EMPTY:
						case OUTEMPTY:
							printf("�@");
							break;
						case BROCK:
							TEXT_BLACK;
							printf("��");
							break;
						case MESS:
							TEXT_IRED;
							printf("��");
							break;
						case OSS:
							TEXT_IBLUE;
							printf("��");
							break;
						case MONSTER:
							TEXT_IBLUE;
							printf("�v");
							break;
						case STER:
							TEXT_GREEN;
							printf("��");
							break;
						case SSTER:
							TEXT_GREEN;
							printf("��");
							break;
						case NEEDLE:
							TEXT_BLACK;
							printf("��");
							break;
						case FROOR:
							TEXT_BLACK;
							printf("��");
							break;
						case SAVE:
							TEXT_IGREEN;
							printf("��");
							break;
						case GOAL:
							TEXT_RED;
							printf("��");
							break;
						case DAMAGE:
							TEXT_IRED;
							printf("�w");
							mlife--;
							stage3[messy][messx] = NEEDLE;
							if( savecheck == 1 )
								messx = savex, messy = savey;
							else
								messx = 2, messy = 11;
							break;
						case CLEAR:
							TEXT_IRED;
							printf("��");
							break;
						}
					}
				}
				printf("\n");
			}
			printf("\n");

			/*�@�G������ꏊ�ɂ��Ƃ��Ƃ��������̂�߂������@*/
			if( stagenum == 1 )	/*----------STAGE1----------*/
				stage1[m1y][m1x] = m1field_old;
			else if( stagenum == 2 )	/*----------STAGE2----------*/
			{
				stage2[m1y][m1x] = m1field_old;
				stage2[m2y][m2x] = m2field_old;
			}
			else if( stagenum == 3 )	/*----------STAGE3----------*/
			{
				stage3[ossy][ossx] = ossfield_old;
				stage3[m1y][m1x] = m1field_old;
				stage3[m2y][m2x] = m2field_old;
				stage3[m3y][m3x] = m3field_old;
			}

			/*�@�Q�[���I�[�o�[�@*/
			if( mlife == 0 )
			{
				if( stagenum == 1 )
					mciSendString("stop story1", NULL, 0, NULL);	//BGM��~
				else if( stagenum == 2 )
					mciSendString("stop story2", NULL, 0, NULL);	//BGM��~
				else if( stagenum == 3 )
					mciSendString("stop story3", NULL, 0, NULL);	//BGM��~

				PlaySound("gameoversound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�

				for( i = 0; i < 3; i++ )	//�_��
				{
					system("cls");
					Sleep( 500 );
					TEXT_IBLUE;
					printf("�������������������������@���������@����������������\n");
					printf("���@�������������������@�@���@�����@�������@��������\n");
					printf("���������@�����@���������@��������������������������\n");
					Sleep( 500 );
					startnum = EXIT;
				}
				break;	//�J��Ԃ����甲����
			}

			/*�@�X�e�[�W�N���A�@*/
			if( stagenum == 1 )
			{
				if( stage1[messy][messx] == CLEAR )
				{
					Sleep( 2500 );
					system("cls");
					mciSendString("stop story1", NULL, 0, NULL);	//BGM��~
					stagenum = 2;
					break;	//�J��Ԃ����甲����
				}
			}
			else if( stagenum == 2 )
			{
				if( stage2[messy][messx] == CLEAR )
				{
					Sleep( 2500 );
					system("cls");
					mciSendString("stop story2", NULL, 0, NULL);	//BGM��~
					stagenum = 3;
					break;	//�J��Ԃ����甲����
				}
			}
			else if( stagenum == 3 )
			{
				if( stage3[messy][messx] == CLEAR )
				{
					Sleep( 2500 );
					system("cls");
					mciSendString("stop story3", NULL, 0, NULL);	//BGM��~
					startnum = EXIT;
					finish = 1;	//�Q�[���N���A
					break;	//�J��Ԃ����甲����
				}
			}

			Sleep( 100 );
			system("cls");

			if( stagenum == 2 || stagenum == 3 )	//�t���A�J�E���g��1�����₷
				froorcount++;
		}
	}

	/*�@BGM�t�@�C���N���[�Y�@*/
	mciSendString("close title", NULL, 0, NULL);	//�^�C�g�����
	mciSendString("close rule", NULL, 0, NULL);		//���[������
	mciSendString("close bmenu", NULL, 0, NULL);	//�ΐ탁�j���[
	mciSendString("close battle", NULL, 0, NULL);	//�ΐ�
	mciSendString("close story1", NULL, 0, NULL);	//�X�g�[���[1
	mciSendString("close story2", NULL, 0, NULL);	//�X�g�[���[2
	mciSendString("close story3", NULL, 0, NULL);	//�X�g�[���[3

	PlaySound("finishsound.wav", NULL, SND_FILENAME | SND_ASYNC);	//���ʉ�
	TEXT_IRED;
	printf("�@���@�@�@�@�@");TEXT_IBLUE;printf("��\n");
	TEXT_GREEN;
	printf("������������������\n");
	printf("�����@���������@��\n");
	printf("������������������\n\n");
	Sleep( 1500 );

	if( finish )
	{
		TEXT_BLACK;
		printf("��������������������������\n");
		printf("���l������");TEXT_GREEN;printf("��");TEXT_BLACK;printf("�̐��F%2d�^%2d��\n", getster, allster);
		printf("��POINT�F%3d�@�@�@�@�@�@��\n", point);
		printf("��������������������������\n\n");
	}

	return 0;
}
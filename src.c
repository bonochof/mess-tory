/*
*	春休み課題「♀tory」	src.c
*
*	J1318	Takata
*							平成26年4月10日
*							平成26年4月24日	(VSCOM対戦画面の改善, ルール説明2ページ目を改善, コンピュータの敵「♂」が「♀」に近づくよう改善, VSCOM対戦モードで「‡」と「♂」が重なったとき「♂」が消えるバグを修正)
*
*	＜サウンド引用＞魔王魂
*	＜プログラム一部(4行)参考＞ＭＩＤＩ再生	zahyou.6.ql.bz/cgame/mcisendstring.htm
*/


#pragma comment(lib,"winmm")	//winmm.libをリンク

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <mmsystem.h>	//mciSendString()

/*　テキストの色(背景色は全て強調白)　*/
#define TEXT_BLACK SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN )
#define TEXT_IRED SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN )
#define TEXT_RED SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN )
#define TEXT_IBLUE SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN )
#define TEXT_BLUE SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN )
#define TEXT_IGREEN SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN )
#define TEXT_GREEN SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN | BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN )

/*　画面表示サイズ　*/
#define YSIZE 15
#define XSIZE 30
/*　タイトル画面表示　*/
#define POINTER 1
#define TITLE 0
/*　タイトル画面ジャンプ　*/
#define RULE 1
#define STORY 2
#define BATTLE 3
#define EXIT 4
/*　対戦メニュー　*/
#define VSCOM 1
#define VSPLAYER 2
/*　画面表示　*/
#define EMPTY 0
#define OUTEMPTY 1	//進入禁止EMPTY
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
	int tx = 0, ty = 0;	//タイトル画面ポインター
	int tx_old, ty_old;
	int startnum = 0;
	int bx = 0, by = 0;	//対戦メニューポインター
	int bx_old, by_old;
	int rulepage = 1;
	int jumptime;	//ジャンプタイム
	int ossjumptime, m1jumptime, m2jumptime, m3jumptime;	//敵ジャンプタイム
	int froor1x = 8;
	int froor2x = 5;
	int froor3x = 3;
	int froor1check;
	int froor2check;
	int froor3check;
	int froorcount = 0;	//フロア(動く床)の動くタイミング
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
	int ossfield_old, m1field_old, m2field_old, m3field_old;	//敵の移動先にあったものを保存
	int x, y;
	int r1, r2, r3, or;	//乱数
	int key;
	int whovs;
	int battleend;	//対戦中止
	int point = 0;
	int savecheck = 0;
	int savex, savey;
	int mlife, olife;
	int getster = 0;
	int allster = 0;
	int finish = 0;	//ゲームクリアしたかどうか
	int title[4][2] = //タイトル画面
	{
		{1,0},
		{0,0},
		{0,0},
		{0,0}
	};
	int battlemenu[2][2] = //対戦メニュー
	{
		{1,0},
		{0,0}
	};
	int vsc_battlestage[YSIZE][XSIZE] = //対戦ステージ(VSコンピュータ)
	{
		{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　　　　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　　　　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　　　　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　　　　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　　　　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　　　　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　　　　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　　　　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　　　　　■■■■　　　　　　　　　　　　■
		{2,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　　　■■■■■■■■　　　　　　　　　　■
		{2,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　■■■■■■■■■■■■　　　　　　　　■
		{2,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,2},// ■　　　　　　■■■■■■■■■■■■■■■■　　　　　　■
		{2,0,3,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,10,2},//■　♀　　■■■■■■■■■■■■■■■■■■■■　　♂‡■
		{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2} // ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	};
	int vsp_battlestage[YSIZE][XSIZE] = //対戦ステージ(VSプレイヤー)
	{
		{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　　　　　■
		{2,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,0,2},// ■　　■■■■■　　　　　　　　　　　　　　　　■■■■　■
		{2,0,0,2,1,1,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,2},// ■　　■　　　■　　　　　　　　　　　　　　　　　　■■　■
		{2,0,0,2,1,1,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,2},// ■　　■　　　■　　　　　　　　　　　　　　　　　■■■　■
		{2,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,0,2,0,2},// ■　　■■■■■　　　　　　　　　　　　■■■■■■　■　■
		{2,0,0,0,0,2,0,0,0,0,0,0,2,2,2,2,2,2,0,0,2,1,1,1,2,0,0,0,0,2},// ■　　　　■　　　　　　■■■■■■　　■　　　■　　　　■
		{2,0,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,2,1,1,1,2,0,0,0,0,2},// ■　■■■■■■■　　　　　　　　　　　■　　　■　　　　■
		{2,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1,1,1,2,0,0,0,0,2},// ■　　　　■　　　　　　　　　　　　　　■　　　■　　　　■
		{2,0,0,0,0,2,0,0,0,0,0,2,2,0,0,0,0,2,2,0,2,2,2,2,2,0,0,0,0,2},// ■　　　　■　　　　　■■　　　　■■　■■■■■　　　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　　　　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　　　　　■
		{2,0,0,0,0,0,0,0,0,2,2,2,2,2,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　■■■■■　　■■■■■　　　　　　　　■
		{2,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,2},//■　♀　　　　　　　　　　　　　　　　　　　　　　　　♂‡■
		{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2} // ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	};
	int stage1[YSIZE][XSIZE] = //ステージ1
	{
		{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　　　　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　　　　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　　　　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　　　　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　　　　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　　　　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,9,0,0,5,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　　　　　★　　　†　　☆　　　　　　　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,2,2,2,0,2,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　　　　■■■　■■■　■　　　　　　　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　　　　　　　　　　■　■　　　　　　　　■
		{2,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　　☆　　　　　　　■　■　　　　　　　　■
		{2,0,0,0,0,0,0,5,0,2,2,2,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,2},// ■　　　　　　☆　■■■　　　　　　■　■　　　　　　　　■
		{2,0,3,0,5,0,2,2,2,2,2,2,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,10,0,2},//■　♀　☆　■■■■■■　　　　　　■　■　　　　　　‡　■
		{2,2,2,0,2,2,2,2,2,2,2,2,0,0,5,0,0,0,2,0,2,2,2,2,2,7,2,2,2,2},// ■■■　■■■■■■■■　　☆ё　　■　■■■■■Λ■■■■
		{2,2,2,7,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,7,2,0,0,0,0,0,0,0,0,2} // ■■■∧■■■■■■■■■■■■■■■∧■　　　　　　　　■
	};
	int stage2[YSIZE][XSIZE] = //ステージ2
	{
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　　　　　■
		{2,0,0,0,0,0,0,0,0,5,0,0,0,5,0,0,0,5,0,0,0,0,0,0,1,0,0,0,0,2},// ■　　　　　　　　☆　　　☆　　　☆　　　　　　　　　　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　　　　　■
		{2,6,0,0,0,0,0,5,0,0,0,5,0,0,0,5,0,0,0,5,0,0,0,0,1,0,0,0,10,2},//■★ё　　　　☆　　　☆　　　☆　　　☆　　　ё　　　　‡■
		{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,2,2,2,2},// ■■■■■■■■■■■■■■■■■■■■■■■■■　■■■■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　　　　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　☆　　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　■　　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,2},// ■　　　　　　　　　　　　　　　　　　　　　　　☆　　　　■
		{2,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,2},// ■　　　　　■　　　　　　　　　　　　　　　　　■　　　　■
		{2,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,2},// ■　　　　　■　　　　　　　　　　　　　　　　☆　　　　　■
		{2,0,0,0,2,0,2,8,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,2},// ■　　　■　■━━━　　　　　　　　　　　　　■　　　　　■
		{2,0,0,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ■　　　■　■　　　　　　　　　　　　　　　　　　　　　　■
		{2,3,2,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,0,2},// ■♀■　■　■　　　　　　　　　　　　　　　†　　　　　　■
		{2,2,2,7,2,7,2,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,2,2,2,2,2,2,2,2}//  ■■■∧■∧■∧∧∧∧∧∧∧∧∧∧∧∧∧∧∧■■■■■■■■
	};
	int stage3[YSIZE][XSIZE] = //ステージ3
	{
		{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,2},//■　　　　　　　　　　　　　　　　　　　　　　　　　　　‡■
		{2,0,8,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2},// ■　━━━　　　　　　　　　　　　　　　　　　　　■■■■■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　　†　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　■■　　■
		{2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,2},// ■■　　　　　　　　　　　　　　　　　　　　　　　■　　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,2,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　■　　■■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,6,0,0,2},// ■　♂　　　　　　　　　　　　　　　　　　　　　　■★　　■
		{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,2},// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■　■
		{2,0,5,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,2},// ■　☆　　　　　　　　　　　　　　　　　　　　　　☆　　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2},// ■　　　　　　　　　　　　　　　　　　　　　　　　　　　■■
		{2,0,3,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ■　♀　　　　　　　　　　　　　　　　　　　　　　　　　　■
		{2,2,2,2,8,8,8,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ■■■■━━━　　　　　　　　　　　　　　　　　　　　　　■
		{2,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},// ■ёёё　　　　　　　　　　　　　　　　　　　　　　　　　■
		{2,2,2,2,2,2,2,2,2,2,2,2,2,2,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,2}//  ■■■■■■■■■■■■■■∧∧∧∧∧∧∧∧∧∧∧∧∧∧∧■
	};

	/*　BGMファイルオープン　*/
	mciSendString("open titlebgm.mp3 alias title", NULL, 0, NULL);		//タイトル画面
	mciSendString("open rulebgm.mp3 alias rule", NULL, 0, NULL);		//ルール説明
	mciSendString("open battlemenubgm.mp3 alias bmenu", NULL, 0, NULL);	//対戦メニュー
	mciSendString("open battlebgm.mp3 alias battle", NULL, 0, NULL);	//対戦
	mciSendString("open story1bgm.mp3 alias story1", NULL, 0, NULL);		//ストーリー1
	mciSendString("open story2bgm.mp3 alias story2", NULL, 0, NULL);		//ストーリー2
	mciSendString("open story3bgm.mp3 alias story3", NULL, 0, NULL);		//ストーリー3

	/*　最大ループ(タイトルに戻る, ルール説明の次のページを表示する)　*/
	while( startnum == TITLE || startnum == RULE )
	{
		//=================================================================================================
		//	タイトル画面
		//=================================================================================================
		if( startnum == TITLE )
		{
			while(1)
			{
				/*　＜参考＞BGMループ再生　*/
				TCHAR BGMStatus[256] = {0};
				mciSendString("status title mode", BGMStatus, 256, NULL); //BGM状態の取得
				if(lstrcmp(BGMStatus, "stopped") == 0)
					mciSendString("play title from 0", NULL, 0, NULL); //BGM停止中なら再生

				/*　タイトル画面・キー入力　*/
				if( kbhit() )
				{
					key = getch();

					tx_old = tx, ty_old = ty;

					switch( key )
					{
					case '2':
						PlaySound("menusound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
						ty++;
						break;
					case '8':
						PlaySound("menusound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
						ty--;
						break;
					case ' ':
						PlaySound("decidesound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
						mciSendString("stop title", NULL, 0, NULL);	//BGM停止
						tx++;
						break;
					}
					ty = ( ty + 4 ) % 4;
					title[ty_old][tx_old] = EMPTY;
				}
				title[ty][tx] = POINTER;

				/*　タイトル画面・画面表示　*/
				TEXT_IRED;
				printf("\t\t┏━┓\n");
				printf("\t\t┃　┃\t　　メストーリー\n");
				printf("\t\t┗┳┛");TEXT_RED; printf("┣┳┫┏━┓┏━┓┳　┳\n");
				TEXT_IRED;
				printf("\t\t━╋━");TEXT_RED; printf("　┃　┃　┃┣┳┛┗┳┛\n");
				TEXT_IRED;
				printf("\t\t　┃　");TEXT_RED; printf("　┻　┗━┛┻┗┛　┻　\n");

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
							printf("＞");
							break;
						default:
							printf("　");
							break;
						}

						if( y == 0 && x == 1 )
						{
							TEXT_GREEN;
							printf("ルール");
						}
						if( y == 1 && x == 1 )
						{
							TEXT_RED;
							printf("対戦");
						}
						if( y == 2 && x == 1 )
						{
							TEXT_BLUE;
							printf("ストーリー");
						}
						if( y == 3 && x == 1 )
						{
							TEXT_BLACK;
							printf("終了");
						}
					}
					printf("\n");
				}
				printf("\n\n");
				printf("\t\t↑(8)↓(2)：選択　　スペースキー：決定\n");

				/*　ページジャンプ処理　*/
				if( title[0][1] == POINTER )	//ルール説明
				{
					startnum = RULE;
					Sleep( 100 );
					system("cls");
					break;
				}
				else if( title[1][1] == POINTER )	//対戦
				{
					startnum = BATTLE;
					Sleep( 100 );
					system("cls");
					break;
				}
				else if( title[2][1] == POINTER )	//ストーリー
				{
					startnum = STORY;
					Sleep( 100 );
					system("cls");
					break;
				}
				else if( title[3][1] == POINTER )	//終了
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
		//	ルール説明
		//=================================================================================================
		if( startnum == RULE )
		{
			/*　＜参考＞BGM再生　*/
			TCHAR BGMStatus[256] = {0};
			mciSendString("status rule mode", BGMStatus, 256, NULL); //BGM状態の取得
			if(lstrcmp(BGMStatus, "stopped") == 0)
				mciSendString("play rule from 0", NULL, 0, NULL); //BGM停止中なら再生

			TEXT_IRED;
			printf("\t\t┏━┓\n");
			printf("\t\t┃　┃\t　　メストーリー\n");
			printf("\t\t┗┳┛");TEXT_RED;printf("┣┳┫┏━┓┏━┓┳　┳");TEXT_GREEN;printf("　　┓　┃━╋　\n");
			TEXT_IRED;
			printf("\t\t━╋━");TEXT_RED;printf("　┃　┃　┃┣┳┛┗┳┛");TEXT_GREEN;printf("　┏┻┛┃┏╋┓\n");
			TEXT_IRED;
			printf("\t\t　┃　");TEXT_RED;printf("　┻　┗━┛┻┗┛　┻　");TEXT_GREEN;printf("　┗━━┗┗┛　\n\n");

			if( rulepage == 1 )
			{
				TEXT_RED;
				printf("<<対戦モード>>\n");
				TEXT_IRED;printf("♀");TEXT_RED;printf("と");TEXT_IBLUE;printf("♂");TEXT_RED;printf("とが戦うモードです。\n");
				printf("・VSコンピュータ\n");
				printf("　　あなたとコンピュータとで戦います。\n");
				printf("　　あなたがコンピュータ側の‡を3回ゲットしたらあなたの勝ちです。\n");
				printf("　　");TEXT_IBLUE;printf("♂");TEXT_RED;printf("に3回当たったらあなたの負けです。\n");
				printf("・VSプレイヤー\n");
				printf("　　あなたともう一人の相手とで戦います。\n");
				TEXT_IBLUE;printf("　　♂");TEXT_RED;printf("の‡を3回ゲットしたら");TEXT_IRED;printf("♀");TEXT_RED;printf("の勝ちです。\n");
				TEXT_IBLUE;printf("　　♂");TEXT_RED;printf("に3回当たったら");TEXT_IRED;printf("♀");TEXT_RED;printf("の負けです。\n");
				printf("操作方法は対戦モードで確認してください。\n\n");
				TEXT_GREEN;
				printf("*PRESS SPEAS KEY*\n");
				if( getch() == ' ' )	//rulepageは変わるがstartnumはRULE(1)のままなので、ループして戻ってくる
				{
					system("cls");
					rulepage = 2;
				}
				else	//startnumはRULE(1)のままなので、ループして戻ってくる
					system("cls");
			}
			else if( rulepage == 2 )
			{
				TEXT_BLUE;
				printf("<<ストーリーモード>>\n");
				TEXT_IRED;printf("♀");TEXT_BLUE;printf("が主人公となって冒険するモードです。\n");
				printf("・操作方法\n");
				printf("　　移動：←(4)・→(6)\n");
				printf("　　ジャンプ：↑(8)\n");
				printf("・セーブポイント\n");
				TEXT_IGREEN;printf("　　†");TEXT_BLUE;printf("をゲットすると\n　　ダメージを受けた時にその地点に復活します。\n");
				printf("・ステージクリア条件\n");
				TEXT_RED;printf("　　‡");TEXT_BLUE;printf("をゲットすると次のステージに進みます。\n");
				printf("・ゲームオーバー条件\n");
				printf("　　残りライフが0になるとゲームオーバーです。\n");
				printf("・得点表\n");
				printf("　　┏━┳━━━┓\n");
				printf("　　┃");TEXT_GREEN;printf("☆");TEXT_BLUE;printf("┃　１０┃\n");
				printf("　　┃");TEXT_GREEN;printf("★");TEXT_BLUE;printf("┃１００┃\n");
				printf("　　┗━┻━━━┛\n");
				TEXT_GREEN;
				printf("*PRESS SPEAS KEY*\n");
				if( getch() == ' ' )
				{
					PlaySound("backsound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
					mciSendString("stop rule", NULL, 0, NULL);	//BGM停止
					system("cls");
					rulepage = 1;
					startnum = TITLE;
				}
				else	//startnumはRULE(1)のままなので、ループして戻ってくる
					system("cls");
			}
		}

		//=================================================================================================
		//	対戦モード
		//=================================================================================================
		if( startnum == BATTLE )
		{
			/*　対戦メニュー　*/
			while(1)
			{
				/*　＜参考＞BGMループ再生　*/
				TCHAR BGMStatus[256] = {0};
				mciSendString(TEXT("status bmenu mode"), BGMStatus, 256, NULL); // 状態の取得
				if(lstrcmp(BGMStatus, TEXT("stopped")) == 0)
					mciSendString(TEXT("play bmenu from 0"), NULL, 0, NULL); // 停止中なら演奏

				/*　対戦メニュー・キー入力　*/
				if( kbhit() )
				{
					key = getch();

					bx_old = bx, by_old = by;

					switch( key )
					{
					case '2':
						PlaySound("menusound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
						by++;
						break;
					case '8':
						PlaySound("menusound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
						by--;
						break;
					case ' ':
						PlaySound("decidesound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
						bx++;
						break;
					}
					by = ( by + 2 ) % 2;
					battlemenu[by_old][bx_old] = EMPTY;
				}
				battlemenu[by][bx] = POINTER;

				/*　対戦メニュー・画面表示　*/
				TEXT_RED;
				printf("\t┏━━━━━━┓\n");
				printf("\t┃対戦メニュー┃\n");
				printf("\t┗━━━━━━┛\n\n");
				for( y = 0; y < 2; y++ )
				{
					printf("\t");
					for( x = 0; x < 2; x++ )
					{
						switch( battlemenu[y][x] )
						{
						case POINTER:
							TEXT_BLACK;
							printf("＞");
							break;
						default:
							printf("　");
							break;
						}
						TEXT_RED;
						if( y == 0 && x == 1 )
							printf("VSコンピュータ");
						if( y == 1 && x == 1 )
							printf("VSプレイヤー");
					}
					printf("\n");
				}
				printf("\n\n");
				TEXT_BLACK;
				printf("\t\t↑(8)↓(2)：選択　　スペースキー：決定\n");


				if( battlemenu[0][1] == POINTER )		//VSコンピュータ
				{
					whovs = VSCOM;
					Sleep( 100 );
					system("cls");
					battlemenu[by][bx] = EMPTY;
					bx = 0; by = 0;

					break;
				}
				else if( battlemenu[1][1] == POINTER )	//VSプレイヤー
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
					printf("\t\t┏━━━━━━━━━━━━━┓\n");
					printf("\t\t┃対戦モード　VSコンピュータ┃\n");
					printf("\t\t┗━━━━━━━━━━━━━┛\n\n");
					printf("\t\tあなた↓\tコンピュータ↓\n");
					TEXT_IRED;
					printf("\t\t　　　♀");TEXT_IBLUE;printf("\t　　　　　　♂\n\n\n");
					TEXT_RED;
					printf("\t操作方法\n");
					printf("\t━━━━\n");
					TEXT_BLACK;
					printf("\t移動：←(4),→(6)\n");
					printf("\tジャンプ：↑(8)\n\n");
				}
				else if( whovs == VSPLAYER )	/*----------VSPLAYER----------*/
				{
					TEXT_RED;
					printf("\t\t┏━━━━━━━━━━━━┓\n");
					printf("\t\t┃対戦モード　VSプレイヤー┃\n");
					printf("\t\t┗━━━━━━━━━━━━┛\n\n");
					printf("\t\t１Ｐ↓\t２Ｐ↓\n");
					TEXT_IRED;
					printf("\t\t　　♀");TEXT_IBLUE;printf("\t　　♂\n\n\n");
					TEXT_RED;
					printf("\t操作方法\n");
					printf("\t━━━━\n");
					TEXT_BLACK;
					printf("\t＜１Ｐ＞\n");
					printf("\t移動：←(4),→(6)\n");
					printf("\tジャンプ：↑(8)\n");
					printf("\t＜２Ｐ＞\n");
					printf("\t移動：Ａ,Ｄ\n");
					printf("\tジャンプ：Ｗ\n\n");
				}
				printf("\t中断：Escキー\n\n\n");
				printf("\t\t\t\t開始まであと%d秒\n",i);
				Sleep( 1000 );
				system("cls");
			}
			mciSendString("stop bmenu", NULL, 0, NULL);	//BGM停止

			/*　対戦　*/

			/*　初期処理　*/
			messx = 2, messy = 13;	//♀初期配置
			mlife = 3;
			ossx = 27, ossy = 13;	//♂初期配置
			olife = 3;
			battleend = 0;			//中断用変数の初期化
			jumptime = 0;			//ジャンプタイム初期化
			ossjumptime = 0;

			while(1)
			{
				/*　＜参考＞BGMループ再生　*/
				TCHAR BGMStatus[256] = {0};
				mciSendString("status battle mode", BGMStatus, 256, NULL); //BGM状態の取得
				if(lstrcmp(BGMStatus, "stopped") == 0)
					mciSendString("play battle from 0", NULL, 0, NULL); //BGM停止中なら演奏

				messx_old = messx;
				messy_old = messy;

				ossx_old = ossx;
				ossy_old = ossy;

				/*　♀移動　*/
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
								jumptime = 7;	//ジャンプタイム初期化
						}
						else if( whovs == VSPLAYER )	/*----------VSPLAYER----------*/
						{
							if( vsp_battlestage[messy+1][messx] == BROCK )
								jumptime = 7;
						}
						break;
						/*　♂移動　*/	/*----------VSPLAYER----------*/
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
						battleend = 1;	//中断(1)
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
					case 5:	//1/2の確率で♀に近づく
						if( messx < ossx )
							ossx--;
						else if( messx > ossx )
							ossx++;
					}
				}

				/*　ジャンプ処理　*/
				if( jumptime > 3 )	//♀
				{
					messy--;	//跳躍作用
					jumptime--;
				}
				if( ossjumptime > 3 )	//♂
				{
					ossy--;	//跳躍作用
					ossjumptime--;
				}

				if( whovs == VSCOM )	/*----------VSCOM----------*/
				{
					/*　ジャンプ処理　*/
					if( vsc_battlestage[messy+1][messx] != BROCK && jumptime <= 3 )
					{
						messy++;	//重力作用
						jumptime--;
					}
					if( vsc_battlestage[ossy+1][ossx] != BROCK && ossjumptime <= 3 )
					{
						ossy++;	//重力作用
						ossjumptime--;
					}

					/*　移動処理　*/
					vsc_battlestage[messy_old][messx_old] = EMPTY;

					switch( vsc_battlestage[messy][messx] )	//♀壁処理
					{
					case BROCK:
						messx = messx_old;
						messy = messy_old;
						break;
					}
					switch( vsc_battlestage[ossy][ossx] )	//♂壁処理
					{
					case BROCK:
						ossx = ossx_old;
						ossy = ossy_old;
						break;
					}

					ossfield_old = vsc_battlestage[ossy][ossx];	//♂の移動先に何があったかを保存

					/*　判定　*/
					if( vsc_battlestage[messy][messx] == GOAL )
					{
						PlaySound("goalsound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
						vsc_battlestage[messy][messx] = CLEAR;
						olife--;
					}
					else if( messx == ossx && messy == ossy )
					{
						PlaySound("damagesound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
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
					/*　ジャンプ処理　*/
					if( vsp_battlestage[messy+1][messx] != BROCK && jumptime <= 3 )	//重力作用
					{
						messy++;	//重力作用
						jumptime--;
					}
					if( vsp_battlestage[ossy+1][ossx] != BROCK && ossjumptime <= 3 )	//重力作用
					{
						ossy++;	//重力作用
						ossjumptime--;
					}

					/*　移動処理　*/
					vsp_battlestage[messy_old][messx_old] = EMPTY;

					switch( vsp_battlestage[messy][messx] )	//♀壁処理
					{
					case BROCK:
					case OUTEMPTY:
						messx = messx_old;
						messy = messy_old;
						break;
					}
					switch( vsp_battlestage[ossy][ossx] )	//♂壁処理
					{
					case BROCK:
					case OUTEMPTY:
						ossx = ossx_old;
						ossy = ossy_old;
						break;
					}

					ossfield_old = vsp_battlestage[ossy][ossx];	//♂の移動先に何があったかを保存

					/*　判定　*/
					if( vsp_battlestage[messy][messx] == GOAL )
					{
						PlaySound("goalsound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
						vsp_battlestage[messy][messx] = CLEAR;
						olife--;
					}
					else if( messx == ossx && messy == ossy )
					{
						PlaySound("damagesound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
						vsp_battlestage[messy][messx] = DAMAGE;
						mlife--;
					}
					else
					{
						vsp_battlestage[messy][messx] = MESS;
						vsp_battlestage[ossy][ossx] = OSS;
					}
				}

				/*　画面表示　*/
				printf("┏━━━━━┳━━━━━┓\n");
				printf("┃");TEXT_IRED;printf("♀");TEXT_BLACK;printf("LIFE：%2d┃", mlife);TEXT_IBLUE;printf("♂");TEXT_BLACK;printf("LIFE：%2d┃\n", olife);
				printf("┗━━━━━┻━━━━━┛\n");
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
								printf("　");
								break;
							case BROCK:
								TEXT_BLACK;
								printf("■");
								break;
							case MESS:
								TEXT_IRED;
								printf("♀");
								break;
							case OSS:
								TEXT_IBLUE;
								printf("♂");
								break;
							case GOAL:
								TEXT_RED;
								printf("‡");
								break;
							case DAMAGE:
								TEXT_IRED;
								printf("Ｘ");
								Sleep( 300 );
								vsc_battlestage[messy][messx] = EMPTY;
								messx = 2, messy = 13;
								ossx = 27, ossy = 13;
								break;
							case CLEAR:
								TEXT_IRED;
								printf("◎");
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
								printf("　");
								break;
							case BROCK:
								TEXT_BLACK;
								printf("■");
								break;
							case MESS:
								TEXT_IRED;
								printf("♀");
								break;
							case OSS:
								TEXT_IBLUE;
								printf("♂");
								break;
							case GOAL:
								TEXT_RED;
								printf("‡");
								break;
							case DAMAGE:
								TEXT_IRED;
								printf("Ｘ");
								Sleep( 300 );
								vsp_battlestage[messy][messx] = EMPTY;
								messx = 2, messy = 13;
								ossx = 27, ossy = 13;
								break;
							case CLEAR:
								TEXT_IRED;
								printf("◎");
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

				/*　敵がいた場所にあったものを戻す　*/
				if( whovs == VSCOM )
					vsc_battlestage[ossy][ossx] = ossfield_old;
				else if( whovs == VSPLAYER )
					vsp_battlestage[ossy][ossx] = ossfield_old;

				/*　終了　*/
				if( mlife == 0 || olife == 0 || battleend )	//♀,♂のライフが0になるか中断(1)された場合に実行
				{
					mciSendString("stop battle", NULL, 0, NULL);	//BGM停止

					PlaySound("battlefinishsound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音

					for( i = 0; i < 3; i++ )	//点滅
					{
						system("cls");
						Sleep( 500 );
						TEXT_GREEN;
						printf("┏━┓　┳　┏┓┳　┳　┏━┓┳　┳　┃\n");
						printf("┣┫　　┃　┃┃┃　┃　┗━┓┣━┫　┃\n");
						printf("┻　　　┻　┻┗┛　┻　┗━┛┻　┻　★\n");
						Sleep( 500 );
					}

					if( mlife == 0 )
					{
						printf("WINNER:");TEXT_IBLUE;printf("♂\n");
						Sleep( 1000 );
					}
					else if( olife == 0 )
					{
						printf("WINNER:");TEXT_IRED;printf("♀\n");
						Sleep( 1000 );
					}


					if( whovs == VSCOM )	/*----------VSCOM----------*/
					{
						vsp_battlestage[messy][messx] = EMPTY;	//♀がいる座標をEMPTY(0)にする
						vsp_battlestage[ossy][ossx] = EMPTY;	//♂がいる座標をEMPTY(0)にする
					}
					else if( whovs == VSPLAYER )	/*----------VSPLAYER----------*/
					{
						vsp_battlestage[messy][messx] = EMPTY;	//♀がいる座標をEMPTY(0)にする
						vsp_battlestage[ossy][ossx] = EMPTY;	//♂がいる座標をEMPTY(0)にする
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
	//	ストーリー
	//=================================================================================================
	while( startnum == STORY )
	{
		/*　星の数数え上げ　*/
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

		/*　初期化処理　*/
		jumptime = 0;	//ジャンプタイム初期化
		m1jumptime = 0;
		m2jumptime = 0;
		m3jumptime = 0;
		savecheck = 0;	//セーブ初期化
		if( stagenum == 1 )	//ライフ初期化
			mlife = 5;

		/*　初期配置　*/
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
			/*　＜参考＞BGMループ再生　*/
			if( stagenum == 1 )
			{
				TCHAR BGMStatus[256] = {0};
				mciSendString("status story1 mode", BGMStatus, 256, NULL); //BGM状態の取得
				if(lstrcmp(BGMStatus, "stopped") == 0)
					mciSendString("play story1 from 0", NULL, 0, NULL); //BGM停止中なら演奏
			}
			else if( stagenum == 2 )
			{
				TCHAR BGMStatus[256] = {0};
				mciSendString("status story2 mode", BGMStatus, 256, NULL); //BGM状態の取得
				if(lstrcmp(BGMStatus, "stopped") == 0)
					mciSendString("play story2 from 0", NULL, 0, NULL); //BGM停止中なら演奏
			}
			else if( stagenum == 3 )
			{
				TCHAR BGMStatus[256] = {0};
				mciSendString("status story3 mode", BGMStatus, 256, NULL); //BGM状態の取得
				if(lstrcmp(BGMStatus, "stopped") == 0)
					mciSendString("play story3 from 0", NULL, 0, NULL); //BGM停止中なら演奏
			}

			/*　♀移動　*/
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
			/*　敵移動　*/
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
				case 3:	//1/4の確率で♀に近づく
					if( messx < ossx )
						ossx--;
					else if( messx > ossx )
						ossx++;
				}
				r3 = rand() % 3;
				switch( r3 )	//敵3
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
				switch( r2 )	//敵2
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
				switch( r1 )	//敵1
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

			/*　ジャンプ処理　*/
			if( jumptime > 3 )	//♀ジャンプ
			{
				messy--;	//跳躍作用
				jumptime--;
			}
			if( ossjumptime > 3 )	//♂ジャンプ
			{
				ossy--;	//跳躍作用
				ossjumptime--;
			}
			if( m1jumptime > 2 )	//敵1ジャンプ
			{
				m1y--;	//跳躍作用
				m1jumptime--;
			}
			if( m2jumptime > 2 )	//敵2ジャンプ
			{
				m2y--;	//跳躍作用
				m2jumptime--;
			}
			if( m3jumptime > 2 )	//敵3ジャンプ
			{
				m3y--;	//跳躍作用
				m3jumptime--;
			}

			if( stagenum == 1 )	/*----------STAGE1----------*/
			{
				/*　ジャンプ処理　*/
				if( stage1[messy+1][messx] != BROCK && jumptime <= 3 )
				{
					messy++;	//重力作用
					jumptime--;
				}
				if( stage1[m1y+1][m1x] != BROCK && m1jumptime <= 2 )
				{
					m1y++;	//重力作用
					m1jumptime--;
				}

				/*　♀処理　*/
				stage1[messy_old][messx_old] = EMPTY;

				switch( stage1[messy][messx] )
				{
				case BROCK:	//壁処理
					messx = messx_old;
					messy = messy_old;
					break;
				case STER:	//得点
					PlaySound("getstersound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
					point += 10;
					getster++;
					break;
				case SSTER:	//得点
					PlaySound("getsstersound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
					point += 100;
					getster++;
					break;
				case SAVE:	//セーブポイント
					PlaySound("savesound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
					savecheck = 1;
					savex = 17, savey = 7;
					break;
				}

				/*　敵処理　*/
				if( stage1[m1y][m1x] == BROCK )	//敵1壁処理
				{
					m1x = m1x_old;
					m1y = m1y_old;
				}

				m1field_old = stage1[m1y][m1x];	//敵の移動先に何があるかを保存
				if( m1field_old == MESS )		//敵の移動先が♀だった場合、EMPTY(0)を代入
					m1field_old = EMPTY;
				stage1[m1y][m1x] = MONSTER;

				if( stage1[messy][messx] == NEEDLE || stage1[messy][messx] == MONSTER )	//当たり判定
				{
					PlaySound("damagesound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
					stage1[messy][messx] = DAMAGE;
				}
				else if( stage1[messy][messx] == GOAL )	//ゴール判定
				{
					PlaySound("goalsound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
					stage1[messy][messx] = CLEAR;
				}
				else
					stage1[messy][messx] = MESS;
			}
			else if( stagenum == 2 )	/*----------STAGE2----------*/
			{
				/*　ジャンプ処理　*/
				if( stage2[messy+1][messx] != BROCK && stage2[messy+1][messx] != FROOR && jumptime <= 3 )
				{
					messy++;	//重力作用
					jumptime--;
				}
				if( stage2[m1y+1][m1x] != BROCK && m1jumptime <= 2 )
				{
					m1y++;	//重力作用
					m1jumptime--;
				}
				if( stage2[m2y+1][m2x] != BROCK && m2jumptime <= 2 )
				{
					m2y++;	//重力作用
					m2jumptime--;
				}

				/*　♀処理　*/
				stage2[messy_old][messx_old] = EMPTY;

				switch( stage2[messy][messx] )
				{
				case BROCK:	//壁処理
				case FROOR:
					messx = messx_old;
					messy = messy_old;
					break;
				case STER:	//得点
					PlaySound("getstersound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
					point += 10;
					getster++;
					break;
				case SSTER:	//得点
					PlaySound("getsstersound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
					point += 100;
					getster++;
					break;
				case SAVE:	//セーブポイント
					PlaySound("savesound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
					savecheck = 1;
					savex = 22, savey = 13;
					break;
				}

				/*　敵処理　*/
				if( stage2[m1y][m1x] == BROCK || stage2[m1y][m1x] == OUTEMPTY )	//敵1壁処理
				{
					m1x = m1x_old;
					m1y = m1y_old;
				}
				if( stage2[m2y][m2x] == BROCK || stage2[m2y][m2x] == OUTEMPTY )	//敵2壁処理
				{
					m2x = m2x_old;
					m2y = m2y_old;
				}

				m1field_old = stage2[m1y][m1x];	//敵の移動先に何があるかを保存
				if( m1field_old == MESS )		//敵の移動先が♀だった場合、EMPTY(0)を代入
					m1field_old = EMPTY;
				m2field_old = stage2[m2y][m2x];	//敵の移動先に何があるかを保存
				if( m2field_old == MESS )		//敵の移動先が♀だった場合、EMPTY(0)を代入
					m2field_old = EMPTY;

				stage2[m1y][m1x] = MONSTER;
				stage2[m2y][m2x] = MONSTER;

				/*　判定　*/
				if( stage2[messy][messx] == NEEDLE || stage2[messy][messx] == MONSTER )	//当たり判定
				{
					PlaySound("damagesound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
					stage2[messy][messx] = DAMAGE;
				}
				else if( stage2[messy][messx] == GOAL )	//ゴール判定
				{
					PlaySound("goalsound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
					stage2[messy][messx] = CLEAR;
				}
				else
					stage2[messy][messx] = MESS;

				/*　フロア処理　*/
				if( froorcount % 5 == 0 )	//0.5秒刻みで動く
				{
					//移動チェック
					if( stage2[11][froor1x-2] == BROCK )
						froor1check = 1;
					else if( stage2[11][froor1x+2] == BROCK )
						froor1check = -1;

					//移動処理
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
				/*　ジャンプ処理　*/
				if( stage3[messy+1][messx] != BROCK && stage3[messy+1][messx] != FROOR && jumptime <= 3 )
				{
					messy++;	//重力作用
					jumptime--;
				}
				if( stage3[ossy+1][ossx] != BROCK && stage3[ossy+1][ossx] != FROOR && ossjumptime <= 3 )
				{
					ossy++;	//重力作用
					ossjumptime--;
				}
				if( stage3[m1y+1][m1x] != BROCK && stage3[m1y+1][m1x] != FROOR && m1jumptime <= 2 )
				{
					m1y++;	//重力作用
					m1jumptime--;
				}
				if( stage3[m2y+1][m2x] != BROCK && stage3[m2y+1][m2x] != FROOR && m2jumptime <= 2 )
				{
					m2y++;	//重力作用
					m2jumptime--;
				}
				if( stage3[m3y+1][m3x] != BROCK && stage3[m3y+1][m3x] != FROOR && m3jumptime <= 2 )
				{
					m3y++;	//重力作用
					m3jumptime--;
				}

				/*　♀処理　*/
				stage3[messy_old][messx_old] = EMPTY;

				switch( stage3[messy][messx] )
				{
				case BROCK:	//壁処理
				case FROOR:
					messx = messx_old;
					messy = messy_old;
					break;
				case STER:	//得点
					PlaySound("getstersound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
					point += 10;
					getster++;
					break;
				case SSTER:	//得点
					PlaySound("getsstersound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
					point += 100;
					getster++;
					break;
				case SAVE:	//セーブポイント
					PlaySound("savesound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
					savecheck = 1;
					savex = 26, savey = 3;
					break;
				}

				/*　敵処理　*/
				if( stage3[ossy][ossx] == BROCK || stage3[ossy][ossx] == FROOR )	//♂壁処理
				{
					ossx = ossx_old;
					ossy = ossy_old;
				}
				if( stage3[m1y][m1x] == BROCK || stage3[m1y][m1x] == FROOR || stage3[m1y][m1x] == OUTEMPTY )	//敵1壁処理
				{
					m1x = m1x_old;
					m1y = m1y_old;
				}
				if( stage3[m2y][m2x] == BROCK || stage3[m2y][m2x] == FROOR || stage3[m2y][m2x] == OUTEMPTY )	//敵2壁処理
				{
					m2x = m2x_old;
					m2y = m2y_old;
				}
				if( stage3[m3y][m3x] == BROCK || stage3[m3y][m3x] == FROOR || stage3[m2y][m2x] == OUTEMPTY )	//敵3壁処理
				{
					m3x = m3x_old;
					m3y = m3y_old;
				}

				ossfield_old = stage3[ossy][ossx];	//敵の移動先に何があるかを保存
				if( ossfield_old == MESS )		//敵の移動先が♀だった場合、EMPTY(0)を代入
					ossfield_old = EMPTY;
				m1field_old = stage3[m1y][m1x];	//敵の移動先に何があるかを保存
				if( m1field_old == MESS )		//敵の移動先が♀だった場合、EMPTY(0)を代入
					m1field_old = EMPTY;
				m2field_old = stage3[m2y][m2x];	//敵の移動先に何があるかを保存
				if( m2field_old == MESS )		//敵の移動先が♀だった場合、EMPTY(0)を代入
					m2field_old = EMPTY;
				m3field_old = stage3[m3y][m3x];	//敵の移動先に何があるかを保存
				if( m3field_old == MESS )		//敵の移動先が♀だった場合、EMPTY(0)を代入
					m3field_old = EMPTY;

				stage3[ossy][ossx] = OSS;
				stage3[m1y][m1x] = MONSTER;
				stage3[m2y][m2x] = MONSTER;
				stage3[m3y][m3x] = MONSTER;

				/*　判定　*/
				if( stage3[messy][messx] == NEEDLE || stage3[messy][messx] == MONSTER || stage3[messy][messx] == OSS )	//当たり判定
				{
					PlaySound("damagesound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
					stage3[messy][messx] = DAMAGE;
				}
				else if( stage3[messy][messx] == GOAL )	//ゴール判定
				{
					PlaySound("goalsound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
					stage3[messy][messx] = CLEAR;
				}
				else
					stage3[messy][messx] = MESS;

				/*　フロア処理　*/
				if( froorcount % 5 == 0 )	//0.5秒刻みで動く
				{
					//移動チェック
					if( stage3[12][froor2x-2] == BROCK )
						froor2check = 1;
					else if( stage3[12][froor2x+2] == BROCK )
						froor2check = -1;

					//移動処理
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

					//移動チェック
					if( stage3[2][froor3x-3] == BROCK )
						froor3check = 1;
					else if( stage3[2][froor3x+2] == BROCK )
						froor3check = -1;

					//移動処理
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

			/*　画面表示　*/
			printf("┏━━━━━┳━━━━┓\n");
			printf("┃POINT：%3d┃LIFE：%2d┃\n",point, mlife);
			printf("┗━━━━━┻━━━━┛\n");
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
							printf("　");
							break;
						case BROCK:
							TEXT_BLACK;
							printf("■");
							break;
						case MESS:
							TEXT_IRED;
							printf("♀");
							break;
						case MONSTER:
							TEXT_IBLUE;
							printf("ё");
							break;
						case STER:
							TEXT_GREEN;
							printf("☆");
							break;
						case SSTER:
							TEXT_GREEN;
							printf("★");
							break;
						case NEEDLE:
							TEXT_BLACK;
							printf("Λ");
							break;
						case SAVE:
							TEXT_IGREEN;
							printf("†");
							break;
						case GOAL:
							TEXT_RED;
							printf("‡");
							break;
						case DAMAGE:
							TEXT_IRED;
							printf("Ｘ");
							mlife--;
							stage1[messy][messx] = NEEDLE;
							if( savecheck == 1 )
								messx = savex, messy = savey;
							else
								messx = 2, messy = 12;
							break;
						case CLEAR:
							TEXT_IRED;
							printf("◎");
							break;
						}
					}
					else if( stagenum == 2 )	/*----------STAGE2----------*/
					{
						switch( stage2[y][x] )
						{
						case EMPTY:
						case OUTEMPTY:
							printf("　");
							break;
						case BROCK:
							TEXT_BLACK;
							printf("■");
							break;
						case MESS:
							TEXT_IRED;
							printf("♀");
							break;
						case MONSTER:
							TEXT_IBLUE;
							printf("ё");
							break;
						case STER:
							TEXT_GREEN;
							printf("☆");
							break;
						case SSTER:
							TEXT_GREEN;
							printf("★");
							break;
						case NEEDLE:
							TEXT_BLACK;
							printf("Λ");
							break;
						case FROOR:
							TEXT_BLACK;
							printf("━");
							break;
						case SAVE:
							TEXT_IGREEN;
							printf("†");
							break;
						case GOAL:
							TEXT_RED;
							printf("‡");
							break;
						case DAMAGE:
							TEXT_IRED;
							printf("Ｘ");
							mlife--;
							stage2[messy][messx] = NEEDLE;
							if( savecheck == 1 )
								messx = savex, messy = savey;
							else
								messx = 1, messy = 13;
							break;
						case CLEAR:
							TEXT_IRED;
							printf("◎");
							break;
						}
					}
					else if( stagenum == 3 )	/*----------STAGE3----------*/
					{
						switch( stage3[y][x] )
						{
						case EMPTY:
						case OUTEMPTY:
							printf("　");
							break;
						case BROCK:
							TEXT_BLACK;
							printf("■");
							break;
						case MESS:
							TEXT_IRED;
							printf("♀");
							break;
						case OSS:
							TEXT_IBLUE;
							printf("♂");
							break;
						case MONSTER:
							TEXT_IBLUE;
							printf("ё");
							break;
						case STER:
							TEXT_GREEN;
							printf("☆");
							break;
						case SSTER:
							TEXT_GREEN;
							printf("★");
							break;
						case NEEDLE:
							TEXT_BLACK;
							printf("Λ");
							break;
						case FROOR:
							TEXT_BLACK;
							printf("━");
							break;
						case SAVE:
							TEXT_IGREEN;
							printf("†");
							break;
						case GOAL:
							TEXT_RED;
							printf("‡");
							break;
						case DAMAGE:
							TEXT_IRED;
							printf("Ｘ");
							mlife--;
							stage3[messy][messx] = NEEDLE;
							if( savecheck == 1 )
								messx = savex, messy = savey;
							else
								messx = 2, messy = 11;
							break;
						case CLEAR:
							TEXT_IRED;
							printf("◎");
							break;
						}
					}
				}
				printf("\n");
			}
			printf("\n");

			/*　敵がいる場所にもともとあったものを戻す処理　*/
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

			/*　ゲームオーバー　*/
			if( mlife == 0 )
			{
				if( stagenum == 1 )
					mciSendString("stop story1", NULL, 0, NULL);	//BGM停止
				else if( stagenum == 2 )
					mciSendString("stop story2", NULL, 0, NULL);	//BGM停止
				else if( stagenum == 3 )
					mciSendString("stop story3", NULL, 0, NULL);	//BGM停止

				PlaySound("gameoversound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音

				for( i = 0; i < 3; i++ )	//点滅
				{
					system("cls");
					Sleep( 500 );
					TEXT_IBLUE;
					printf("┏━┓┏━┓┏┳┓┏━┫　┏━┓┳　┳┏━┫┏━┓┃\n");
					printf("┃　┓┣━┫┃┃┃┣━　　┃　┃┃　┃┣━　┣┳┛┃\n");
					printf("┗━┛┻　┻┻　┻┗━┫　┗━┛┗━┛┗━┫┻┗┛★\n");
					Sleep( 500 );
					startnum = EXIT;
				}
				break;	//繰り返しから抜ける
			}

			/*　ステージクリア　*/
			if( stagenum == 1 )
			{
				if( stage1[messy][messx] == CLEAR )
				{
					Sleep( 2500 );
					system("cls");
					mciSendString("stop story1", NULL, 0, NULL);	//BGM停止
					stagenum = 2;
					break;	//繰り返しから抜ける
				}
			}
			else if( stagenum == 2 )
			{
				if( stage2[messy][messx] == CLEAR )
				{
					Sleep( 2500 );
					system("cls");
					mciSendString("stop story2", NULL, 0, NULL);	//BGM停止
					stagenum = 3;
					break;	//繰り返しから抜ける
				}
			}
			else if( stagenum == 3 )
			{
				if( stage3[messy][messx] == CLEAR )
				{
					Sleep( 2500 );
					system("cls");
					mciSendString("stop story3", NULL, 0, NULL);	//BGM停止
					startnum = EXIT;
					finish = 1;	//ゲームクリア
					break;	//繰り返しから抜ける
				}
			}

			Sleep( 100 );
			system("cls");

			if( stagenum == 2 || stagenum == 3 )	//フロアカウントを1ずつ増やす
				froorcount++;
		}
	}

	/*　BGMファイルクローズ　*/
	mciSendString("close title", NULL, 0, NULL);	//タイトル画面
	mciSendString("close rule", NULL, 0, NULL);		//ルール説明
	mciSendString("close bmenu", NULL, 0, NULL);	//対戦メニュー
	mciSendString("close battle", NULL, 0, NULL);	//対戦
	mciSendString("close story1", NULL, 0, NULL);	//ストーリー1
	mciSendString("close story2", NULL, 0, NULL);	//ストーリー2
	mciSendString("close story3", NULL, 0, NULL);	//ストーリー3

	PlaySound("finishsound.wav", NULL, SND_FILENAME | SND_ASYNC);	//効果音
	TEXT_IRED;
	printf("　♀　　　　　");TEXT_IBLUE;printf("♂\n");
	TEXT_GREEN;
	printf("┏━┫┏┓┳┳━┓\n");
	printf("┣━　┃┃┃┃　┃\n");
	printf("┗━┫┻┗┛┻━┛\n\n");
	Sleep( 1500 );

	if( finish )
	{
		TEXT_BLACK;
		printf("┏━━━━━━━━━━━┓\n");
		printf("┃獲得した");TEXT_GREEN;printf("☆");TEXT_BLACK;printf("の数：%2d／%2d┃\n", getster, allster);
		printf("┃POINT：%3d　　　　　　┃\n", point);
		printf("┗━━━━━━━━━━━┛\n\n");
	}

	return 0;
}

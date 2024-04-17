#include "DxLib.h"
#include "Common.h"
#include "Game.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ウィンドウモードに設定
	ChangeWindowMode(TRUE);

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1) { return -1; }

	// ウィンドウのタイトル
	SetWindowText("TETRAS C"); 

	// ウィンドウの解像度
	SetGraphMode(WINDOW_W, WINDOW_H, 32);

	// ウィンドウリサイズ可能にする
	SetWindowSizeChangeEnableFlag(TRUE, TRUE);

	// ウィンドウの背景色
	SetBackgroundColor(0xFF, 0, 0xFF);

	// 透過色を設定しておきます。マゼンタです。
	SetTransColor(0xFF, 0, 0xFF);

	// 裏画面描画
	SetDrawScreen(DX_SCREEN_BACK);

	while ((ScreenFlip() == 0) && (ClearDrawScreen != 0) && (ProcessMessage() == 0))
	{
		// 画面を初期化
		ClearDrawScreen();

		updateGame();
		if (Game() == FALSE) { break; }
	}

	// ＤＸライブラリ使用の終了処理
	DxLib_End();

	// ソフトの終了
	return 0;
}


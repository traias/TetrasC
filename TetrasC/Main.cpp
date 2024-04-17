#include "DxLib.h"
#include "Common.h"
#include "Game.h"

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �E�B���h�E���[�h�ɐݒ�
	ChangeWindowMode(TRUE);

	// �c�w���C�u��������������
	if (DxLib_Init() == -1) { return -1; }

	// �E�B���h�E�̃^�C�g��
	SetWindowText("TETRAS C"); 

	// �E�B���h�E�̉𑜓x
	SetGraphMode(WINDOW_W, WINDOW_H, 32);

	// �E�B���h�E���T�C�Y�\�ɂ���
	SetWindowSizeChangeEnableFlag(TRUE, TRUE);

	// �E�B���h�E�̔w�i�F
	SetBackgroundColor(0xFF, 0, 0xFF);

	// ���ߐF��ݒ肵�Ă����܂��B�}�[���^�ł��B
	SetTransColor(0xFF, 0, 0xFF);

	// ����ʕ`��
	SetDrawScreen(DX_SCREEN_BACK);

	while ((ScreenFlip() == 0) && (ClearDrawScreen != 0) && (ProcessMessage() == 0))
	{
		// ��ʂ�������
		ClearDrawScreen();

		updateGame();
		if (Game() == FALSE) { break; }
	}

	// �c�w���C�u�����g�p�̏I������
	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}


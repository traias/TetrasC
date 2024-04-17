#include "DxLib.h"
#include "Common.h"

/// <summary>
/// �Q�[���̏�����
/// </summary>
/// <returns>����</returns>
BOOL Init(void)
{
	// �t�H���g�̃��[�h
	LPCSTR font_path = "JF-Dot-MPlus10.ttf";
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) == -1)
	{
		// �t�H���g�Ǎ��G���[
		MessageBox(NULL, "�t�H���g�Ǎ����s", "", MB_OK);
		return FALSE;
	}
	ChangeFont("JF�h�b�gM+10", DX_CHARSET_DEFAULT);

	return TRUE;
}


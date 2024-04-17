#include "DxLib.h"
#include "Common.h"

/// <summary>
/// ゲームの初期化
/// </summary>
/// <returns>成功</returns>
BOOL Init(void)
{
	// フォントのロード
	LPCSTR font_path = "JF-Dot-MPlus10.ttf";
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) == -1)
	{
		// フォント読込エラー
		MessageBox(NULL, "フォント読込失敗", "", MB_OK);
		return FALSE;
	}
	ChangeFont("JFドットM+10", DX_CHARSET_DEFAULT);

	return TRUE;
}


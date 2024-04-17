#pragma once

///　キーの入力定数です。
#define INPUT_U         (1 << 0)
#define INPUT_D         (1 << 1)
#define INPUT_L         (1 << 2)
#define INPUT_R         (1 << 3)
#define INPUT_A         (1 << 4)
#define INPUT_B         (1 << 5)
#define INPUT_H         (1 << 6)
#define INPUT_ESC       (1 << 7)
#define INPUT_ENTER     (1 << 8)


/// <summary>
/// キーボード入力を管理します。
/// </summary>
extern void updateKey(void);

/// <summary>
/// キー入力をトリガとして取得します。
/// </summary>
/// <returns>トリガキー</returns>
extern int getKeyTrigger(void);

/// <summary>
/// キー入力を状態として取得します。
/// </summary>
/// <returns>キー状態</returns>
extern int getKeyState(void);

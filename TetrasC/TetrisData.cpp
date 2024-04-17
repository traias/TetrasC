#include <stdlib.h>
#include <time.h>
#include "DxLib.h"
#include "Common.h"
#include "Tetris.h"
#include "TetrisData.h"





/// NEXTを取り出します。
static void getNext(TETRIS_DATA* tet);

/// TSpinかどうかの判定を行います。
/// 1はTSpin、2はTSpinMiniです
static int CheckTSpin(MINO_TYPE* board, MINO_TYPE current, POINT* currentPoint, int r, bool spin, int srs);

/// 送信ライン数の計算を行います。
static int CheckSendline(MINO_TYPE* board, int deleteLine, int tSpinType, int ren, BOOL btb);





/// <summary>
/// テトリスデータを初期化します。
/// </summary>
/// <param name="tet"></param>
/// <returns></returns>
void newTetrisData(TETRIS_DATA* tet)
{
    /// ボードをクリアしておきます
    for (int x = 0; x < BOARD_W; x++)
    {
        for (int y = 0; y < BOARD_H; y++)
        {
            tet->board[x + (y * BOARD_W)] = N;
        }
    }

    tet->holdEnable = TRUE;
    tet->hold = N;

    tet->control.prevSrs = 0;
    tet->control.prevSpin = 0;
    tet->control.btb = FALSE;
    tet->control.ren = 0;
    tet->control.delay = 0;

    tet->prevControl.current = N;
    tet->prevControl.pos = { 0, 0 };
    tet->prevControl.r = 0;
    tet->prevControl.prevSrs = 0;
    tet->prevControl.prevSpin = 0;
    tet->prevControl.btb = FALSE;
    tet->prevControl.ren = 0;
    tet->prevControl.delay = 0;

    tet->prevResult.deleteLine = 0;
    tet->prevResult.sendLine = 0;
    tet->prevResult.Tsm = 0;
    tet->prevResult.Tsmd = 0;
    tet->prevResult.Tss = 0;
    tet->prevResult.Tsd = 0;
    tet->prevResult.Tst = 0;
    tet->prevResult.Tetris = 0;
    tet->prevResult.Pc = 0;
    tet->prevResult.Delay = 0;

    tet->result.deleteLine = 0;
    tet->result.sendLine = 0;
    tet->result.Tsm = 0;
    tet->result.Tsmd = 0;
    tet->result.Tss = 0;
    tet->result.Tsd = 0;
    tet->result.Tst = 0;
    tet->result.Tetris = 0;
    tet->result.Pc = 0;
    tet->result.Delay = 0;

    for (tet->bagTarget = 0; tet->bagTarget < NEXT_MAX; tet->bagTarget++)
    {
        tet->next[tet->bagTarget] = getBagMino(tet->bagTarget);
    }

    /// 新しいミノを取り出します。
    BOOL result = newMino(tet);
}

/// <summary>
/// 新しいミノを取り出します。
/// </summary>
/// <param name="tet"></param>
/// <returns></returns>
BOOL newMino(TETRIS_DATA* tet)
{
    // 前回の捜査情報をとっておきます。
    memcpy_s(&tet->prevControl, sizeof(CONTROL), &tet->control, sizeof(CONTROL));
    tet->control.prevSpin = FALSE;
    tet->control.delay = 0;

    /// とりあえずカレントをNEXTから取り出す。
    getNext(tet);
    tet->holdEnable = TRUE;

    /// カレントの位置を設定します。
    BOOL result = GetMinoPisitionStart(tet->board, tet->control.current, &tet->control.pos);
    tet->control.r = 0;

    return result;
}

/// <summary>
/// 設置後の処理です。
/// </summary>
/// <param name="tet"></param>
/// <returns></returns>
void finishDrop(TETRIS_DATA* tet)
{
    /// T-Spin判定を行います。
    int tSpinType = CheckTSpin(tet->board, tet->control.current, &tet->control.pos, tet->control.r, tet->control.prevSpin, tet->control.prevSrs);

    /// ラインの消去を行います。
    tet->prevResult.deleteLine = CorrectBoard(tet->board, FALSE);
    tet->result.deleteLine += tet->prevResult.deleteLine;

    /// 火力数を計算します。
    tet->prevResult.sendLine = CheckSendline(tet->board, tet->prevResult.deleteLine, tSpinType, tet->control.ren, tet->control.btb);
    tet->result.sendLine += tet->prevResult.sendLine;

    /// btbを設定します。
    if (tet->prevResult.deleteLine > 0)
    {
        if (tSpinType != 0)
        {
            tet->control.btb = TRUE;
        }
        else if (tet->prevResult.deleteLine >= 4)
        {
            tet->control.btb = TRUE;
        }
        else
        {
            tet->control.btb = TRUE;
        }
    }

    /// renを設定します。
    if (tet->prevResult.deleteLine > 0)
    {
        tet->control.ren++;
    }
    else
    {
        tet->control.ren = 0;
    }

    /// 結果データの設定を行います。
    tet->prevResult.Tsm = 0;
    tet->prevResult.Tsmd = 0;
    tet->prevResult.Tss = 0;
    tet->prevResult.Tsd = 0;
    tet->prevResult.Tst = 0;
    tet->prevResult.Tetris = 0;
    tet->prevResult.Pc = 0;
    if (tet->prevResult.sendLine >= 10)
    {
        tet->prevResult.Pc = 1;
        tet->result.Pc++;
        tet->control.delay++;
    }
    if ((tSpinType == 2) && (tet->prevResult.deleteLine == 1))
    {
        tet->prevResult.Tsm = 1;
        tet->result.Tsm++;
        tet->control.delay += 40;
    }
    if ((tSpinType == 2) && (tet->prevResult.deleteLine == 2))
    {
        tet->prevResult.Tsmd = 1;
        tet->result.Tsmd++;
        tet->control.delay += 45;
    }
    if ((tSpinType == 1) && (tet->prevResult.deleteLine == 1))
    {
        tet->prevResult.Tss = 1;
        tet->result.Tss++;
        tet->control.delay += 40;
    }
    if ((tSpinType == 1) && (tet->prevResult.deleteLine == 2))
    {
        tet->prevResult.Tsd = 1;
        tet->result.Tsd++;
        tet->control.delay += 45;
    }
    if ((tSpinType == 1) && (tet->prevResult.deleteLine == 3))
    {
        tet->prevResult.Tst = 1;
        tet->result.Tst++;
        tet->control.delay += 45;
    }
    if (tet->prevResult.deleteLine == 4)
    {
        tet->prevResult.Tetris = 1;
        tet->result.Tetris++;
        tet->control.delay += 50;
    }
    tet->prevResult.Delay = tet->control.delay;
    tet->result.Delay += tet->control.delay;
}

/// <summary>
/// ミノの形状を考慮した開始座標を取得します
/// </summary>
/// <returns></returns>
BOOL GetMinoPisitionStart(MINO_TYPE* board, MINO_TYPE current, POINT* pos)
{
    POINT currrentPoint = { 3, 20 };

    /// 20段目でぶつかっていなかったら、出現成功
    POINT CellPoints[4];
    GetCellPoints(current, &currrentPoint, 0, CellPoints);

    if (CheckCollision(board, CellPoints) == FALSE)
    {
        pos->x = currrentPoint.x;
        pos->y = currrentPoint.y;
        return TRUE;
    }

    /// 21段目の出現をチェックします。
    currrentPoint.y++;

    /// 21段目でぶつかっていなかったら、出現成功
    GetCellPoints(current, &currrentPoint, 0, CellPoints);
    if (CheckCollision(board, CellPoints) == FALSE)
    {
        pos->x = currrentPoint.x;
        pos->y = currrentPoint.y;
        return TRUE;
    }

    /// 死亡
    return FALSE;
}

/// <summary>
/// ホールドを取り出す動きです。
/// </summary>
BOOL Hold(TETRIS_DATA* tet)
{
    if (tet->holdEnable == FALSE)
    {
        return FALSE;
    }

    /// ホールドとカレントを入れ替えます。
    MINO_TYPE temp = tet->control.current;
    tet->control.current = tet->hold;
    tet->hold = temp;

    /// カレントが空の場合はNEXTから取り出します。
    if (tet->control.current == 0)
    {
        getNext(tet);
    }

    /// カレントの位置を設定します。
    BOOL result = GetMinoPisitionStart(tet->board, tet->control.current, &tet->control.pos);
    if (result == FALSE)
    {
        return FALSE;
    }

    tet->holdEnable = false;
    tet->control.delay += 3;


    return TRUE;
}

/// <summary>
/// カレントミノを左に動かします。
/// </summary>
/// <returns></returns>
BOOL MoveLeft(TETRIS_DATA* tet)
{
    POINT movePoint = { tet->control.pos.x, tet->control.pos.y };
    movePoint.x--;
    POINT dest_points[4];
    GetCellPoints(tet->control.current, &movePoint, tet->control.r, dest_points);

    BOOL result = CheckCollision(tet->board, dest_points);

    if (result == TRUE)
    {
        return FALSE;
    }

    tet->control.pos = { movePoint.x, movePoint.y };
    tet->control.prevSrs = 0;
    tet->control.prevSpin = FALSE;
    tet->control.delay++;

    return TRUE;
}

/// <summary>
/// カレントミノを右に動かします。
/// </summary>
/// <returns></returns>
BOOL MoveRight(TETRIS_DATA* tet)
{
    POINT movePoint = { tet->control.pos.x, tet->control.pos.y };
    movePoint.x++;
    POINT dest_points[4];
    GetCellPoints(tet->control.current, &movePoint, tet->control.r, dest_points);

    BOOL result = CheckCollision(tet->board, dest_points);

    if (result == TRUE)
    {
        return FALSE;
    }

    tet->control.pos = { movePoint.x, movePoint.y };
    tet->control.prevSrs = 0;
    tet->control.prevSpin = FALSE;
    tet->control.delay++;

    return TRUE;
}

/// <summary>
/// カレントミノを下に動かします。
/// </summary>
/// <returns></returns>
BOOL MoveDown(TETRIS_DATA* tet)
{
    POINT movePoint = { tet->control.pos.x, tet->control.pos.y };
    movePoint.y--;
    POINT dest_points[4];
    GetCellPoints(tet->control.current, &movePoint, tet->control.r, dest_points);

    BOOL result = CheckCollision(tet->board, dest_points);

    if (result == TRUE)
    {
        return FALSE;
    }

    tet->control.pos = { movePoint.x, movePoint.y };
    tet->control.prevSrs = 0;
    tet->control.prevSpin = FALSE;
    tet->control.delay += 3;

    return TRUE;
}

/// <summary>
/// カレントミノを最下段に動かします。
/// </summary>
/// <returns></returns>
int MoveDD(TETRIS_DATA* tet)
{
    POINT movePoint;
    int dropCount = getHarddropPos(tet, &movePoint);

    tet->control.pos = { movePoint.x, movePoint.y };
    if (dropCount > 0)
    {
        tet->control.prevSrs = 0;
        tet->control.prevSpin = FALSE;
    }
    return dropCount;
}

/// <summary>
/// カレントミノをAに動かします。
/// </summary>
/// <returns></returns>
BOOL MoveA(TETRIS_DATA* tet)
{
    if (tet->control.current == O)
    {
        return FALSE;
    }

    POINT movePoint = { tet->control.pos.x, tet->control.pos.y };
    tet->control.prevSrs = GetSuperRotateMinoPoint(tet->board, tet->control.current, &movePoint, tet->control.r, 1);

    /// 回転できなかった場合です。
    if (tet->control.prevSrs < 0)
    {
        tet->control.prevSpin = FALSE;
        return FALSE;
    }

    /// ミノを回転させます。
    tet->control.r += 1;
    if (tet->control.r >= 4)
    {
        tet->control.r = 0;
    }
    tet->control.pos = { movePoint.x, movePoint.y };
    tet->control.prevSpin = TRUE;
    tet->control.delay++;

    return TRUE;
}

/// <summary>
/// カレントミノをBに動かします。
/// </summary>
/// <returns></returns>
BOOL MoveB(TETRIS_DATA* tet)
{
    if (tet->control.current == O)
    {
        return FALSE;
    }

    POINT movePoint = { tet->control.pos.x, tet->control.pos.y };
    tet->control.prevSrs = GetSuperRotateMinoPoint(tet->board, tet->control.current, &movePoint, tet->control.r, -1);

    /// 回転できなかった場合です。
    if (tet->control.prevSrs < 0)
    {
        tet->control.prevSpin = FALSE;
        return FALSE;
    }

    /// ミノを回転させます。
    tet->control.r -= 1;
    if (tet->control.r < 0)
    {
        tet->control.r = 3;
    }
    tet->control.pos = { movePoint.x, movePoint.y };
    tet->control.prevSpin = TRUE;
    tet->control.delay++;

    return TRUE;
}

/// <summary>
/// カレントミノを設置します。
/// </summary>
/// <returns></returns>
BOOL MovePut(TETRIS_DATA* tet)
{
    POINT movePoint;
    int dropCount = getHarddropPos(tet, &movePoint);

    tet->control.pos = { movePoint.x, movePoint.y };
    if (dropCount > 0)
    {
        tet->control.prevSrs = 0;
        tet->control.prevSpin = FALSE;
    }

    putBoard(tet->board, tet->control.current, tet->control.r, &movePoint);
    tet->control.delay++;

    return TRUE;
}

/// <summary>
/// ゴーストのセル位置を取得します。
/// </summary>
void getGhostCell(TETRIS_DATA* tet, POINT* cells)
{
    POINT movePoint;
    getHarddropPos(tet, &movePoint);
    GetCellPoints(tet->control.current, &movePoint, tet->control.r, cells);
}

/// <summary>
/// ハードドロップした場所を取得します。
/// </summary>
/// <param name="tets"></param>
int getHarddropPos(TETRIS_DATA* tet, POINT* pos)
{
    POINT movePoint = { tet->control.pos.x, tet->control.pos.y };
    BOOL result = FALSE;
    int dropCount = 0;

    POINT dest_points[4];
    GetCellPoints(tet->control.current, &movePoint, tet->control.r, dest_points);

    while (result == FALSE)
    {
        dest_points[0].y--;
        dest_points[1].y--;
        dest_points[2].y--;
        dest_points[3].y--;
        result = CheckCollision(tet->board, dest_points);

        if (result == TRUE)
        {
            dest_points[0].y++;
            dest_points[1].y++;
            dest_points[2].y++;
            dest_points[3].y++;
            break;
        }
        dropCount++;
    }

    pos->x = movePoint.x;
    pos->y = movePoint.y - dropCount;

    return dropCount;
}

/// <summary>
/// ハードドロップした場所を取得します。
/// </summary>
/// <param name="tets"></param>
int getHarddropPos(MINO_TYPE* board, MINO_TYPE current, int r, POINT* pos)
{
    POINT movePoint = { pos->x, pos->y };
    BOOL result = FALSE;
    int dropCount = 0;

    POINT dest_points[4];
    GetCellPoints(current, &movePoint, r, dest_points);

    while (result == FALSE)
    {
        dest_points[0].y--;
        dest_points[1].y--;
        dest_points[2].y--;
        dest_points[3].y--;
        result = CheckCollision(board, dest_points);

        if (result == TRUE)
        {
            dest_points[0].y++;
            dest_points[1].y++;
            dest_points[2].y++;
            dest_points[3].y++;
            break;
        }
        dropCount++;
    }

    pos->x = movePoint.x;
    pos->y = movePoint.y - dropCount;

    return dropCount;
}

/// <summary>
/// ボードに置きます。
/// </summary>
/// <param name="board"></param>
/// <param name="current"></param>
/// <param name="r"></param>
/// <param name="pos"></param>
void putBoard(MINO_TYPE* board, MINO_TYPE current, int r, POINT* pos)
{
    POINT dest_points[4];
    GetCellPoints(current, pos, r, dest_points);
    for (int i = 0; i < 4; i++)
    {
        board[dest_points[i].x + (dest_points[i].y * BOARD_W)] = current;
    }
}



/// <summary>
/// NEXTを取り出します。
/// </summary>
/// <param name="tet"></param>
static void getNext(TETRIS_DATA* tet)
{
    tet->control.current = tet->next[0];

    for (int i = 1; i < NEXT_MAX; i++)
    {
        tet->next[i - 1] = tet->next[i];
    }
    tet->next[NEXT_MAX - 1] = getBagMino(tet->bagTarget);
    tet->bagTarget++;

    if (tet->bagTarget >= BAG_MAX)
    {
        tet->bagTarget = 0;
    }

    /// それでもカレントが存在しなければホールドからデータを取り出します。
    if (tet->control.current == N)
    {
        Hold(tet);
    }
}

/// ミノのデフォルト形状です。ポイントで表します。
static const POINT MINO_FORM[7][4][4] =
{
    {
        { { 0, -1}, { 1, -1}, { 2, -1}, { 3, -1} },
        { { 2,  0}, { 2, -1}, { 2, -2}, { 2, -3} },
        { { 0, -2}, { 1, -2}, { 2, -2}, { 3, -2} },
        { { 1,  0}, { 1, -1}, { 1, -2}, { 1, -3} },
    },
    {
        { { 0,  0}, { 0, -1}, { 1, -1}, { 2, -1} },
        { { 1,  0}, { 2,  0}, { 1, -1}, { 1, -2} },
        { { 0, -1}, { 1, -1}, { 2, -1}, { 2, -2} },
        { { 1,  0}, { 1, -1}, { 0, -2}, { 1, -2} },
    },
    {
        { { 2,  0}, { 0, -1}, { 1, -1}, { 2, -1} },
        { { 1,  0}, { 1, -1}, { 1, -2}, { 2, -2} },
        { { 0, -1}, { 1, -1}, { 2, -1}, { 0, -2} },
        { { 0,  0}, { 1,  0}, { 1, -1}, { 1, -2} },
    },
    {
        { { 1,  0}, { 2,  0}, { 1, -1}, { 2, -1} },
        { { 1,  0}, { 2,  0}, { 1, -1}, { 2, -1} },
        { { 1,  0}, { 2,  0}, { 1, -1}, { 2, -1} },
        { { 1,  0}, { 2,  0}, { 1, -1}, { 2, -1} },
    },
    {
        { { 1,  0}, { 2,  0}, { 0, -1}, { 1, -1} },
        { { 1,  0}, { 1, -1}, { 2, -1}, { 2, -2} },
        { { 1, -1}, { 2, -1}, { 0, -2}, { 1, -2} },
        { { 0,  0}, { 0, -1}, { 1, -1}, { 1, -2} },
    },
    {
        { { 1,  0}, { 0, -1}, { 1, -1}, { 2, -1} },
        { { 1,  0}, { 1, -1}, { 2, -1}, { 1, -2} },
        { { 0, -1}, { 1, -1}, { 2, -1}, { 1, -2} },
        { { 1,  0}, { 0, -1}, { 1, -1}, { 1, -2} },
    },
    {
        { { 0,  0}, { 1,  0}, { 1, -1}, { 2, -1} },
        { { 2,  0}, { 1, -1}, { 2, -1}, { 1, -2} },
        { { 0, -1}, { 1, -1}, { 1, -2}, { 2, -2} },
        { { 1,  0}, { 0, -1}, { 1, -1}, { 0, -2} },
    },
};

/// <summary>
/// 回転させたミノの位置を返します。
/// </summary>
/// <param name="current"></param>
/// <param name="currentPoint"></param>
/// <param name="r"></param>
/// <param name=""></param>
/// <returns></returns>
void GetCellPoints(MINO_TYPE current, POINT* currentPoint, int r, POINT* cells)
{
    int minoIndex = 0;
    switch (current)
    {
    case I: minoIndex = 0; break;
    case J: minoIndex = 1; break;
    case L: minoIndex = 2; break;
    case O: minoIndex = 3; break;
    case S: minoIndex = 4; break;
    case T: minoIndex = 5; break;
    case Z: minoIndex = 6; break;
    }

    for (int i = 0; i < 4; i++)
    {
        cells[i].x = currentPoint->x + MINO_FORM[minoIndex][r][i].x;
        cells[i].y = currentPoint->y + MINO_FORM[minoIndex][r][i].y;
    }
}

/// <summary>
/// ミノとフィールドが衝突しているかどうかを返します。
/// </summary>
/// <param name="current"></param>
/// <param name="currentPoint"></param>
/// <param name="r"></param>
/// <param name=""></param>
/// <returns></returns>
BOOL CheckCollision(MINO_TYPE *board, POINT* cellPoints)
{
    for (int i = 0; i < 4; i++)
    {
        if (cellPoints[i].x > (10 - 1)) return true;
        if (cellPoints[i].x < 0) return true;
        if (cellPoints[i].y < 0) return true;
        if (cellPoints[i].y > (24 - 1)) return true;

        if (board[cellPoints[i].x + (cellPoints[i].y * BOARD_W)] != N)
        {
            return TRUE;
        }
    }

    return FALSE;
}

static const POINT SRS[2][2][4][5] =
{
    {
        {
            { { 0, 0}, { -1, 0}, { -1, -1}, { 0, +2}, { -1, +2} }, // D->A
            { { 0, 0}, { -1, 0}, { -1, +1}, { 0, -2}, { -1, -2} }, // A->B
            { { 0, 0}, { +1, 0}, { +1, -1}, { 0, +2}, { +1, +2} }, // B->C
            { { 0, 0}, { +1, 0}, { +1, +1}, { 0, -2}, { +1, -2} }, // C->D
        },
        {
            { { 0, 0}, { +1, 0}, { +1, -1}, { 0, +2}, { +1, +2} }, // B->A
            { { 0, 0}, { -1, 0}, { -1, +1}, { 0, -2}, { -1, -2} }, // C->B
            { { 0, 0}, { -1, 0}, { -1, -1}, { 0, +2}, { -1, +2} }, // D->C
            { { 0, 0}, { +1, 0}, { +1, +1}, { 0, -2}, { +1, -2} }, // A->D
        },
    },
    {
        {
            { { 0, 0}, { +1, 0}, { -2, 0}, { +1, -2}, { -2, +1} }, // D->A
            { { 0, 0}, { -2, 0}, { +1, 0}, { -2, -1}, { +1, +2} }, // A->B
            { { 0, 0}, { -1, 0}, { +2, 0}, { -1, +2}, { +2, -1} }, // B->C
            { { 0, 0}, { +2, 0}, { -1, 0}, { +2, +1}, { -1, -2} }, // C->D
        },
        {
            { { 0, 0}, { +2, 0}, { -1, 0}, { +2, +1}, { -1, -2} }, // B->A
            { { 0, 0}, { +1, 0}, { -2, 0}, { +1, -2}, { -2, +1} }, // C->B
            { { 0, 0}, { -2, 0}, { +1, 0}, { -2, -1}, { +1, +2} }, // D->C
            { { 0, 0}, { -1, 0}, { +2, 0}, { -1, +2}, { +2, -1} }, // A->D
        },
    },
};

/// <summary>
/// スーパーローテーションシステムを実装します。返り値はSRSパターンです
/// </summary>
/// <param name="board"></param>
/// <param name="current"></param>
/// <param name="currentPoint"></param>
/// <param name="r"></param>
/// <param name="nextR"></param>
/// <returns></returns>
int GetSuperRotateMinoPoint(MINO_TYPE* board, MINO_TYPE current, POINT* currentPoint, int r, int nextR)
{
    // Oミノは回転しません。
    if (current == O)
    {
        return 0;
    }

    /// 回転先の位置を決定しておきます。
    r += nextR;
    if (r >= 4)
    {
        r = 0;
    }
    if (r < 0)
    {
        r = 3;
    }

    /// 使用する軸ずれテーブルを取得します。
    int minoType = 0;
    int RotateType = 0;

    if (current == I)
    {
        minoType = 1;

        /// +方向の回転、右回転の処理です。
        if (nextR > 0)
        {
            RotateType = 0;
        }

        /// -方向の回転、左回転の処理です。
        if (nextR < 0)
        {
            RotateType = 1;
        }
    }
    else
    {
        minoType = 0;

        /// +方向の回転、右回転の処理です。
        if (nextR > 0)
        {
            RotateType = 0;
        }

        /// -方向の回転、左回転の処理です。
        if (nextR < 0)
        {
            RotateType = 1;
        }
    }

    POINT destPoint = { currentPoint->x, currentPoint->y };

    //// どことも衝突していなければ回転成立として値を返します。
    {
        POINT mino_point[4];
        GetCellPoints(current, &destPoint, r, mino_point);

        if (CheckCollision(board, mino_point) == FALSE)
        {
            return 0;
        }
    }

    //// 軸補正を行います。
    for (int i = 1; i < 5; i++)
    {
        //// 衝突していた場合は、次の場所に移動してチェックを行います。
        ///// まず移動を戻します。
        destPoint = { currentPoint->x, currentPoint->y };

        ///// 移動させます。
        destPoint.x += SRS[minoType][RotateType][r][i - 1].x;
        destPoint.y += SRS[minoType][RotateType][r][i - 1].y;

        //// どことも衝突していなければ回転成立として値を返します。
        {
            POINT mino_point[4];
            GetCellPoints(current, &destPoint, r, mino_point);

            if (CheckCollision(board, mino_point) == false)
            {
                currentPoint->x = destPoint.x;
                currentPoint->y = destPoint.y;
                return i;
            }
        }
    }

    /// 回転できませんでした。
    return -1;
}

/// <summary>
/// TSpinかどうかの判定を行います。
/// 1はTSpin、2はTSpinMiniです
/// </summary>
/// <param name="board"></param>
/// <param name="current"></param>
/// <param name="currentPoint"></param>
/// <param name="r"></param>
/// <param name="spin"></param>
/// <param name="srs"></param>
/// <returns></returns>
int CheckTSpin(MINO_TYPE* board, MINO_TYPE current, POINT* currentPoint, int r, bool spin, int srs)
{
    /// TミノでなければTスピンできません
    /// スピンしてなければTスピンできません
    if ((spin == false) || current != T)
    {
        return 0;
    }

    /// 4隅のセル情報を取得します。
    /// 向きにかかわらず凸として左上、右上、右下、左下を設定します。
    /// 凸の後ろも取りたいので、4方向取っておきます。
    BOOL cell[4] = { FALSE };

    if ((currentPoint->x >= 0) && (currentPoint->x < BOARD_W) && (currentPoint->y >= 0) && (currentPoint->y < 24))
    {
        if (board[currentPoint->x, currentPoint->y] != 0)
        {
            cell[0] = TRUE;
        }
    }
    else
    {
        cell[0] = TRUE;
    }
    if ((currentPoint->x >= -2) && (currentPoint->x < (BOARD_W - 2)) && (currentPoint->y >= 0) && (currentPoint->y < 24))
    {
        if (board[currentPoint->x + 2, currentPoint->y] != 0)
        {
            cell[1] = TRUE;
        }
    }
    else
    {
        cell[1] = TRUE;
    }
    if ((currentPoint->x >= -2) && (currentPoint->x < (BOARD_W - 2)) && (currentPoint->y >= 2) && (currentPoint->y < 24))
    {
        if (board[currentPoint->x + 2, currentPoint->y - 2] != 0)
        {
            cell[2] = TRUE;
        }
    }
    else
    {
        cell[2] = TRUE;
    }
    if ((currentPoint->x >= 0) && (currentPoint->x < BOARD_W) && (currentPoint->y >= 2) && (currentPoint->y < 24))
    {
        if (board[currentPoint->x, currentPoint->y - 2] != 0)
        {
            cell[3] = TRUE;
        }
    }
    else
    {
        cell[3] = TRUE;
    }

    /// 回転に合わせてセル情報を回転させます。
    for (int i = r; i > 0; i--)
    {
        bool tempCell = cell[3];
        cell[3] = cell[0];
        cell[0] = cell[1];
        cell[1] = cell[2];
        cell[2] = tempCell;
    }

    /// 最低3隅が埋まっていなければTSpinになりません。
    int count = 0;
    for (int i = 0; i < 4; i++)
    {
        if (cell[i] == TRUE) count++;
    }
    if (count < 3) return 0;

    /// 3隅のうち、0,1が埋まっていなければTSpinMiniです。
    if (cell[0] != cell[1])
    {
        if (srs != 4) return 2;
    }

    return 1;
}

/// <summary>
/// フィールドを正規化し、ライン消去を行います。
/// 0-19ライン目までしか消去しません。
/// </summary>
int CorrectBoard(MINO_TYPE* board, BOOL clear)
{
    int deleteCount = 0;

    for (int y = 0; y < 20; y++)
    {
        BOOL fill = TRUE;
        for (int x = 0; x < BOARD_W; x++)
        {
            if (board[x + (y * BOARD_W)] == 0)
            {
                fill = FALSE;
                break;
            }
        }

        if (fill == TRUE)
        {
            deleteCount++;
            if (clear == TRUE)
            {
                for (int y2 = y; y2 < (24 - 1); y2++)
                {
                    for (int x = 0; x < BOARD_W; x++)
                    {
                        board[x + (y2 * BOARD_W)] = board[x + ((y2 + 1) * BOARD_W)];
                    }
                }
                y--;
            }
        }
    }

    return deleteCount;
}

/// <summary>
/// 埋まっているラインに消去用セルを設定します。
/// </summary>
int setFillBoardDeleteCell(MINO_TYPE* board)
{
    int deleteLine = 0;
    for (int y = 0; y < 20; y++)
    {
        BOOL fill = TRUE;
        for (int x = 0; x < BOARD_W; x++)
        {
            if (board[x + (y * BOARD_W)] == 0)
            {
                fill = FALSE;
                break;
            }
        }

        if (fill == TRUE)
        {
            for (int x = 0; x < BOARD_W; x++)
            {
                board[x + (y * BOARD_W)] = C;
            }
            deleteLine++;
        }
    }

    return deleteLine;
}

/// <summary>
/// 送信ライン数の計算を行います。
/// </summary>
/// <param name="board"></param>
/// <param name="deleteLine"></param>
/// <param name="tSpinType"></param>
/// <param name="ren"></param>
/// <param name="btb"></param>
/// <returns></returns>
static int CheckSendline(MINO_TYPE* board, int deleteLine, int tSpinType, int ren, BOOL btb)
{
    /// 何も消していなければスコア0です。
    if (deleteLine == 0)
    {
        return 0;
    }

    /// パフェ状態
    int cellCount = 0;
    for (int x = 0; x < 10; x++)
    {
        for (int y = 0; y < 5; y++)
        {
            if ((board[x + (y * BOARD_W)] != N) && (board[x + (y * BOARD_W)] != C))
            {
                cellCount++;
                break;
            }
        }
        if (cellCount > 0) { break; }
    }
    if (cellCount == 0)
    {
        return 10;
    }

    /// 消したラインにより火力が変動します。
    int attack = 0;
    if (deleteLine == 1)
    {
        if (tSpinType == 1)
        {
            attack = 2;
            if (btb == TRUE) attack++;
        }
        else if (tSpinType == 2)
        {
            if (btb == TRUE) attack++;
        }
    }
    if (deleteLine == 2)
    {
        if (tSpinType == 1)
        {
            attack = 4;
            if (btb == TRUE) attack++;
        }
        else if (tSpinType == 2)
        {
            attack = 1;
            if (btb == TRUE) attack++;
        }
        else attack = 1;
    }
    if (deleteLine == 3)
    {
        if (tSpinType == 1)
        {
            attack = 6;
            if (btb == TRUE) attack++;
        }
        else attack = 2;
    }
    if (deleteLine == 4)
    {
        attack = 4;
        if (btb == TRUE) attack++;
    }

    /// REN火力を加算します。
    switch (ren)
    {
    case 0:
    case 1:
    case 2:
    case 3: attack += 0; break;
    case 4:
    case 5: attack += 1; break;
    case 6:
    case 7: attack += 2; break;
    case 8:
    case 9: attack += 3; break;
    case 10:
    case 11: attack += 4; break;
    case 12:
    case 13: attack += 5; break;
    default: attack += 6; break;
    }

    return attack;
}

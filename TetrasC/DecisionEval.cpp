#include <math.h>
#include "DxLib.h"
#include "TetrisData.h"
#include "DecisionEval.h"


/// 関数プロトタイプ宣言です


/// 高さを取得します
void GetHeight(
    double* EvalT,
    TETRIS_DATA* Tetris,
    double* out1,
    double* out2,
    double* out3,
    bool* c4w
);

/// 屋根を取得します
void GetRoof(
    double* EvalT,
    TETRIS_DATA* Tetris,
    int* BoardHeight,
    double* out1,
    double* out2
);

/// 井戸を取得します
/// 返り値は井戸の列です
int GetWell(
    double* EvalT,
    TETRIS_DATA* Tetris,
    int* BoardHeight,
    int* Depth,
    double* out1
);

/// 表面の凸凹を取得します
/// 一番深い井戸は無視します
void GetEvalSurface(
    double* EvalT,
    TETRIS_DATA* Tetris,
    int* BoardHeight,
    int WellColumn,
    int Depth,
    double* out1,
    double* out2,
    bool c4w
);

// 洞窟数を取得
void getEvelCaveCount(
    double* EvalT,
    TETRIS_DATA* Tetris,
    int* BoardHeight,
    double* out1,
    double* out2
);

// 閉塞数を取得
void getEvalClosedAreaCount(
    double* EvalT,
    TETRIS_DATA* Tetris,
    double* out1,
    double* out2
);

/// 閉塞エリアを求めます。
int setClosedArea(
    MINO_TYPE* Board,
    int* ClosedBoard,
    int x,
    int y,
    int Kind,
    int CloseCell
);

/// 火力地形を取得します。
void getEvalPrevAttackPattern(
    double* EvalT,
    TETRIS_DATA* Tetris,
    int* BoardHeight,
    double* out1
);

// ホールド評価を取得
void getEvalHold(
    double* EvalT,
    TETRIS_DATA* Tetris,
    double* out1,
    double* out2,
    double* out3
);

// パフェ評価を取得
void GetEvalPerfectClear(
    double* EvalT,
    TETRIS_DATA* Tetris,
    double* out1
);

// フィールド的にパフェ可能かどうかを返します。
// 可能な場合は手数を返します。-1はパフェ不可です。
int CheckPerfectClear(
    MINO_TYPE* Board
);

bool checkPerfectClearSplit(
    MINO_TYPE* Board,
    int Height
);

// ライン消去評価を取得
void GetEvalLineClear(
    double* EvalT,
    TETRIS_DATA* Tetris,
    double* out1
);


/// 関数の実装です

// 評価関数
void GetEvaluation(
    TETRIS_DATA* tetris,
    double *eval,
    double* evalT
)
{
    int BoardHeight[BOARD_W] = { 0 };

    /// 各列の高さを事前に取得しておきます
    for (int x = 0; x < BOARD_W; x++)
    {
        for (int i = (BOARD_H - 1); i >= 0; i--)
        {
            if ((tetris->board[x + (i * BOARD_W)]) != N)
            {
                BoardHeight[x] = (i + 1);
                break;
            }
        }
    }

    int eval_index = 0;
    bool c4w = false;

    /// 高さ評価です0 1 2
    {
        double out1 = 0;
        double out2 = 0;
        double out3 = 0;
        GetHeight(evalT, tetris, &out1, &out2, &out3, &c4w);
        eval[eval_index++] = out1;
        eval[eval_index++] = out2;
        eval[eval_index++] = out3;
    }

    /// 屋根評価です3 4
    {
        double out1 = 0;
        double out2 = 0;
        GetRoof(evalT, tetris, BoardHeight, &out1, &out2);
        eval[eval_index++] = out1;
        eval[eval_index++] = out2;
    }

    /// 井戸評価です5
    int wellColumn = -1;
    int Depth = 0;
    {
        double out1 = 0;
        wellColumn = GetWell(evalT, tetris, BoardHeight, &Depth, &out1);
        eval[eval_index++] = out1;
    }

    /// 凸凹評価です6 7
    {
        double out1 = 0;
        double out2 = 0;
        GetEvalSurface(evalT, tetris, BoardHeight, wellColumn, Depth, &out1, &out2, c4w);
        eval[eval_index++] = out1;
        eval[eval_index++] = out2;
    }

    /// 洞窟評価です8 9
    {
        double out1 = 0;
        double out2 = 0;
        getEvelCaveCount(evalT, tetris, BoardHeight, &out1, &out2);
        eval[eval_index++] = out1;
        eval[eval_index++] = out2;
    }

    /// 閉塞評価です10 11
    {
        double out1 = 0;
        double out2 = 0;
        getEvalClosedAreaCount(evalT, tetris, &out1, &out2);
        eval[eval_index++] = out1;
        eval[eval_index++] = out2;
    }

    /// 地形評価です12
    {
        double out1 = 0;
        getEvalPrevAttackPattern(evalT, tetris, BoardHeight, &out1);
        eval[eval_index++] = out1;
    }

    /// ホールド評価です14 15 16
    {
        double out1 = 0;
        double out2 = 0;
        double out3 = 0;
        getEvalHold(evalT, tetris, &out1, &out2, &out3);
        eval[eval_index++] = out1;
        eval[eval_index++] = out2;
        eval[eval_index++] = out3;
    }

    /// パフェ評価です19
    {
        double out1 = 0;
        GetEvalPerfectClear(evalT, tetris, &out1);
        eval[eval_index++] = out1;
    }

    /// ディレイ評価です13
    {
        eval[eval_index++] = tetris->result.Delay * evalT[64];
    }

    /// Btb評価です17
    {
        if (tetris->control.btb != 0) eval[eval_index++] = evalT[65];
        else eval[eval_index++] = 0;
    }

    /// ライン消去火力評価です18
    {
        double out1 = 0;
        GetEvalLineClear(evalT, tetris, &out1);
        eval[eval_index++] = out1 * evalT[66];
    }
}


/// 高さを取得します
void GetHeight(
    double* EvalT,
    TETRIS_DATA* Tetris,
    double* out1,
    double* out2,
    double* out3,
    bool* c4w
)
{
    double height = 0;

    double t[BOARD_W] = { EvalT[0], EvalT[1], EvalT[2], EvalT[3], EvalT[4], EvalT[4], EvalT[3], EvalT[2], EvalT[1], EvalT[0] };

    for (int x = 0; x < BOARD_W; x++)
    {
        for (int y = (BOARD_H - 1); y >= 0; y--)
        {
            if ((Tetris->board[x + (y * BOARD_W)]) != 0)
            {
                if (height < (y * t[x])) { height = (y * t[x]); }
                break;
            }
        }
    }

    *out1 = (height * EvalT[5]);

    /// 約半分の高さ評価です
    if (height >= 8)
        *out2 = ((height - 8) * EvalT[6]);
    else
        *out2 = 0;

    /// 上部の高さ評価です
    if (height >= 15)
        *out3 = ((height - 15) * EvalT[7]);
    else
        *out3 = 0;
}


/// 屋根を取得します
/// 屋根の厚みを返します。
void GetRoof(
    double* EvalT,
    TETRIS_DATA* Tetris,
    int* BoardHeight,
    double* out1,
    double* out2
)
{
    double roof[BOARD_W] = { 0 };
    double roof2[BOARD_W] = { 0 };
    for (int i = 0; i < BOARD_W; i++)
    {
        double t[BOARD_W] = { EvalT[8], EvalT[9], EvalT[10], EvalT[11], EvalT[12], EvalT[12], EvalT[11], EvalT[10], EvalT[9], EvalT[8] };

        for (int y = (BoardHeight[i] - 1); y >= 0; y--)
        {
            if ((Tetris->board[i + (y * BOARD_W)]) == N)
            {
                int cell = BoardHeight[i] - y;
                roof[i] += (cell * t[i]);
                roof2[i] += ((double)cell * cell * t[i]);
            }
        }
    }

    double roofs = 0;
    double roofs2 = 0;
    for (int i = 0; i < BOARD_W; i++)
    {
        roofs += roof[i];
        roofs2 += roof2[i];
    }

    *out1 = (roofs * EvalT[13]);
    *out2 = (roofs2 * EvalT[14]);
}


/// 井戸を取得します
/// 返り値は井戸の列です。井戸は完成品を調べます
int GetWell(
    double* EvalT,
    TETRIS_DATA* Tetris,
    int* BoardHeight,
    int* Depth,
    double* out1
)
{
    double column_score[BOARD_W] = { EvalT[15], EvalT[16], EvalT[17], EvalT[18], EvalT[19], EvalT[19], EvalT[18], EvalT[17], EvalT[16], EvalT[15] };

    /// 取りあえず一番深い列を井戸認定します
    int wellColumn = 0;
    int wellColumnOther = 0;
    *out1 = 0;
    for (int i = 1; i < BOARD_W; i++)
    {
        if (BoardHeight[i] < BoardHeight[wellColumn])
        {
            wellColumn = i;
            wellColumnOther = 0;
            *out1 = 0;
        }
        else if (BoardHeight[i] == BoardHeight[wellColumn])
        {
            *out1 += column_score[wellColumn];
            wellColumnOther++;
        }
    }

    /// 複数の井戸が見つかったらそれは井戸ではない
    if (wellColumnOther > 0)
    {
        return -1;
    }

    /// 一番深い井戸の一番下から数えて4段を最大として井戸扱いにします
    (*Depth) = 0;
    bool finish = false;
    for (int i = BoardHeight[wellColumn]; i < (BoardHeight[wellColumn] + 4); i++)
    {
        BOOL fill = true;
        for (int x = 0; x < BOARD_W; x++)
        {
            if (x == wellColumn) { continue; }
            if (Tetris->board[x + (i * BOARD_W)] == N)
            {
                fill = FALSE;
                break;
            }
        }

        if (fill == TRUE)
        {
            if (finish == true)
            {
                (*Depth) = 0;
                break;
            }
            (*Depth)++;
        }
        else
        {
            if ((*Depth) > 0)
            {
                finish = true;
            }
            break;
        }
    }

    *out1 += (((double)(*Depth)) * EvalT[20]);
    *out1 += column_score[wellColumn];

    return wellColumn;
}


/// 表面の凸凹を取得します
/// 一番深い井戸は無視します
void GetEvalSurface(
    double* EvalT,
    TETRIS_DATA* Tetris,
    int* BoardHeight,
    int WellColumn,
    int Depth,
    double* out1,
    double* out2,
    bool c4w
)
{
    double Surface = 0;
    double Surface2 = 0;

    int prevHeight = BoardHeight[0];

    for (int i = 0; i < BOARD_W; i++)
    {
        int Height = BoardHeight[i] - 1;
        if (i == WellColumn) { Height += Depth; }

        double s = abs(prevHeight - Height);
        if (s < 0) { s = 0; }

        if ((i == 0) && (prevHeight > Height))
        {
            s *= EvalT[21];
        }
        if ((i == 1) && (prevHeight > Height))
        {
            s *= EvalT[22];
        }
        if ((i == 2) && (prevHeight > Height))
        {
            s *= EvalT[23];
        }
        if ((i == 7) && (prevHeight < Height))
        {
            s *= EvalT[23];
        }
        if ((i == 8) && (prevHeight < Height))
        {
            s *= EvalT[22];
        }
        if ((i == 9) && (prevHeight < Height))
        {
            s *= EvalT[21];
        }

        Surface += s;
        Surface2 += (s * s);
        prevHeight = Height;
    }

    *out1 = (Surface * EvalT[24]);
    *out2 = (Surface2 * EvalT[25]);
}


// 洞窟数を取得
void getEvelCaveCount(
    double* EvalT,
    TETRIS_DATA* Tetris,
    int* BoardHeight,
    double* out1,
    double* out2
)
{
    double cave = 0;
    for (int x = 0; x < BOARD_W; x++)
    {
        for (int y = 0; y < BoardHeight[x]; y++)
        {
            if ((Tetris->board[x + (y * BOARD_W)]) == 0)
            {
                cave++;
            }
        }
    }
    *out1 = cave * EvalT[26];
    *out2 = (cave * cave) * EvalT[27];
}

// 閉塞数を取得
void getEvalClosedAreaCount(
    double* EvalT,
    TETRIS_DATA* Tetris,
    double* out1,
    double* out2
)
{
    /*
    ■■■■■■■■■
    ■□■■□□■■■
    ■■■■■□■■■
    ■■■□□□■■■
    ■■■■■■■□□
    こんな感じの場合は閉塞数3になります。
    */

    int ClosedBoard[21 * BOARD_W] = { 0 };

    int AreaKind = 0;
    int ClosedCount = 0;
    for (int y = 0; y < 21; y++)
    {
        for (int x = 0; x < BOARD_W; x++)
        {
            if ((ClosedBoard[(y * BOARD_W) + x] == 0) && ((Tetris->board[x + (y * BOARD_W)]) == 0))
            {
                AreaKind++;
                if (setClosedArea(Tetris->board, ClosedBoard, x, y, AreaKind, 1) != 0)
                {
                    ClosedCount++;
                }
            }
        }
    }

    *out1 = (ClosedCount * EvalT[28]);
    *out2 = (((double)ClosedCount * ClosedCount * ClosedCount) * EvalT[29]);
}

/// 閉塞エリアを求めます。
int setClosedArea(
    MINO_TYPE* board,
    int* ClosedBoard,
    int x,
    int y,
    int Kind,
    int CloseCell
)
{
    if (y < 0) return CloseCell;
    if (y >= 21) return 0;
    if (x < 0) return CloseCell;
    if (x >= BOARD_W) return CloseCell;

    // 今参照しているINDEXを持っておきます。
    int index = (y * BOARD_W) + x;

    if ((ClosedBoard[index] == 0) && ((board[x + (y * BOARD_W)]) == 0))
    {
        ClosedBoard[index] = Kind;
        CloseCell = setClosedArea(board, ClosedBoard, x + 1, y, Kind, CloseCell);
        CloseCell = setClosedArea(board, ClosedBoard, x, y - 1, Kind, CloseCell);
        CloseCell = setClosedArea(board, ClosedBoard, x - 1, y, Kind, CloseCell);
        CloseCell = setClosedArea(board, ClosedBoard, x, y + 1, Kind, CloseCell);
    }

    return CloseCell;
}

void getEvalPrevAttackPattern(
    double* EvalT,
    TETRIS_DATA* Tetris,
    int* BoardHeight,
    double* out1
)
{
    double hyoka = 0.0;
    double hyoka0 = 0.0;
    double hyoka1 = 0.0;
    int Board[BOARD_H] = { 0 };

    for (int y = 0; y < BOARD_H; y++)
    {
        for (int x = 0; x < BOARD_W; x++)
        {
            if (Tetris->board[x + (y * BOARD_W)] != N)
            {
                Board[y] |= (1 << x);
            }
        }
    }

    for (int x = 0; x < (BOARD_W - 3); x++)
    {
        for (int y = BoardHeight[x] + 2; y >= 2; y--)
        {
            // yがBOARD_H以上または0未満の場合、次の繰り返しに進む
            if (y >= BOARD_H || y < 0) { continue; }

            // 事前にビット操作を行い、結果を変数に保存
            int boardY_2 = Board[y - 2];
            int boardY_1 = Board[y - 1];
            int boardY_0 = Board[y - 0];

            // 埋まっていたら次のX軸に移る
            if ((boardY_2 & (7 << x)) == 0x7)
            {
                break;
            }

            // 一番下の段の評価
            double hyouka1 = 0.0;
            if (
                (boardY_2 & (1 << x)) != 0 &&
                (boardY_2 & (1 << (x + 1))) == 0 &&
                (boardY_2 & (1 << (x + 2))) != 0)
            {
                hyouka1 += EvalT[30];
                if ((boardY_2 | (7 << x)) == 0x3FF)
                {
                    hyouka1 += EvalT[31];
                }
            }
            else
            {
                continue;
            }

            // ２番下の段の評価
            double hyouka2 = 0.0;
            if (
                (boardY_1 & (1 << x)) == 0 &&
                (boardY_1 & (1 << (x + 1))) == 0 &&
                (boardY_1 & (1 << (x + 2))) == 0)
            {
                hyouka2 += EvalT[32];
                if ((boardY_1 | (7 << x)) == 0x3FF)
                {
                    hyouka2 += EvalT[33];
                }
            }
            else
            {
                continue;
            }

            // ３番下の段の評価
            double hyouka3 = 0.0;
            if (
                (boardY_0 & (1 << x)) != 0 &&
                (boardY_0 & (1 << (x + 1))) == 0 &&
                (boardY_0 & (1 << (x + 2))) == 0)
            {
                hyouka3 += EvalT[34];
            }
            else if (
                (boardY_0 & (1 << x)) == 0 &&
                (boardY_0 & (1 << (x + 1))) == 0 &&
                (boardY_0 & (1 << (x + 2))) != 0)
            {
                hyouka3 += EvalT[34];
            }
            else if ((boardY_0 & (1 << (x + 1))) != 0)
            {
                continue;
            }

            // 最終評価の計算
            double totalHyoka = hyouka1 + hyouka2 + hyouka3;
            if (hyoka0 < totalHyoka)
            {
                hyoka0 = totalHyoka;
            }
            else if (hyoka1 < totalHyoka)
            {
                hyoka1 = totalHyoka;
            }
        }
    }

    hyoka = hyoka0 + hyoka1;

#if true

    double hyoka2 = 0.0;
    double hyoka3 = 0.0;

    for (int x = 0; x < (BOARD_W - 4); x++)
    {
        for (int y = BoardHeight[x] + 5; y >= 5; y--)
        {
            // yがBOARD_H以上または0未満の場合、次の繰り返しに進む
            if (y >= BOARD_H || y < 0) { continue; }

            int boardY_4 = Board[y - 4];
            int boardY_3 = Board[y - 3];
            int boardY_2 = Board[y - 2];
            int boardY_1 = Board[y - 1];
            int boardY_0 = Board[y - 0];

            // TSTのB回転の評価計算
            double bHyoka1 = 0.0, bHyoka2 = 0.0, bHyoka3 = 0.0;
            if (!(boardY_4 & (1 << (x + 2))) && (boardY_4 & (1 << (x + 3))))
            {
                bHyoka1 += EvalT[35];
                if ((boardY_4 | (0x4 << x)) == 0x3FF)
                {
                    bHyoka1 += EvalT[36];
                }
            }

            if (bHyoka1 != 0.0 &&
                !(boardY_3 & ((1 << (x + 1)) | (1 << (x + 2)))) &&
                boardY_2 & (1 << (x + 1)) &&
                !(boardY_2 & (1 << (x + 2))) &&
                boardY_2 & (1 << (x + 3)) &&
                !(boardY_1 & ((1 << (x + 0)) | (1 << (x + 1)) | (1 << (x + 2)))) &&
                !(boardY_0 & ((1 << (x + 0)) | (1 << (x + 1)))) &&
                boardY_0 & (1 << (x + 2))
                )
            {
                bHyoka3 += EvalT[39];
            }
            else if (
                (boardY_3 & (1 << (x + 1))) ||
                (boardY_3 & (1 << (x + 2))) ||
                (boardY_2 & (1 << (x + 2))) ||
                (boardY_1 & (1 << (x + 0))) ||
                (boardY_1 & (1 << (x + 1))) ||
                (boardY_1 & (1 << (x + 2))) ||
                (boardY_0 & (1 << (x + 0))) ||
                (boardY_0 & (1 << (x + 1)))
                )
            {
                bHyoka2 = 0;
                bHyoka1 = 0;
            }

            double totalBHyoka = bHyoka1 + bHyoka2 + bHyoka3;
            if (hyoka2 < totalBHyoka)
            {
                hyoka2 = totalBHyoka;
            }

            // TSTのA回転の評価計算
            double aHyoka1 = 0.0, aHyoka2 = 0.0, aHyoka3 = 0.0;
            if (!(boardY_4 & (1 << (x + 1))) && (boardY_4 & (1 << (x + 2))))
            {
                aHyoka1 += EvalT[34];
                if ((boardY_4 | (0x2 << x)) == 0x3FF)
                {
                    aHyoka1 += EvalT[35];
                }
            }

            if (aHyoka1 != 0.0 &&
                !(boardY_3 & ((1 << (x + 1)) | (1 << (x + 2)))) &&
                boardY_2 & (1 << (x + 0)) &&
                !(boardY_2 & (1 << (x + 1))) &&
                boardY_2 & (1 << (x + 2)) &&
                !(boardY_1 & ((1 << (x + 1)) | (1 << (x + 2)) | (1 << (x + 3)))) &&
                !(boardY_0 & ((1 << (x + 2)) | (1 << (x + 3)))) &&
                boardY_0 & (1 << (x + 1))
                )
            {
                aHyoka3 += EvalT[39];
            }
            else if (
                (boardY_3 & (1 << (x + 1))) ||
                (boardY_3 & (1 << (x + 2))) ||
                (boardY_2 & (1 << (x + 1))) ||
                (boardY_1 & (1 << (x + 1))) ||
                (boardY_1 & (1 << (x + 2))) ||
                (boardY_1 & (1 << (x + 3))) ||
                (boardY_0 & (1 << (x + 2))) ||
                (boardY_0 & (1 << (x + 3)))
                )
            {
                aHyoka2 = 0;
                aHyoka1 = 0;
            }

            double totalAHyoka = aHyoka1 + aHyoka2 + aHyoka3;
            if (hyoka3 < totalAHyoka)
            {
                hyoka3 = totalAHyoka;
            }
        }
    }

    *out1 = hyoka + hyoka2 + hyoka3;
#endif

}

// ホールド評価を取得
void getEvalHold(
    double* EvalT,
    TETRIS_DATA* Tetris,
    double* out1,
    double* out2,
    double* out3
)
{
    /// ホールドTミノ評価です
    if (Tetris->hold == T)*out1 = EvalT[40];
    else *out1 = 0;

    /// ホールドIミノ評価です
    if (Tetris->hold == I)*out2 = EvalT[41];
    else *out2 = 0;

    /// ホールドOミノ評価です
    if (Tetris->hold == O)*out3 = EvalT[42];
    else *out3 = 0;
}


// パフェ評価を取得
void GetEvalPerfectClear(
    double* EvalT,
    TETRIS_DATA* Tetris,
    double* out1
)
{
    int result = CheckPerfectClear(Tetris->board);

    if (result < 0)*out1 = 0.00;
    else *out1 = EvalT[43];
}



// フィールド的にパフェ可能かどうかを返します。
// 可能な場合は手数を返します。-1はパフェ不可です。
int CheckPerfectClear(
    MINO_TYPE* Board
)
{
    /// すでにパフェ
    if (Board[0] == 0)
    {
        return 1;
    }

    int space_count = 0;
    bool check_split = false;

    /// パフェが取れそうな高さです。
    int height = 0;

    /// とりあえず高さを調べます。
    for (int y = 0; y <= 6; y++)
    {
        for (int x = 0; x < BOARD_W; x++)
        {
            /// 空白セル数をカウントします。
            if ((Board[y] & (1 << x)) == 0)
            {
                space_count++;
            }
        }

        /// 何もない段の場合はそこまででパフェの計算を行います。
        if (Board[y] == 0)
        {
            /// 4の倍数でないとだめです。(+1段の高さを見ています。)
            if ((space_count % 4) == 0)
            {
                height = y + 1;
                break;
            }

            /// 一段引いて4の倍数でないとだめです。(ギリギリの高さを見ています。)
            else
            {
                space_count -= 10;
                if ((space_count % 4) != 0) return -1;

                check_split = true;
                height = y;
                break;
            }
        }
    }

    /// 結局高さ0の場合はパフェが取れません。
    if (height == 0 || height > 6) return -1;

    /// 分割空間(縦埋まりの計算)のセル数も4の倍数でないとパフェが取れません。
    if (check_split == true)
    {
        if (checkPerfectClearSplit(Board, height) == false)
        {
            if (height >= 4)
            {
                return -1;
            }

            if (checkPerfectClearSplit(Board, height + 2) == false)
            {
                return -1;
            }
            space_count += 20;
        }
    }

    switch (space_count)
    {
    case 20: return 5;
    case 16: return 4;
    case 12: return 3;
    case 8: return 2;
    case 4: return 1;
    }

    return -1;
}


bool checkPerfectClearSplit(
    MINO_TYPE* Board,
    int Height
)
{
    // 空きセル数をカウントします。
    int space_count = 0;
    for (int x = 0; x < BOARD_W; x++)
    {
        // 埋まってるセル数をカウントします。
        int fill_count = 0;

        // 縦閉塞を調べます。
        bool side_space = false;
        int line_space = 0;

        bool side_space_left = x > 0;
        bool side_space_right = x < (BOARD_W - 1);

        for (int y = 0; y < Height; y++)
        {
            // 空きセルのカウントを行います。
            if ((Board[y] & (1 << x)) == 0)
            {
                space_count++;
                line_space++;

                // 両サイドのいずれかが空きセルかどうかを調べます。
                if (side_space_left && (Board[y] & (1 << (x - 1))) == 0) side_space = true;
                if (side_space_right && (Board[y] & (1 << (x + 1))) == 0) side_space = true;
            }

            // ミノセルのカウントを行います。
            else fill_count++;
        }

        // スペースがありサイドが全部埋まってたらパフェはできません。
        if ((line_space > 0) && (side_space == false)) { return false; }

        // 縦埋めがあった場合、以前のスペースが4の倍数でなければパフェはできません。
        if (fill_count == Height)
        {
            if ((space_count % 4) != 0) return false;
            space_count = 0;
        }
    }

    // 最後ももちろん空きセルが4の倍数でないとパフェできません。
    if ((space_count % 4) != 0) return false;

    // ここまでくればパフェ可能です。
    return true;
}



// ライン消去評価を取得
void GetEvalLineClear(double* EvalT, TETRIS_DATA* Tetris, double* out1)
{
    double attack = 0;
    *out1 = 0;

    /// 何も消していなければスコア0です。
    if (Tetris->prevResult.deleteLine == 0)
    {
        /// Tミノは火力の損があるのでマイナス評価です。
        if (Tetris->prevControl.current == T) { *out1 = EvalT[44]; }
        if (Tetris->prevControl.current == I) { *out1 = EvalT[45]; }

        return;
    }

    /// パフェ状態、パフェは最大
    if (Tetris->prevResult.Pc != 0)
    {
        *out1 = EvalT[46];
        return;
    }

    if (Tetris->prevResult.deleteLine == 1)
    {
        if (Tetris->prevResult.Tss != 0)
        {
            attack = EvalT[47];
        }
        else if (Tetris->prevResult.Tsm != 0)
        {
            attack = EvalT[48];
        }
        else
        {
            attack = EvalT[49];
        }
    }
    if (Tetris->prevResult.deleteLine == 2)
    {
        if (Tetris->prevResult.Tsd != 0)
        {
            attack = EvalT[50];
        }
        else if (Tetris->prevResult.Tsmd != 0)
        {
            attack = EvalT[51];
        }
        else
        {
            attack = EvalT[52];
        }
    }
    if (Tetris->prevResult.deleteLine == 3)
    {
        if (Tetris->prevResult.Tst != 0)
        {
            attack = EvalT[53];
        }
        else
        {
            attack = EvalT[54];
        }
    }
    if (Tetris->prevResult.deleteLine == 4)
    {
        attack = EvalT[55];
    }

    /// REN火力を加算します。
    if (Tetris->control.ren >= 1)
    {
        double ren = ((double)Tetris->control.ren - 1);
        attack += ren * ren * EvalT[56];
    }

    (*out1) = attack;
}


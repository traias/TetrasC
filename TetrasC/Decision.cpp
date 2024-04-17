#include <vector>
#include "DxLib.h"
#include "Common.h"
#include "TetrisData.h"
#include "Decision.h"
#include "DecisionPattern.h"
#include "DecisionEval.h"
#include <algorithm>
using namespace std;



/// <summary>
/// ドロップパターンを取得します。
/// </summary>
/// <param name="tetris"></param>
/// <param name="parent"></param>
/// <returns></returns>
static int getDropPatterns(TETRIS_DATA* tet, DECISION_TETRIS** pattern, DECISION_TETRIS* parent);

/// <summary>
// 全ドロップパターンの評価値(決定評価)取得します
/// </summary>
/// <param name="base"></param>
/// <param name="pattern"></param>
/// <param name="patternMax"></param>
/// <param name="evalT"></param>
/// <returns></returns>
static int getScoreList(TETRIS_DATA* base, DECISION_TETRIS** pattern, int patternMax, double* evalTable);


/// <summary>
/// かぶったボードのデータを無効にします。
/// </summary>
/// <param name=""></param>
void distinctData(DECISION_TETRIS** pattern, int max);



void debugDrawBoard(DECISION_TETRIS** pattern, int max);

void debugDrawBoard(DECISION_TETRIS* pattern);

void debugDrawBoard(DECISION_TETRIS** pattern);



/// <summary>
/// 評価テーブルです。
/// </summary>
double evalTable[80] =
{
    /// 高さの列に対する重みです。(左右対称なので半分だけです。)
    /// 0-4
    0.9,
    0.95,
    0.98,
    1,
    1,

    /// 高さ評価(-7, 8-14, 15-)
    /// 5-7
    -0.40,
    -0.10,
    -0.10,

    /// 屋根の高さに対する重みです。(左右対称なので半分だけです。)
    /// 8-12
    1,
    2,
    1.5,
    1,
    1,

    /// 屋根の高さ評価
    /// 13-14
    -0.15,
    -0.03,

    /// 井戸の列に対する重みです。(左右対称なので半分だけです。)
    /// 15-19
    -0.5,
    -0.7,
    -0.2,
    0.7,
    0.7,

    /// 井戸の深さ評価
    /// 20
    0.05,

    /// 表面形状の列に対する重みです。(左右対称なので半分だけです。)
    /// 21-23
    0.7,
    0.8,
    0.9,

    /// 表面形状評価
    /// 24-25
    -0.2,
    -0.2,

    /// 洞窟評価
    /// 26-27
    -0.08,
    -0.05,

    /// 閉塞評価
    /// 28-29
    -3.6,
    -0.3,

    /// 火力地形評価(TSD/TST)
    /// 30-39
    0.0,
    0.2,
    0.1,
    0.7,
    1.8,
    0.0,
    0.0,
    0.0,
    0.0,
    5.0,

    /// ホールド評価
    /// 40-42
    0.05,
    0.10,
    -0.02,

    /// パフェ地形評価
    /// 43
    0.5,

    /// 火力評価
    /// 44-63
    -3.00,
    -0.50,
    99,

    2.0,
    -2.0,
    -1.0,

    2.8,
    -1.5,
    -8.8,
    7.20,
    -1.0,
    -7.6,
    9,
    -7.4,
    4.5,
    0.6,

    3.0,
    0.1,
    0.07,
    20.0,

    /// ディレイ評価
    /// 64
    -0.01,

    /// Btb評価
    /// 65
    0.5,

    /// 火力乗算評価
    /// 66
    1.0,
};


/// <summary>
/// 一手目のデータです。
/// </summary>
static DECISION_TETRIS* topPattern;

/// <summary>
/// 二手目のデータです。
/// </summary>
static DECISION_TETRIS* secondPattern;

/// <summary>
/// 意思決定されたデータです。
/// </summary>
static DECISION_TETRIS* decisionPattern;

/// <summary>
/// 探索に使用するデータです。
/// </summary>
static DECISION_TETRIS* searchPattern;


/// <summary>
/// 探索時に使用する子供として使うデータです。
/// </summary>
static DECISION_TETRIS* childPattern;




int cmpQsort(const void* d1, const void* d2);




/// <summary>
/// 意思決定中はTRUEになります。
/// </summary>
static BOOL decisionRun = FALSE;


/// <summary>
/// 中断フラグです。
/// </summary>
static BOOL abortRun = FALSE;



/// <summary>
/// 意思決定システムの初期化を行います。
/// これはメモリの動的確保を処理中に行わないようにするためです。
/// </summary>
void DecisionSystemInit(void)
{
    topPattern = (DECISION_TETRIS*)malloc(sizeof(DECISION_TETRIS) * 96);
    secondPattern = (DECISION_TETRIS*)malloc(sizeof(DECISION_TETRIS) * 96 * 96);
    childPattern = (DECISION_TETRIS*)malloc(sizeof(DECISION_TETRIS) * 96);
    searchPattern = (DECISION_TETRIS*)malloc(sizeof(DECISION_TETRIS) * 200 * 96);
}

/// <summary>
/// 意思決定システムの解放を行います。
/// これを呼び出すと意思決定システムは使用不能になります。
/// </summary>
void DecisionSystemRelease(void)
{
    free(searchPattern);
    free(childPattern);
    free(secondPattern);
    free(topPattern);
}


/// <summary>
/// 意思決定を行います。
/// </summary>
void Decision(TETRIS_DATA* tet)
{
    decisionRun = TRUE;
    Debug("意思決定を開始します。\n");

    // 仮想NEXTを作成します。

    // 一手目をすべて先読みします。
    int patternCount = getDropPatterns(tet, &topPattern, NULL);
    Debug("一手目をチェックしました。[%d]パターン見つかりました。\n", patternCount);

    // 二手目をすべて先読みします。
    int searchCount = 0;
    {
        for (int i = 0; i < patternCount; i++)
        {
            DECISION_TETRIS* second = &secondPattern[searchCount];

            searchCount += getDropPatterns(&topPattern[i].tetris, &second, &topPattern[i]);
        }
        Debug("二手目をチェックしました。[%d]パターン見つかりました。\n", searchCount);

        // 被ったデータを削除します。
        //distinctData(&secondPattern, searchCount);

        // 評価値でソートします。
        qsort(secondPattern, searchCount, sizeof(DECISION_TETRIS), cmpQsort);
        decisionPattern = &secondPattern[0];
    }

    // 中断が指定されていたらこのタイミングで中断します。
    if (abortRun == TRUE)
    {
        decisionRun = FALSE;
        return;
    }

    // 三手目以降を調べます。上位200手だけを採用して次の手を見ます。
    {
        int nextSearch = searchCount;
        if (nextSearch > 200)
        {
            nextSearch = 200;
        }

        int nextSearchCount = 0;
        for (int i = 0; i < nextSearch; i++)
        {
            // 次の手を見ます。
            DECISION_TETRIS* thard = &searchPattern[nextSearchCount];
            nextSearchCount += getDropPatterns(&secondPattern[i].tetris, &thard, &secondPattern[i]);
        }

        Debug("三手目をチェックしました。[%d]パターン見つかりました。\n", nextSearchCount);

        // 被ったデータを削除します。
        //distinctData(&searchPattern, nextSearchCount);

        // 評価値でソートします。
        qsort(searchPattern, nextSearchCount, sizeof(DECISION_TETRIS), cmpQsort);
        decisionPattern = &searchPattern[0];
    }

    decisionRun = FALSE;
}


/// <summary>
/// 意思決定の中断を行います。
/// </summary>
void decisionAbort()
{
    abortRun = TRUE;
}

/// <summary>
/// ドロップパターンを取得します。
/// </summary>
/// <param name="tetris"></param>
/// <param name="parent"></param>
/// <returns></returns>
static int getDropPatterns(TETRIS_DATA* tet, DECISION_TETRIS** pattern, DECISION_TETRIS* parent)
{
    /// カレントとホールドで取りうるすべてのドロップパターンとスコアを取得します
    int count = getScoreList(tet, pattern, 96, evalTable);

    /// 必要なデータの親子を設定します。
    for (int i = 0; i < count; i++)
    {
        DECISION_TETRIS* d = &(*pattern)[i];

        if (d->enable == FALSE) { continue; }

        // 初手はNULLなので自分が親になります。
        if (parent == NULL)
        {
            d->top = d;
            d->parent = d;
        }
        else
        {
            d->top = parent->top;
            d->parent = parent;
        }
        d->child = NULL;
        d->childCount = 0;
    }

    return count;
}

/// <summary>
// 全ドロップパターンの評価値(決定評価)取得します
/// </summary>
/// <param name="base"></param>
/// <param name="pattern"></param>
/// <param name="patternMax"></param>
/// <param name="evalT"></param>
/// <returns></returns>
static int getScoreList(TETRIS_DATA* base, DECISION_TETRIS** pattern, int patternMax, double* evalTable)
{
    /// とりあえず全ドロップパターンを取得します
    int patternCount = GetDropPattern(base, pattern, patternMax);

    /// ドロップパターンに対してスコアを登録します
    for (int i = 0; i < patternCount; i++)
    {
        /// 不要なドロップパターンは何もしません
        if ((*pattern)[i].enable == FALSE) continue;

        /// ドロップパターンに対して評価畳み込みデータを作成します
        GetEvaluation(&(*pattern)[i].tetris, (*pattern)[i].eval.eval, evalTable);
        (*pattern)[i].totalEval = 0;
        for (int n = 0; n < 20; n++)
        {
            (*pattern)[i].totalEval += (*pattern)[i].eval.eval[n];
        }
    }

    return patternCount;
}

/// <summary>
/// 意思決定されたコマンドを取得します。
/// </summary>
/// <returns></returns>
char* getDecisionCommand(void)
{
    Debug("　意思決定したコマンドが要求されました。\n");

    while (decisionRun == TRUE)
    {
        Sleep(0);
    }

    Debug("　意思決定したコマンドを返します。[%s]です。\n", decisionPattern->top->command);

    return decisionPattern->top->command;
}

/// <summary>
/// 意思決定されたデータを返します。
/// </summary>
/// <param name="tetris"></param>
void getDecisionTetrisData(TETRIS_DATA *tetris)
{
    Debug("　意思決定したテトリスデータが要求されました。\n");

    while (decisionRun == TRUE)
    {
        Sleep(0);
    }

    Debug("　意思決定したテトリスデータを返します。\n");

    memcpy_s(tetris, sizeof(TETRIS_DATA), &decisionPattern->top->tetris, sizeof(TETRIS_DATA));
}

/// <summary>
/// かぶったボードのデータを無効にします。
/// </summary>
/// <param name=""></param>
void distinctData(DECISION_TETRIS** pattern, int max)
{
    for (int i = 0; i < (max - 1); i++)
    {
        if ((*pattern)[i].enable == FALSE) { continue; }

        for (int n = (i + 1); n < max; n++)
        {
            if ((*pattern)[n].enable == FALSE) { continue; }

            // 同じボードだった
            if (
                ((*pattern)[i].boardHash[0] == (*pattern)[n].boardHash[0]) &&
                ((*pattern)[i].boardHash[1] == (*pattern)[n].boardHash[1]) &&
                ((*pattern)[i].boardHash[2] == (*pattern)[n].boardHash[2]) &&
                ((*pattern)[i].boardHash[3] == (*pattern)[n].boardHash[3])
                )
            {
                /// ディレイが短いデータを採用します
                if ((*pattern)[i].tetris.result.Delay > (*pattern)[n].tetris.result.Delay)
                {
                    (*pattern)[i].enable = FALSE;
                }
                else if ((*pattern)[i].tetris.result.Delay < (*pattern)[n].tetris.result.Delay)
                {
                    (*pattern)[n].enable = FALSE;
                }
                else
                {
                    (*pattern)[n].enable = FALSE;
                }
            }
        }
    }
}



int cmpQsort(const void* d1, const void* d2)
{
    if (((DECISION_TETRIS*)d1)->enable == FALSE)
    {
        return 1;
    }
    if (((DECISION_TETRIS*)d2)->enable == FALSE)
    {
        return -1;
    }

    if ((((DECISION_TETRIS*)d1)->totalEval) > (((DECISION_TETRIS*)d2)->totalEval))
    {
        return -1;
    }
    if ((((DECISION_TETRIS*)d1)->totalEval) < (((DECISION_TETRIS*)d2)->totalEval))
    {
        return 1;
    }

    return 0;
}



void debugDrawBoard(DECISION_TETRIS** pattern, int max)
{
    for (int i = 0; i < max; i++)
    {
        if ((*pattern)[i].enable == FALSE) { continue; }

        TETRIS_DATA *t = &(*pattern)[i].tetris;

        Debug("■%s\n", (*pattern)[i].command);
        Debug("eval=%lf\n", (*pattern)[i].totalEval);
        Debug("current(%d), x=%d, y=%d, r=%d\n", t->prevControl.current, t->prevControl.pos.x, t->prevControl.pos.y, t->prevControl.r);
        Debug("HASH = %I64X, %I64X, %I64X, %I64X\n", (*pattern)[i].boardHash[0], (*pattern)[i].boardHash[1], (*pattern)[i].boardHash[2], (*pattern)[i].boardHash[3]);

        for (int y = 8; y >= 0; y--)
        {
            char line[11] = { 0 };
            for (int x = 0; x < BOARD_W; x++)
            {
                if (t->board[x + (y * BOARD_W)] == 0)
                {
                    line[x] = '_';
                }
                else
                {
                    line[x] = 'X';
                }
            }
            Debug("%s\n", line);
        }
        Debug("\n");
    }
}


void debugDrawBoard(DECISION_TETRIS* pattern)
{
    if (pattern->enable == FALSE) { return; }

    TETRIS_DATA* t = &pattern->tetris;

    Debug("■%s\n", pattern->command);
    Debug("eval=%lf\n", pattern->totalEval);
    Debug("current(%d), x=%d, y=%d, r=%d\n", t->prevControl.current, t->prevControl.pos.x, t->prevControl.pos.y, t->prevControl.r);

    for (int y = 8; y >= 0; y--)
    {
        char line[11] = { 0 };
        for (int x = 0; x < BOARD_W; x++)
        {
            if (t->board[x + (y * BOARD_W)] == 0)
            {
                line[x] = '_';
            }
            else
            {
                line[x] = 'X';
            }
        }
        Debug("%s\n", line);
    }
    Debug("\n");
}


void debugDrawBoard(DECISION_TETRIS** pattern)
{
    DECISION_TETRIS* p = NULL;

    do
    {
        if (p == NULL)
        {
            p = (*pattern);
        }
        else
        {
            p = p->parent;
        }

        if (p->enable == FALSE) { return; }

        TETRIS_DATA* t = &p->tetris;

        Debug("■%s\n", p->command);
        Debug("eval=%lf\n", p->totalEval);
        Debug("current(%d), x=%d, y=%d, r=%d\n", t->prevControl.current, t->prevControl.pos.x, t->prevControl.pos.y, t->prevControl.r);

        for (int y = 8; y >= 0; y--)
        {
            char line[11] = { 0 };
            for (int x = 0; x < BOARD_W; x++)
            {
                if (t->board[x + (y * BOARD_W)] == 0)
                {
                    line[x] = '_';
                }
                else
                {
                    line[x] = 'X';
                }
            }
            Debug("%s\n", line);
        }
        Debug("\n");

    } while (p->parent != p);
}


#include "DxLib.h"
#include "Common.h"
#include "Key.h"
#include "TetrisData.h"
#include "AI.h"
#include <format>
#include <time.h>
#include <iostream>
using namespace std;


typedef enum
{
    TETRIS_INIT,
    TETRIS_INPUT,
    TETRIS_CLEAR,
    TETRIS_NEWMINO,
    TETRIS_GAMEOVER,
} TETRIS_SCENE;





TETRIS_SCENE TetrisInput(void);
TETRIS_SCENE TetrisAiInput(void);
TETRIS_SCENE TetrisClear(void);
TETRIS_SCENE TetrisNewmino(void);
TETRIS_SCENE TetrisGameover(void);




static TETRIS_SCENE tetrisScene;

static TETRIS_DATA tetrisData;

static int cellHandle[24];
static int boardHandle;
static int frameHandle;
static int minoHandle[8];


/// <summary>
/// ライン消去タイマー
/// </summary>
static int clearTimer = 0;
static int clearTimerStart = 0;


/// <summary>
/// BAG関係
/// </summary>
static int bagSeed;
static MINO_TYPE bag[BAG_MAX];






/// 新しいBagを作成します。
static void getNewBag(MINO_TYPE* newBag, int bagSeed);




/// <summary>
/// テトリスに使用するリソースをすべて初期化します。
/// </summary>
/// <returns></returns>
BOOL TetrisResourceInit(void)
{
    int result = LoadDivGraph("../Resource/cell.png", 24, 4, 6, 32, 32, cellHandle);
    if (result == -1) { return FALSE; }

    boardHandle = LoadGraph("../Resource/board.png");
    if (boardHandle == -1) { return FALSE; }

    frameHandle = LoadGraph("../Resource/frame.png");
    if (frameHandle == -1) { return FALSE; }

    minoHandle[0] = LoadGraph("../Resource/x.png");
    if (minoHandle[0] == -1) { return FALSE; }
    minoHandle[1] = LoadGraph("../Resource/minoI.png");
    if (minoHandle[1] == -1) { return FALSE; }
    minoHandle[2] = LoadGraph("../Resource/minoJ.png");
    if (minoHandle[2] == -1) { return FALSE; }
    minoHandle[3] = LoadGraph("../Resource/minoL.png");
    if (minoHandle[3] == -1) { return FALSE; }
    minoHandle[4] = LoadGraph("../Resource/minoO.png");
    if (minoHandle[4] == -1) { return FALSE; }
    minoHandle[5] = LoadGraph("../Resource/minoS.png");
    if (minoHandle[5] == -1) { return FALSE; }
    minoHandle[6] = LoadGraph("../Resource/minoT.png");
    if (minoHandle[6] == -1) { return FALSE; }
    minoHandle[7] = LoadGraph("../Resource/minoZ.png");
    if (minoHandle[7] == -1) { return FALSE; }

    return TRUE;
}



/// <summary>
/// テトリスに使用するステータスをすべて初期化します。
/// </summary>
/// <returns></returns>
void TetrisStatusInit(void)
{
    tetrisScene = TETRIS_INIT;

    /// すべてのBagを作成します。
    bagSeed = (unsigned int)time(NULL);
    getNewBag(bag, bagSeed);

    // テトリスデータを初期化します。
    newTetrisData(&tetrisData);

    // 意思決定を開始します。
    AiInit();
    AiDecision(&tetrisData);
}




/// <summary>
/// テトリスを動かします。
/// </summary>
/// <returns></returns>
BOOL TetrisPlay(void)
{
    switch (tetrisScene)
    {
    case TETRIS_INIT:
        tetrisScene = TETRIS_INPUT;
        break;
    case TETRIS_INPUT:
        //tetrisScene = TetrisInput();
        tetrisScene = TetrisAiInput();
        break;
    case TETRIS_CLEAR:
        tetrisScene = TetrisClear();
        break;
    case TETRIS_NEWMINO:
        tetrisScene = TetrisNewmino();
        break;
    case TETRIS_GAMEOVER:
        tetrisScene = TetrisGameover();
        break;
    }

    return TRUE;
}



TETRIS_SCENE TetrisInput(void)
{
    int keyTrigger = getKeyTrigger();
    int keyStatus = getKeyState();

    if ((keyTrigger & INPUT_L) != 0)
    {
        MoveLeft(&tetrisData);
    }
    if ((keyTrigger & INPUT_R) != 0)
    {
        MoveRight(&tetrisData);
    }
    if ((keyStatus & INPUT_D) != 0)
    {
        MoveDown(&tetrisData);
    }
    if ((keyTrigger & INPUT_A) != 0)
    {
        MoveA(&tetrisData);
    }
    if ((keyTrigger & INPUT_B) != 0)
    {
        MoveB(&tetrisData);
    }
    if ((keyTrigger & INPUT_H) != 0)
    {
        Hold(&tetrisData);
    }
    if ((keyTrigger & INPUT_U) != 0)
    {
        MovePut(&tetrisData);
        finishDrop(&tetrisData);
        int deleteLine = setFillBoardDeleteCell(tetrisData.board);
        switch (deleteLine)
        {
        case 0:
            clearTimer = 0;
            break;
        case 1:
            clearTimer = 40;
            clearTimerStart = 40;
            break;
        case 2:
        case 3:
            clearTimer = 45;
            clearTimerStart = 45;
            break;
        case 4:
            clearTimer = 50;
            clearTimerStart = 50;
            break;

        }
        return TETRIS_CLEAR;
    }

    return TETRIS_INPUT;
}


static int aiInputStatus = 0;
static char aiInputCommand[32] = { 0 };
static int aiInputCommandTarget = 0;
TETRIS_SCENE TetrisAiInput(void)
{
    if (aiInputStatus == 0)
    {
        // 入力コマンドを取得します。
        char* solutionCommand = AiGetSolution();
        strcpy_s(aiInputCommand, 32, solutionCommand);

        // 有効データを事前に作成して次の意思決定を実行します。
        {
            TETRIS_DATA d;
            AiGetSolutionTetris(&d);

            Debug("%s\n", aiInputCommand);

            AiDecision(&d);
        }

        aiInputStatus++;
    }

    if (aiInputStatus == 1)
    {
        Debug("　　[%c]を実行します。\n", aiInputCommand[aiInputCommandTarget]);

        switch (aiInputCommand[aiInputCommandTarget])
        {
        case 'l':
            MoveLeft(&tetrisData);
            aiInputCommandTarget++;
            break;
        case 'r':
            MoveRight(&tetrisData);
            aiInputCommandTarget++;
            break;
        case 'd':
            if (MoveDown(&tetrisData) == FALSE)
            {
                aiInputCommandTarget++;
            }
            break;
        case 'R':
            MoveA(&tetrisData);
            aiInputCommandTarget++;
            break;
        case 'L':
            MoveB(&tetrisData);
            aiInputCommandTarget++;
            break;
        case 'h':
            Hold(&tetrisData);
            aiInputCommandTarget++;
            break;
        case 'u':
            MovePut(&tetrisData);
            finishDrop(&tetrisData);
            int deleteLine = setFillBoardDeleteCell(tetrisData.board);
            switch (deleteLine)
            {
            case 0:
                clearTimer = 0;
                break;
            case 1:
                clearTimer = 40;
                clearTimerStart = 40;
                break;
            case 2:
            case 3:
                clearTimer = 45;
                clearTimerStart = 45;
                break;
            case 4:
                clearTimer = 50;
                clearTimerStart = 50;
                break;

            }
            aiInputCommandTarget = 0;
            aiInputStatus = 0;
            return TETRIS_CLEAR;
        }
    }

    return TETRIS_INPUT;
}


TETRIS_SCENE TetrisClear(void)
{
    clearTimer--;
    if (clearTimer <= 0)
    {
        clearTimer = 0;
        CorrectBoard(tetrisData.board);
        return TETRIS_NEWMINO;
    }
    return TETRIS_CLEAR;
}


TETRIS_SCENE TetrisNewmino(void)
{
    newMino(&tetrisData);
    return TETRIS_INPUT;
}


TETRIS_SCENE TetrisGameover(void)
{
    return TETRIS_GAMEOVER;
}






/// <summary>
/// テトリスを描画します。
/// </summary>
/// <returns></returns>
BOOL TetrisDraw(void)
{
    /// ボード表示
    {
        DrawGraph(110 - 3, 32 - 3, boardHandle, TRUE);
    }
    for (int y = 0; y < 20; y++)
    {
        for (int x = 0; x < BOARD_W; x++)
        {
            MINO_TYPE cell = tetrisData.board[x + (y * BOARD_W)];

            if (cell == N) { continue; }

            int index = 0;
            switch (cell)
            {
            case I: index = 1; break;
            case J: index = 2; break;
            case L: index = 3; break;
            case O: index = 4; break;
            case S: index = 5; break;
            case T: index = 6; break;
            case Z: index = 7; break;
            case G: index = 0; break;
            case C: index = 8; break;
            }

            int yPos = 20 - y;
            int xOffset = 110;
            if ((tetrisScene == TETRIS_CLEAR) && (cell == C))
            {
                int h = 32 / 2;
                double ratio = (double)clearTimer / (double)clearTimerStart;
                h = (int)(h * ratio);

                DrawExtendGraph(x * 32 + xOffset, yPos * 32 + (32 / 2) - h, x * 32 + xOffset + 32, yPos * 32 + (32 / 2) + h, cellHandle[index], TRUE);
            }
            else
            {
                DrawGraph(x * 32 + xOffset, yPos * 32, cellHandle[index], TRUE);
            }
        }
    }

    // カレント表示
    if (tetrisScene == TETRIS_INPUT)
    {
        POINT cell[4];
        GetCellPoints(tetrisData.control.current, &tetrisData.control.pos, tetrisData.control.r, cell);

        int index = 0;
        switch (tetrisData.control.current)
        {
        case I: index = 9; break;
        case J: index = 10; break;
        case L: index = 11; break;
        case O: index = 12; break;
        case S: index = 13; break;
        case T: index = 14; break;
        case Z: index = 15; break;
        case G: index = 0; break;
        case C: index = 8; break;
        }
        for (int i = 0; i < 4; i++)
        {
            int yPos = 20 - cell[i].y;
            int xOffset = 110;
            DrawGraph(cell[i].x * 32 + xOffset, yPos * 32, cellHandle[index], TRUE);
        }
    }

    // ゴースト表示
    if (tetrisScene == TETRIS_INPUT)
    {
        POINT cell[4];
        getGhostCell(&tetrisData, cell);

        int index = 16;
        switch (tetrisData.control.current)
        {
        case I: index = 17; break;
        case J: index = 18; break;
        case L: index = 19; break;
        case O: index = 20; break;
        case S: index = 21; break;
        case T: index = 22; break;
        case Z: index = 23; break;
        }
        for (int i = 0; i < 4; i++)
        {
            int yPos = 20 - cell[i].y;
            int xOffset = 110;
            DrawGraph(cell[i].x * 32 + xOffset, yPos * 32, cellHandle[index], TRUE);
        }
    }

    // ネクストフレーム表示
    for (int i = 0; i < NEXT_MAX; i++)
    {
        DrawGraph(450, 32 + (i * 80), frameHandle, TRUE);
    }

    // ホールドフレーム表示
    DrawGraph(10, 32, frameHandle, TRUE);

    // ネクスト表示
    int frameW;
    int frameH;
    GetGraphSize(frameHandle, &frameW, &frameH);
    for (int i = 0; i < NEXT_MAX; i++)
    {
        int index = 0;
        switch (tetrisData.next[i])
        {
        case I: index = 1; break;
        case J: index = 2; break;
        case L: index = 3; break;
        case O: index = 4; break;
        case S: index = 5; break;
        case T: index = 6; break;
        case Z: index = 7; break;
        case G: index = 0; break;
        case C: index = 0; break;
        case N: index = 0; break;
        }

        int minoW;
        int minoH;
        GetGraphSize(minoHandle[index], &minoW, &minoH);

        int x = (frameW / 2) - (minoW / 2);
        int y = (frameH / 2) - (minoH / 2);
        DrawGraph(450 + x, 32 + (i * 80) + y, minoHandle[index], TRUE);
    }

    // ホールド表示
    if (tetrisData.hold != N)
    {
        int index = 0;
        switch (tetrisData.hold)
        {
        case I: index = 1; break;
        case J: index = 2; break;
        case L: index = 3; break;
        case O: index = 4; break;
        case S: index = 5; break;
        case T: index = 6; break;
        case Z: index = 7; break;
        case G: index = 0; break;
        case C: index = 0; break;
        case N: index = 0; break;
        }

        int minoW;
        int minoH;
        GetGraphSize(minoHandle[index], &minoW, &minoH);

        int x = (frameW / 2) - (minoW / 2);
        int y = (frameH / 2) - (minoH / 2);
        DrawGraph(10 + x, 32 + + y, minoHandle[index], TRUE);
    }
    if (tetrisData.holdEnable == false)
    {
        int minoW;
        int minoH;
        GetGraphSize(minoHandle[0], &minoW, &minoH);

        int x = (frameW / 2) - (minoW / 2);
        int y = (frameH / 2) - (minoH / 2);
        DrawGraph(10 + x, 32 + +y, minoHandle[0], TRUE);
    }




    unsigned int color = GetColor(0xFF, 0xFF, 0xFF);
    SetFontSize(20);
    int y = 20;
    DrawFormatString(550, y += 20, color, "■Control");
    DrawFormatString(550, y += 20, color, "　prevSrs  = %d", tetrisData.control.prevSrs);
    DrawFormatString(550, y += 20, color, "　prevSpin = %d", tetrisData.control.prevSpin);
    DrawFormatString(550, y += 20, color, "　btb      = %d", tetrisData.control.btb);
    DrawFormatString(550, y += 20, color, "　ren      = %d", tetrisData.control.ren);
    DrawFormatString(550, y += 20, color, "　delay    = %d", tetrisData.control.delay);

    DrawFormatString(550, y += 20, color, "■prevControl");
    DrawFormatString(550, y += 20, color, "　prevSrs  = %d", tetrisData.prevControl.prevSrs);
    DrawFormatString(550, y += 20, color, "　prevSpin = %d", tetrisData.prevControl.prevSpin);
    DrawFormatString(550, y += 20, color, "　btb      = %d", tetrisData.prevControl.btb);
    DrawFormatString(550, y += 20, color, "　ren      = %d", tetrisData.prevControl.ren);
    DrawFormatString(550, y += 20, color, "　delay    = %d", tetrisData.prevControl.delay);

    DrawFormatString(550, y += 20, color, "■prevResult");
    DrawFormatString(550, y += 20, color, "　deleteLine = %d", tetrisData.prevResult.deleteLine);
    DrawFormatString(550, y += 20, color, "　sendLine   = %d", tetrisData.prevResult.sendLine);
    DrawFormatString(550, y += 20, color, "　Tsm        = %d", tetrisData.prevResult.Tsm);
    DrawFormatString(550, y += 20, color, "　Tss        = %d", tetrisData.prevResult.Tss);
    DrawFormatString(550, y += 20, color, "　Tsd        = %d", tetrisData.prevResult.Tsd);
    DrawFormatString(550, y += 20, color, "　Tst        = %d", tetrisData.prevResult.Tst);
    DrawFormatString(550, y += 20, color, "　Tetris     = %d", tetrisData.prevResult.Tetris);
    DrawFormatString(550, y += 20, color, "　Tsmd       = %d", tetrisData.prevResult.Tsmd);
    DrawFormatString(550, y += 20, color, "　Pc         = %d", tetrisData.prevResult.Pc);
    DrawFormatString(550, y += 20, color, "　Delay      = %d", tetrisData.prevResult.Delay);

    DrawFormatString(550, y += 20, color, "■Result");
    DrawFormatString(550, y += 20, color, "　deleteLine = %d", tetrisData.result.deleteLine);
    DrawFormatString(550, y += 20, color, "　sendLine   = %d", tetrisData.result.sendLine);
    DrawFormatString(550, y += 20, color, "　Tsm        = %d", tetrisData.result.Tsm);
    DrawFormatString(550, y += 20, color, "　Tss        = %d", tetrisData.result.Tss);
    DrawFormatString(550, y += 20, color, "　Tsd        = %d", tetrisData.result.Tsd);
    DrawFormatString(550, y += 20, color, "　Tst        = %d", tetrisData.result.Tst);
    DrawFormatString(550, y += 20, color, "　Tetris     = %d", tetrisData.result.Tetris);
    DrawFormatString(550, y += 20, color, "　Tsmd       = %d", tetrisData.result.Tsmd);
    DrawFormatString(550, y += 20, color, "　Pc         = %d", tetrisData.result.Pc);
    DrawFormatString(550, y += 20, color, "　Delay      = %d", tetrisData.result.Delay);

    return TRUE;
}




/// <summary>
/// テトリスリソースデータをすべて解放します。
/// </summary>
/// <returns></returns>
BOOL TetrisRelease(void)
{
    AiRelease();

    for (int i = 0; i < 24; i++)
    {
        DeleteGraph(cellHandle[i]);
    }
    DeleteGraph(boardHandle);
    DeleteGraph(frameHandle);
    for (int i = 0; i < 8; i++)
    {
        DeleteGraph(minoHandle[i]);
    }

    return TRUE;
}





/// <summary>
/// 新しいBagを作成します。
/// </summary>
/// <param name="newBag">要素は1000確保してください。</param>
/// <param name="bagSeed">シード値です</param>
static void getNewBag(MINO_TYPE* newBag, int bagSeed)
{
    srand(bagSeed);

    for (int i = 0; i < BAG_MAX; i += 7)
    {
        /// シャッフルします。
        MINO_TYPE tempBag[7] = { I, J, L, O, S, T, Z };
        for (int n = 0; n < 7; n++)
        {
            int target = rand() % 7;

            MINO_TYPE t = tempBag[n];
            tempBag[n] = tempBag[target];
            tempBag[target] = t;
        }

        for (int n = 0; n < 7; n++)
        {
            newBag[i + n] = tempBag[n];
        }
    }
}

/// <summary>
/// BAGからミノを取得する
/// </summary>
/// <param name="index"></param>
/// <returns></returns>
MINO_TYPE getBagMino(int index)
{
    return bag[index];
}

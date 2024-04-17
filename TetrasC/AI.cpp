#include "DxLib.h"
#include "TetrisData.h"
#include "Decision.h"
#include <thread>

using namespace std;





/// スレッドのインスタンスです。
thread DecisionInstance;

TETRIS_DATA decisionTetris;

/// <summary>
/// AI意思決定の初期化を行います。
/// </summary>
void AiInit(void)
{
    DecisionSystemInit();
}


/// <summary>
/// AI意思決定の解放を行います。
/// </summary>
void AiRelease(void)
{
    DecisionSystemRelease();
}


/// <summary>
/// Aiの意思決定を開始します。
/// </summary>
void AiDecision(TETRIS_DATA* tet)
{
    memcpy_s(&decisionTetris, sizeof(TETRIS_DATA), tet, sizeof(TETRIS_DATA));

    DecisionInstance = thread(Decision, &decisionTetris);
}



/// <summary>
/// 現在の意思決定を停止します。
/// </summary>
void AiAbort(void)
{
    if (DecisionInstance.joinable() == true)
    {
        DecisionInstance.join();
    }
}



/// <summary>
/// 現在の決定されたコマンドを取得します。
/// </summary>
/// <returns></returns>
char* AiGetSolution(void)
{
    if (DecisionInstance.joinable() == true)
    {
        DecisionInstance.join();
    }

    char* command = getDecisionCommand();

    return command;
}



/// <summary>
/// 現在の決定されたデータを取得します。
/// </summary>
/// <returns></returns>
void AiGetSolutionTetris(TETRIS_DATA* solution)
{
    if (DecisionInstance.joinable() == true)
    {
        DecisionInstance.join();
    }

    getDecisionTetrisData(solution);
}



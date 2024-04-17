#include "DxLib.h"
#include "TetrisData.h"
#include "Decision.h"
#include <thread>

using namespace std;





/// �X���b�h�̃C���X�^���X�ł��B
thread DecisionInstance;

TETRIS_DATA decisionTetris;

/// <summary>
/// AI�ӎv����̏��������s���܂��B
/// </summary>
void AiInit(void)
{
    DecisionSystemInit();
}


/// <summary>
/// AI�ӎv����̉�����s���܂��B
/// </summary>
void AiRelease(void)
{
    DecisionSystemRelease();
}


/// <summary>
/// Ai�̈ӎv������J�n���܂��B
/// </summary>
void AiDecision(TETRIS_DATA* tet)
{
    memcpy_s(&decisionTetris, sizeof(TETRIS_DATA), tet, sizeof(TETRIS_DATA));

    DecisionInstance = thread(Decision, &decisionTetris);
}



/// <summary>
/// ���݂̈ӎv������~���܂��B
/// </summary>
void AiAbort(void)
{
    if (DecisionInstance.joinable() == true)
    {
        DecisionInstance.join();
    }
}



/// <summary>
/// ���݂̌��肳�ꂽ�R�}���h���擾���܂��B
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
/// ���݂̌��肳�ꂽ�f�[�^���擾���܂��B
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



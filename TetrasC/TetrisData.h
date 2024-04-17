#pragma once

#include "DxLib.h"



#define BOARD_W     10
#define BOARD_H     44
#define NEXT_MAX    5

/// ����͑S�̂̃~�m���Ȃ̂�7�̔{���Ŏw�肵�Ă��������B
#define BAG_MAX     1400

typedef enum
{
    N,
    I,
    J,
    L,
    O,
    S,
    T,
    Z,
    G,
    C
} MINO_TYPE;


typedef struct
{
    MINO_TYPE current;
    POINT pos;
    int r;
    int prevSrs;
    BOOL prevSpin;
    BOOL btb;
    int ren;
    int delay;
} CONTROL;



typedef struct
{
    int deleteLine;
    int sendLine;
    int Tsm;
    int Tsmd;
    int Tss;
    int Tsd;
    int Tst;
    int Tetris;
    int Pc;
    int Delay;
} RESULT;

/// <summary>
/// �e�g���X�f�[�^�\���̂ł��B
/// ��l�p�̂��ׂẴf�[�^�������܂��B
/// </summary>
typedef struct
{
    MINO_TYPE board[BOARD_W * BOARD_H];
    MINO_TYPE next[NEXT_MAX];
    BOOL holdEnable;
    MINO_TYPE hold;

    CONTROL control;
    CONTROL prevControl;

    RESULT prevResult;
    RESULT result;
    
    int bagTarget;

} TETRIS_DATA;








/// <summary>
/// �e�g���X�f�[�^�����������܂��B
/// �J�����g�����o���ăQ�[���\�ȏ�Ԃɂ��܂��B
/// </summary>
/// <param name="tet"></param>
/// <returns></returns>
extern void newTetrisData(TETRIS_DATA* tet);

/// <summary>
/// �V�����~�m�����o���܂��B
/// </summary>
/// <param name="tet"></param>
/// <returns></returns>
extern BOOL newMino(TETRIS_DATA* tet);

/// <summary>
/// �ݒu��̏����ł��B
/// </summary>
/// <param name="tet"></param>
/// <returns></returns>
extern void finishDrop(TETRIS_DATA* tet);

/// <summary>
/// �z�[���h�����o�������ł��B
/// </summary>
extern BOOL Hold(TETRIS_DATA* tet);

extern BOOL MoveLeft(TETRIS_DATA* tet);
extern BOOL MoveRight(TETRIS_DATA* tet);
extern BOOL MoveDown(TETRIS_DATA* tet);
extern int MoveDD(TETRIS_DATA* tet);
extern BOOL MoveA(TETRIS_DATA* tet);
extern BOOL MoveB(TETRIS_DATA* tet);
extern BOOL MovePut(TETRIS_DATA* tet);

/// <summary>
/// ��]�������~�m�̈ʒu��Ԃ��܂��B
/// </summary>
/// <param name="current"></param>
/// <param name="currentPoint"></param>
/// <param name="r"></param>
/// <param name="cells"></param>
extern void GetCellPoints(MINO_TYPE current, POINT* currentPoint, int r, POINT* cells);

/// <summary>
/// �S�[�X�g�̃Z���ʒu���擾���܂��B
/// </summary>
extern void getGhostCell(TETRIS_DATA* tet, POINT* cells);


/// <summary>
/// �t�B�[���h�𐳋K�����A���C���������s���܂��B
/// 0-19���C���ڂ܂ł����������܂���B
/// </summary>
extern int CorrectBoard(MINO_TYPE* board, BOOL clear = TRUE);


/// <summary>
/// ���܂��Ă��郉�C���ɏ����p�Z����ݒ肵�܂��B
/// </summary>
extern int setFillBoardDeleteCell(MINO_TYPE* board);



/// �~�m�̌`����l�������J�n���W���擾���܂�
extern BOOL GetMinoPisitionStart(MINO_TYPE* board, MINO_TYPE current, POINT* pos);



/// �~�m�ƃt�B�[���h���Փ˂��Ă��邩�ǂ�����Ԃ��܂��B
extern BOOL CheckCollision(MINO_TYPE* board, POINT* cellPoints);


/// <summary>
/// �n�[�h�h���b�v�����ꏊ���擾���܂��B
/// </summary>
extern int getHarddropPos(TETRIS_DATA* tet, POINT* pos);
extern int getHarddropPos(MINO_TYPE* board, MINO_TYPE current, int r, POINT* pos);

/// <summary>
/// �{�[�h�ɒu���܂��B
/// </summary>
/// <param name="board"></param>
/// <param name="current"></param>
/// <param name="r"></param>
/// <param name="pos"></param>
extern void putBoard(MINO_TYPE* board, MINO_TYPE current, int r, POINT* pos);



/// <summary>
/// TSpin���ǂ����̔�����s���܂��B
/// 1��TSpin�A2��TSpinMini�ł�
/// </summary>
/// <param name="board"></param>
/// <param name="current"></param>
/// <param name="currentPoint"></param>
/// <param name="r"></param>
/// <param name="spin"></param>
/// <param name="srs"></param>
/// <returns></returns>
extern int CheckTSpin(MINO_TYPE* board, MINO_TYPE current, POINT* currentPoint, int r, bool spin, int srs);


/// �X�[�p�[���[�e�[�V�����V�X�e�����������܂��B�Ԃ�l��SRS�p�^�[���ł�
extern int GetSuperRotateMinoPoint(MINO_TYPE* board, MINO_TYPE current, POINT* currentPoint, int r, int nextR);


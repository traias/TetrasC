#include <stdio.h>
#include "TetrisData.h"
#include "Decision.h"
#include "DecisionPattern.h"



/// �萔�錾�ł�
/// ���̃t�@�C���ł����g�p���Ȃ��萔��錾���܂�


#define BASE_MOVE_MAX       43

/// �x�[�X�ɂȂ�h���b�v�R�}���h�ł��B
const char bsseDropCommand[BASE_MOVE_MAX][32]
{
    "lllllu",
    "llllu",
    "lllu",
    "llu",
    "lu",
    "u",
    "ru",
    "rru",
    "rrru",
    "rrrru",
    "rrrrru",
    "Rlllllu",
    "Rllllu",
    "Rlllu",
    "Rllu",
    "Rlu",
    "Ru",
    "Rru",
    "Rrru",
    "Rrrru",
    "Rrrrru",
    "Lllllu",
    "Llllu",
    "Lllu",
    "Llu",
    "Lu",
    "Lru",
    "Lrru",
    "Lrrru",
    "Lrrrru",
    "Lrrrrru",
    "RlRllllu",
    "RlRlllu",
    "RlRllu",
    "RlRlu",
    "RlRu",
    "RRu",
    "RrRu",
    "RrRru",
    "RrRrru",
    "RrRrrru",
    "RrRrrrru"
    "LLu",
};

/// ��������֐��v���g�^�C�v�錾�ł�

/// �h���b�v�S�p�^�[�����e�X�g���܂�
/// �h���b�v�S�p�^�[�����e�X�g���܂��B
int GetDropAllPattern(TETRIS_DATA* tetris, DECISION_TETRIS** pattern, int indexStart, int indexMax, const char prefix);

/// �x�[�X�ɂȂ�h���b�v��Ԃ�S�f�[�^�쐬���܂��B
int BaseDropTest(TETRIS_DATA* tetris, DECISION_TETRIS** pattern, int indexStart, int indexMax, const char* command);

/// �h���b�v��Ԃ�S�f�[�^�쐬���܂��B
int DropTest(TETRIS_DATA* tetris, DECISION_TETRIS** pattern, int registCount, int PatternMax, char* command, const char* addCommand);

/// ���łɃp�^�[���o�^�ς݂��ǂ������`�F�b�N���܂�
BOOL findRegistPattern(DECISION_TETRIS* check, DECISION_TETRIS** pattern, int start, int end);

/// �p�^�[���o�^���s���܂�
void registPattern(TETRIS_DATA* tetris, DECISION_TETRIS** pattern, int index, char* command, int harddropCell);

/// �n�b�V���R�[�h�𓾂܂�
unsigned __int64 getHash(MINO_TYPE mino, int r, POINT* cell);

/// �`��n�b�V���R�[�h������̃f�[�^���폜���܂�
void distinctHash(DECISION_TETRIS** pattern, int start, int end);

/// �h���b�v�Ɋւ���f�B���C���擾���܂�
int getDropDelay(TETRIS_DATA* Base, POINT* MinoPoint, int R);


/// �֐��̎����ł�

/// ���݂̔Ֆʂ��玟�̔ՖʂɌ������S�h���b�v�p�^�[�����擾���܂��B
int GetDropPattern(TETRIS_DATA* tetris, DECISION_TETRIS** pattern, int patternMax)
{
    if (tetris->control.current == N) return 0;

    /// �x�[�X�f�[�^��ύX���邽�߁A�x�[�X���R�s�[���܂�
    TETRIS_DATA copyTetris;
    memcpy_s(&copyTetris, sizeof(TETRIS_DATA), tetris, sizeof(TETRIS_DATA));

    /// �h���b�v�p�^�[�����������܂�
    int registCount = GetDropAllPattern(&copyTetris, pattern, 0, patternMax, ' ');

    /// �ݒu�`�󂪓���ȃf�[�^���d���Ƃ��č폜���܂��B
    distinctHash(pattern, 0, registCount);

    /// �z�[���h�����ł��B
    memcpy_s(&copyTetris, sizeof(TETRIS_DATA), tetris, sizeof(TETRIS_DATA));
    if ((copyTetris.hold != copyTetris.control.current))
    {
        /// �z�[���h�̎��o�����s���܂�
        Hold(&copyTetris);

        /// ���o�������ʃz�[���h���g���Ȃ������ꍇ�͏������܂���
        if (copyTetris.control.current != N)
        {
            int prevRegistCount = registCount;

            /// �h���b�v�p�^�[�����������܂�
            registCount = GetDropAllPattern(&copyTetris, pattern, registCount, patternMax, 'h');

            /// �ݒu�`�󂪓���ȃf�[�^���d���Ƃ��č폜���܂��B
            distinctHash(pattern, prevRegistCount, registCount);
        }
    }

    /// �]�����f�[�^�͖����ɂ��Ă����܂��B
    for (int i = registCount; i < patternMax; i++)
    {
        (*pattern)[i].enable = FALSE;
    }

    return registCount;
}




/// �h���b�v�S�p�^�[�����e�X�g���܂��B
int GetDropAllPattern(TETRIS_DATA* tetris, DECISION_TETRIS** pattern, int startIndex, int patternMax, const char prefix)
{
    /// �~�m�������ʒu�ɐݒ肵�܂��B
    GetMinoPisitionStart(tetris->board, tetris->control.current, &tetris->control.pos);

    /// �ŏ�����Ԃ����Ă�����I�����܂��B
    {
        POINT cell[4];
        GetCellPoints(tetris->control.current, &tetris->control.pos, tetris->control.r, cell);
        if (CheckCollision(tetris->board, cell) == TRUE) { return 0; }
    }

    // �o�^�����f�[�^�̐��ł��B
    int registCount = startIndex;

    /// �ŏ��̓x�[�X��o�^���܂�
    /// �~�m�o������̓����ł�
    for (int i = 0; i < BASE_MOVE_MAX; i++)
    {
        if (registCount >= (patternMax - 1)) { return registCount; }

        // �������R�}���h��p�ӂ��܂��B
        char command[32] = { 0 };
        int index = 0;
        if (prefix == 'h')
        {
            command[0] = prefix;
            index++;
        }
        strcpy_s(&command[index], 32 - 1 - index, (const char*)bsseDropCommand[i]);

        // �x�[�X�f�[�^�̃R�s�[�����Ă����܂��B
        // ���̃R�s�[�𓮂����ăf�[�^���쐬���܂��B
        TETRIS_DATA copyTetris;
        memcpy_s(&copyTetris, sizeof(TETRIS_DATA), tetris, sizeof(TETRIS_DATA));

        registCount = BaseDropTest(&copyTetris, pattern, registCount, patternMax, command);
    }

    /// �o�^�����f�[�^�x�[�X�̃J�E���g�ł��B
    if (registCount >= (patternMax - 1)) { return registCount; }
    
    /// �n�[�h�h���b�v�̑���Ƀ\�t�h�������A�ݒu��̓�����o�^���܂�
    int checkRegistCount = startIndex;
    int prevRegistCount = registCount;
    do
    {
        prevRegistCount = registCount;
        for (int i = checkRegistCount; i < prevRegistCount; i++)
        {
            if ((*pattern)[i].enable == FALSE) { continue; }

            // �r���܂ł̃R�}���h��p�ӂ���
            char command[32] = { 0 };
            strcpy_s(command, 32, (*pattern)[i].command);

            command[strlen(command) - 1] = '\0';
            if ((*pattern)[i].harddropCell > 0)
            {
                strcat_s(command, 32, "d");
            }

            // �h���b�v�̃e�X�g�����s[��]]
            {
                const char* addCommand[2] = { "Lu", "Ru" };

                for (int n = 0; n < 2; n++)
                {
                    // ���ʂ̃f�[�^����T���p�f�[�^�𕜋�������
                    TETRIS_DATA copyTetris;
                    memcpy_s(&copyTetris, sizeof(TETRIS_DATA), tetris, sizeof(TETRIS_DATA));
                    memcpy_s(&copyTetris.control, sizeof(CONTROL), &(*pattern)[i].tetris.prevControl, sizeof(CONTROL));
                    copyTetris.control.delay += (*pattern)[i].harddropCell * 3;

                    int regist = DropTest(&copyTetris, pattern, registCount, patternMax, command, addCommand[n]);
                    if (regist == registCount) { continue; }

                    // �o�^�������̂��O��Ɠ���f�[�^�ɑ��݂��Ă����珈�����܂���B
                    if (findRegistPattern(&(*pattern)[registCount], pattern, startIndex, registCount) == TRUE)
                    {
                        (*pattern)[registCount].enable = FALSE;
                    }
                    else
                    {
                        registCount = regist;
                    }
                }
            }

            // �h���b�v�̃e�X�g�����s[���ړ�]
            {
                const char* addCommand[6] = { "lu", "llu", "lllu", "llllu", "lllllu", "llllllu" };

                for (int n = 0; n < 6; n++)
                {
                    // ���ʂ̃f�[�^����T���p�f�[�^�𕜋�������
                    TETRIS_DATA copyTetris;
                    memcpy_s(&copyTetris, sizeof(TETRIS_DATA), tetris, sizeof(TETRIS_DATA));
                    memcpy_s(&copyTetris.control, sizeof(CONTROL), &(*pattern)[i].tetris.prevControl, sizeof(CONTROL));
                    copyTetris.control.delay += (*pattern)[i].harddropCell * 3;

                    int regist = DropTest(&copyTetris, pattern, registCount, patternMax, command, addCommand[n]);
                    if (regist == registCount) { break; }

                    // �o�^�������̂��O��Ɠ���f�[�^�ɑ��݂��Ă����珈�����܂���B
                    if (findRegistPattern(&(*pattern)[registCount], pattern, startIndex, registCount) == TRUE)
                    {
                        (*pattern)[registCount].enable = FALSE;
                    }
                    else
                    {
                        registCount = regist;
                    }
                }
            }

            {
                // �h���b�v�̃e�X�g�����s[�E�ړ�]
                const char* addCommand[6] = { "ru", "rru", "rrru", "rrrru", "rrrrru", "rrrrrru" };

                for (int n = 0; n < 6; n++)
                {
                    // ���ʂ̃f�[�^����T���p�f�[�^�𕜋�������
                    TETRIS_DATA copyTetris;
                    memcpy_s(&copyTetris, sizeof(TETRIS_DATA), tetris, sizeof(TETRIS_DATA));
                    memcpy_s(&copyTetris.control, sizeof(CONTROL), &(*pattern)[i].tetris.prevControl, sizeof(CONTROL));
                    copyTetris.control.delay += (*pattern)[i].harddropCell * 3;

                    int regist = DropTest(&copyTetris, pattern, registCount, patternMax, command, addCommand[n]);
                    if (regist == registCount) { break; }

                    // �o�^�������̂��O��Ɠ���f�[�^�ɑ��݂��Ă����珈�����܂���B
                    if (findRegistPattern(&(*pattern)[registCount], pattern, startIndex, registCount) == TRUE)
                    {
                        (*pattern)[registCount].enable = FALSE;
                    }
                    else
                    {
                        registCount = regist;
                    }
                }
            }
        }
        checkRegistCount = prevRegistCount;
    } while (prevRegistCount != registCount);

    return registCount;
}

/// �x�[�X�ɂȂ�h���b�v��Ԃ�S�f�[�^�쐬���܂��B
int BaseDropTest(TETRIS_DATA* tetris, DECISION_TETRIS** pattern, int registCount, int patternMax, const char* command)
{
    // �R�}���h�ʂ�ɓ������܂��B
    for (int i = 0; command[i]; i++)
    {
        switch (command[i])
        {
            case 'L':
                if (MoveB(tetris) == FALSE) { return registCount; }
                break;

            case 'R':
                if (MoveA(tetris) == FALSE) { return registCount; }
                break;

            case 'l':
                if (MoveLeft(tetris) == FALSE) { return registCount; }
                break;

            case 'r':
                if (MoveRight(tetris) == FALSE) { return registCount; }
                break;

            // �ŉ��i�܂ŗ��Ƃ��ēo�^���ďI���ɂ���
            case 'u':
                int dropCount = MoveDD(tetris);

                /// �V�K�o�^�ł�
                registPattern(tetris, pattern, registCount, (char*)command, dropCount);
                return registCount + 1;
        }
    }

    return registCount;
}

/// �h���b�v��Ԃ�S�f�[�^�쐬���܂��B
int DropTest(TETRIS_DATA* tetris, DECISION_TETRIS** pattern, int registCount, int PatternMax, char* command, const char* addCommand)
{
    // �R�}���h�ʂ�ɓ������܂��B
    for (int i = 0; addCommand[i]; i++)
    {
        switch (addCommand[i])
        {
        case 'L':
            if (MoveB(tetris) == FALSE) { return registCount; }
            break;

        case 'R':
            if (MoveA(tetris) == FALSE) { return registCount; }
            break;

        case 'l':
            if (MoveLeft(tetris) == FALSE) { return registCount; }
            break;

        case 'r':
            if (MoveRight(tetris) == FALSE) { return registCount; }
            break;

            // �ŉ��i�܂ŗ��Ƃ��ēo�^���ďI���ɂ���
        case 'u':
            int dropCount = MoveDD(tetris);

            /// �V�K�o�^�ł�
            char c[32] = { 0 };
            strcpy_s(c, 32, command);
            strcat_s(c, 32, addCommand);
            registPattern(tetris, pattern, registCount, c, dropCount);
            return registCount + 1;
        }
    }
    
    return registCount;
}

/// ���łɃp�^�[���o�^�ς݂��ǂ������`�F�b�N���܂�
BOOL findRegistPattern(DECISION_TETRIS* check, DECISION_TETRIS** pattern, int start, int end)
{
    for (int i = start; i < end; i++)
    {
        if ((*pattern)[i].enable == FALSE) { continue; }
        if ((*pattern)[i].hash == check->hash)
        {
            return TRUE;
        }
    }

    return FALSE;
}


/// �p�^�[���o�^���s���܂�
void registPattern(TETRIS_DATA* tetris, DECISION_TETRIS** pattern, int index, char* command, int harddropCell)
{
    /// �f�[�^�𕡐����܂��B
    memcpy_s(&(*pattern)[index].tetris, sizeof(TETRIS_DATA), tetris, sizeof(TETRIS_DATA));

    // �ݒu���܂��B
    MovePut(&(*pattern)[index].tetris);
    finishDrop(&(*pattern)[index].tetris);
    CorrectBoard((*pattern)[index].tetris.board);
    newMino(&(*pattern)[index].tetris);

    (*pattern)[index].harddropCell = harddropCell;
    strcpy_s((*pattern)[index].command, 32, command);
    {
        POINT cell[4];
        GetCellPoints((*pattern)[index].tetris.prevControl.current, &(*pattern)[index].tetris.prevControl.pos, (*pattern)[index].tetris.prevControl.r, cell);
        (*pattern)[index].hash = getHash((*pattern)[index].tetris.prevControl.current, (*pattern)[index].tetris.prevControl.r, cell);
    }

    /// �f�[�^��L���ɐݒ肵�܂�
    (*pattern)[index].enable = TRUE;
}



/// �n�b�V���R�[�h�𓾂܂�
unsigned __int64 getHash(MINO_TYPE mino, int r, POINT* cell)
{
    return (
        ((((unsigned __int64)mino) << 56UL) & 0xFF00000000000000) |
        ((((unsigned __int64)r) << 48UL) & 0x00FF000000000000) |
        ((((unsigned __int64)cell[3].y) << 40UL) & 0x0000FF0000000000) |
        ((((unsigned __int64)cell[3].x) << 36UL) & 0x000000F000000000) |
        ((((unsigned __int64)cell[2].y) << 28UL) & 0x0000000FF0000000) |
        ((((unsigned __int64)cell[2].x) << 24UL) & 0x000000000F000000) |
        ((((unsigned __int64)cell[1].y) << 16UL) & 0x0000000000FF0000) |
        ((((unsigned __int64)cell[1].x) << 12UL) & 0x000000000000F000) |
        ((((unsigned __int64)cell[0].y) << 4UL) & 0x0000000000000FF0) |
        ((((unsigned __int64)cell[0].x) << 0UL) & 0x000000000000000F)
        );
}

/// �n�b�V��������̃f�[�^���폜���܂�
void distinctHash(DECISION_TETRIS** pattern, int start, int end)
{
    for (int i = start; i < (end - 1); i++)
    {
        if ((*pattern)[i].enable == FALSE) { continue; }

        for (int n = (i + 1); n < end; n++)
        {
            if ((*pattern)[n].enable == FALSE) { continue; }

            /// ��]����������̃f�[�^����������
            if (((*pattern)[i].hash & 0xFF00FFFFFFFFFFFF) == ((*pattern)[n].hash & 0xFF00FFFFFFFFFFFF))
            {
                /// �f�B���C���Z���f�[�^���̗p���܂�
                if ((*pattern)[i].tetris.prevResult.Delay > (*pattern)[n].tetris.prevResult.Delay)
                {
                    (*pattern)[i].enable = FALSE;
                }
                else if ((*pattern)[i].tetris.prevResult.Delay < (*pattern)[n].tetris.prevResult.Delay)
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


/// �h���b�v�Ɋւ���f�B���C���擾���܂�
int getDropDelay(TETRIS_DATA* tetris, POINT* pos, int r)
{
    if (pos == NULL) return 0;

    /// �~�m�z�u�������{�[�h��p�ӂ��܂��B
    MINO_TYPE Board[BOARD_W * BOARD_H] = { N };
    memcpy_s(Board, sizeof(MINO_TYPE) * BOARD_W * BOARD_H, tetris->board, sizeof(MINO_TYPE) * BOARD_W * BOARD_H);
    putBoard(Board, tetris->control.current, tetris->control.r, pos);

    // ��������s���폜���܂��B
    int deleteLine = CorrectBoard(Board);

    BOOL pc = TRUE;
    for (int x = 0; x < BOARD_W; x++)
    {
        if (Board[x] != N) { pc = FALSE; break; }
    }

    /// �f�B���C���v�Z���܂��B
    int delay = 0;
    if (pc == TRUE)
    {
        delay += 1;
    }
    else
    {
        switch (deleteLine)
        {
        case 1:
            delay += 40;
            break;
        case 2:
        case 3:
            delay += 45;
            break;
        case 4:
            delay += 50;
            break;
        }
    }

    return delay;
}

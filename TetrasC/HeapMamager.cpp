#include <corecrt_malloc.h>
#include "Decision.h"


#define HEAP_STRUCT_SIZE    (sizeof (DECISION_TETRIS))
#define HEAP_STRUCT_COUNT   (1024 * 1024)


static DECISION_TETRIS* heap;



/// <summary>
/// �q�[�v�����������܂��B
/// </summary>
void initHeapManager()
{
    heap = (DECISION_TETRIS*)calloc(HEAP_STRUCT_COUNT, HEAP_STRUCT_SIZE);
}


/// <summary>
/// �q�[�v���J�����܂��B
/// </summary>
void releaseHeapManager()
{
    free(heap);
}


/// <summary>
/// �q�[�v����������|�C���^�z����擾���܂��B
/// </summary>
/// <param name="count"></param>
/// <param name=""></param>
/// <returns></returns>
BOOL getHeap(DECISION_TETRIS** data, int count)
{
    int n = 0;
    for (int i = 0; i < HEAP_STRUCT_COUNT; i++)
    {
        if (heap[i].enable == FALSE)
        {
            data[n] = &heap[i];
            n++;
        }

        if (n >= count) { return TRUE; }
    }

    return FALSE;
}



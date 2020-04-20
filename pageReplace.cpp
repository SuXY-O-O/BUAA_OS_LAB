#include "pageReplace.h"

void pageReplace (long* physic_memery, long nwAdd)
{
    long page = nwAdd >> 12;
    unsigned int group = page & 0b11;
    unsigned int index = group << 4;
    unsigned int next_index = ((group + 1) % 4) << 4;
    int i;
    int static sign[64] = {0};
    for (i = 0; i < 64; i++)
        sign[i]++;
    for (i = 0; i < 16; i++)
        {
            if (page == physic_memery[index + i])
            {
                sign[index + i] = 0;
                return;
            }
        }
    for (i = 0; i < 16; i++)
    {
        if (physic_memery[i + index] == 0)
        {
            physic_memery[i + index] = page;
            sign[index + i] = 0;
            physic_memery[i + next_index] = page + 1;
            sign[next_index + i] = 0;
            return;
        }
    }
    int max = 0;
    int next_max = 0;
    for (i = 0; i < 16; i++)
    {
        if (sign[index + i] > sign[index + max])
            max = i;
        if (sign[next_index + i] > sign[next_index + next_max])
            next_max = i;
    }
    physic_memery[max + index] = page;
    physic_memery[next_max + next_index] = page + 1;
    sign[index + max] = 0;
    sign[next_index + next_max] = 0;
}

#include "pageReplace.h"

void pageReplace (long* physic_memery, long nwAdd)
{
    long page = nwAdd >> 12;
    unsigned int group = (page & 0b111) << 3;
    unsigned int end = group + 8;
    int static sign[64] = {0};
    for (int i = 0; i < 64; i++)
        sign[i]++;
    int max = 0;
    for (int i = group; i < end; i++)
    {
        if (page == physic_memery[i])
        {
            sign[i] = 0;
            return;
        }
        if (sign[i] > sign[max])
            max = i;
    }
    physic_memery[max] = page;
    sign[max] = 0;
    return;
}


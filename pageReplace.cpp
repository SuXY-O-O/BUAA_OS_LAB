#include "pageReplace.h"

void pageReplace (long* physic_memery, long nwAdd)
{
    long page = nwAdd >> 12;
    unsigned int group = (page & 0b1111) << 2;
    unsigned int end = group + 4;
    int i;
    int static sign[64] = {0};
    for (i = 0; i < 64; i++)
        sign[i]++;
    for (i = group; i < end; i++)
    {
        if (page == physic_memery[i])
        {
            sign[i] = 0;
            return;
        }
    }
    int max = 0;
    for (i = group; i < end; i++)
    {
        if (sign[i] > sign[max])
            max = i;
    }
    physic_memery[max] = page;
    sign[max] = 0;
    return;
}

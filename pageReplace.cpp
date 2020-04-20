#include "pageReplace.h"

void pageReplace (long* physic_memery, long nwAdd)
{
    long page = nwAdd >> 12;
    unsigned int group = (page & 0b111) << 3;
    int i;
    int static sign[64] = {0};
    int static count[64] = {0};
    for (i = 0; i < 64; i++)
        sign[i]++;
    for (i = group; i < group + 8; i++)
    {
        if (page == physic_memery[i])
        {
            count[i]++;
            if (count[i] == 2)
                sign[i] = 0;
            else
                sign[i] -= 5;
            return;
        }
    }
    int max = 0;
    for (i = group; i < group + 8; i++)
    {
        if (sign[i] > sign[max])
            max = i;
    }
    physic_memery[max] = page;
    sign[max] = 0;
    count[max] = 1;
    return;
}

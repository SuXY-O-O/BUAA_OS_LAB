#include "pageReplace.h"

void pageReplace (long* physic_memery, long nwAdd)
{
    long page = nwAdd >> 12;
    unsigned int group = (page & 0b111) << 3;
    int i;
    int static sign[64] = {0};
    for (i = 0; i < 64; i++)
        sign[i]++;
    for (i = group; i < group + 8; i++)
        {
            if (page == physic_memery[i])
            {
                sign[i] = 0;
                return;
            }
        }
    for (i = group; i < group + 8; i++)
    {
        if (physic_memery[i] == 0)
        {
            physic_memery[i] = page;
            sign[i] = 0;
            return;
        }
    }
    int max = 0;
    for (i = group; i < group + 8; i++)
    {
        if (sign[i] >= sign[max])
            max = i;
    }
    physic_memery[max] = page;
    sign[max] = 0;
    return;
}

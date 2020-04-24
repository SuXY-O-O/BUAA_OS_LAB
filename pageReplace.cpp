#include "pageReplace.h"

void pageReplace (long* physic_memery, long nwAdd)
{
    long page = nwAdd >> 12;
    unsigned int group0 = (page & 0b1111) << 2;
    unsigned int group1 = group0 + 1;
    unsigned int group2 = group0 + 2;
    unsigned int group3 = group0 + 3;
    int static sign[64] = {0};
    sign[group0]++;
    sign[group1]++;
    sign[group2]++;
    sign[group3]++;
    if (page == physic_memery[group0])
    {
        sign[group0] = 0;
        return;
    }
    if (page == physic_memery[group1])
    {
        sign[group1] = 0;
        return;
    }
    if (page == physic_memery[group2])
    {
        sign[group2] = 0;
        return;
    }
    if (page == physic_memery[group3])
    {
        sign[group3] = 0;
        return;
    }
    int max = group0;
    if (sign[group1] > sign[max])
        max = group1;
    if (sign[group2] > sign[max])
        max = group2;
    if (sign[group3] > sign[max])
        max = group3;
    physic_memery[max] = page;
    sign[max] = 0;
    return;
}

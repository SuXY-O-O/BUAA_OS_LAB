#include <stdlib.h>
#include <time.h>
//#include <stdio.h>
#include "pageReplace.h"

void pageReplace (long* physic_memery, long nwAdd)
{
    long page = nwAdd >> 12;
    unsigned int group = (page & 0b1111) << 2;
    int i;
    for (i = group; i < group + 4; i++)
    {
        if (page == physic_memery[i])
        {
            return;
        }
    }
    i = ((rand() % 2) << 1) + (rand() % 2);
    physic_memery[i] = page;
    return;
}

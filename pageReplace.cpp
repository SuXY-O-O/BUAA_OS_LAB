#include <stdlib.h>
#include <time.h>
#include "pageReplace.h"

void pageReplace (long* physic_memery, long nwAdd)
{
    long page = nwAdd >> 12;
    unsigned int group = (page & 0b11) << 4;
    int i;
    //int static sign[64] = {0};
    int static first = 1;
    //for (i = 0; i < 64; i++)
    //    sign[i]++;
    for (i = group; i < group + 16; i++)
        {
            if (page == physic_memery[i])
            {
                return;
               // sign[i] = 0;
            }
        }
    for (i = group; i < group + 16; i++)
    {
        if (physic_memery[i] == 0)
        {
            physic_memery[i] = page;
            //sign[i] = 0;
            if ((i + 1) < (group + 16))
            {
                if (physic_memery[i + 1] == 0)
                {
                    physic_memery[i + 1] = page + 4;
                    //sign[i + 1] = 0;
                }
            }
            return;
        }
    }
    /* for (i = group; i < group + 16; i++)
    {
        if (sign[i] >= 8)
        {
            physic_memery[i] = page;
            sign[i] = 0;
            return;
        }
    }*/
    if (first)
    {
        srand((unsigned int)time(0));
        first = 0;
    }
    i = ((rand() % 2) << 3) + ((rand() % 2) << 2) + ((rand() % 2) << 1) + (rand() % 2);
    physic_memery[i + group] = page;
    //sign[i + group] = 0;
}


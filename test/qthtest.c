/**
 * \file        qthtest.c
 * Test program for the libqth library.
 * Can be linked with both the static and shared libraries.
 *
 * \details
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../qthlib.h"

#define ARRSIZE     255
int
main()
{
    int         i, check, *dp;
    int         arr[ARRSIZE];
    QTHERR      qerr;
    qthhdl_t    qhdl;

    printf("Test 1 - Test adding and removing a bunch of items from the queue as a FIFO\n");
    assert(QTHERR_OK == qthinit(&qhdl, malloc, free));
    printf("Adding in : ");
    for (i=0; i < ARRSIZE; i++)
    {
        arr[i] = i;
        qerr = qthtailadd(qhdl, &arr[i]);
        printf("%d ", i);
    }

    printf("\nRemoving items from queue\n");
    for (i=0; i < ARRSIZE; i++)
    {
        dp = qthheadrem(qhdl, &qerr);
        printf("%d ", *dp);
        assert(*dp == i);
    }
    assert(QTHERR_EMPTY == qthfree(qhdl));

    printf("\n\nTest 2 - Test adding and removing a bunch of items from the queue as a LIFO\n");
    assert(QTHERR_OK == qthinit(&qhdl, malloc, free));
    printf("Adding in : ");
    for (i=0; i < ARRSIZE; i++)
    {
        arr[i] = i;
        qerr = qthheadadd(qhdl, &arr[i]);
        printf("%d ", i);
    }

    printf("\nRemoving items from queue\n");
    i = ARRSIZE;
    while (i--) {
        dp = qthheadrem(qhdl, &qerr);
        printf("%d ", *dp);
        assert(*dp == i);
    }
    printf("\n");
    assert(QTHERR_EMPTY == qthfree(qhdl));

    printf("\n\nTest 3 - Test adding a bunch of items and just freeing it\n");
    assert(QTHERR_OK == qthinit(&qhdl, malloc, free));
    printf("Adding in : ");
    for (i=0; i < ARRSIZE; i++)
    {
        arr[i] = i;
        qerr = qthtailadd(qhdl, &arr[i]);
        printf("%d ", i);
    }
    assert(QTHERR_OK == qthfree(qhdl));

    printf("\n\nTest 4 - Test adding items to tail then head and repeating, and removing them all\n");
    assert(QTHERR_OK == qthinit(&qhdl, malloc, free));
    printf("Adding in : ");
    i = 0;
    while (i < 200)
    {
        arr[i] = i;
        qerr = qthtailadd(qhdl, &arr[i]);
        printf("%d ", i);
        i++;
        arr[i] = i;
        qerr = qthheadadd(qhdl, &arr[i]);
        printf("%d ", i);
        i++;
    }

    printf("\nRemoving from queue : ");
    check = 199;
    for (i=0; i < 100; i++)
    {
        dp = qthheadrem(qhdl, &qerr);
        printf("%d ", *dp);
        assert(*dp == check);
        check -= 2;
    }
    check = 0;
    for (i=0; i < 100; i++)
    {
        dp = qthheadrem(qhdl, &qerr);
        printf("%d ", *dp);
        assert(*dp == check);
        check += 2;
    }
    assert(QTHERR_EMPTY == qthfree(qhdl));

    printf("\n\nTests complete\n");
}
# qth-tail-head - Tail-Head Queue library

## Synopsis
Queue library that provides O(1) add t head, add to tail, and remove head operations.

This queue lib manages the allocation and freeing of the queue header and linkage.
The user does not add links to their own structures or manage a queue header.

This queue manager uses the data poiners passed in and does not copy the data.
Do not release the data while a queue is in use! 
The passed in alloc function is used to allocate a queue header and 
element links, and the free function is used to release element links as 
element data is removed from the queue, and free the queue header when the
queue is freed.

## API Reference
```
#include <qthlib.h>

/** Queueing Library Error codes */
typedef enum {
    QTHERR_OK = 0,      /*!< OK  */
    QTHERR_NOMEM,       /*!< No memory  */
    QTHERR_EMPTY,       /*!< Queue empty, this is OK for freeing  */
    QTHERR_CORRUPT,     /*!< Queue is corrupt  */
    QTHERR_LAST
} QTHERR;

QTHERR qthinit(qthhdl_t *qthhdlp, void *(*alloc)(unsigned long), void (*free)(void *ptr));
QTHERR qthheadadd(qthhdl_t qthhdl, void *datap);
QTHERR qthtailadd(qthhdl_t qthhdl, void *datap);
void *qthheadrem(qthhdl_t qthhdl, QTHERR *qerr);
QTHERR qthfree(qthhdl_t qthhdl);

The qthinit() function initializes a queue and sets the alloction and free functions

The qthheadadd() function adds a data element to the head of a queue.

The qthtailadd() function adds a data element to the tail of a queue.

The qthheadrem() function removes and returns the head of a queue.

The qthfree() function frees any elements and the header of the quque.

```
## Motivation
The benefits of this Queue library are:
  - O(1) add to head, add to tail, and remove from head operations
  - The library manages all queue overhead and linkage
  - Uses user supplied allocation and free functions for managing queue linkage

## Code Example
```
int         i, *dp;
int         arr[ARRSIZE];
QTHERR      qerr;
qthhdl_t    qhdl;

qthinit(&qhdl, malloc, free);
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
```
## Running the test code
```
$ cd qth
$ make
$ cd test
$ make
$ ./qthtest
$ ./qthtest_dyn
```
## License
MIT License

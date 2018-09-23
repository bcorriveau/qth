# qth-tail-head - Tail-Head Queue library

## Synopsis
Queue library that provides O(1) add to head, add to tail, and remove head operations.

This queue library manages the allocation and freeing of the queue header and linkage.
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

QTHERR qth_init(qthhdl_t *qthhdlp, void *(*alloc)(unsigned long), void (*free)(void *ptr));
QTHERR qth_addh(qthhdl_t qthhdl, void *datap);
QTHERR qth_add(qthhdl_t qthhdl, void *datap);
void *qth_iter(qthhdl_t qthhdl, void **iterp);
void *qth_remove(qthhdl_t qthhdl, QTHERR *qerr);
QTHERR qth_free(qthhdl_t qthhdl);

The qth_init() function initializes a queue and sets the alloction and free functions.

The qth_addh() function adds a data element to the head of a queue.

The qth_add() function adds a data element to the tail of a queue.

The qth_iter() function walks the data elements in a queue from head to tail.

The qth_remove() function removes and returns the head of a queue.

The qth_free() function frees any queue elements and the header.

```
## Motivation
The benefits of this Queue library are:
  - O(1) add to head, add to tail, and remove from head operations
  - Iterate through a queue without removing elements
  - The library manages all queue overhead and linkage
  - Uses user supplied allocation and free functions for managing queue linkage

## Code Example
```
int         i, *dp;
int         arr[ARRSIZE];
QTHERR      qerr;
qthhdl_t    qhdl;

qth_init(&qhdl, malloc, free);
for (i=0; i < ARRSIZE; i++)
{
    arr[i] = i;
    qerr = qth_add(qhdl, &arr[i]);
    printf("%d ", i);
}
printf("\nRemoving items from queue\n");
for (i=0; i < ARRSIZE; i++)
{
        dp = qth_remove(qhdl, &qerr);
        printf("%d ", *dp);
        assert(*dp == i);
}
assert(QTHERR_EMPTY == qth_free(qhdl));
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

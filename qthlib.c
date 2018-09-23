/**
 * \brief
 * Queuing library - Uses Tail-Head queue linking organization
 */

#include <stdlib.h>
#include <stdio.h>

#include "qthlib.h"

/**
 * \brief 
 * Queue link for a data item
 *
 * \details
 * Queue link used to link a data into a queue.
 * This queue has the following properties:
 * Empty queue   - hdl is null
 * One element   - hdl points to head, head points to itself
 * Two elements  - hdl points to tail, tail points to head, head points to tail
 * >two elements - hdl points to tail, tail points to head, head points to next..next..etc.
 *                 tail points to head
 */
typedef struct qthlink_s {
    struct qthlink_s    *link;  /*!< link to next q element  */
    void                *datap; /*!< pointer to data for this element  */
} qthlink_t;

/**
 * \brief 
 * Queue header
 *
 * \details
 * The header has a link to the tail of the queue (may be the same as the head)
 * The user sets the alloc and free functions on initialization for what is used
 * to allocate the header and the links. They can just pass in malloc() and free(), or
 * use something more specialized.
 */

typedef struct qthhdr_s {
    qthlink_t *link;                /*!< pointer to tail of queue  */
    void *(*alloc)(unsigned long);  /*!< alloc function for queue linkage  */
    void (*free)(void *);           /*!< free function for queue linkage  */
} *qthhdl_t;


/**
 * \brief
 * Intialize queue
 *
 * \details
 * Allocs and initializes a q. Uses the provided allocation function to allocate
 * the queue header and queue linking. The provided free function is used for
 * freeing operations.
 *
 * \param[in,out]   qthhdrp   pointer to q header pointer
 * \param[in]       alloc     user allocation function used for all q memory allocations
 * \param[in]       free      user free function used for all q memory freeing
 *
 * \return          QTHERR_OK on success, QTHERR_NOMEM if allocation fails
 *
 */
QTHERR
qth_init(qthhdl_t *qthhdlp, void *(*alloc)(unsigned long), void (*free)(void *))
{
    *qthhdlp = alloc(sizeof(struct qthhdr_s));
    if (*qthhdlp) {
        (*qthhdlp)->link = NULL;
        (*qthhdlp)->alloc = alloc;
        (*qthhdlp)->free = free;
    }
    return (*qthhdlp != NULL) ? QTHERR_OK : QTHERR_NOMEM;
}


/**
 * \brief
 * Adds to the head of the queue (LIFO or stack style)
 *
 * \details
 * Allocates a q link for the given data and adds it to the front of the queue
 *
 * \param[in]       qthhdl    handle to q
 * \param[in]       datap     user data to add to queue
 *
 * \return          QTHERR_OK on success
 *
 */
QTHERR
qth_addh(qthhdl_t qthhdl, void *datap)
{
    qthlink_t *new, *tail, *head;

    new = qthhdl->alloc(sizeof(qthlink_t));
    new->link = new;
    new->datap = datap;
    tail = qthhdl->link;
    /* if there is a tail insert the new head and point the tail at it */
    if (tail) {
        head = tail->link;
        new->link = head;
        tail->link = new;
    } else {
        /* otherwise this is the new head */
        qthhdl->link = new;
    }
    return QTHERR_OK;
}


/**
 * \brief
 * Adds to the tail of the queue (FIFO style)
 *
 * \details
 * Allocs a q link for the given data and adds it to the tail of the queue
 *
 * \param[in]       qthhdl    handle to q
 * \param[in]       datap     user data to add to queue
 *
 * \return          QTHERR_OK on success
 *
 */
QTHERR
qth_add(qthhdl_t qthhdl, void *datap)
{
    qthlink_t *new, *tail, *head;

    new = qthhdl->alloc(sizeof(qthlink_t));
    new->link = new;
    new->datap = datap;
    tail = qthhdl->link;
    /* if there is a tail then insert the new tail and point it at the head */
    if (tail) {
        head = tail->link;
        new->link = head;
        qthhdl->link = new;
        tail->link = new;
    } else {
        /* this is the new tail/head */
        qthhdl->link = new;
    }
    return QTHERR_OK;
}


/**
 * \brief
 * Removes the head of the queue
 *
 * \details
 * Removes the head from the queue, frees the link, and returns the user
 * data pointer
 *
 * \param[in]       qthhdl    handle to q
 * \param[out]      qerr      QTHERR_OK or error code for operation
 *
 * \return          The data pointer from the removed head of the queue
 *
 */
void *
qth_remove(qthhdl_t qthhdl, QTHERR *qerr)
{
    qthlink_t *tail, *head;
    void      *datap;

    *qerr = QTHERR_OK;

    tail = qthhdl->link;
    if (tail == NULL) {
        *qerr = QTHERR_EMPTY;
        return NULL;
    }

    head = tail->link;
    if (tail != head) {
        /* take off the head, point the tail at the new head */
        tail->link = head->link;
    } else {
        /* remove last element */
        qthhdl->link = NULL;
    }
    datap = head->datap;
    qthhdl->free(head);
    return datap;
}


/**
 * \brief
 * Iterate through data elements in the queue
 *
 * \details
 * Returns the data element from the queue, sets the q handle
 * for the next call iterate call.
 *
 * \param[in]       qthhdl    pointer to handle to q for iterating
 * \param[in,out]   iterp     pointer to the iterator
 *
 * \return          The current data pointer from the queue, NULL if no more
 *
 * \note            Set the *iterp to the queue handle to start the iteration
 *
 */
void *
qth_iter(qthhdl_t qthhdl, void **iterp)
{
    void        *datap;
    qthlink_t   *next;

    /* if first iteration set the iter to the head of the queue */
    if (qthhdl == *iterp) {
        next = (qthlink_t *)qthhdl->link->link;
    } else {
        next = (qthlink_t *) *iterp;
    }

    /* get the data and set up for the next iteration */
    if (next) {
        datap = next->datap;
        *iterp = next->link;
        /* if back at the head then we are done */
        if (*iterp == qthhdl->link->link) {
            *iterp = NULL;
        }
    } else {
        datap = NULL;
    }

    return datap;
}


/**
 * \brief
 * Frees queue
 *
 * \details
 * Frees any queue elements, and the queue handle.
 *
 * \param[in]       qthhdl          handle to q
 *
 * \return          QTHERR_OK       on success
 *                  QTHERR_EMPTY    if queue was aleady empty (header freed)
 *
 */
QTHERR
qth_free(qthhdl_t qthhdl)
{
    qthlink_t *tail, *head, *link;

    /* get the tail, and make sure there is something in the queue */
    tail = qthhdl->link;
    if (tail == NULL) {
        qthhdl->free(qthhdl);
        return QTHERR_EMPTY;
    }

    /* walk through and release the queue elements */
    head = tail->link;
    while (head != tail) {
        link = head;
        head = head->link;
        qthhdl->free(link);
    }
    qthhdl->free(head);

    /* release queue header as well */
    qthhdl->free(qthhdl);
    return QTHERR_OK;
}
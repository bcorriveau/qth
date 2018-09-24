/**
 * \brief
 * Queuing library - Uses Tail-Head queue linking organization
 *
 * \details
 * The queues have single links where the queue header points to the tail,
 * element which points to the head element, which points to the remaining 
 * elements. An empty queue has a NULL hdr This organization allows adding 
 * to the tail and head, and removing from the head in O(1) time.
 *
 * \note
 * Any type of data can be added to a queue.
 *
 * This queue library manages the allocation and freeing of the queue header and linkage.
 * The user does not add links to their own structures or manage a queue header.
 *
 * This queue manager uses the data pointers passed in and does not copy the data.
 * Do not release the data while the queue is in use!
 * The passed in alloc function is used to allocate a queue header and 
 * element links, and the free function is used to release element links as 
 * element data is removed from the queue, and free the queue header when the
 * queue is freed.
 *
 */

/** Handle to a queue */
typedef struct qthhdr_s *qthhdl_t;


/** Queueing Library Error codes */
typedef enum {
    QTHERR_OK = 0,      /*!< OK  */
    QTHERR_NOMEM,       /*!< No memory  */
    QTHERR_EMPTY,       /*!< Queue empty, this is OK for freeing  */
    QTHERR_CORRUPT,     /*!< Queue is corrupt  */
    QTHERR_LAST
} QTHERR;

/**
 * \brief
 * Intialize queue
 *
 * \details
 * Allocs and initializes a q. Uses the provided allocation function to allocate
 * the queue header and queue linking. The provided free function is used for and 
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
qth_init(qthhdl_t *qthhdlp, void *(*alloc)(unsigned long), void (*free)(void *ptr));

/**
 * \brief
 * Adds to the head of the queue (LIFO or stack style)
 *
 * \details
 * Allocs a q link for the given data and adds it to the front of the queue
 *
 * \param[in]       qthhdl    handle to q
 * \param[in]       datap     user data to add to queue
 *
 * \return          QTHERR_OK on success
 *
 */
QTHERR
qth_addh(qthhdl_t qthhdl, void *datap);

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
qth_add(qthhdl_t qthhdl, void *datap);

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
qth_remove(qthhdl_t qthhdl, QTHERR *qerr);

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
qth_iter(qthhdl_t qthhdl, void **iterp);

/**
 * \brief
 * Frees queue
 *
 * \details
 * Frees any queue element linkage, and the queue handle.
 *
 * \param[in]       qthhdl          handle to q
 *
 * \return          QTHERR_OK       on success
 *                  QTHERR_EMPTY    if queue was aleady empty (header freed)
 *
 */
QTHERR
qth_free(qthhdl_t qthhdl);

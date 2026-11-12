typedef struct link link_t;
typedef struct pqueue pqueue_t;

typedef int get_priority_fn(void *);

/// @brief Create a priority queue
/// @param get_priority Function to access the priority of an item
/// @return A priority queue
pqueue_t *pqueue_create(get_priority_fn *get_priority);

/// @brief Destroy a priority queue, freeing any allocated memory
/// @param pqueue The priority queue to be destroyed
void pqueue_destroy(pqueue_t *pqueue);

/// @brief Add an item to a priority queue
/// @param pqueue The priority queue
/// @param data A pointer to the item
void pqueue_enqueue(pqueue_t *pqueue, void *item);


/// @brief Get the highest priority object from a priority queue
/// @param pqueue The priority queue
/// @return A pointer to the highest priority item in the queue, or NULL if the queue is empty.
void *pqueue_dequeue(pqueue_t *pqueue);

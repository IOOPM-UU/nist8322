# Instructions

## In Windows

After signing in, search for "ThinLinc-klient" in the search box
in the lower left corner and open it. Enter the server
"thinlinc.student.it.uu.se", and your own username and password.
If you are asked whether you trust this host, click "Continue".
The new window you have is now running on the department's Linux
machines. You can make the window full-screen by clicking the
square in the top-right corner of the window. Avoid pressing the
"x" as this will close your connection!

## In Linux

Open a terminal and immediately write `mkdir kodprovC250613`. Go to
this directory with `cd kodprovC250613`. Download the code exam to
your computer with the following command:

    curl -L --remote-name http://eliasc.github.io/ioopm/racoon.zip

You now have a zip file with the code for the exam. Unpack it with
`unzip racoon.zip` (and enter the password given in the lab room)
and you will get *another* zip file called `kodprov.zip`.
Unpacking this with `unzip kodprov.zip` will get you a directory
`handout` containing the task to be solved, as well as a
`Makefile` used for handing in the code exam.

## Handing in

Go to the directory `kodprovC250613`. If you are unsure about where
you are in the file system you can write `cd; cd kodprovC250613`.
Write `make handin` to hand in the code exam. This will create a
zip file containing the file you are supposed to hand in (and no
other files), and this file will be saved in a location where we
can correct it.

## General rules

- The same rules as for a written exam applies: no mobile phones,
  no text messaging, no conversations with anyone except the
  course staff, regardless of medium.
- You must be able to identify yourself.
- You are allowed to bring a hand-written paper (maximum size A4).
  You can write on both sides but cannot bring a magnifying glass.
- You are allowed to bring a book, physically or on a tablet or
  laptop. You are not allowed to run any other programs on this
  machine, and you cannot use them for anything other than reading
  course literature.
- You must write all the code yourself, except the code that is
  given.

# Priority queues

A priority queue is a variation of the queue data structure. A queue provides two operations, `enqueue` and `dequeue`.
	
Data is entered into the queue with `enqueue` and taken out with `dequeue`. Data is taken out of the queue in the same order as it is entered.

A priority queue differs from a regular queue in that each piece of data is assigned a priority. Data with the same priority is taken out in the same order as it was entered, just as in a regular queue, but data of higher priority is always taken out before data of lower priority regardless of the order they were entered into the priority queue.

Let's give an example. Suppose we have some data A (priority 10), B (priority 20), C (priority 20) and D (priority 30).

If these data items are enqueued in the order A, B, C and D they will be dequeued in the order D, B, C and A.

D will be dequeued first as it has highest priority and A dequeued last as it has lowest priority. B and C, having the same priority, are dequeued in the order they were enqueued.

Of course, we don't have to enqueue all data before dequeueing them – these operations can be interleaved as in this example:

- A is enqueued.
- D is enqueued.
- B is enqueued.
- Dequeueing gives D.
- C is enqueued.
- Dequeueing gives B.
- Dequeueing gives C.
- Dequeueing gives A.


## A Priority Queue Library

The file `pqueue.h` defines the interface for a priority queue library. It provides functions for creating and destroying priority queues, and for enqueueing and dequeueing data. Your task is to implement the library.

The priority is stored "inside" each data item and the library should work with any kind of data. This means that there is no uniform way of getting the priority of a data item which can be "hardwired" in the library code. Instead, when a priority queue is created, the library is provided with an accessor function that takes a pointer to a data item and returns its priority as an `int`. The type of the accessor function is `get_priority_fn`, defined by a `typedef` in the header file. A pointer to the accessor function 
 must be saved in the priority queue data structure so that other library functions can use it to get the priority.

Priority queues have type `pqueue_t`, defined as `struct pqueue` by a `typedef` in the header file. You must implement this structure. As the library should work with any kind of data, void pointers are used for data items.

You must implement the following library funcions:

- `pqueue_t *pqueue_create(get_priority_fn *get_priority)` -- Create a priority queue with priority accessor function `get_priority`
- `void pqueue_destroy(pqueue_t *pqueue)` -- Destroy a priority queue
- `void pqueue_enqueue(pqueue_t *pqueue, void *item)` -- Enter the item into a priority queue
- `void *pqueue_dequeue(pqueue_t *pqueue)` -- Return the highest priority item from a priority queue (and remove it from the queue). If the queue was empty, return `NULL`.

When you are done, you should be able to run the tests through Valgrind without any memory errors or leaks.


## Files

- `pqueue.h` -- The interface you should implement
- `pqueue.c` -- You should write your implementation here. This is the only file you should modify.
- `pqueue_test.c` -- Tests for your library
- `Makefile`  -- A makefile that supports the following targets
  - `all`     -- Compile all the files
  - `test`    -- Build and run the tests
  - `memtest` -- Build and run the tests with valgrind
  - `clean`   -- Remove any built files


## Hints

- There are many ways to implement priority queues, but the most straightforward is using a linked list.
- The implementations of linked lists we have discussed in the course will work fine. Of course you need a new pointer for the priority accessor function. You can likely do without a pointer to the last list link as there should not be any need to access that link directly.
- Insert items into the list so that they are always in descending order of priority, i.e. the highest priority item is alway the first one.

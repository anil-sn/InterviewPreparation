#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

/*
    3.6 Animal Shelter: An animal shelter, which holds only dogs and cats, operates on a strictly"first in, first
        out" basis. People must adopt either the "oldest" (based on arrival time) of all animals at the shelter,
        or they can select whether they would prefer a dog or a cat (and will receive the oldest animal of
        that type). They cannot select which specific animal they would like. Create the data structures to
        maintain this system and implement operations such as enqueue, dequeueAny, dequeueDog,
        and dequeueCat. You may use the built-in Linkedlist data structure.

    SOLUTION
        We could explore a variety of solutions to this problem. For instance, we could maintain a single queue.
        This would make dequeueAny easy, but dequeueDog and dequeueCat would require iteration through
        the queue to find the first dog or cat. This would increase the complexity of the solution and decrease the
        efficiency.
        An alternative approach that is simple, clean and efficient is to simply use separate queues for dogs and
        cats, and to place them within a wrapper class called AnimalQueue. We then store some sort of timestamp
        to mark when each animal was enqueued. When we call dequeueAny, we peek at the heads of both the
        dog and cat queue and return the oldest.

        It is important that Dog and Cat both inherit from an Animal class since dequeueAny() needs to be able
        to support returning both Dog and Cat objects.
        If we wanted, order could be a true timestamp with the actual date and time. The advantage of this is that
        we wouldn't have to set and maintain the numerical order. If we somehow wound up with two animals with
        the same timestamp, then (by definition) we don't have an older animal and we could return either one.
*/

typedef enum {
    CAT = 0x1,
    DOG = 0x2,
    MAX = 0xFF
}AnimalType;

u_int32_t g_serialnumber = 1;

/* structure of a stack node */
typedef struct Animal {
    u_int32_t serialNumber;
    struct Animal* next;
} ANIMAL;

/* structure of queue having two stacks */
typedef struct AnimalShelter {
    ANIMAL* dogQueue;
    ANIMAL* catQueue;
    u_int32_t dogQueueAge;
    u_int32_t catQueueAge;
} AS;

/* Function to enqueue an item to queue */
void
enQueue(AS* q, AnimalType type) {

    /* allocate node */
    ANIMAL* new_node = (ANIMAL*)malloc(sizeof(ANIMAL));
    new_node->next = NULL;
    assert(new_node);
    new_node->serialNumber = g_serialnumber++;

    if (CAT == type) {
        q->catQueueAge = new_node->serialNumber;
        if (NULL == q->catQueue) {
            q->catQueue = new_node;
            return;
        }
        new_node->next = q->catQueue;
        q->catQueue = new_node;
    }

    if (DOG == type) {
        q->dogQueueAge = new_node->serialNumber;
        if (NULL == q->dogQueue) {
            q->dogQueue = new_node;
            return;
        }
        new_node->next = q->dogQueue;
        q->dogQueue = new_node;
    }

    return;
}

u_int32_t
deleteLast(ANIMAL** animal) {

    ANIMAL* cur = *animal;
    u_int32_t serialNumber;

    if (cur == NULL) {
        return 0;
    }

    if (cur->next == NULL) {
        serialNumber = cur->serialNumber;
        *animal = NULL;
        return serialNumber;
    }

    while (NULL != cur) {
        if (NULL == cur->next->next) {
            serialNumber = cur->next->serialNumber;
            cur->next = NULL;
            return serialNumber;
        }
        cur = cur->next;
    }

    return 0;
}

/* Function to deQueue an item from queue */
u_int32_t
deQueue(AS* q) {
    int x;
    ANIMAL** cur = NULL;

    /* is cat oldest entry */
    if (q->catQueueAge < q->dogQueueAge) {
        cur = &(q->catQueue);
    } else {
        cur = &(q->dogQueue);
    }

    return(deleteLast(cur));
}

/* Function to deQueue an item from queue */
u_int32_t
deQueueType(AS* q, AnimalType type) {
    int x;
    ANIMAL** cur = NULL;

    /* is cat oldest entry */
    if (CAT == type) {
        cur = &(q->catQueue);
    } else {
        cur = &(q->dogQueue);
    }

    return(deleteLast(cur));
}

/* Function to deQueue an item from queue */
void
printQueues(AS *animalShelter) {

    ANIMAL* temp = NULL;

    if (NULL != animalShelter->dogQueue) {

        printf("List of Dogs in the queue [%d]: ", animalShelter->dogQueueAge);
        temp = animalShelter->dogQueue;
        while (NULL != temp) {
            printf("-> %d ", temp->serialNumber);
            temp = temp->next;
        }

        printf("\r\n");
    }

    if (NULL != animalShelter->catQueue) {
        printf("List of Cats in the queue [%d]: ", animalShelter->catQueueAge);
        temp = animalShelter->catQueue;
        while (NULL != temp) {
            printf("-> %d ", temp->serialNumber);
            temp = temp->next;
        }

        printf("\r\n");
    }

    printf("\r\n");
    return;
}

void
main () {

    AS animalShelter;

    animalShelter.catQueue = NULL;
    animalShelter.catQueueAge = UINT_MAX;
    animalShelter.dogQueue = NULL;
    animalShelter.dogQueueAge = UINT_MAX;

    printf ("Add a DOG\r\n");
    enQueue(&animalShelter, DOG);
    printQueues(&animalShelter);

    printf ("Add a DOG\r\n");
    enQueue(&animalShelter, DOG);
    printQueues(&animalShelter);

    printf ("Add a DOG\r\n");
    enQueue(&animalShelter, DOG);
    printQueues(&animalShelter);

    printf ("Add a CAT\r\n");
    enQueue(&animalShelter, CAT);
    printQueues(&animalShelter);


    printf ("Add a CAT\r\n");
    enQueue(&animalShelter, CAT);
    printQueues(&animalShelter);

    printf ("Add a CAT\r\n");
    enQueue(&animalShelter, CAT);
    printQueues(&animalShelter);

    printf ("Add a DOG\r\n");
    enQueue(&animalShelter, DOG);
    printQueues(&animalShelter);

    printf ("Add a DOG\r\n");
    enQueue(&animalShelter, DOG);
    printQueues(&animalShelter);

    printf ("Add a CAT\r\n");
    enQueue(&animalShelter, CAT);
    printQueues(&animalShelter);

    printf ("Delete a CAT\r\n");
    deQueueType(&animalShelter,CAT);
    printQueues(&animalShelter);

    printf ("Delete a ANY\r\n");
    deQueue(&animalShelter);
    printQueues(&animalShelter);

    printf ("Delete a ANY\r\n");
    deQueue(&animalShelter);
    printQueues(&animalShelter);

    printf ("Delete a DOG\r\n");
    deQueueType(&animalShelter,DOG);
    printQueues(&animalShelter);

    return;
}


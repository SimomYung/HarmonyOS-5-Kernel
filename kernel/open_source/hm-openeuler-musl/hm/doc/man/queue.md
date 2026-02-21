# TAILQ_HEAD(), TAILQ_ENTRY(), TAILQ_HEAD_INITIALIZER(), TAILQ_INIT(), TAILQ_EMPTY(), TAILQ_FIRST(), TAILQ_NEXT(), TAILQ_LAST(), TAILQ_PREV(), TAILQ_INSERT_HEAD(), TAILQ_INSERT_TAIL(), TAILQ_INSERT_AFTER(), TAILQ_INSERT_BEFORE(), TAILQ_REMOVE(), TAILQ_FOREACH(), TAILQ_FOREACH_REVERSE(), TAILQ_CONCAT(), CIRCLEQ_HEAD(), CIRCLEQ_ENTRY(), CIRCLEQ_INIT(), CIRCLEQ_EMPTY(), CIRCLEQ_FIRST(), CIRCLEQ_INSERT_TAIL(), CIRCLEQ_REMOVE(), CIRCLEQ_FOREACH()
The implementations of tail queue and circle queue.

## Synopsis
```c
#include <sys/queue.h>

TAILQ_HEAD(headType, objType);

TAILQ_ENTRY(objType);

TAILQ_HEAD_INITIALIZER(head);

TAILQ_INIT(head);

TAILQ_EMPTY(head);

TAILQ_FIRST(head);

TAILQ_NEXT(obj, entryName);

TAILQ_LAST(head, headType);

TAILQ_PREV(obj, headType, entryName);

TAILQ_INSERT_HEAD(head, obj, entryName);

TAILQ_INSERT_TAIL(head, obj, entryName);

TAILQ_INSERT_AFTER(head, listObj, obj, entryName);

TAILQ_INSERT_BEFORE(listObj, obj, entryName);

TAILQ_REMOVE(head, obj, entryName);

TAILQ_FOREACH(obj, head, entryName);

TAILQ_FOREACH_REVERSE(obj, head, headType, entryName);

TAILQ_CONCAT(head1, head2, entryName);

CIRCLEQ_HEAD(headType, objType);

CIRCLEQ_ENTRY(objType);

CIRCLEQ_INIT(head);

CIRCLEQ_EMPTY(head);

CIRCLEQ_FIRST(head);

CIRCLEQ_INSERT_TAIL(head, obj, entryName);

CIRCLEQ_REMOVE(head, obj, entryName);

CIRCLEQ_FOREACH(obj, head, entryName);
```

## Arguments

- **fieldType**
The struct name of the entry of the queue element, can be empty for temporary use.
&nbsp;
- **objType**
The type of the queue element.
&nbsp;
- **headType**
The type of the queue head
&nbsp;
- **head**
The head of queue
&nbsp;
- **head1**
The head of queue
&nbsp;
- **head2**
The head of queue
&nbsp;
- **obj**
The element of queue or to be inserted to queue
&nbsp;
- **listObj**
The element of queue
&nbsp;
- **entryName**
The variable name of the entry of the queue element

## Library
libc

## Description

These marcos define two kind of data structures: tail queue and circle queue. they all support these operations:
1) Insert an object at the tail of queue
2) Remove an object of queue
3) Traversal queue from head to tail
For the above, tail queue support the reverse operations.

To use them, user should define object with TAILQ_ENTRY and CIRCLEQ_ENTRY, and define a head with TAILQ_HEAD and CIRCLEQ_HEAD.

## Returns
&nbsp;

## Errors
&nbsp;

## Example
```c
#include <sys/queue.h>
#include <stdio.h>

struct TqeObject {
    int value;
    TAILQ_ENTRY(TqeObject) entry;
};

TAILQ_HEAD(TqeHead, TqeObject);

int main(void)
{
    struct TqeHead *head = (struct TqeHead *)malloc(sizeof(struct TqeHead));
    TAILQ_INIT(head);

    struct TqeObject *obj1 = (struct TqeObject*)malloc(sizeof(struct TqeObject));
    obj1->value = 0;
    TAILQ_INSERT_TAIL(head, obj1, entry);

    struct TqeObject *obj2 = (struct TqeObject*)malloc(sizeof(struct TqeObject));
    obj2->value = 0;
    TAILQ_INSERT_TAIL(head, obj2, entry);

    TAILQ_REMOVE(head, obj1, entry);

    TAILQ_FOREACH(obj, head, entry)
    {
        printf("obj value = %d\n", obj->value);
    }

    TAILQ_REMOVE(head, obj2, entry);

    return 0;
}
```

## Classification
Not in POSIX.1-2001 or POSIX.1-2008.  Not in POSIX.1.  Present on the BSDs.  queue functions first appeared in 4.4BSD.

## Function Safety
TODO

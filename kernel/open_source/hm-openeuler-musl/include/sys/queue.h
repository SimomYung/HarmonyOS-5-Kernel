/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Provides the queue data structure
 * Author: Huawei RTOS
 * Create: Tue Jan 18 11:57:00 2022
 */
#ifndef _SYS_QUEUE_H
#define _SYS_QUEUE_H
#ifdef __cplusplus
extern "C" {
#endif

// Tail queue
#define TAILQ_FIELD(fieldType, objType) \
struct fieldType { \
    objType *next; \
    objType **prev; \
}

#define TAILQ_HEAD(headType, objType) TAILQ_FIELD(headType, struct objType)

#define TAILQ_ENTRY(objType) TAILQ_FIELD(, struct objType)

#define TAILQ_HEAD_INITIALIZER(head) \
    { NULL, &(head).next }

#define TAILQ_INIT(head) do {\
    (head)->next = NULL; \
    (head)->prev = &(head)->next; \
} while(0)

#define TAILQ_EMPTY(head) ((head)->next == NULL)

#define TAILQ_FIRST(head) ((head)->next)

#define TAILQ_NEXT(obj, entryName) ((obj)->entryName.next)

#define TAILQ_LAST(head, headType) (*(((struct headType*)((head)->prev))->prev))

#define TAILQ_PREV(obj, headType, entryName) (*(((struct headType*)((obj)->entryName.prev))->prev))

#define TAILQ_INSERT_HEAD(head, obj, entryName) do { \
    (obj)->entryName.next = (head)->next; \
    if ((head)->next == NULL) { \
        (head)->prev = &(obj)->entryName.next; \
    } else { \
        (head)->next->entryName.prev = &(obj)->entryName.next; \
    } \
    (head)->next = (obj); \
    (obj)->entryName.prev = &(head)->next; \
} while(0)

#define TAILQ_INSERT_TAIL(head, obj, entryName) do { \
    (obj)->entryName.next = NULL; \
    (obj)->entryName.prev = (head)->prev; \
    *(head)->prev = (obj); \
    (head)->prev = &(obj)->entryName.next; \
} while(0)

#define TAILQ_INSERT_AFTER(head, listObj, obj, entryName) do { \
    (obj)->entryName.next = (listObj)->entryName.next; \
    if ((listObj)->entryName.next == NULL) { \
        (head)->prev = &(obj)->entryName.next;\
    } else { \
        (listObj)->entryName.next->entryName.prev = &(obj)->entryName.next; \
    } \
    (obj)->entryName.prev = &(listObj)->entryName.next; \
    (listObj)->entryName.next = (obj); \
} while(0)

#define TAILQ_INSERT_BEFORE(listObj, obj, entryName) do { \
    *(listObj)->entryName.prev = (obj); \
    (obj)->entryName.prev = (listObj)->entryName.prev; \
    (listObj)->entryName.prev = &(obj)->entryName.next; \
    (obj)->entryName.next = (listObj); \
} while(0)

#define TAILQ_REMOVE(head, obj, entryName) do { \
    *(obj)->entryName.prev = (obj)->entryName.next;\
    if ((obj)->entryName.next == NULL) { \
        (head)->prev = (obj)->entryName.prev; \
    } else { \
        (obj)->entryName.next->entryName.prev = (obj)->entryName.prev; \
    } \
} while(0)

#define TAILQ_FOREACH(obj, head, entryName) \
    for ((obj) = ((head)->next); \
        (obj) != NULL; \
        (obj) = ((obj)->entryName.next))

#define TAILQ_FOREACH_REVERSE(obj, head, headType, entryName) \
    for ((obj) = TAILQ_LAST((head), headType); \
        (obj) != NULL; \
        (obj) = TAILQ_PREV((obj), headType, entryName))

#define TAILQ_CONCAT(head1, head2, entryName) do { \
    if ((head2)->next != NULL) {\
        *(head1)->prev = (head2)->next; \
        (head2)->next->entryName.prev = (head1)->prev; \
        (head1)->prev = (head2)->prev; \
        TAILQ_INIT((head2)); \
    } \
} while(0)

// Circular queue
#define CIRCLEQ_FIELD(fieldType, objType) \
struct fieldType { \
    objType *next; \
    objType *prev; \
}

#define CIRCLEQ_HEAD(headType, objType) CIRCLEQ_FIELD(headType, struct objType)

#define CIRCLEQ_ENTRY(objType) CIRCLEQ_FIELD(, struct objType)

#define CIRCLEQ_INIT(head) do {\
    (head)->next = (void*)(head); \
    (head)->prev = (void*)(head); \
} while(0)

#define CIRCLEQ_EMPTY(head) ((head)->next == (void*)(head))

#define CIRCLEQ_FIRST(head) ((head)->next)

#define CIRCLEQ_INSERT_TAIL(head, obj, entryName) do { \
    (obj)->entryName.next = (void*)(head); \
    (obj)->entryName.prev = (head)->prev; \
    if ((head)->prev == (void*)(head)) { \
        (head)->next = (obj); \
    } else { \
        (head)->prev->entryName.next = (obj); \
    } \
    (head)->prev = (obj); \
} while(0)

#define CIRCLEQ_REMOVE(head, obj, entryName) do { \
    ((obj)->entryName.prev == (void*)(head)) \
    ? ((head)->next = (obj)->entryName.next) \
    : ((obj)->entryName.prev->entryName.next = (obj)->entryName.next); \
    ((obj)->entryName.next == (void*)(head)) \
    ? ((head)->prev = (obj)->entryName.prev) \
    : ((obj)->entryName.next->entryName.prev = (obj)->entryName.prev); \
} while(0)

#define CIRCLEQ_FOREACH(obj, head, entryName) \
    for ((obj) = ((head)->next); \
        (obj) != (void*)(head); \
        (obj) = ((obj)->entryName.next))

#ifdef __cplusplus
}
#endif
#endif
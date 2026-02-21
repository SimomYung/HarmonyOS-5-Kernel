/**
 * @file cstl_rawlist.c
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * @brief raw_list 实现
 * @details raw_list源码
 * @author anonymous
 * @date 2021-04-15
 * @version v0.1.0
 * *******************************************************************************************
 * @par 修改日志：
 * <table>
 * <tr><th>Date        <th>Version  <th>Author    <th>Description
 * <tr><td>2021-04-15  <td>0.1.0    <td>anonymous <td>创建初始版本
 * </table>
 * *******************************************************************************************
 */

#include <stdlib.h>
#include "cstl_public_inner.h"
#include "cstl_rawlist.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 内部函数定义 */
CSTL_STATIC inline bool CstlRawListNodeInList(const CstlRawListNode *node)
{
    if ((node->next != NULL) && (node->prev != NULL) &&
        (node->next->prev == node) && (node->prev->next == node)) {
        return true;
    }

    return false;
}

CSTL_STATIC inline bool CstlRawListEmptyCheck(const CstlRawList *list)
{
    if (list->count == 0) {
        return true;
    }

    return false;
}

CSTL_STATIC inline void CstlRawListAddAfterNode(CstlRawListNode *node, CstlRawListNode *where)
{
    node->next       = (where)->next;
    node->prev       = (where);
    where->next      = node;
    node->next->prev = node;
}

CSTL_STATIC inline void CstlRawListAddBeforeNode(CstlRawListNode *node, const CstlRawListNode *where)
{
    CstlRawListAddAfterNode(node, where->prev);
}

CSTL_STATIC inline bool CstlRawListIsFirstNode(const CstlRawList *list, const CstlRawListNode *node)
{
    if (list->head.next == node) {
        return true;
    }

    return false;
}

CSTL_STATIC inline bool CstlRawListIsLastNode(const CstlRawList *list, const CstlRawListNode *node)
{
    if (list->head.prev == node) {
        return true;
    }

    return false;
}

void CstlRawListInit(CstlRawList *list, CstlFreeFunc freeFunc)
{
    if (list == NULL) {
        return;
    }

    list->head.next = &list->head;
    list->head.prev = &list->head;
    list->count     = 0;
    list->freeFunc  = freeFunc;
}

/* 删除链表节点，内部函数，不做入参校验 */
CSTL_STATIC void CstlRawListRemoveNode(CstlRawList *list, const CstlRawListNode *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;

    if (list->freeFunc != NULL) {
        list->freeFunc((void *)node);
    }

    (list)->count--;
}

void CstlRawListClear(CstlRawList *list)
{
    if (list == NULL) {
        return;
    }

    while (!CstlRawListEmptyCheck(list)) {
        CstlRawListRemoveNode(list, list->head.next);
    }

    list->count = 0;
}

void CstlRawListDeinit(CstlRawList *list)
{
    if (list == NULL) {
        return;
    }

    CstlRawListClear(list);

    list->freeFunc = NULL;
}

bool CstlRawListEmpty(const CstlRawList *list)
{
    if (list == NULL) {
        return true;
    }

    return CstlRawListEmptyCheck(list);
}

size_t CstlRawListSize(const CstlRawList *list)
{
    if (list == NULL) {
        return 0;
    }

    return list->count;
}

void CstlRawListPushFront(CstlRawList *list, CstlRawListNode *node)
{
    if ((list == NULL) || (node == NULL)) {
        return;
    }

    CstlRawListAddAfterNode(node, &(list->head));
    list->count++;
}

void CstlRawListPushBack(CstlRawList *list, CstlRawListNode *node)
{
    if ((list == NULL) || (node == NULL)) {
        return;
    }

    CstlRawListAddBeforeNode(node, &(list->head));
    list->count++;
}

void CstlRawListInsert(CstlRawList *list, const CstlRawListNode *curNode, CstlRawListNode *newNode)
{
    if ((list == NULL) || (curNode == NULL) || (newNode == NULL) || (!CstlRawListNodeInList(curNode))) {
        return;
    }

    CstlRawListAddBeforeNode(newNode, curNode);
    list->count++;
}

void CstlRawListPopFront(CstlRawList *list)
{
    const CstlRawListNode *firstNode;

    if ((list == NULL) || (CstlRawListEmptyCheck(list))) {
        return;
    }

    firstNode = list->head.next;
    CstlRawListRemoveNode(list, firstNode);
}

void CstlRawListPopBack(CstlRawList *list)
{
    const CstlRawListNode *lastNode;

    if ((list == NULL) || (CstlRawListEmptyCheck(list))) {
        return;
    }

    lastNode = list->head.prev;
    CstlRawListRemoveNode(list, lastNode);
}

void CstlRawListErase(CstlRawList *list, const CstlRawListNode *node)
{
    if ((list == NULL) || (node == NULL) ||  (CstlRawListEmptyCheck(list)) || (!CstlRawListNodeInList(node))) {
        return;
    }

    CstlRawListRemoveNode(list, node);
}

CstlRawListNode *CstlRawListFront(const CstlRawList *list)
{
    if ((list == NULL) || (CstlRawListEmptyCheck(list))) {
        return NULL;
    }

    return list->head.next;
}

CstlRawListNode *CstlRawListBack(const CstlRawList *list)
{
    if ((list == NULL) || (CstlRawListEmptyCheck(list))) {
        return NULL;
    }

    return list->head.prev;
}

CstlRawListNode *CstlRawListPrev(const CstlRawList *list, const CstlRawListNode *node)
{
    if ((list == NULL) || (node == NULL) ||
        (CstlRawListEmptyCheck(list)) || (CstlRawListIsFirstNode(list, node)) || (!CstlRawListNodeInList(node))) {
        return NULL;
    }

    return node->prev;
}

CstlRawListNode *CstlRawListNext(const CstlRawList *list, const CstlRawListNode *node)
{
    if ((list == NULL) || (node == NULL) ||
        (CstlRawListEmptyCheck(list)) || (CstlRawListIsLastNode(list, node)) || (!CstlRawListNodeInList(node))) {
        return NULL;
    }

    return node->next;
}

/* 链表排序函数，此处函数钩子cmpFunc的两个入参，类型需由用户保证是（CstlRawListNode *） */
int32_t CstlRawListSort(CstlRawList *list, CstlDataCmpFunc cmpFunc)
{
    int32_t ret;
    int32_t i, j;
    int32_t tmpCount;
    CstlRawListNode *node;
    CstlRawListNode *nextNode;

    if ((list == NULL) || (cmpFunc == NULL)) {
        return CSTL_ERROR;
    }

    tmpCount = (int32_t)list->count;

    for (i = 0; i < (tmpCount - 1); ++i) {
        node = CstlRawListFront(list);
        for (j = 0; j < (tmpCount - i - 1); ++j) {
            nextNode = node->next;
            ret = cmpFunc(node, nextNode);
            if (ret > 0) {
                node->prev->next = nextNode;
                nextNode->prev = node->prev;

                node->prev = nextNode;
                node->next = nextNode->next;
                nextNode->next->prev = node;
                nextNode->next = node;

                continue;
            }
            node = node->next;
        }
    }

    return CSTL_OK;
}

/* 链表节点查找函数，此处函数钩子nodeMatchFunc的第一个入参，类型需由用户保证是（CstlRawListNode *） */
CstlRawListNode *CstlRawListNodeFind(const CstlRawList *list, CstlMatchFunc nodeMatchFunc, uintptr_t data)
{
    CstlRawListNode *node;
    const CstlRawListNode *head;

    if ((list == NULL) || (nodeMatchFunc == NULL)) {
        return NULL;
    }

    head = (CstlRawListNode *)&list->head;
    node = head->next;
    while (node != head) {
        if (nodeMatchFunc((void *)node, data)) {
            return node;
        }
        node = node->next;
    }

    return NULL;
}

#ifdef __cplusplus
}
#endif
/**
 * @file cstl_list.c
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * @brief cstl_list 实现源码
 * @details 链表实现源码
 * @author anonymous
 * @date 2021-05-14
 * @version v0.1.0
 * *******************************************************************************************
 * @par 修改日志：
 * <table>
 * <tr><th>Date        <th>Version  <th>Author    <th>Description
 * <tr><td>2021-05-14  <td>0.1.0    <td>anonymous <td>创建初始版本
 * </table>
 * *******************************************************************************************
 */

#include <stdlib.h>
#include <stdint.h>
#include "cstl_public_inner.h"
#include "cstl_list.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup cstl_list
 * 该结构被用来保存双向链表中节点的前向指针和后向指针，以及少量用户数据或指针。
 */
typedef struct TagCstlListNode {
    CstlRawListNode rawNode;
    uintptr_t      userdata;
} CstlListNode;

CSTL_STATIC inline CstlListIterator CstlListIterEndGet(const CstlList *list)
{
    return (CstlListIterator)&list->rawList.head;
}

void CstlListInit(CstlList *list, const CstlDupFreeFuncPair *dataFunc)
{
    if (list == NULL) {
        return;
    }

    CstlRawListInit(&list->rawList, NULL);

    if (dataFunc == NULL) {
        list->dataFunc.dupFunc = NULL;
        list->dataFunc.freeFunc = NULL;
    } else {
        list->dataFunc.dupFunc = dataFunc->dupFunc;
        list->dataFunc.freeFunc = dataFunc->freeFunc;
    }
}

CSTL_STATIC void CstlListRemoveNode(CstlList *list, CstlListNode *node)
{
    if (list->dataFunc.freeFunc != NULL) {
        (list->dataFunc.freeFunc((void *)((CstlListNode *)node)->userdata));
    }

    CstlRawListErase(&list->rawList, &node->rawNode);
    free(node);
}

void CstlListClear(CstlList *list)
{
    CstlListNode *node;
    const CstlRawList *rawList;
    const CstlRawListNode *head;

    if (list == NULL) {
        return;
    }

    rawList = &list->rawList;
    head = &rawList->head;
    while (!CstlRawListEmpty(rawList)) {
        node = CSTL_CONTAINER_OF(head->next, CstlListNode, rawNode);
        CstlListRemoveNode(list, node);
    }
}

void CstlListDeinit(CstlList *list)
{
    if (list == NULL) {
        return;
    }

    CstlListClear(list);

    list->dataFunc.dupFunc = NULL;
    list->dataFunc.freeFunc = NULL;
}

CSTL_STATIC int32_t CstlListWriteUserdata(const CstlList *list, CstlListNode *node,
                                          uintptr_t userData, size_t userDataSize)
{
    const void *copyBuff;

    if (list->dataFunc.dupFunc == NULL) {
        node->userdata = userData;
    } else {
        copyBuff = list->dataFunc.dupFunc((void *)userData, userDataSize);
        if (copyBuff == NULL) {
            return CSTL_ERROR;
        }

        node->userdata = (uintptr_t)copyBuff;
    }

    return CSTL_OK;
}

CSTL_STATIC CstlListNode *CstlNewNodeCreateByUserData(const CstlList *list, uintptr_t userData, size_t userDataSize)
{
    CstlListNode *node;

    if (list == NULL) {
        return NULL;
    }

    node = (CstlListNode *)malloc(sizeof(CstlListNode));
    if (node == NULL) {
        return NULL;
    }

    if (CstlListWriteUserdata(list, node, userData, userDataSize) != CSTL_OK) {
        free(node);
        return NULL;
    }

    return node;
}

int32_t CstlListPushFront(CstlList *list, uintptr_t userData, size_t userDataSize)
{
    CstlListNode *node = NULL;

    node = CstlNewNodeCreateByUserData(list, userData, userDataSize);
    if (node == NULL) {
        return CSTL_ERROR;
    }

    CstlRawListPushFront(&list->rawList, &node->rawNode);

    return CSTL_OK;
}

int32_t CstlListPushBack(CstlList *list, uintptr_t userData, size_t userDataSize)
{
    CstlListNode *node;

    if (list == NULL) {
        return CSTL_ERROR;
    }

    node = CstlNewNodeCreateByUserData(list, userData, userDataSize);
    if (node == NULL) {
        return CSTL_ERROR;
    }

    CstlRawListPushBack(&list->rawList, &node->rawNode);

    return CSTL_OK;
}

int32_t CstlListInsert(CstlList *list, const CstlListIterator it, uintptr_t userData, size_t userDataSize)
{
    CstlListNode *node;

    if ((list == NULL) || (it == NULL)) {
        return CSTL_ERROR;
    }

    node = CstlNewNodeCreateByUserData(list, userData, userDataSize);
    if (node == NULL) {
        return CSTL_ERROR;
    }

    CstlRawListInsert(&list->rawList, &it->rawNode, &node->rawNode);

    return CSTL_OK;
}

bool CstlListEmpty(const CstlList *list)
{
    if (list == NULL) {
        return true;
    }

    return CstlRawListEmpty(&list->rawList);
}

void CstlListPopFront(CstlList *list)
{
    CstlListNode *firstNode;

    if (CstlListEmpty(list)) {
        return;
    }

    firstNode = CSTL_CONTAINER_OF(list->rawList.head.next, CstlListNode, rawNode);
    CstlListRemoveNode(list, firstNode);
}

void CstlListPopBack(CstlList *list)
{
    CstlListNode *lastNode = NULL;

    if (CstlListEmpty(list)) {
        return;
    }

    lastNode  = CSTL_CONTAINER_OF(list->rawList.head.prev, CstlListNode, rawNode);
    CstlListRemoveNode(list, lastNode);
}

CstlListIterator CstlListIterErase(CstlList *list, CstlListIterator it)
{
    CstlListIterator retIt = NULL;

    if (CstlListEmpty(list) || (it == NULL) || (it == CstlListIterEndGet(list))) {
        return NULL;
    }

    retIt = CSTL_CONTAINER_OF(it->rawNode.next, CstlListNode, rawNode);
    CstlListRemoveNode(list, it);

    return retIt;
}

uintptr_t CstlListFront(const CstlList *list)
{
    const CstlListNode *node;

    if (CstlListEmpty(list)) {
        return 0;
    }

    node = CSTL_CONTAINER_OF(list->rawList.head.next, CstlListNode, rawNode);

    return node->userdata;
}

uintptr_t CstlListBack(const CstlList *list)
{
    const CstlListNode *node;

    if (CstlListEmpty(list)) {
        return 0;
    }

    node = CSTL_CONTAINER_OF(list->rawList.head.prev, CstlListNode, rawNode);

    return node->userdata;
}

CstlListIterator CstlListIterBegin(const CstlList *list)
{
    if (list == NULL) {
        return NULL;
    }

    return CSTL_CONTAINER_OF(list->rawList.head.next, CstlListNode, rawNode);
}

CstlListIterator CstlListIterEnd(const CstlList *list)
{
    if (list == NULL) {
        return NULL;
    }

    return CstlListIterEndGet(list);
}

size_t CstlListSize(const CstlList *list)
{
    if (list == NULL) {
        return 0;
    }

    return CstlRawListSize(&list->rawList);
}

CstlListIterator CstlListIterPrev(const CstlList *list, const CstlListIterator it)
{
    if (CstlListEmpty(list) || (it == NULL)) {
        return NULL;
    }

    return CSTL_CONTAINER_OF(it->rawNode.prev, CstlListNode, rawNode);
}

CstlListIterator CstlListIterNext(const CstlList *list, const CstlListIterator it)
{
    if (CstlListEmpty(list) || (it == NULL)) {
        return NULL;
    }

    return CSTL_CONTAINER_OF(it->rawNode.next, CstlListNode, rawNode);
}

uintptr_t CstlListIterData(const CstlListIterator it)
{
    if (it == NULL) {
        return 0;
    }

    return it->userdata;
}

/* 链表排序函数，此处函数钩子cmpFunc的两个入参，类型是每个迭代器的userdata */
int32_t CstlListSort(const CstlList *list, CstlKeyCmpFunc cmpFunc)
{
    int32_t ret;
    int32_t i, j;
    int32_t tmpCount;
    uintptr_t tmpData;
    CstlListIterator it;
    CstlListIterator nextIt;
    const CstlRawList *rawList;
    const CstlRawListNode *head;

    if ((list == NULL) || (cmpFunc == NULL)) {
        return CSTL_ERROR;
    }

    rawList = &list->rawList;
    head = &rawList->head;
    tmpCount = (int32_t)rawList->count;
    for (i = 0; i < (tmpCount - 1); ++i) {
        it = CSTL_CONTAINER_OF(head->next, CstlListNode, rawNode);
        for (j = 0; j < (tmpCount - i - 1); ++j) {
            nextIt = CSTL_CONTAINER_OF(it->rawNode.next, CstlListNode, rawNode);
            ret = cmpFunc(it->userdata, nextIt->userdata);
            if (ret > 0) {
                tmpData = it->userdata;
                it->userdata = nextIt->userdata;
                nextIt->userdata = tmpData;
            }

            it = CSTL_CONTAINER_OF(it->rawNode.next, CstlListNode, rawNode);
        }
    }

    return CSTL_OK;
}

/* 链表节点查找函数，此处函数钩子iterCmpFunc的第一个入参，类型是每个迭代器的userdata */
CstlListIterator CstlListIterFind(const CstlList *list, CstlKeyCmpFunc iterCmpFunc, uintptr_t data)
{
    CstlListIterator it;
    CstlListIterator headIt;

    if ((list == NULL) || (iterCmpFunc == NULL)) {
        return NULL;
    }

    headIt = (CstlListIterator)&list->rawList.head;
    it = CSTL_CONTAINER_OF(list->rawList.head.next, CstlListNode, rawNode);
    while (it != headIt) {
        if (iterCmpFunc(it->userdata, data) == 0) {
            return it;
        }
        it = CSTL_CONTAINER_OF(it->rawNode.next, CstlListNode, rawNode);
    }

    return NULL;
}

#ifdef __cplusplus
}
#endif
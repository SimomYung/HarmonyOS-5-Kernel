/**
 * @file cstl_rawlist.h
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * @brief cstl_rawlist 对外头文件
 * @details
 * Notes: 1、链表遍历过程中删除节点，必须先调 next 函数获取到下一个节点，才能调 erase 删除当前节点。
 *        2、链表节点在删除时会自动释放内存，但如果节点的用户数据中还有句柄、指针等私有资源，这些资源只能由用户自行释放。
 *           对于这种情况，用户必须在初始化时注册free函数。
 *   +-----------------------------------------------------------+
 *   | +------------------------------------------------------+  |
 *   | |       head                                           |  |
 *   | |   +----------+    +----------+    +--------------+   |  |
 *   | +---|   prev   |<---|   prev   |<---|   prev       |<--+  |
 *   +---->|   next   |--->|   next   |--->|   next       |------+
 *         +----------+    +----------+    +--------------+
 *         | count    |    | userdata |    | userdata1    |
 *         | freefunc |    |   ...    |    | pointer      |-----------+  用户私有资源
 *         +----------+    +----------+    | userdata...  |          \|/
 *                                         +--------------+    +-------------+
 *                                                             | privatedata |
 *                                                             +-------------+
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

/**
 * @defgroup cstl_rawlist Raw双向链表
 * @ingroup cstl
 */
#ifndef CSTL_RAWLIST_H
#define CSTL_RAWLIST_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "cstl_public.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup cstl_rawlist
 * 该结构被用来保存双向链表中节点的前向指针和后向指针。
 * 这个链表不包含实质的数据区，一般用于组织(串接)数据节点
 */
typedef struct tagCstlRawListNode {
    struct tagCstlRawListNode *next;     /**< 指向下一个节点 */
    struct tagCstlRawListNode *prev;     /**< 指向前一个节点 */
} CstlRawListNode;

/**
 * @ingroup cstl_rawlist
 * 链表头
 */
typedef struct {
    CstlRawListNode    head;       /**< 链表头 */
    size_t             count;      /**< 链表上的节点个数 */
    CstlFreeFunc       freeFunc;   /**< 节点内存释放函数 */
} CstlRawList;

/**
 * @ingroup cstl_rawlist
 * @brief 初始化链表
 * @par 描述：初始化链表，按需注册用户数据中的私有资源释放函数。本函数不会申请资源。
 * @attention
 *  1:rawlist模块中的链表节点由用户进行封装并申请内存，rawlist仅做链表的维护，其资源释放应由用户在freeFunc中完成。\
 *  2:用户在添加数据时，传递给rawlist的参数为CstlRawListNode节点，因此rawlist传递给用户freeFunc的参数也是CstlRawListNode节点。
 * @param list        [IN]  链表。
 * @param freeFunc    [IN]  用户资源释放函数。
 * @li cstl_rawlist.h：该接口声明所在的头文件。
 */
void CstlRawListInit(CstlRawList *list, CstlFreeFunc freeFunc);

/**
 * @ingroup cstl_rawlist
 * @brief 链表节点清空，删除所有节点
 * @par 描述：链表节点清空，删除所有节点，调用户注册的free函数释放用户私有资源，回归链表初始化后的状态。
 * @param list    [IN]  链表
 * @li cstl_rawlist.h：该接口声明所在的头文件。
 */
void CstlRawListClear(CstlRawList *list);

/**
 * @ingroup cstl_rawlist
 * @brief 链表去初始化
 * @par 描述：链表去初始化：删除所有节点，调用户注册的free函数释放用户私有资源，去注册钩子函数。但链表头还在。
 * @param list    [IN]  链表
 * @li cstl_rawlist.h：该接口声明所在的头文件。
 */
void CstlRawListDeinit(CstlRawList *list);

/**
 * @ingroup cstl_rawlist
 * @brief 检查链表是否为空
 * @param list    [IN] 待检查的链表
 * @retval #true  1，链表为空。
 * @retval #false 0，链表不为空。
 * @li cstl_rawlist.h：该接口声明所在的头文件。
 */
bool CstlRawListEmpty(const CstlRawList *list);

/**
 * @ingroup cstl_rawlist
 * @brief 获取链表中节点个数
 * @param list  [IN] 链表
 * @retval 链表节点个数
 * @li cstl_rawlist.h：该接口声明所在的头文件。
 */
size_t CstlRawListSize(const CstlRawList *list);

/**
 * @ingroup cstl_rawlist
 * @brief 在链表头部插入节点
 * @param list  [IN] 链表
 * @param node  [IN] 待插入的节点
 * @li cstl_rawlist.h：该接口声明所在的头文件。
 */
void CstlRawListPushFront(CstlRawList *list, CstlRawListNode *node);

/**
 * @ingroup cstl_rawlist
 * @brief 在链表尾插入节点
 * @param list  [IN] 链表
 * @param node  [IN] 待插入的节点
 * @li cstl_rawlist.h：该接口声明所在的头文件。
 */
void CstlRawListPushBack(CstlRawList *list, CstlRawListNode *node);

/**
 * @ingroup cstl_rawlist
 * @brief 在指定节点前插入节点
 * @param list    [IN] 链表
 * @param curNode [IN] 指定的节点
 * @param newNode [IN] 待插入的节点
 * @li cstl_rawlist.h：该接口声明所在的头文件。
 */
void CstlRawListInsert(CstlRawList *list, const CstlRawListNode *curNode, CstlRawListNode *newNode);

/**
 * @ingroup cstl_rawlist
 * @brief 从链表头部POP一个节点
 * @par 描述：从链表中移除头节点，同时释放节点内存。如果在初始化时注册了free函数，还会调该钩子函数释放用户资源。\n
 * 如果链表为空，则不做任何事情。
 * @param list [IN]  链表
 * @li cstl_rawlist.h：该接口声明所在的头文件。
 */
void CstlRawListPopFront(CstlRawList *list);

/**
 * @ingroup cstl_rawlist
 * @brief 从链表中尾部POP一个节点
 * @par 描述：从链表中移除尾节点，同时释放节点内存。如果在初始化时注册了free函数，还会调该钩子函数释放用户资源。\n
 * 如果链表为空，则不做任何事情。
 * @param list [IN]  链表
 * @li cstl_rawlist.h：该接口声明所在的头文件。
 */
void CstlRawListPopBack(CstlRawList *list);

/**
 * @ingroup cstl_rawlist
 * @brief 删除链表中指定节点，同时释放节点内存。
 * @par 描述：从链表中删除指定节点，同时释放节点内存。如果在初始化时注册了free函数，还会调该钩子函数释放用户资源。
 * @param list [IN] 链表
 * @param node [IN] 待删除的节点
 * @li cstl_rawlist.h：该接口声明所在的头文件。
 */
void CstlRawListErase(CstlRawList *list, const CstlRawListNode *node);

/**
 * @ingroup cstl_rawlist
 * @brief 返回头节点指针
 * @par 描述：仅用于访问头节点，不会删除该节点；如果链表为空，则返回NULL。
 * @param list  [IN] 链表
 * @retval 非NULL 头节点的指针
 * @retval NULL   链表为空
 * @li cstl_rawlist.h：该接口声明所在的头文件。
 */
CstlRawListNode *CstlRawListFront(const CstlRawList *list);

/**
 * @ingroup cstl_rawlist
 * @brief 返回尾节点指针。
 * @par 描述：仅用于访问尾节点，不会删除该节点；如果链表为空，则返回NULL。
 * @param list  [IN] 链表
 * @retval 非NULL 尾节点的指针
 * @retval NULL   链表为空
 * @li cstl_rawlist.h：该接口声明所在的头文件。
 */
CstlRawListNode *CstlRawListBack(const CstlRawList *list);

/**
 * @ingroup cstl_rawlist
 * @brief 获取当前节点的前一个节点。
 * @par 描述：获取当前节点的前一个节点指针。如果当前节点是头节点，则返回NULL。
 * @param list  [IN] 链表
 * @param node  [IN] 当前节点
 * @retval 非NULL 当前节点的前一个节点
 * @retval NULL   头节点的前一个节点为空
 * @li cstl_rawlist.h：该接口声明所在的头文件。
 */
CstlRawListNode *CstlRawListPrev(const CstlRawList *list, const CstlRawListNode *node);

/**
 * @ingroup cstl_rawlist
 * @brief 获取当前节点的后一个节点。
 * @par 描述：获取当前节点的后一个节点指针。如果当前节点是尾节点，则返回NULL。
 * @param list  [IN] 链表
 * @param node  [IN] 当前节点
 * @retval 非NULL 当前节点的后一个节点
 * @retval NULL   尾节点的下一个节点为空
 * @li cstl_rawlist.h：该接口声明所在的头文件。
 */
CstlRawListNode *CstlRawListNext(const CstlRawList *list, const CstlRawListNode *node);

/**
 * @ingroup cstl_rawlist
 * @brief 根据用户定义的排序函数，对链表节点进行排序。
 * @par 描述：根据用户定义的排序函数，对链表节点进行排序，排序顺序按排序函数定义实行。
 * @attention \n
 * 1、此处用户输入的排序函数钩子，其两个入参为两个待比较的节点值，入参类型是（CstlRawListNode *）。\n
 * 2、用户在排序函数内部的实现，需根据节点地址偏移到用户结构体信息再进行排序比较。
 * @param list      [IN] 链表
 * @param cmpFunc   [IN] 排序函数钩子
 * @retval CSTL_OK      排序成功
 * @retval CSTL_ERROR   排序失败
 * @li cstl_rawlist.h：该接口声明所在的头文件。
 */
int32_t CstlRawListSort(CstlRawList *list, CstlDataCmpFunc cmpFunc);

/**
 * @ingroup cstl_rawlist
 * @brief 根据用户定义的节点匹配函数，搜索用户想要的节点。
 * @par 描述：根据用户定义的节点匹配函数，搜索用户想要的节点。
 * @attention \n
 * 1、将从链表头往后遍历，对每个节点依次调用匹配函数，直到找到第一个匹配的节点或者遍历到链表尾部结束。\n
 * 2、此处用户输入的匹配函数钩子，其第一个入参地址是每个待搜索的节点值，入参类型是（CstlRawListNode *）。\n
 * 3、用户在匹配钩子内部的实现，需根据节点地址偏移到用户结构体信息再进行匹配比较。
 * @param list           [IN] 链表
 * @param nodeMatchFunc  [IN] 匹配函数钩子
 * @param data           [IN] 关键信息
 * @retval 非NULL 查询成功，返回节点指针
 * @retval NULL   查询失败，未找到匹配的节点
 * @li cstl_rawlist.h：该接口声明所在的头文件。
 */
CstlRawListNode *CstlRawListNodeFind(const CstlRawList *list, CstlMatchFunc nodeMatchFunc, uintptr_t data);

#ifdef __cplusplus
}
#endif

#endif /* CSTL_RAWLIST_H */
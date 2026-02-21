/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#ifndef V_LEGACYAVL_H
#define V_LEGACYAVL_H

#include <stdint.h>
#include <stddef.h>
#include "himalloc_inner.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AVL_MAXHEIGHT 41 /* why this? a small exercise */

/* ************************************************
 This Macro get the handle value of AVL node ****
************************************************* */
#define VOS_AVL_GET_HANDLE(pNode) ((pNode)->ulHandle)

/* ************************************************
 This Macro set the handle value of AVL node ****
************************************************* */
#define VOS_AVL_SET_HANDLE(pNode, ulValue) ((pNode)->ulHandle = (uint32_t)(ulValue))

/* ************************************************
   macro versions of a few avl functions
************************************************* */
#define VOS_FIND_AVL_ENTRY(entry, init, look, comp, found, type) \
    do {                                                         \
        int32_t lCmp;                                            \
        (entry) = (init);                                        \
        (found) = (type)0;                                       \
        while (entry) {                                          \
            lCmp = (comp)((look), (entry));                      \
            if (lCmp > 0) {                                      \
                (entry) = (entry)->pLeft;                        \
            } else if (lCmp < 0) {                               \
                (entry) = (entry)->pRight;                       \
            } else {                                             \
                (found) = (type)(entry);                         \
                break;                                           \
            }                                                    \
        }                                                        \
    } while (0)

#define VOS_FIND_LESS_OR_EQUAL_AVL_ENTRY(entry, init, look, comp, found, type) \
    do {                                                                       \
        int32_t lCmp;                                                          \
        (entry) = (init);                                                      \
        (found) = (type)0;                                                     \
        while (entry) {                                                        \
            lCmp = (comp)((look), (entry));                                    \
            if (lCmp > 0) {                                                    \
                (found) = (type)(entry);                                       \
                (entry) = (entry)->pLeft;                                      \
            } else if (lCmp < 0) {                                             \
                (entry) = (entry)->pRight;                                     \
            } else {                                                           \
                (found) = (type)(entry);                                       \
                break;                                                         \
            }                                                                  \
        }                                                                      \
    } while (0)

/* avl tree type define */
/**
 * @ingroup v_legacyavl
 * 维护信息。
 */
#pragma pack(4)
typedef struct tagAVL_NODE {
    struct tagAVL_NODE *pLeft;  /**< 左节点的地址  */
    struct tagAVL_NODE *pRight; /**< 右节点的地址 */
    uint32_t ulHeight;          /**< AVL节点的高度  */
    size_t ulHandle;            /**< 数据或关键字 */
} AVL_NODE_S, VOS_AVL_NODE;

#pragma pack()
/**
 * @ingroup v_legacyavl
 * 该钩子函数用来比较两个节点，pNode1pNode2。该函数在AVL API中递归调用。
 * 如果函数返回值大于0，则数据移到被检查节点的右子树。\n
 * 如果函数返回值小于0，则数据移到被检查节点的左子树。\n
 * 如果函数返回值等于0，则表示数据相同。这个API在从AVL树中删除节点很有用。如果用户
 * 的函数没有返回0，则这个钩子函数被#VOS_AVLAdd, #VOS_AVLDel, #VOS_AVLFind,
 * #VOS_AVLFindLessOrEqual, #VOS_AVLNext和 #VOS_AVLPrev调用
 */
typedef int32_t (*VOS_AVL_CMP_FUNC)(VOS_AVL_NODE *pNode1, /**< 要比较的第一个节点 */
    VOS_AVL_NODE *pNode2);                                /**< 要比较的第二个节点 */
/**
 * @ingroup v_legacyavl
 * 该函数指针用来释放分配的节点数据，它仅能被 #VOS_AVLFreeAll 调用，且在内部递归调
 * 用。这个函数应该实现释放数据节点所用到所有的内存空间，否则会造成内存泄漏；而对
 * 于不想释放的内存空间要进行备份。
 */
typedef void (*VOS_AVL_FREE_FUNC)(VOS_AVL_NODE *pNode); /**< 要释放的节点 */
/**
 * @ingroup v_legacyavl
 * 这是个用户自己定义的一个函数指针，用于协助完成整棵树的遍历。这个函数对于处理用
 * 户数据尤其有用，尤其是用户自己记录到节点数据中的内容，如以用户自己定义的数据格
 * 式打印树中所有的数据。
 */
typedef int32_t (*VOS_AVL_WALK_FUNC)(AVL_NODE_S *pNode, /**< 要遍历的节点 */
    void *pArg);                                        /**< 用户进行操作的算法 */

/* ************************************************
    avl tree prototypes
 ************************************************* */
/**
 * @ingroup v_legacyavl
 * @brief 检查指定节点是否在AVL树中。
 *
 * @par 描述:
 * 检查指定节点是否在AVL树中。如果该节点存在，则返回这个节点在AVL树中的位置，否则返回#HIMALLOC_NULL_PTR。
 * @par[V1兼容差异]
 * [部分兼容]该API所在头文件不兼容，V1是在v_avl.h头文件中，V3是在v_legacyavl.h头文件中。
 * @attention
 * 如果传入的比较函数pfnCompare为#HIMALLOC_NULL_PTR，则调用默认的比较函数avl_compare。
 *
 * @param *pstEntry [IN]  该参数表示开始查找的入口节点。取值范围为有效指针。
 * @param *pstLookfor [IN]  该参数表示要查找的节点。取值范围为有效指针。
 * @param pfnCompare [IN]  该参数表示比较函数。取值范围为有效指针或 #HIMALLOC_NULL_PTR。
 *
 * @retval 如果该节点存在，则返回这个节点在AVL树中的位置。表示成功。
 * @retval #HIMALLOC_NULL_PTR(0L)表示失败。
 * @par 依赖: 如下
 * @li v_legacyavl：该接口所属的开发包。
 * @li v_legacyavl.h：该接口声明所在的头文件。
 * @since V200R002C01
 * @see 无。
 */
extern HIDDEN AVL_NODE_S *VOS_AVLFind(AVL_NODE_S *pstEntry, void *pstLookfor, VOS_AVL_CMP_FUNC pfnCompare);

/**
 * @ingroup v_legacyavl
 * @brief 该API用来查找AVL树中最小值的节点。
 *
 * @par 描述:
 * 该API用来查找AVL树中最小值的节点。
 * @par[V1兼容差异]
 * [部分兼容]该API所在头文件不兼容，V1是在v_avl.h头文件中，V3是在v_legacyavl.h头文件中。
 * @attention
 * 最大的节点总是在树的最左边。
 *
 * @param *pstHead [IN]  该参数表示指向AVL树的头节点。取值范围为有效指针。
 *
 * @retval 最小值的节点表示成功。
 * @retval 如果pstHead是HIMALLOC_NULL_PTR，返回HIMALLOC_NULL_PTR(0L)。
 * @par 依赖: 如下
 * @li v_legacyavl：该接口所属的开发包。
 * @li v_legacyavl.h：该接口声明所在的头文件。
 * @since V200R002C01
 * @see
 * @li VOS_AVLFindLargest
 * @li VOS_AVLFindLessOrEqual
 */
extern HIDDEN AVL_NODE_S *VOS_AVLFindSmallest(AVL_NODE_S *pstHead);

/**
 * @ingroup v_legacyavl
 * @brief 该API用来查找AVL树中最大值的节点。
 *
 * @par 描述:
 * 该API用来查找AVL树中最大值的节点。
 * @par[V1兼容差异]
 * [部分兼容]该API所在头文件不兼容，V1是在v_avl.h头文件中，V3是在v_legacyavl.h头文件中。
 * @attention
 * 最大的节点总是在树的最右边。
 *
 * @param *pstHead [IN]  该参数表示指向AVL树的头节点。取值范围为有效指针。
 *
 * @retval 最大值的节点表示成功。
 * @retval 如果pstHead是HIMALLOC_NULL_PTR，返回HIMALLOC_NULL_PTR(0L)表示失败。
 * @par 依赖: 如下
 * @li v_legacyavl：该接口所属的开发包。
 * @li v_legacyavl.h：该接口声明所在的头文件。
 * @since V200R002C01
 * @see
 * @li VOS_AVLFindSmallest
 * @li VOS_AVLFindLessOrEqual
 */
extern HIDDEN AVL_NODE_S *VOS_AVLFindLargest(AVL_NODE_S *pstHead);

/**
 * @ingroup v_legacyavl
 * @brief 查找小于或等于给定值的节点。
 *
 * @par 描述:
 * 查找小于或等于给定值的节点。返回第一个小于或等于输入节点的节点。
 * @par[V1兼容差异]
 * [部分兼容]该API所在头文件不兼容，V1是在v_avl.h头文件中，V3是在v_legacyavl.h头文件中。
 * @attention
 * 如果输入的比较函数为#HIMALLOC_NULL_PTR，则调用默认的比较函数avl_compare。
 *
 * @param *pstEntry [IN]  该参数表示指向要查找的节点。取值范围为有效指针。
 * @param *pstLookfor [IN]  该参数表示指向要比较节点。取值范围为有效指针。
 * @param pfnCompare [IN]  该参数表示指向比较函数的指针。取值范围为有效的函数指针或#HIMALLOC_NULL_PTR。
 *
 * @retval AVL树中满足条件的第一个有效节点表示成功。
 * @retval 如果没有找到节点，返回HIMALLOC_NULL_PTR(0L)。
 * @par 依赖: 如下
 * @li v_legacyavl：该接口所属的开发包。
 * @li v_legacyavl.h：该接口声明所在的头文件。
 * @since V200R002C01
 * @see
 * @li VOS_AVLFindLargest
 * @li VOS_AVLFindSmallest
 */
extern HIDDEN AVL_NODE_S *VOS_AVLFindLessOrEqual(AVL_NODE_S *pstEntry, void *pstLookfor, VOS_AVL_CMP_FUNC pfnCompare);

/**
 * @ingroup v_legacyavl
 * @brief 向AVL树中添加一个节点。
 *
 * @par 描述:
 * 向AVL树中添加一个节点。如果是加入第一个节点，pstHead必须是#HIMALLOC_NULL_PTR，这等同于生成一个新树。
 * @par[V1兼容差异]
 * [部分兼容]该API所在头文件不兼容，V1是在v_avl.h头文件中，V3是在v_legacyavl.h头文件中。
 * @attention
 * @li 如果输入的比较函数为#HIMALLOC_NULL_PTR，则调用默认的比较函数avl_compare。
 * @li 如果pstEntry是#HIMALLOC_NULL_PTR，则返回当前的头节点。
 * @li 如果pstHead是#HIMALLOC_NULL_PTR，并且加入节点失败，则API返回#HIMALLOC_NULL_PTR。
 * @li 在使用VOS_AVLAdd和VOS_AVLDel对同一AVL树进行插入和删除必须进行互斥保护。
 *
 * @param *pstHead [IN]  该参数表示指向AVL树的头节点。取值范围为有效指针。
 * @param *pstEntry [IN]  该参数表示指向要加入的节点。取值范围为有效指针。
 * @param pfnCompare [IN]  该参数表示指向比较函数。取值范围为有效的函数指针或#HIMALLOC_NULL_PTR。
 *
 * @retval 加入节点后的AVL树的头节点成功。
 * @retval pstHead失败。
 * @par 依赖: 如下
 * @li v_legacyavl：该接口所属的开发包。
 * @li v_legacyavl.h：该接口声明所在的头文件。
 * @since V200R002C01
 * @see VOS_AVLDel
 */
extern HIDDEN AVL_NODE_S *VOS_AVLAdd(AVL_NODE_S *pstHead, AVL_NODE_S *pstEntry, VOS_AVL_CMP_FUNC pfnCompare);

/**
 * @ingroup v_legacyavl
 * @brief 从AVL树中删除一个节点。
 *
 * @par 描述:
 * 从AVL树中删除一个节点。
 * @par[V1兼容差异]
 * [部分兼容]该API所在头文件不兼容，V1是在v_avl.h头文件中，V3是在v_legacyavl.h头文件中。
 * @attention
 * @li 如果输入的比较函数为#HIMALLOC_NULL_PTR，则调用缺省的比较函数avl_compare。
 * @li 如果头节点pstHead是#HIMALLOC_NULL_PTR，则返回#HIMALLOC_NULL_PTR。
 * @li 如果pstEntry是#HIMALLOC_NULL_PTR，则返回当前的头节点。
 * @li 删除节点后，不会释放节点和数据。从树中删除节点后，必须释放该节点。
 * @li 在使用VOS_AVLAdd和VOS_AVLDel对同一AVL树进行插入和删除必须进行互斥保护。
 *
 * @param *pstHead [IN]  该参数表示指向AVL树的头节点。取值范围为有效指针。
 * @param *pstEntry [IN]  该参数表示指向要删除的节点。取值范围为有效指针。
 * @param pfnCompare [IN]  该参数表示指向比较函数。取值范围为有效的函数指针或#HIMALLOC_NULL_PTR。
 *
 * @retval 删除节点后的AVL树的头节点表示成功。
 * @retval #HIMALLOC_NULL_PTR(0L)表示失败。
 * @par 依赖: 如下
 * @li v_legacyavl：该接口所属的开发包。
 * @li v_legacyavl.h：该接口声明所在的头文件。
 * @since V200R002C01
 * @see VOS_AVLAdd
 */
extern HIDDEN AVL_NODE_S *VOS_AVLDel(AVL_NODE_S *pstHead, AVL_NODE_S *pstEntry, VOS_AVL_CMP_FUNC pfnCompare);

/**
 * @ingroup v_legacyavl
 * @brief 查找指定节点的前驱节点。
 *
 * @par 描述:
 * 查找指定节点的前驱节点。前驱节点是小于输入节点的最大节点。
 * @par[V1兼容差异]
 * [部分兼容]该API所在头文件不兼容，V1是在v_avl.h头文件中，V3是在v_legacyavl.h头文件中。
 * @attention
 * @li 如果输入的比较函数为#HIMALLOC_NULL_PTR，则调用默认的比较函数avl_compare。
 * @li 如果查找的pstSearchNode是AVL树中的最小节点，则返回#HIMALLOC_NULL_PTR。
 * @li 如果输入的根节点不是头节点，则搜索结果无法预料。
 *
 * @param *pstSearchNode [IN]  该参数表示指向查找到的前驱节点的指针。取值范围为有效指针。
 * @param *pstRootNode [IN]  该参数表示指向AVL树的根节点的指针。取值范围为有效指针。
 * @param pfnCompare [IN]  该参数表示指向比较函数的指针。取值范围为有效的函数指针或#HIMALLOC_NULL_PTR。
 *
 * @retval 前驱节点表示成功。
 * @retval #HIMALLOC_NULL_PTR(0L)表示失败。
 * @par 依赖: 如下
 * @li v_legacyavl：该接口所属的开发包。
 * @li v_legacyavl.h：该接口声明所在的头文件。
 * @since V200R002C01
 * @see VOS_AVLNext
 */
extern HIDDEN AVL_NODE_S *VOS_AVLPrev(AVL_NODE_S *pstSearchNode, AVL_NODE_S *pstRootNode, VOS_AVL_CMP_FUNC pfnCompare);

/**
 * @ingroup v_legacyavl
 * @brief 查找指定节点的后继节点。
 *
 * @par 描述:
 * 查找指定节点的后继节点。后继节点是大于输入节点中的最小一个节点。
 * @par[V1兼容差异]
 * [部分兼容]该API所在头文件不兼容，V1是在v_avl.h头文件中，V3是在v_legacyavl.h头文件中。
 * @attention
 * @li 如果输入的比较函数为#HIMALLOC_NULL_PTR，则调用默认的比较函数avl_compare。
 * @li 如果查找的pstSearchNode是AVL树中的最大节点，则返回#HIMALLOC_NULL_PTR。
 * @li 如果输入的根节点不是头节点，则搜索结果无法预料。
 *
 * @param *pstSearchNode [IN]  该参数表示指向查找到的后继节点的指针。取值范围为有效指针。
 * @param *pstRootNode [IN]  该参数表示指向AVL树的根节点的指针。取值范围为有效指针。
 * @param pfnCompare [IN]  该参数表示指向比较函数的指针。取值范围为有效的函数指针或#HIMALLOC_NULL_PTR。
 *
 * @retval 后继节点表示成功。
 * @retval #HIMALLOC_NULL_PTR(0L)表示失败。
 * @par 依赖: 如下
 * @li v_legacyavl：该接口所属的开发包。
 * @li v_legacyavl.h：该接口声明所在的头文件。
 * @since V200R002C01
 * @see VOS_AVLPrev
 */
extern HIDDEN AVL_NODE_S *VOS_AVLNext(AVL_NODE_S *pstSearchNode, AVL_NODE_S *pstRootNode, VOS_AVL_CMP_FUNC pfnCompare);

extern HIDDEN AVL_NODE_S *VosAVLFindBiggerOrEqual(AVL_NODE_S *pstEntry, AVL_NODE_S *pstLookfor,
    VOS_AVL_CMP_FUNC pfnCompare);

#ifdef __cplusplus
}
#endif

#endif /* V_LEGACYAVL_H */

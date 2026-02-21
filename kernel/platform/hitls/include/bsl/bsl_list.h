/**
 * @defgroup    bsl_list   bsl_list.h
 * @ingroup     bsl
 * @copyright   Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * @brief       Linked list module
 */

#ifndef BSL_LIST_H
#define BSL_LIST_H

#include <stdint.h>
#include "bsl_errno.h"
#include "bsl_sal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* for handling ASN.1 SET OF type */
/**
 * @defgroup BslListNode
 * @ingroup sec_listStructures
 * @par Prototype
 * @code
 * typedef struct BslListNode
 * {
 * struct BslListNode *prev
 * struct BslListNode *next
 * void                  *data
 * } BslListNode
 * @endcode
 *
 * @datastruct prev The previous node in the list.
 * @datastruct next The next node in the list.
 * @datastruct data This must be the last field of this structure.
 */
typedef struct BslListNode {
    struct BslListNode *prev; /* The previous node in the list */
    struct BslListNode *next; /* The next node in the list */
    void *data;               /* This must be the last field of this structure  */
} BslListNode;

/**
 * @defgroup BslList
 * @ingroup sec_listStructures
 * @par Prototype
 * @code
 * typedef struct BslList
 * {
 * BslListNode *first
 * BslListNode *last
 * BslListNode *curr
 * int32_t            count
 * int32_t            dataSize
 * } BslList
 * @endcode
 *
 * @datastruct first The first node in the list.
 * @datastruct last The last node in the list.
 * @datastruct curr The current node in the list.
 * @datastruct count Number of elements in list.
 * @datastruct dataSize Space required in each node for the data.
 */
typedef struct BslList {
    BslListNode *first; /* The first node in the list */
    BslListNode *last;  /* The last node in the list */
    BslListNode *curr;  /* The current node in the list */
    int32_t count;          /* count of elements */
    int32_t dataSize;       /* Memory needed for each node data */
} BslList;

/**
 * the enum for specifying whether to add the element before/after the
 * current element.  It is used in BSL_LIST_AddElement()
 */
/**
 * @defgroup BslListPosition
 * @ingroup sec_listEnum
 * @par Prototype
 * @code
 * typedef enum enSEC_Position
 * {
 * BSL_LIST_POS_BEFORE,
 * BSL_LIST_POS_AFTER,
 * BSL_LIST_POS_BEGIN,
 * BSL_LIST_POS_END
 * } BslListPosition;
 * @endcode
 *
 * @datastruct BSL_LIST_POS_BEFORE Indication to to add the element before the current element.
 * @datastruct BSL_LIST_POS_AFTER Indication to to add the element after the current element.
 * @datastruct BSL_LIST_POS_BEGIN Indication to to add the element at the beginning of the list.
 * @datastruct BSL_LIST_POS_END Indication to to add the element at the end of the list.
 */
typedef enum {
    BSL_LIST_POS_BEFORE, /* Indication to to add the element before the current element */
    BSL_LIST_POS_AFTER,  /* Indication to to add the element after the current element */
    BSL_LIST_POS_BEGIN,  /* Indication to to add the element at the beginning of the list */
    BSL_LIST_POS_END     /* Indication to to add the element at the end of the list */
} BslListPosition;

/**
 * @defgroup BSL_LIST_PFUNC_CMP
 * @ingroup Function Pointer
 * @par Description
 * This is a pointer to the list comparison function used in BSL_LIST_Search function.
 * It takes two pointers and compares them based
 * on a criteria.  If the two are equal a zero is returned.  If the first
 * should preceed the second, a negative is returned.  Else a positive value
 * is returned.
 * @par Prototype
 * @code
 * typedef int32_t (* BSL_LIST_PFUNC_CMP) (const void *,const void *);
 * @endcode
 *
 */
typedef int32_t (*BSL_LIST_PFUNC_CMP)(const void *, const void *);

/**
 * @defgroup BSL_LIST_PFUNC_FREE
 * @ingroup Function Pointer
 * @par Description
 * This is a pointer to the free function.
 * The free function takes a pointer to data structure to be freed and must return void.
 * @par Prototype
 * @code
 * typedef void (* BSL_LIST_PFUNC_FREE) (void *);
 * @endcode
 *
 */
typedef void (*BSL_LIST_PFUNC_FREE)(void *);

/**
 * @defgroup BSL_LIST_PFUNC_DUP
 * @ingroup Function Pointer
 * @par Description
 * This is a pointer to the Copy function.
 * The copy function takes a pointer to data structure to be freed and must return void.
 * @par Prototype
 * @code
 * typedef void* (*BSL_LIST_PFUNC_DUP) (const void*) ;
 * @endcode
 *
 */
typedef void *(*BSL_LIST_PFUNC_DUP)(const void *);

/*
  The following macros return the specified element of the list.  They do
  not change the current list pointer.
 */
/* returns the current element */
#define BSL_LIST_CURR_ELMT(pList) ((pList) ? ((pList)->curr ? ((pList)->curr->data) : NULL) : NULL)

/* returns the next element */
#define BSL_LIST_NEXT_ELMT(pList) \
    ((pList) ? ((pList)->curr ? ((pList)->curr->next ? ((pList)->curr->next->data) : NULL) : NULL) : NULL)

/* returns the previous element */
#define BSL_LIST_PREV_ELMT(pList) \
    ((pList) ? ((pList)->curr ? ((pList)->curr->prev ? ((pList)->curr->prev->data) : NULL) : NULL) : NULL)

/* returns the last element */
#define BSL_LIST_LAST_ELMT(pList) ((pList) ? ((pList)->last ? ((pList)->last->data) : NULL) : NULL)

/* returns the first element */
#define BSL_LIST_FIRST_ELMT(pList) ((pList) ? ((pList)->first ? ((pList)->first->data) : NULL) : NULL)

/* checks if the list is NULL */
#define BSL_LIST_EMPTY(pList) (((pList) != NULL) ? ((pList)->count == 0) : 0)

/* returns the number of nodes in the list */
#define BSL_LIST_COUNT(pList) ((pList) ? ((pList)->count) : 0)

/* checks if current node is the end */
#define BSL_LIST_IS_END(pList) ((pList) ? (NULL == (pList)->curr) : 0)

/* checks if current node is the first one */
#define BSL_LIST_IS_START(pList) ((pList) ? ((pList)->first == (pList)->curr) : 0)

/* Get the next element */
#define BSL_LIST_GET_NEXT(pList) ((pList) ? (BSL_LIST_Next(pList) ? BSL_LIST_CURR_ELMT(pList) : NULL) : NULL)

/* Get the previous element */
#define BSL_LIST_GET_PREV(pList) ((pList) ? (BSL_LIST_Prev(pList) ? BSL_LIST_CURR_ELMT(pList) : NULL) : NULL)

/* Get the first element */
#define BSL_LIST_GET_FIRST(pList) ((pList) ? (BSL_LIST_First(pList) ? BSL_LIST_CURR_ELMT(pList) : NULL) : NULL)

/* Get the last element */
#define BSL_LIST_GET_LAST(pList) ((pList) ? (BSL_LIST_Last(pList) ? BSL_LIST_CURR_ELMT(pList) : NULL) : NULL)

/* Delete all the nodes in the list and then frees the header */
#define BSL_LIST_FREE(pList, pFreeFunc)        \
    do {                                       \
        BSL_LIST_DeleteAll((pList), pFreeFunc); \
        if (NULL != (pList)) {             \
            BSL_SAL_FREE(pList);                  \
            (pList) = NULL;                    \
            (void)(pList);                     \
        }                                      \
    } while (0)

/* Delete all the nodes in the list and then frees the header.
 But it does not delete the data pointers inside the list nodes.
 It is used only after sort to delete the input list to the sort function.
 */
#define BSL_LIST_FREE_AFTER_SORT(pList)    \
    do {                                   \
        BSL_LIST_DeleteAllAfterSort(pList); \
        if (NULL != (pList)) {         \
            BSL_SAL_FREE(pList);              \
            (pList) = NULL;                \
        }                                  \
    } while (0)

#define SEC_INT_ERROR (-2)

/**
 * @defgroup sec_listFunctions
 * @ingroup sec_list
 * This section contains the sec_list Functions
 */
/**
 * @defgroup BSL_LIST_SetMaxElements
 * @ingroup sec_listFunctions
 * @par Prototype
 * @code
 * int32_t BSL_LIST_SetMaxElements(int32_t iMaxElements)
 * @endcode
 *
 * @par Purpose
 * Set Max Element in BSL_LIST.
 *
 * @par Description
 * This function sets the max element in BSL_LIST.Default value is 10000000 (10 Million).
 *
 * @param[in] iMaxElements Max allowed element in BSL_LIST. It should be in range[0xffff, 0xfffffff] [N/A]
 * @param[out] N/A N/A [N/A]
 *
 * @retval int32_t If input falls outside the range [BSL_INVALID_ARG|N/A]
 * @retval int32_t If successful [BSL_SUCCESS|N/A]
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 *
 * N/A
 *
 * @par Related Topics
 * N/A
 */
int32_t BSL_LIST_SetMaxElements(int32_t iMaxElements);

/**
 * @defgroup BSL_LIST_GetMaxElements
 * @ingroup sec_listFunctions
 * @par Prototype
 * @code
 * int32_t BSL_LIST_GetMaxElements(void)
 * @endcode
 *
 * @par Purpose
 * Returns the Max allowed elements in BSL_LIST.
 *
 * @par Description
 * This function returns the max allowed elements in BSL_LIST.
 *
 * @param[in] N/A N/A [N/A]
 * @param[out] N/A N/A [N/A]
 *
 * @retval int32_t Max configured elements in BSL_LIST
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 *
 * N/A
 *
 * @par Related Topics
 * N/A
 */
int32_t BSL_LIST_GetMaxElements(void);

/**
 * @defgroup BSL_LIST_AddElement
 * @ingroup sec_listFunctions
 * @par Prototype
 * @code
 * int32_t BSL_LIST_AddElement (BslList *pList, void *pData,
 * BslListPosition enPosition );
 * @endcode
 *
 * @par Purpose
 * To create a new node before, after or at the begining or end of the current node.
 *
 * @par Description
 * This function creates a new node before, after or at the begining or end of the current node. If the list was already
 * NULL, the node will be added as the only node.The current pointer is changed to point to the newly added node in the
 * list. If the current pointer is NULL then this operation fails.
 *
 * @param[in] pList The list [N/A]
 * @param[in] pData The element to be added [N/A]
 * @param[in] enPosition Whether the elemnt is to be added before or after the list [N/A]
 * @param[out] N/A N/A [N/A]
 *
 * @retval int32_t The error code [uint32_t|N/A]
 * @retval int32_t If current pointer is NULL and list is not NULL [SEC_ERR_LIST_CURRENT_NULL|N/A]
 * @retval int32_t If data pointer is NULL [SEC_ERR_DATA_NOT_AVAILABLE|N/A]
 * @retval int32_t If failure to allocate memory for new node [SEC_ERR_MALLOC_FAIL|N/A]
 * @retval int32_t If successful [BSL_SUCCESS|N/A]
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 *
 * N/A
 *
 * @par Related Topics
 * N/A
 */
int32_t BSL_LIST_AddElement(BslList *pList, void *pData, BslListPosition enPosition);

/**
 * @defgroup BSL_LIST_DeleteAll
 * @ingroup sec_listFunctions
 * @par Prototype
 * @code
 * void BSL_LIST_DeleteAll (BslList *pList,
 * BSL_LIST_PFUNC_FREE pfFreeFunc );
 * @endcode
 *
 * @par Purpose
 * To delete all the nodes of the list but not to delete the list header.
 *
 * @par Description
 * This function deletes all the nodes of the list but does not delete the list header.
 *
 * @param[in] pList The list [N/A]
 * @param[in] pfFreeFunc The freefunction to free the data pointer in each node [N/A]
 * @param[out] N/A N/A [N/A]
 *
 * @retval Void This does not return any value [N/A|N/A]
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 * Does not free the list header.
 *
 * @par Related Topics
 * N/A
 */
void BSL_LIST_DeleteAll(BslList *pList, BSL_LIST_PFUNC_FREE pfFreeFunc);

/**
 * @defgroup BSL_LIST_DeleteCurrent
 * @ingroup sec_listFunctions
 * @par Prototype
 * @code
 * void BSL_LIST_DeleteCurrent (BslList *pList, BSL_LIST_PFUNC_FREE pfFreeFunc );
 * @endcode
 *
 * @par Purpose
 * To delete the current element of list.
 *
 * @par Description
 * This function deletes the current element of list.
 *
 * @param[in] pList The list [N/A]
 * @param[in] pfFreeFunc The pointer to the free function
 * of data [N/A]
 * @param[out] N/A N/A [N/A]
 *
 * @retval Void This does not return any value [N/A|N/A]
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 *
 * N/A
 *
 * @par Related Topics
 * N/A
 */
void BSL_LIST_DeleteCurrent(BslList *pList, BSL_LIST_PFUNC_FREE pfFreeFunc);

/**
 * @defgroup BSL_LIST_DetachCurrent
 * @ingroup sec_listFunctions
 * @par Prototype
 * @code
 * void BSL_LIST_DetachCurrent (BslList *pList);
 * @endcode
 *
 * @par Purpose
 * To detach the current element from the list, and to free the current node, but not too free the
 * data contained in the current node.
 *
 *
 * @par Description
 * This function detaches the current element from the list, the current node will be freed, but the data contained
 * in the current node will not be freed.Also the pList->first, pList->curr and pList->last will be appropriately
 * updated. If the current node is the last node, then pList->curr will point to its previous node after detachment,
 * else it will point to its next node.
 *
 * @param[in] pList The list [N/A]
 * @param[out] N/A N/A [N/A]
 *
 * @retval Void This does not return any value [N/A|N/A]
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 *
 * N/A
 *
 * @par Related Topics
 * N/A
 */
void BSL_LIST_DetachCurrent(BslList *pList);

/**
 * @defgroup BSL_LIST_Search
 * @ingroup sec_listFunctions
 * @par Prototype
 * @code
 * void* BSL_LIST_Search (BslList *pList, void *pSearchFor, BSL_LIST_PFUNC_CMP pSearcher );
 * @endcode
 *
 * @par Purpose
 * To search a list based on the comparator function supplied (3rd param).
 *
 * @par Description
 * This function searches a list based on the comparator function
 * supplied (3rd param). The second param is given to the
 * comparator as its second param and each data item on the
 * list is given as its first param while searching. The
 * comparator must return 0 to indicate a match.
 *
 * @param[in] pList The list [N/A]
 * @param[in] pSearchFor The element to be searched [N/A]
 * @param[in] pSearcher The pointer to the comparison function
 * of data [N/A]
 * @param[out] N/A N/A [N/A]
 *
 * @retval Void* The element which was found [Void*|N/A]
 * @retval Void* If none found [NULL|N/A]
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 *
 * N/A
 *
 * @par Related Topics
 * N/A
 */
void *BSL_LIST_Search(BslList *pList, const void *pSearchFor, BSL_LIST_PFUNC_CMP pSearcher, int32_t *pstErr);

/**
 * @defgroup BSL_LIST_GetIndexNode
 * @ingroup sec_listFunctions
 * @par Prototype
 * @code
 * void* BSL_LIST_GetIndexNode(uint32_t ulIndex, BslList* pList)
 * @endcode
 *
 * @par Purpose
 * To return the node at the given index in the list, starting at 0.
 *
 * @par Description
 * This function returns the node at the given index in the list, starting at 0.
 *
 * @param[in] pList The list [N/A]
 * @param[in] ulIndex The index in the list [N/A]
 * @param[out] N/A N/A [N/A]
 *
 * @retval Void* The element which was found [Void*|N/A]
 * @retval Void* If none found [NULL|N/A]
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 *
 * N/A
 *
 * @par Related Topics
 * N/A
 */
void *BSL_LIST_GetIndexNode(uint32_t ulIndex, BslList *pList);

/**
 * @defgroup BSL_LIST_Copy
 * @ingroup sec_listFunctions
 * @par Prototype
 * @code
 * BslList* BSL_LIST_Copy(BslList* pSrcList,BSL_LIST_PFUNC_DUP pFuncCpy,BSL_LIST_PFUNC_FREE pfFreeFunc)
 * @endcode
 *
 * @par Purpose
 * To dup a list by copying the list by creating a copy of the list
 * and to return the destinaton list pointer.
 *
 * @par Description
 * This function dups a list by copying the list by creating a copy of list
 * and returns the destinaton list pointer.
 *
 * @param[in] pSrcList The list [N/A]
 * @param[in] pFuncCpy The dup function for the data in the node [N/A]
 * @param[in] pfFreeFunc The pointer to the free function for the data in the node
 * of data [N/A]
 * @param[out] N/A N/A [N/A]
 *
 * @retval BslList* The duplicated List pointer [BslList*|N/A]
 * @retval BslList* If dup failed or memory allocation fails.[NULL|N/A]
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 * - If the datasize of the pSrcList is 0 and the pFuncCpy is NULL than BSL_LIST_Copy will return fail
 * N/A
 *
 * @par Related Topics
 * N/A
 */
BslList *BSL_LIST_Copy(BslList *pSrcList, BSL_LIST_PFUNC_DUP pFuncCpy, BSL_LIST_PFUNC_FREE pfFreeFunc);

/**
 * @defgroup BSL_LIST_Sort
 * @ingroup sec_listFunctions
 * @par Prototype
 * @code
 * BslList* BSL_LIST_Sort(BslList *pList, BSL_LIST_PFUNC_CMP pfCmp)
 * @endcode
 *
 * @par Purpose
 * To sort the list using the comparison function provided.
 *
 * @par Description
 * This function sorts the list using the comparison function provided.
 *
 * @param[in] pList The list [N/A]
 * @param[in] pfCmp The comparison function [N/A]
 * @param[out] N/A N/A [N/A]
 *
 * @retval BslList* If unsuccessful [NULL|N/A]
 * @retval BslList* If successful [The destination sorted list|N/A]
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 * The comparison function function must act on the double pointer
 * of the data. Sorts the list using qsort.  It deletes the source
 * list after sorting and returns the destination list.
 *
 * @par Related Topics
 * N/A
 */
BslList *BSL_LIST_Sort(BslList *pList, BSL_LIST_PFUNC_CMP pfCmp);

/**
 * @defgroup BSL_LIST_New
 * @ingroup sec_listFunctions
 * @par Prototype
 * @code
 * BslList *BSL_LIST_New  (int32_t dataSize);
 * @endcode
 *
 * @par Purpose
 * To create a new list.
 *
 * @par Description
 * This function is used to create a new list.
 *
 * @param[in] dataSize Size of the data inside the list node [N/A]
 * @param[out] N/A N/A [N/A]
 *
 * @retval BslList* An NULL list [BslList*|N/A]
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 * - It is recommended that the dataSize should not be passed as 0. It is always be the size of the data i.e. the size
 * which is required by a node to store the application data. N/A
 *
 * @par Related Topics
 * N/A
 */
BslList *BSL_LIST_New(int32_t dataSize);

/**
 * @defgroup BSL_LIST_Curr
 * @ingroup sec_listFunctions
 * @par Prototype
 * @code
 * void *BSL_LIST_Curr  (const BslList *pstList);
 * @endcode
 *
 * @par Purpose
 * To return the data of the current element in the list.
 *
 * @par Description
 * This function returns the data of the current element in the list.
 *
 * @param[in] pstList Input list [N/A]
 * @param[out] N/A N/A [N/A]
 *
 * @retval void* Data at the current element in the list [void*|N/A]
 * @retval void* If the current element does not exist in the list [NULL|N/A]
 * @retval void* If memory allocation fails. [NULL|N/A]
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 *
 * N/A
 *
 * @par Related Topics
 * N/A
 */
void *BSL_LIST_Curr(const BslList *pstList);

/**
 * @defgroup BSL_LIST_First
 * @ingroup sec_listFunctions
 * @par Prototype
 * @code
 * void *BSL_LIST_First (BslList *pstList);
 * @endcode
 *
 * @par Purpose
 * To return the data at the first element of the list.
 *
 * @par Description
 * This function returns the data at the first element of the list.
 *
 * @param[in] pstList the list [N/A]
 * @param[out] N/A N/A [N/A]
 * @retval void* Data at the first element of the list [void*|N/A]
 * @retval void* If the first element does not exist [NULL|N/A]
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 *
 * N/A
 *
 * @par Related Topics
 * N/A
 */
void *BSL_LIST_First(BslList *pstList);

/**
 * @defgroup BSL_LIST_Last
 * @ingroup sec_listFunctions
 * @par Prototype
 * @code
 * void *BSL_LIST_Last (BslList *pstList);
 * @endcode
 *
 * @par Purpose
 * To return the data at the last element of the list.
 *
 * @par Description
 * This function returns the data at the last element of the list.
 *
 * @param[in] pstList The list [N/A]
 * @param[out] N/A N/A [N/A]
 *
 * @retval void* Data at the last element of the list [void*|N/A]
 * @retval void* If the last element does not exist [NULL|N/A]
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 *
 * N/A
 *
 * @par Related Topics
 * N/A
 */
void *BSL_LIST_Last(BslList *pstList);

/**
 * @defgroup BSL_LIST_Next
 * @ingroup sec_listFunctions
 * @par Prototype
 * @code
 * void *BSL_LIST_Next (BslList *pstList);
 * @endcode
 *
 * @par Purpose
 * To advance the current pointer by one and return the data address of the new
 * current node.
 *
 * @par Description
 * This function advances the current pointer by one and returns the data address of the new
 * current node. If the current pointer is off the list, the new current node
 * will be the first node of the list (unless the list is NULL).
 *
 * @param[in] pstList The list [N/A]
 * @param[out] N/A N/A [N/A]
 *
 * @retval void* Pointer to the next element in the list [void*|N/A]
 * @retval void* If the next element does not exist [NULL|N/A]
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 *
 * N/A
 *
 * @par Related Topics
 * N/A
 */
void *BSL_LIST_Next(BslList *pstList);

/**
 * @defgroup BSL_LIST_Prev
 * @ingroup sec_listFunctions
 * @par Prototype
 * @code
 * void *BSL_LIST_Prev (BslList *pstList);
 * @endcode
 *
 * @par Purpose
 * To back up the current pointer by one and return the data address of the new
 * current node.
 *
 * @par Description
 * backs up the current pointer by one and returns the data address of the new
 * current node. If the current pointer is off the list, the new current node
 * will be the last node of the list (unless the list is NULL).
 *
 * @param[in] pstList The list [N/A]
 * @param[out] N/A N/A [N/A]
 * @retval void* Pointer to the previous element in the list [void*|N/A]
 * @retval void* If the previous element does not exist[NULL|N/A]
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 *
 * N/A
 *
 * @par Related Topics
 * N/A
 */
void *BSL_LIST_Prev(BslList *pstList);

/**
 * @defgroup BSL_LIST_GetElmtIndex
 * @ingroup sec_listFunctions
 * @par Prototype
 * @code
 * int32_t BSL_LIST_GetElmtIndex  (const void *elmt, BslList *pstList);
 * @endcode
 *
 * @par Purpose
 * To return the index (starting a 0 for the first element)
 * of the given element in the given list.
 *
 * @par Description
 * This function returns the index (starting a 0 for the first element)
 * of the given element in the given list.
 * Returns -1, if the element is not in the list.
 * Assumes that the list node contains a single pointer.
 *
 * @param[in] elmt The element whose index is to be retrieved [N/A]
 * @param[in] pstList The list to which the element belongs to [N/A]
 * @param[out] N/A N/A [N/A]
 *
 * @retval int32_t The index of the specified element in the given list [int32_t|N/A]
 * @retval int32_t If the element is not found in the list [-1|N/A]
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 *
 * N/A
 *
 * @par Related Topics
 * N/A
 */
int32_t BSL_LIST_GetElmtIndex(const void *elmt, BslList *pstList);

/**
 * @defgroup BSL_LIST_Concat
 * @ingroup sec_listFunctions
 * @par Prototype
 * @code
 * BslList *BSL_LIST_Concat  (BslList *pDestList, const BslList *pSrcList);
 * @endcode
 *
 * @par Purpose
 * To concatenate list 2 to list 1.
 *
 * @par Description
 * This function is used to concatenate list 2 to list 1.
 *
 * @param[in] pDestList The list to which the 2nd list is to be concatenated to. [N/A]
 * @param[in] pSrcList The list which is to be concatenated. [N/A]
 * @param[out] N/A N/A [N/A]
 *
 * @retval BslList* The concatenated list. [BslList*|N/A]
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 * - If the total number of nodes of pDestList + pSrcList is greater than the value set by the BSL_LIST_SetMaxElements
 * API then the API fails. Default value configured for the maximum number of elements in a list which is 10,000,000.
 *
 *
 * @par Related Topics
 * N/A
 */
BslList *BSL_LIST_Concat(BslList *pDestList, const BslList *pSrcList);

/**
 * @defgroup BSL_LIST_FreeWithoutData
 * @ingroup sec_listFunctions
 * @par Prototype
 * @code
 * void BSL_LIST_FreeWithoutData  ( BslList *pstList );
 * @endcode
 *
 * @par Purpose
 * To free the Asn list.
 *
 * @par Description
 * This function is used to  free the Asn list.
 *
 * @param[in] list Pointer to the Asn list which has to be freed [N/A]
 * @param[out] N/A N/A [N/A]
 *
 * @retval void This function does not return any value. [N/A|N/A]
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 *
 * N/A
 *
 * @par Related Topics
 * N/A
 */
void BSL_LIST_FreeWithoutData(BslList *pstList);

/**
 * @defgroup BSL_LIST_RevList
 * @ingroup sec_listFunctions
 * @par Prototype
 * @code
 * void BSL_LIST_RevList(BslList *pstList)
 * @endcode
 *
 * @par Purpose
 * To reverse the linked list.
 *
 * @par Description
 * This function is used to  reverse the linked list.
 *
 * @param[in] pstList Pointer to the list which has to be reversed [N/A]
 * @param[out] N/A N/A [N/A]
 *
 * @retval void This function does not return any value. [N/A|N/A]
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 *
 * N/A
 *
 * @par Related Topics
 * N/A
 */
void BSL_LIST_RevList(BslList *pstList);

/**
 * @defgroup BSL_LIST_SetMaxQsortCount
 * @ingroup sec_listFunctions
 * @par Prototype
 * @code
 * int32_t BSL_LIST_SetMaxQsortCount(uint32_t uiQsortSize)
 * @endcode
 *
 * @par Purpose
 * To set the Max qsort Size.
 *
 * @par Description
 * This function set the max qsort Size.Default value is 100000
 *
 * @param[in] uiQsortSize Max Buff Size. it should in range of [10000, 67108864] Default value is 100000 [N/A]
 * @param[out] N/A N/A [N/A]
 *
 * @retval int32_t BSL_SUCCESS on success BSL_INVALID_ARG on Failure [N/A|N/A]
 *
 * @par Dependency
 * sec_common.h
 *
 * @par Note
 * - Increasing the qsort increases recursion.N/A
 * - This is a global function and should not be called in multithread.
 *
 * @par Related Topics
 * N/A
 */
int32_t BSL_LIST_SetMaxQsortCount(uint32_t uiQsortSize);

/**
 * @defgroup BSL_LIST_GetMaxQsortCount
 * @ingroup sec_listFunctions
 * @par Prototype
 * @code
 * uint32_t BSL_LIST_GetMaxQsortCount()
 * @endcode
 *
 * @par Purpose
 * This function returns the MAX qsort Size.
 *
 * @par Description
 * This function returns the MAX qsort Size
 *
 * @param[in] N/A N/A [N/A]
 * @param[out] N/A N/A [N/A]
 *
 * @retval uint32_t Returns PSE qsort Size[N/A|N/A]
 *
 * @par Dependency
 * sec_common.h
 *
 * @par Note
 *
 * N/A
 *
 * @par Related Topics
 * N/A
 */
uint32_t BSL_LIST_GetMaxQsortCount(void);

/* Delete all the nodes in the list.
 But it does not delete the data pointers inside the list nodes.
 It is used only after sort to delete the input list to the sort function.
 */
void BSL_LIST_DeleteAllAfterSort(BslList *pList);

/**
 * @ingroup bsl_list_ex
 * @par Prototype
 * @code
 * BslListNode *BSL_LIST_FirstNode ( const BslList *list);
 * @endcode
 *
 * @par Purpose
 * This function will return the first node in the list.
 *
 * @par Description
 * This function returns the first element of the list.
 *
 * @param[in] list The list. [N/A]
 * @param[out] N/A N/A [N/A]
 * @retval BslListNode* first element of the list [BslListNode*|N/A]
 * @retval BslListNode* If the first element does not exist [SEC_NULL|N/A]
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 *
 * N/A
 *
 * @par Related Topics
 * N/A
 */
BslListNode *BSL_LIST_FirstNode(const BslList *list);

/**
 * @ingroup bsl_list_ex
 * @par Prototype
 * @code
 * void *BSL_LIST_GetData ( const BslListNode *pstNode);
 * @endcode
 *
 * @par Purpose
 * To return the data of the passed list node.
 *
 * @par Description
 * This function returns the data of the passed list node.
 *
 * @param[in] pstNode The node.[N/A]
 * @param[out] N/A N/A [N/A]
 *
 * @retval void* Data of the passed list node. [void*|N/A]
 * @retval void* If the data is not present in the list node. [SEC_NULL|N/A]
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 *
 * N/A
 *
 * @par Related Topics
 * N/A
 */
void *BSL_LIST_GetData(const BslListNode *pstNode);

/**
 * @ingroup bsl_list_ex
 * @par Prototype
 * @code
 * void *BSL_LIST_GetNextNode ( const BslList *pstList,
 * const BslListNode *pstListNode);
 * @endcode
 *
 * @par Purpose
 * To advance the current reference pointer by one and return the new
 * current node.
 *
 * @par Description
 * This function advances the current reference pointer by one and returns the
 * new current node. If the current reference pointer is off the list,
 * the new current node will be the first node of the list
 * (unless the list is NULL).
 *
 * @param[in] pstList The list. [N/A]
 * @param[in] pstListNode The list node.[N/A]
 * @param[out] N/A N/A [N/A]
 *
 * @retval BslListNode* Pointer to next element in the list. [void*|N/A]
 * @retval BslListNode* If the next element does not exist. [SEC_NULL|N/A]
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 *
 * N/A
 *
 * @par Related Topics
 * N/A
 */
BslListNode *BSL_LIST_GetNextNode(const BslList *pstList, const BslListNode *pstListNode);

/**
 * @ingroup bsl_list_ex
 * @par Prototype
 * @code
 * void *BSL_LIST_GetPrevNode ( const BslListNode *pstListNode);
 * @endcode
 *
 * @par Purpose
 * To back up the current reference pointer by one and return the new
 * current node.
 *
 * @par Description
 * This function backs up the current reference pointer by one and returns the
 * new current node.
 *
 * @param[in] pstListNode The list node.[N/A]
 * @param[out] N/A N/A [N/A]

 * @retval BslListNode* Pointer to the previous element in the list
 * [void*|N/A]
 * @retval BslListNode* If the previous element does not exist[SEC_NULL|N/A]
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 *
 * N/A
 *
 * @par Related Topics
 * N/A
 */
BslListNode *BSL_LIST_GetPrevNode(const BslListNode *pstListNode);

/**
 * @ingroup bsl_list_ex
 * @par Prototype
 * @code
 * void BSL_LIST_DeleteNode( BslList *pstList,
 * const BslListNode *pstListNode,
 * BSL_LIST_PFUNC_FREE pfFreeFunc)
 * @endcode
 *
 * @par Purpose
 * It will delete the matching input node from the input list.
 *
 * @par Description
 * This function deletes the matching input node from the input list.
 *
 * @param[in] pstList The list. [N/A]
 * @param[in] pstListNode The current reference node.[N/A]
 * @param[in] pfFreeFunc The pointer to the free function
 * of data. [N/A]
 * @param[out] N/A N/A [N/A]
 *
 * @retval Void This does not return any value. [N/A|N/A]
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 *
 * N/A
 *
 * @par Related Topics
 * N/A
 */
void BSL_LIST_DeleteNode(BslList *pstList, const BslListNode *pstListNode, BSL_LIST_PFUNC_FREE pfFreeFunc);

/**
 * @ingroup bsl_list_ex
 * @par Prototype
 * @code
 * void BSL_LIST_DetachNode ( BslList *pstList,
 * BslListNode *pstListNode);
 * @endcode
 *
 * @par Purpose
 * It will detach the matching input node from the input list,
 * and it will free the current node, but not frees the data
 * contained in the current node.
 *
 * @par Description
 * This function detaches the matching input node from the input list.
 * The node will be freed but, the data contained in the
 * node will not be freed, and also the pList->first, pList->curr,
 * and pList->last will be appropriately updated. If the matching node
 * is the last node, then pList->curr will point to its previous node
 * after detachment, else it will point to its next node.
 *
 * @param[in] pstList The list. [N/A]
 * @param[in] pstListNode The list node.[N/A]
 * @param[out] N/A N/A [N/A]
 *
 * @retval void This does not return any value. [N/A|N/A]
 *
 * @par Dependency
 * sec_list.h
 *
 * @par Note
 *
 * N/A
 *
 * @par Related Topics
 * N/A
 */
void BSL_LIST_DetachNode(BslList *pstList, BslListNode **pstListNode);

#ifdef __cplusplus
}
#endif /* end of __cplusplus */
#endif /* BSL_LIST_H  */

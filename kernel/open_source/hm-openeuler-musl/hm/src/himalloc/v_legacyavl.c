/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#include "v_legacyavl.h"
#include "mem_hm_inner.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

/*
 * task->mm->mmap_avl is the AVL tree corresponding to task->mm->mmap
 * or, more exactly, its root.
 * A avl_struct has the following fields:
 * pLeft     left son of a tree node
 * pRight    right son of a tree node
 * height    1+max(HEIGHTOF(left),HEIGHTOF(right))
 * The empty tree is represented as NULL.
 */
/* Since the trees are balanced, their height will never be large. */
#define HEIGHTOF(tree) (HIMALLOC_NULL_PTR == (tree) ? 0U : (uint32_t)((tree)->ulHeight))

static void avl_rebalance_left_tree(VOS_AVL_NODE *pstNode, VOS_AVL_NODE *pstNodeleft,
    VOS_AVL_NODE **ppstNodePlace, int32_t siHeightleft)
{
    VOS_AVL_NODE *pstNodeleftleft = HIMALLOC_NULL_PTR;
    VOS_AVL_NODE *pstNodeleftright = HIMALLOC_NULL_PTR;
    uint32_t uiHeightleftright;

    if (pstNodeleft != HIMALLOC_NULL_PTR) {
        pstNodeleftleft = pstNodeleft->pLeft;
        pstNodeleftright = pstNodeleft->pRight;
    }

    uiHeightleftright = HEIGHTOF(pstNodeleftright);
    if (HEIGHTOF(pstNodeleftleft) >= uiHeightleftright) {
        if (pstNodeleft != HIMALLOC_NULL_PTR) {
            pstNode->pLeft = pstNodeleftright;
            pstNodeleft->pRight = pstNode;
            pstNode->ulHeight = 1U + uiHeightleftright;
            pstNodeleft->ulHeight = 1U + pstNode->ulHeight;
        }

        *ppstNodePlace = pstNodeleft;
    } else {
        if ((pstNodeleft != HIMALLOC_NULL_PTR) && (pstNodeleftright != HIMALLOC_NULL_PTR)) {
            pstNodeleft->pRight = pstNodeleftright->pLeft;
            pstNode->pLeft = pstNodeleftright->pRight;
            pstNodeleftright->pLeft = pstNodeleft;
            pstNodeleftright->pRight = pstNode;
            pstNodeleft->ulHeight = pstNode->ulHeight = uiHeightleftright;
            pstNodeleftright->ulHeight = (uint32_t)siHeightleft;
        }

        *ppstNodePlace = pstNodeleftright;
    }
}

static void avl_rebalance_right_tree(VOS_AVL_NODE *pstNode, VOS_AVL_NODE *pstNoderight,
    VOS_AVL_NODE **ppstNodePlace, int32_t siHeightright)
{
    VOS_AVL_NODE *pstNoderightright = HIMALLOC_NULL_PTR;
    VOS_AVL_NODE *pstNoderightleft = HIMALLOC_NULL_PTR;
    uint32_t uiHeightrightleft;

    if (pstNoderight != HIMALLOC_NULL_PTR) {
        pstNoderightright = pstNoderight->pRight;
        pstNoderightleft = pstNoderight->pLeft;
    }

    uiHeightrightleft = HEIGHTOF(pstNoderightleft);
    if (HEIGHTOF(pstNoderightright) >= uiHeightrightleft) {
        pstNode->pRight = pstNoderightleft;
        if (pstNoderight != HIMALLOC_NULL_PTR) {
            pstNoderight->pLeft = pstNode;
            pstNoderight->ulHeight = 1U + (pstNode->ulHeight = 1U + uiHeightrightleft);
        }

        *ppstNodePlace = pstNoderight;
    } else {
        if ((pstNoderight != HIMALLOC_NULL_PTR) && (pstNoderightleft != HIMALLOC_NULL_PTR)) {
            pstNoderight->pLeft = pstNoderightleft->pRight;
            pstNode->pRight = pstNoderightleft->pLeft;
            pstNoderightleft->pRight = pstNoderight;
            pstNoderightleft->pLeft = pstNode;
            pstNoderight->ulHeight = pstNode->ulHeight = uiHeightrightleft;
            pstNoderightleft->ulHeight = (uint32_t)siHeightright;
        }

        *ppstNodePlace = pstNoderightleft;
    }
}

/* ****************************************************************************
 Function   : avl_rebalance
 Description: Rebalance a tree.
              After inserting or deleting a node of a tree we have a sequence
              of subtrees
              nodes[0]..nodes[k-1] such that
              nodes[0] is the root and nodes[i+1] = nodes[i]->{pLeft|pRight}.
 Calls:       None
 Called By:   avl_insert(), avl_remove().
 Input      : pppstNodePlacesPtr : Head node of tree
              siCount             : number of nodes to look for while rebalancing
 Output     : pNodePlacesPtr : contains the current head node after rebalance
 Return     : None
 **************************************************************************** */
static void avl_rebalance(int32_t siCount, VOS_AVL_NODE ***pppstNodePlacesPtr)
{
    int32_t siCountTmp = siCount;
    for (; siCountTmp > 0; siCountTmp--) {
        VOS_AVL_NODE **ppstNodePlace = *--pppstNodePlacesPtr;
        VOS_AVL_NODE *pstNode = *ppstNodePlace;
        VOS_AVL_NODE *pstNodeleft = pstNode->pLeft;
        VOS_AVL_NODE *pstNoderight = pstNode->pRight;
        int32_t siHeightleft = (int32_t)HEIGHTOF(pstNodeleft);
        int32_t siHeightright = (int32_t)HEIGHTOF(pstNoderight);
        if (siHeightright + 1 < siHeightleft) {
            avl_rebalance_left_tree(pstNode, pstNodeleft, ppstNodePlace, siHeightleft);
        } else if (siHeightleft + 1 < siHeightright) {
            avl_rebalance_right_tree(pstNode, pstNoderight, ppstNodePlace, siHeightright);
        } else {
            int32_t siHeight = ((siHeightleft < siHeightright) ? siHeightright : siHeightleft) + 1;
            if ((uint32_t)siHeight == pstNode->ulHeight) {
                /* tree is balanced now */
                break;
            }

            pstNode->ulHeight = (uint32_t)siHeight;
        }
    }
}

static int32_t avl_compare(VOS_AVL_NODE *pstLookfor, VOS_AVL_NODE *pstEntry)
{
    int32_t siKey;
    VOS_AVL_NODE *pNode = pstLookfor;
    if ((pNode->ulHandle) > (pstEntry->ulHandle)) {
        siKey = 1;
    } else if ((pNode->ulHandle) < (pstEntry->ulHandle)) {
        siKey = -1;
    } else {
        siKey = 0;
    }

    return siKey;
}
/* v71154: Defect YDID03264 fix: Start here */
/* ****************************************************************************
 Function   : avl_insert
 Description: Insert a node into a tree.
 Calls:       avl_rebalance()
 Called By:   VOS_AVLAdd().
 Input      : pstNewNode : Pointer of new node to insert in avl tree.
              ppstTree   : Root node of avl tree
              pfnCompare : node compare function pointer, return >0 if first
                           parm is bigger , 0 if equal, <0 if smaller
 Output     : pstTree : Current head node of tree after inserting
 Return     : HIMALLOC_OK on success
  **************************************************************************** */
static int32_t avl_insert(VOS_AVL_NODE *pstNewNode, VOS_AVL_NODE **ppstTree, VOS_AVL_CMP_FUNC pfnCompare)
{
    VOS_AVL_NODE **ppstNodePlace = ppstTree;
    VOS_AVL_NODE **astStack[AVL_MAXHEIGHT] = {0};
    int32_t siStackCount = 0;
    VOS_AVL_NODE ***pppstStack = &astStack[0]; /* = &astStack[stackcount] */
    int32_t siRet;
    VOS_AVL_CMP_FUNC pfnCompareTmp = pfnCompare;

    if (pfnCompareTmp == HIMALLOC_NULL_PTR) {
        pfnCompareTmp = avl_compare;
    }

    if ((pstNewNode == HIMALLOC_NULL_PTR) || (ppstTree == HIMALLOC_NULL_PTR)) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERR;
    }

    for (; siStackCount < AVL_MAXHEIGHT; siStackCount++) {
        VOS_AVL_NODE *pstNode = *ppstNodePlace;

        if (pstNode == HIMALLOC_NULL_PTR) {
            /* reached end of tree */
            break;
        }

        *(pppstStack++) = ppstNodePlace;
        siRet = pfnCompareTmp(pstNewNode, pstNode);
        if (siRet == 0) {
            __hmattr_sideff_resolved("no relative side effect");
            return HIMALLOC_ERR;
        }
        ppstNodePlace = (siRet < 0) ? (&pstNode->pLeft) : (&pstNode->pRight);
    }

    if (siStackCount == AVL_MAXHEIGHT) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERR;
    }

    pstNewNode->pLeft = pstNewNode->pRight = HIMALLOC_NULL_PTR;
    pstNewNode->ulHeight = 1U;
    *ppstNodePlace = pstNewNode;

    avl_rebalance(siStackCount, pppstStack);

    return HIMALLOC_OK;
}

static void avl_remove_sub_node(VOS_AVL_NODE ****pppstStack, VOS_AVL_NODE *pstNodeToDelete,
                                VOS_AVL_NODE **ppstNodePlaceToDelete, int32_t *siStackCount)
{
    VOS_AVL_NODE ***pppstStackPtrToDelete = *pppstStack;
    VOS_AVL_NODE **ppstNodePlacelocal = &(pstNodeToDelete->pLeft);
    VOS_AVL_NODE *pstNode = HIMALLOC_NULL_PTR;

    for (;;) {
        pstNode = *ppstNodePlacelocal;

        if (pstNode->pRight == HIMALLOC_NULL_PTR) {
            /* traverse to the rightmost node of tree */
            break;
        }

        *((*pppstStack)++) = ppstNodePlacelocal;
        (*siStackCount)++;
        ppstNodePlacelocal = &pstNode->pRight;
    }

    *ppstNodePlacelocal = pstNode->pLeft;

    /* node replaces pstNodeToDelete */
    pstNode->pLeft = pstNodeToDelete->pLeft;
    pstNode->pRight = pstNodeToDelete->pRight;
    pstNode->ulHeight = pstNodeToDelete->ulHeight;

    /* replace pstNodeToDelete */
    *ppstNodePlaceToDelete = pstNode;

    /* replace &pstNodeToDelete->pLeft */
    *pppstStackPtrToDelete = &pstNode->pLeft;

    return;
}

/* v71154: Defect YDID03264 fix: End here */
/* ****************************************************************************
 Function   : avl_remove
 Description: Removes a node out of a tree.
 Calls:       avl_rebalance()
 Called By:   VOS_AVLDel().
 Input      : pstNodeToDelete : Pointer of node to be removed from avl tree.
              ppstTree        : Root node of avl tree.
              pfnCompare      : node compare function pointer, return >0 if
                                first parm is bigger , 0 if equal, <0 if smaller
 Output     : pstTree : Current head node of tree after removing.
 Return     : None.
  **************************************************************************** */
static void avl_remove(VOS_AVL_NODE *pstNodeToDelete, VOS_AVL_NODE **ppstTree, VOS_AVL_CMP_FUNC pfnCompare)
{
    VOS_AVL_NODE **ppstNodePlace = ppstTree;
    VOS_AVL_NODE  **astStack[AVL_MAXHEIGHT] = {0};
    int32_t siStackCount = 0;
    VOS_AVL_NODE ***pppstStack = &astStack[0]; /* = &astStack[stackcount] */
    VOS_AVL_NODE **ppstNodePlaceToDelete = HIMALLOC_NULL_PTR;
    VOS_AVL_CMP_FUNC pfnCompareTmp = pfnCompare;

    if (pfnCompareTmp == HIMALLOC_NULL_PTR) {
        pfnCompareTmp = avl_compare;
    }

    if ((pstNodeToDelete == HIMALLOC_NULL_PTR) || (ppstTree == HIMALLOC_NULL_PTR)) {
        __hmattr_sideff_resolved("no relative side effect");
        return;
    }

    for (;;) {
        VOS_AVL_NODE *pstNode = *ppstNodePlace;

        if (pstNode == HIMALLOC_NULL_PTR) {
            /* what? pstNodeToDelete not found in tree? */
            /* node to delete not found in tree */
            __hmattr_sideff_resolved("no relative side effect");
            return;
        }

        *(pppstStack++) = ppstNodePlace;
        siStackCount++;
        if (pfnCompareTmp(pstNodeToDelete, pstNode) == 0) {
            if (pstNode == pstNodeToDelete) {
                break;
            } else {
                __hmattr_sideff_resolved("no relative side effect");
                return;
            }
        }

        if (pfnCompareTmp(pstNodeToDelete, pstNode) < 0) {
            ppstNodePlace = &pstNode->pLeft;
        } else {
            ppstNodePlace = &pstNode->pRight;
        }
    }

    ppstNodePlaceToDelete = ppstNodePlace;

    /* Have to remove pstNodeToDelete = *nodeplace_to_delete. */
    if (pstNodeToDelete->pLeft == HIMALLOC_NULL_PTR) {
        *ppstNodePlaceToDelete = pstNodeToDelete->pRight;
        pppstStack--;
        siStackCount--;
    } else {
        (void)avl_remove_sub_node(&pppstStack, pstNodeToDelete, ppstNodePlaceToDelete, &siStackCount);
    }

    avl_rebalance(siStackCount, pppstStack);
}

/* modified back by Wood to remove compilation warning */
/* ****************************************************************************
 Function   : avl_find
 Description: Find a node with the specific key in a tree.
 Called By:   VOS_AVLFind().
 Input      : pstEntry   -- Root node of avl tree.
              pstFind -- Pointer of node with the specific key to find in a tree
              pfnCompare -- node compare function pointer, return >0 if first
              parm is bigger , 0 if equal, <0 if smaller.
 Output     : None.
 Return     : Node of tree having same key value as key value of pstFind
  **************************************************************************** */
static VOS_AVL_NODE *avl_find(VOS_AVL_NODE *pstEntry, VOS_AVL_NODE *pstFind, VOS_AVL_CMP_FUNC pfnCompare)
{
    int32_t siCmp;
    VOS_AVL_CMP_FUNC pfnCompareTmp = pfnCompare;

    if (pfnCompareTmp == HIMALLOC_NULL_PTR) {
        pfnCompareTmp = avl_compare;
    }

    if (pstFind == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    while (pstEntry != HIMALLOC_NULL_PTR) {
        siCmp = pfnCompareTmp(pstFind, pstEntry);
        if (siCmp < 0) {
            pstEntry = pstEntry->pLeft;
        } else if (siCmp > 0) {
            pstEntry = pstEntry->pRight;
        } else {
            __hmattr_sideff_resolved("no relative side effect");
            return pstEntry;
        }
    }

    return HIMALLOC_NULL_PTR;
}

/* ****************************************************************************
 Function   : VOS_AVLFind
 Description: find a node.
 Calls:       avl_find().
 Called By:   application.
 Input      : pstEntry   -- Root node of avl tree.
              pstLookfor -- Pointer of node with the specific key to find in a
                            tree.
              pfnCompare -- node compare function pointer, return >0 if first
              parm is bigger , 0 if equal, <0 if smaller.

 Output     : None
 Return     : Node of tree having same key value as key value of pstLookfor.
 **************************************************************************** */
HIDDEN AVL_NODE_S *VOS_AVLFind(AVL_NODE_S *pstEntry, void *pstLookfor, VOS_AVL_CMP_FUNC pfnCompare)
{
    if (pstEntry == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    if ((pfnCompare == HIMALLOC_NULL_PTR) && (pstLookfor == HIMALLOC_NULL_PTR)) {
        __hmattr_sideff_resolved("no relative side effect");
        return pstEntry;
    }

    return avl_find((VOS_AVL_NODE *)pstEntry, pstLookfor, pfnCompare);
}

/* ****************************************************************************
 Function   : VOS_AVLDel
 Description: delete a tree node from avl tree.
 Calls:       avl_remove().
 Called By:   application.
 Input      : pstHead    -- current tree root, maybe null
              pstEntry   -- a node to delete, must in tree!
              pfnCompare -- node compare function pointer, return >0 if first
              parm is bigger , 0 if equal, <0 if smaller
 Output     : None
 Return     : new tree root, others for error
 Other      : call it as  root=VOS_AVLDel(root,node,comp)
 **************************************************************************** */
HIDDEN AVL_NODE_S *VOS_AVLDel(AVL_NODE_S *pstHead, AVL_NODE_S *pstEntry, VOS_AVL_CMP_FUNC pfnCompare)
{
    if (pstHead == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    if (pstEntry == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return pstHead;
    }

    avl_remove((VOS_AVL_NODE *)pstEntry, (VOS_AVL_NODE **)&pstHead, pfnCompare);
    return pstHead;
}

/* ****************************************************************************
 Function   :  VOS_AVLAdd
 Description:  insert a new node into avl tree.
 Calls      :  avl_insert().
 Called By  :  application.
 Input      :  pstHead    -- current tree root, maybe null
               pstEntry   -- new node to insert,must not in tree already!
               pfnCompare -- node compare function pointer, return >0 if first
               parm is bigger , 0 if equal, <0 if smaller
 Output     :  None
 Return     :  new tree root.
 Other      :  call it as  root=VOS_AVLAdd(root,newnode,comp);
 **************************************************************************** */
HIDDEN AVL_NODE_S *VOS_AVLAdd(AVL_NODE_S *pstHead, AVL_NODE_S *pstEntry, VOS_AVL_CMP_FUNC pfnCompare)
{
    AVL_NODE_S *pstTemp = HIMALLOC_NULL_PTR;
    int32_t siReturn;

    pstTemp = pstHead; /* pstHead can be null */
    /* v71154: Defect YDID03264 fix: Start here */
    siReturn = avl_insert((VOS_AVL_NODE *)pstEntry, (VOS_AVL_NODE **)&pstTemp, pfnCompare);
    if (siReturn != HIMALLOC_OK) {
        __hmattr_sideff_resolved("no relative side effect");
        return pstHead;
    }

    /* v71154: Defect YDID03264 fix: End here */
    return pstTemp;
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */

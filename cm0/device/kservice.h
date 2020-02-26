/*
 * Copyright 2016, yichip Semiconductor(shenzhen office)
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Yichip Semiconductor;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Yichip Semiconductor.
 */
 
 /** @file
 *
 * list support for application
 */

#ifndef ___KSERVICE_H__
#define ___KSERVICE_H__
#include "ycdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup KernelService
 */

/*@{*/

/**
 * @brief initialize a list object
 */
#define LIST_OBJECT_INIT(object) { &(object), &(object) }

/**
 * @brief initialize a list
 *
 * @param l list to be initialized
 */
static inline void LIST_Init(list_t *l)
{
    l->next = l->prev = l;
}

/**
 * @brief insert a node after a list
 *
 * @param l list to insert it
 * @param n new node to be inserted
 */
static inline void LIST_InsertAfter(list_t *l, list_t *n)
{
    l->next->prev = n;
    n->next = l->next;

    l->next = n;
    n->prev = l;
}

/**
 * @brief insert a node before a list
 *
 * @param n new node to be inserted
 * @param l list to insert it
 */
static inline void LIST_InsertBefore(list_t *l, list_t *n)
{
    l->prev->next = n;
    n->prev = l->prev;

    l->prev = n;
    n->next = l;
}

/**
 * @brief remove node from list.
 * @param n the node to remove from the list.
 */
static inline void LIST_Remove(list_t *n)
{
    n->next->prev = n->prev;
    n->prev->next = n->next;

    n->next = n->prev = n;
}

/**
 * @brief tests whether a list is empty
 * @param l the list to test.
 */
static inline int LIST_Isempty(const list_t *l)
{
    return l->next == l;
}

/**
 * @brief get the struct for this entry
 * @param node the entry point
 * @param type the type of structure
 * @param member the name of list in structure
 */
#define LIST_Entry(node, type, member) \
    ( (type *)((char *)(node) - (unsigned long)(&((type *)0)->member)) )

/**
 * LIST_ForEachEntry  -   iterate over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define LIST_ForEachEntry(pos, head, member)              \
    for (pos = LIST_Entry((head)->next, typeof(*pos), member);  \
         &pos->member != (head);    \
         pos = LIST_Entry(pos->member.next, typeof(*pos), member))

/**
 * LIST_FirstEntry - get the first element from a list
 * @ptr:    the list head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define LIST_FirstEntry(ptr, type, member) \
    LIST_Entry((ptr)->next, type, member)
/*@}*/

#ifdef __cplusplus
}
#endif

#endif

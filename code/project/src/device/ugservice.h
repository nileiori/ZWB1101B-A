
#ifndef __UG_SERVICE_H__
#define __UG_SERVICE_H__

#ifdef __cplusplus
extern "C" {
#endif


#define ug_container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))


/**
 * @brief initialize a list object
 */
#define UG_LIST_OBJECT_INIT(object) { &(object), &(object) }

/**
 * @brief initialize a list
 *
 * @param l list to be initialized
 */
ug_inline void ug_list_init(ug_list_t *l)
{
    l->next = l->prev = l;
}

/**
 * @brief insert a node after a list
 *
 * @param l list to insert it
 * @param n new node to be inserted
 */
ug_inline void ug_list_insert_after(ug_list_t *l, ug_list_t *n)
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
ug_inline void ug_list_insert_before(ug_list_t *l, ug_list_t *n)
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
ug_inline void ug_list_remove(ug_list_t *n)
{
    n->next->prev = n->prev;
    n->prev->next = n->next;

    n->next = n->prev = n;
}

/**
 * @brief tests whether a list is empty
 * @param l the list to test.
 */
ug_inline int ug_list_isempty(const ug_list_t *l)
{
    return l->next == l;
}

/**
 * @brief get the list length
 * @param l the list to get.
 */
ug_inline unsigned int ug_list_len(const ug_list_t *l)
{
    unsigned int len = 0;
    const ug_list_t *p = l;
    while (p->next != l)
    {
        p = p->next;
        len ++;
    }

    return len;
}

/**
 * @brief get the struct for this entry
 * @param node the entry point
 * @param type the type of structure
 * @param member the name of list in structure
 */
#define ug_list_entry(node, type, member) \
    ug_container_of(node, type, member)

/**
 * rt_list_for_each - iterate over a list
 * @pos:	the rt_list_t * to use as a loop cursor.
 * @head:	the head for your list.
 */
#define ug_list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * rt_list_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:	the rt_list_t * to use as a loop cursor.
 * @n:		another rt_list_t * to use as temporary storage
 * @head:	the head for your list.
 */
#define ug_list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

/**
 * rt_list_for_each_entry  -   iterate over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define ug_list_for_each_entry(pos, head, member) \
    for (pos = ug_list_entry((head)->next, typeof(*pos), member); \
         &pos->member != (head); \
         pos = ug_list_entry(pos->member.next, typeof(*pos), member))

/**
 * rt_list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:    the type * to use as a loop cursor.
 * @n:      another type * to use as temporary storage
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define ug_list_for_each_entry_safe(pos, n, head, member) \
    for (pos = ug_list_entry((head)->next, typeof(*pos), member), \
         n = ug_list_entry(pos->member.next, typeof(*pos), member); \
         &pos->member != (head); \
         pos = n, n = ug_list_entry(n->member.next, typeof(*n), member))

/**
 * rt_list_first_entry - get the first element from a list
 * @ptr:    the list head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define ug_list_first_entry(ptr, type, member) \
    ug_list_entry((ptr)->next, type, member)

#define UG_SLIST_OBJECT_INIT(object) { UG_NULL }

/**
 * @brief initialize a single list
 *
 * @param l the single list to be initialized
 */
ug_inline void ug_slist_init(ug_slist_t *l)
{
    l->next = UG_NULL;
}

ug_inline void ug_slist_append(ug_slist_t *l, ug_slist_t *n)
{
    struct ug_slist_node *node;

    node = l;
    while (node->next) node = node->next;

    /* append the node to the tail */
    node->next = n;
    n->next = UG_NULL;
}

ug_inline void ug_slist_insert(ug_slist_t *l, ug_slist_t *n)
{
    n->next = l->next;
    l->next = n;
}

ug_inline unsigned int ug_slist_len(const ug_slist_t *l)
{
    unsigned int len = 0;
    const ug_slist_t *list = l->next;
    while (list != UG_NULL)
    {
        list = list->next;
        len ++;
    }

    return len;
}

ug_inline ug_slist_t *ug_slist_remove(ug_slist_t *l, ug_slist_t *n)
{
    /* remove slist head */
    struct ug_slist_node *node = l;
    while (node->next && node->next != n) node = node->next;

    /* remove node */
    if (node->next != (ug_slist_t *)0) node->next = node->next->next;

    return l;
}

ug_inline ug_slist_t *ug_slist_first(ug_slist_t *l)
{
    return l->next;
}

ug_inline ug_slist_t *ug_slist_next(ug_slist_t *n)
{
    return n->next;
}

ug_inline int ug_slist_isempty(ug_slist_t *l)
{
    return l->next == UG_NULL;
}


#define ug_slist_entry(node, type, member) \
    ug_container_of(node, type, member)


#define ug_slist_for_each(pos, head) \
    for (pos = (head)->next; pos != UG_NULL; pos = pos->next)


#define ug_slist_for_each_entry(pos, head, member) \
    for (pos = ug_slist_entry((head)->next, typeof(*pos), member); \
         &pos->member != (UG_NULL); \
         pos = ug_slist_entry(pos->member.next, typeof(*pos), member))

/**
 * rt_slist_first_entry - get the first element from a slist
 * @ptr:    the slist head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the slist_struct within the struct.
 *
 * Note, that slist is expected to be not empty.
 */
#define ug_slist_first_entry(ptr, type, member) \
    ug_slist_entry((ptr)->next, type, member)

/*@}*/

#ifdef __cplusplus
}
#endif

#endif



#pragma once

struct list_head {
	struct list_head *prev, *next;
};

/**
 * @def list_entry(ptr, type, member)
 * @brief Get the struct for this entry.
 * @param ptr The ptr to the entry.
 * @param type The type of the struct this is embedded in.
 * @param member The name of the list_head within the struct.
 */
#define list_entry(ptr, type, member) \
	((type *)((void *)(ptr) - offsetof(type, member)))

/**
 * @def list_for_each(pos, head)
 * @brief Iterate over all entries in a list.
 */
#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != head; pos = pos->next)

/**
 * @def list_next(entry, member)
 * @brief Get the next entry from the list.
 * @param entry The current struct *.
 * @param member The name of the list_head within the struct.
 */
#define list_next(entry, member) \
	(list_entry((entry)->member.next, typeof(*entry), member))

/**
 * @def list_next_circular(entry, member, head)
 * @brief Get the next entry from the list, wrapping around to the start.
 * @param entry The current struct *.
 * @param member The name of the list_head within the struct.
 * @param head A pointer to the head of the list.
 */
#define list_next_circular(entry, member, head)                \
	((entry)->member.next == head                              \
			? list_entry((head)->next, typeof(*entry), member) \
			: list_next(entry, member))

/**
 * @def LIST_HEAD_INIT(name)
 * @brief Statically initialize a list head.
 */
#define LIST_HEAD_INIT(name) {&(name), &(name)}

/**
 * @brief Initialize a list head.
 * @param head The list_head to initialize.
 */
static inline void init_list_head(struct list_head *head) {
	head->prev = head;
	head->next = head;
}

/**
 * @brief Add an entry to a list.
 * @param new The entry to add.
 * @param head The list to add the entry to.
 */
static inline void list_add(struct list_head *new, struct list_head *head) {
	struct list_head *temp = head->next;
	temp->prev = new;
	new->next = temp;
	head->next = new;
	new->prev = head;
}

/**
 * @brief Add an entry to the end of a list.
 * @param new The entry to add.
 * @param head The list to add the entry to.
 */
static inline void list_add_tail(struct list_head *new,
	struct list_head *head) {
	struct list_head *temp = head->prev;
	temp->next = new;
	new->prev = temp;
	head->prev = new;
	new->next = head;
}

/**
 * @brief Delete an element from a list.
 * @param entry The entry to delete.
 */
static inline void list_del(struct list_head *entry) {
	entry->prev->next = entry->next;
	entry->next->prev = entry->prev;
}

/**
 * @brief Check if a given list is empty.
 * @param head The head of the list.
 * @return Whether or not the list is empty.
 */
static inline bool list_empty(struct list_head *head) {
	return head == head->next;
}

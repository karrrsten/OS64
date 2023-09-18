#pragma once

#include <stddef.h>

struct list_head {
	struct list_head *prev, *next;
};

#define LIST_HEAD_INIT(name) \
	{ &(name), &(name) }

#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)

#define list_entry(list_head, type, member) \
	((type *)((void *)list_head - offsetof(type, member)))

#define list_for_each(pos, list) \
	for (struct list_head *pos = (list)->next; pos != list; pos = pos->next)

static inline void list_init(struct list_head *list) {
	list->next = list;
	list->prev = list;
}

static inline void list_add(struct list_head *entry, struct list_head *list) {
	struct list_head *next = list->next;
	list->next = entry;
	entry->prev = list;
	entry->next = next;
	next->prev = entry;
}

static inline void list_del(struct list_head *entry) {
	entry->prev->next = entry->next;
	entry->next->prev = entry->prev;
}

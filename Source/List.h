/*
 * List.h
 *
 *  Created on: Oct 28, 2019
 *      Author: Oleh
 */

#ifndef SOURCE_LIST_H_
#define SOURCE_LIST_H_

#include <stdbool.h>

#define MAX 50

typedef struct temperatures{
	char info[MAX];
} Item;

typedef struct nodelist{
    Item item;
    struct nodelist * next;
} Node;

typedef Node * List;

void InitializeList(List * plist);

bool ListIsFull(const List * plist);

bool ListIsEmpty(const List * plist);

int ListItemCount(const List * plist);

bool AddItemList(Item itemL, List * plist);

void Traverse(const List * plist, void(*pfun)(Item item));

void EmptyTheList(List * plist);

#endif /* SOURCE_LIST_H_ */

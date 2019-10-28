/*
 * List.c
 *
 *  Created on: Oct 28, 2019
 *      Author: Oleh
 */

#include "List.h"
#include <stdlib.h>


void InitializeList(List * plist){
    *plist = NULL;
}

bool ListIsFull(const List * plist){
    Node * pnode;
    bool full;

    pnode = (Node *)malloc(sizeof(Node));
    if (pnode == NULL)
        full = true;
    else
        full = false;
    free(pnode);
    return full;
}

bool ListIsEmpty(const List * plist){
    if (*plist == NULL)
        return true;
    else
        return false;
}

int ListItemCount(const List * plist){
    int count = 0;
    Node * pnode = *plist;

    while (pnode != NULL){
        ++count;
        pnode = pnode->next;
    }
    return count;
}

void CopyToNode(Item itemL, Node * pnode){
    pnode->item = itemL;
}

bool AddItemList(Item item, List * plist){
    Node * pnew;
    Node * scan = *plist;

    pnew = (Node *)malloc(sizeof(Node));
    if (pnew == NULL)
        return false;
    CopyToNode(item,pnew);
    pnew->next = NULL;

    if (scan == NULL)
        *plist = pnew;
    else{
        while (scan->next != NULL)
            scan = scan->next;
        scan->next = pnew;
    }
    return true;
}

void Traverse(const List * plist, void(*pfun)(Item item)){
    Node * pnode = *plist;

    while (pnode != NULL){
        (*pfun)(pnode->item);
        pnode = pnode->next;
    }
}

void EmptyTheList(List * plist){
    Node * psave;

    while (*plist != NULL){
        psave = (*plist)->next;
        free(*plist);
        *plist = psave;
    }
}

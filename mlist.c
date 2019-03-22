

#if XNDEBUG
#define XASSERT(x)
#else 
#include <assert.h>
#define XASSERT(x)       assert(x)
#endif

#ifndef NULL
#define NULL (void *)0
#endif

#define MLIST_MEMBER(x)  \
        x *prev;        \
        x *next;

#define MLIST_NEXT(x)    ((x)->next)
#define MLIST_PREV(x)    ((x)->prev)

struct MList {
        struct MList *prev;
        struct MList *next;
};

void MListInit(void *list)
{
        XASSERT(list);

        struct MList *l = (struct MList *)list;

        l->prev = l->next = l;
}

void MListInsertAfter(void *prev, void *next)
{
        XASSERT(prev);
        XASSERT(next);

        struct MList *mprev = (struct MList *)prev;
        struct MList *mnext = (struct MList *)next;

        mnext->next = mprev->next;
        mnext->prev = mprev;
        mprev->next->prev = mnext;
        mprev->next = mnext;
}

void MListInsertBefore(void *now, void *node)
{
        MListInsertAfter(node, now);
}

void MListInsert(void *head, void *node)
{
        XASSERT(head);
        XASSERT(node);

        struct MList *mprev = (struct MList *)head;
        struct MList *mnext = (struct MList *)node;

        mnext->next = mprev;
        mnext->prev = mprev->prev;
        mprev->prev = mnext;
        mnext->prev->next = mnext;
        
}


/***********************************************/

struct TestMList {
        MLIST_MEMBER(struct TestMList);
        int val;
};

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
        struct TestMList *mylist = NULL;
        struct TestMList *node = NULL;

        mylist = calloc(1, sizeof(struct TestMList));
        XASSERT(mylist);
        MListInit(mylist);

        int i;
        for (i = 0; i < 10; i++) {
                node = calloc(1, sizeof(struct TestMList));
                node->val = i + 1;
                MListInsert(mylist, node);

                printf("%p - %p - %p - %p - %p - %p\n", mylist, node, mylist->next, mylist->prev, node->prev, node->next);
        }

        node = mylist;
        for (i = 0; i < 3; i++) {
                
                printf("%p - %p\n", node, node->next);
                node = node->next;
        }

        node = mylist;
        do {
                printf("%d\n", node->val);

                node = MLIST_NEXT(node);
                
        } while (node != mylist);

        return 0;
        

}















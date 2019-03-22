
#include <stdlib.h>

#if XNDEBUG
#define XASSERT(x)
#else 
#include <assert.h>
#define XASSERT(x)       assert(x)
#endif

struct xdata_ {
        int a;
        int *p;
};
typedef struct xdata_ * xdata;

typedef struct xnode_ {
        struct xnode_ *prev;
        struct xnode_ *next;

        xdata data;
        
} xnode;

typedef xdata (*xassign)(xdata data);
typedef void (*xdel)(xdata data);

typedef struct xlist_ {
        xnode *head;
        xnode *tail;
        int count;
        xassign assign;
        xdel del;
} xlist;

void xlist_init(xlist *list, xassign assign, xdel del)
{
        XASSERT(list);

        list->head = NULL;
        list->tail = NULL;
        list->count = 0;
        list->assign = assign;
        list->del = del;
}

xlist *xlist_new(xassign assign, xdel del)
{
        xlist *list = calloc(1, sizeof(xlist));
        if (!list) {
                return NULL;
        }
        
        xlist_init(list, assign, del);
        return list;
}

void xlist_del(xlist *list)
{
        free(list);
}

xnode *xnode_new(void)
{
        xnode *p = calloc(1, sizeof(xnode));
        return p;
}

void xnode_del(xnode *p)
{
        free(p);
}

int xlist_add_tail(xlist *list, xdata data)
{
        XASSERT(list);

        xnode *node = xnode_new();
        if (!node) {
                return -1;
        }

        if (list->assign != NULL) {
                node->data = list->assign(data);        
        } else {
                node->data = data;
        }

        if (list->head == NULL) {
                list->head = node;
        }
        if (list->tail) {
                list->tail->next = node;
        }
        node->prev = list->tail;

        list->tail = node;
        list->count++;

        return 0;
}

int xlist_add_head(xlist *list, xdata data)
{
        XASSERT(list);

        xnode *node = xnode_new();
        if (!node) {
                return -1;
        }
        if (list->assign != NULL) {
                node->data = list->assign(data);        
        } else {
                node->data = data;
        }
        
        if (list->tail == NULL) {
                list->tail = node;
        }
        if (list->head) {
                list->head->prev = node;
        }
        node->next = list->head;

        list->head = node;
        list->count++;

        return 0;
}

int xlist_del_tail(xlist *list, xdata *data)
{
        XASSERT(list);

        if (list->count == 0) {
                return -1;
        }

        xnode *node = list->tail;
        
        list->tail = list->tail->prev;
        list->tail->next = NULL;
        list->count--;

        if (data) {
                *data = list->assign(node->data);
        }
        xnode_del(node);

        return 0;
}

int xlist_del_head(xlist *list, xdata *data)
{
        XASSERT(list);

        if (list->count == 0) {
                return -1;
        }

        xnode *node = list->head;
        
        list->head = list->head->next;
        list->head->prev = NULL;
        list->count--;

        if (data) {
                *data = list->assign(node->data);
        }
        xnode_del(node);

        return 0;
}

int xlist_count(xlist *list) {
        XASSERT(list);
        return list->count;
}

#define ALL_XLIST_WR(list, n, nn, data) \
        n = list->head; (n && (data = list->assign(n->data), nn = n->next)); n = nn

#define ALL_XLIST_RO(list, n, data) \
        n = list->head; n ; n = n->next

void xlist_clear(xlist *list)
{
        XASSERT(list);

        xnode *node;
        xnode *nextnode;
        xdata data;

        for(ALL_XLIST_WR(list, node, nextnode, data)) {
                if (list->del) {
                        list->del(data);
                }
                xnode_del(node);
        }
        xlist_del(list);
}



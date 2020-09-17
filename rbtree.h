
/* Red and black tree */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
enum color { BLACK, RED };


typedef struct rbnode{
    struct rbnode* p;
    struct rbnode* left;
    struct rbnode* right;
    char *row;
    void *key;
    enum color c;
}rbnode;


typedef struct rbtree{
    rbnode *root;
    rbnode *nil;
}rbtree;


rbnode *make_rbnode(rbtree *t,enum color c,int key,char *row);
void rb_insert_fixup(rbtree* t, rbnode* z);
rbnode* rbinsert(rbtree* t, rbnode* z);
void rb_delete_fixup(rbtree* t, rbnode* x);
rbnode* rbsearch(rbtree* t, int key);
rbnode* tree_maximum(rbtree* t, rbnode* x);
rbnode* tree_minimum(rbtree* t, rbnode* x);
void right_rotate(rbtree* t, rbnode* x);
void left_rotate(rbtree* t, rbnode* x);
void rb_transplant(rbtree* t, rbnode* u, rbnode* v);
void init_rbtree(rbtree* t);
rbnode* make_rbnil();
rbtree* tree_create();
void rbsubstitute(rbtree *t,int key,char *newrow);
void free_tree(rbtree  *T,rbnode *h);
void rb_fixkeys(rbtree  *T,rbnode *x,int c);
rbnode *rb_successor(rbtree *t,rbnode *r);




rbnode* make_rbnode(rbtree* t, enum color c, int key, char *row){
    rbnode* n = (rbnode*) malloc(sizeof(rbnode));
    n->c = c;
    n->p = t->nil;
    n->left = t->nil;
    n->right = t->nil;
    n->key = (void *) key;
    n->row = row;
    return n;
}
rbnode* make_rbnil()
{
    rbnode* nil = (rbnode*) malloc(sizeof(rbnode));
    nil->c = BLACK;
    nil->p = NULL;
    nil->left = NULL;
    nil->right = NULL;
    nil->key = NULL;
    return nil;
}


rbtree* tree_create(){
    rbtree *t;
    t = malloc(sizeof(rbtree));
    assert(t);
    init_rbtree(t);
    return t;
}
void init_rbtree(rbtree* t)
{
    t->nil = make_rbnil();
    t->root = t->nil;
}
rbnode* rbinsert(rbtree* t, rbnode* z)
{
    rbnode* y = t->nil;
    rbnode* x = t->root;
    while (x != t->nil) {
        y = x;
        if (z->key == x->key)
            return NULL;
        if (z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    z->p = y;
    if (y == t->nil) {
        assert(t->root == t->nil);
        t->root = z;
    } else if (z->key < x->key)
        y->left = z;
    else
        y->right = z;


    z->left = t->nil;
    z->right = t->nil;
    z->c = RED;
    rb_insert_fixup(t, z);
    return z;
}


void rb_insert_fixup(rbtree* t, rbnode* z)
{
    assert(t && z);
    while (z->p->c == RED) {
        assert(z->p->p);
        if (z->p == z->p->p->left) {
            rbnode* y = z->p->p->right;
            if (y->c == RED) {
                z->p->c = BLACK;
                y->c = BLACK;
                z->p->p->c = RED;
                z = z->p->p;
            } else {
                if (z == z->p->right) {
                    z = z->p;
                    left_rotate(t, z);
                }
                z->p->c = BLACK;
                z->p->p->c = RED;
                right_rotate(t, z->p->p);
            }
        } else {
            assert(z->p == z->p->p->right);
            rbnode* y = z->p->p->left;
            if (y->c == RED) {
                z->p->c = BLACK;
                y->c = BLACK;
                z->p->p->c = RED;
                z = z->p->p;
            } else {
                if (z == z->p->left) {
                    z = z->p;
                    right_rotate(t, z);
                }
                z->p->c = BLACK;
                z->p->p->c = RED;
                left_rotate(t, z->p->p);
            }
        }
    }
    t->root->c = BLACK;
}


rbnode* rbsearch(rbtree* t, int key)
{
    assert(t);
    rbnode* x = t->root;
    while (x != t->nil ) {
        if( x->key == (void*) key)
            return x;
        else if((void*)key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    return NULL;
}


void rbprint(rbtree *T,int key){


    rbnode *r;
    r = rbsearch(T,key);
    if(r && r->row)
        fputs(r->row,stdout);
    else
        fputs(".\n",stdout);
}
/*
void visit(rbtree* T){
    rbnode *x;
    x = T->root;
    while(x != T->nil){
        printf("%s\n",x->row);
        printf("%d",x->key);
        visit(x->left);
        visit(x-)
    }
*/


void rbdelete(rbtree* t, rbnode* z)
{
    assert(t && z);
    rbnode* x = NULL;
    rbnode* y = z;
    enum color y_orig_c = y->c;
    if (z->left == t->nil) {
        x = z->right;
        rb_transplant(t, z, z->right);
    } else if (z->right == t->nil) {
        x = z->left;
        rb_transplant(t, z, z->left);
    } else {
        y = tree_minimum(t, z->right);
        y_orig_c = y->c;
        x = y->right;
        if (y->p == z)
            x->p = y;
        else {
            rb_transplant(t, y, y->right);
            y->right = z->right;
            y->right->p = y;
        }
        rb_transplant(t, z, y);
        y->left = z->left;
        y->left->p = y;
        y->c = z->c;
    }
    if (y_orig_c == BLACK)
        rb_delete_fixup(t, x);
}




void rb_delete_fixup(rbtree* t, rbnode* x)
{
    assert(t && x);
    while (x != t->root && x->c == BLACK) {
        if (x == x->p->left) {
            rbnode* w = x->p->right;
            if (w->c == RED) {
                w->c = BLACK;
                x->p->c = RED;
                left_rotate(t, x->p);
                w = x->p->right;
            }
            if (w->left->c == BLACK && w->right->c == BLACK) {
                w->c = RED;
                x = x->p;
            } else {
                if (w->right->c == BLACK) {
                    w->left->c = BLACK;
                    w->c = RED;
                    right_rotate(t, w);
                    w = x->p->right;
                }
                w->c = x->p->c;
                x->p->c = BLACK;
                w->right->c = BLACK;
                left_rotate(t, x->p);
                x = t->root;
            }
        } else {
            rbnode* w = x->p->left;
            if (w->c == RED) {
                w->c = BLACK;
                x->p->c = RED;
                right_rotate(t, x->p);
                w = x->p->left;
            }
            if (w->right->c == BLACK && w->left->c == BLACK) {
                w->c = RED;
                x = x->p;
            } else {
                if (w->left->c == BLACK) {
                    w->right->c = BLACK;
                    w->c = RED;
                    left_rotate(t, w);
                    w = x->p->left;
                }
                w->c = x->p->c;
                x->p->c = BLACK;
                w->left->c = BLACK;
                right_rotate(t, x->p);
                x = t->root;
            }
        }
    }
    x->c = BLACK;
}


void rb_transplant(rbtree* t, rbnode* u, rbnode* v)
{
    if (u->p == t->nil)
        t->root = v;
    else if (u == u->p->left)
        u->p->left = v;
    else
        u->p->right = v;
    v->p = u->p;
}


void left_rotate(rbtree* t, rbnode* x)
{
    assert(t && x && x->right);
    rbnode* y = x->right;
    x->right = y->left;
    if (y->left != t->nil)
        y->left->p = x;


    y->p = x->p;
    if (x->p == t->nil) {
        assert(t->root == x);
        t->root = y;
    } else if (x == x->p->left)
        x->p->left = y;
    else {
        assert(x == x->p->right);
        x->p->right = y;
    }


    y->left = x;
    x->p = y;
}


void right_rotate(rbtree* t, rbnode* x)
{
    assert(t && x && x->left);
    rbnode* y = x->left;
    x->left = y->right;
    if (y->right != t->nil)
        y->right->p = x;


    y->p = x->p;
    if (x->p == t->nil) {
        assert(t->root == x);
        t->root = y;
    } else if (x == x->p->left)
        x->p->left = y;
    else {
        assert(x == x->p->right);
        x->p->right = y;
    }


    y->right = x;
    x->p = y;
}


rbnode* tree_maximum(rbtree* t, rbnode* x)
{
    x = t->root;
    assert(t && x && x != t->nil);
    while (x->right != t->nil)
        x = x->right;
    return x;
}


rbnode* tree_minimum(rbtree* t, rbnode* x)
{
    assert(t && x && x != t->nil);
    while (x->left != t->nil)
        x = x->left;
    return x;
}


void rbsubstitute(rbtree *t,int key,char *newrow){


    rbnode *r;
    char *tmp;
    r = rbsearch(t,key);
    tmp = r->row;
    r->row = newrow;
    ///free(tmp);
    
}




void free_tree(rbtree *t,rbnode *h) {


    rbnode *x;
    x = h;
    if (x != t->nil) {
        free_tree(t, x->right);
        free(x->row); //if data was heap allocated, need to free it
        free_tree(t, x->left);
        free(x);
    }
}


void rb_fixkeys(rbtree *T,rbnode *x,int count){


    rbnode *s;
    s = rbsearch(T,x->key+1);
    x->key = x->key - count;
    if(s)
        rb_fixkeys(T,s,count);


}


void rb_fixtext(rbtree *T, rbnode *head, char *new_row){


    rbnode *ptr;
    rbnode *new_node;
    char *row;
    row = head->row;
    head->row = new_row;
    ptr = rbsearch(T,head->key+1);
    if(ptr)
        rb_fixtext(T,ptr,row);
    new_node = make_rbnode(T,BLACK,head->key+1,row);
    new_node = rbinsert(T,new_node);


}


rbnode *rb_successor(rbtree *t,rbnode *r) {


    rbnode *s;
    if (r->right != t->nil)
        return tree_minimum(t, r->right);
    s = r->p;
    while (s != t->nil && s->right == r) {
        r = s;
        s = s->p;
    }
    return s;
}




#define ROWSIZE 1025
#define LIMIT 1020
#define ACTION 50




/* Pass 5/5 Write Only, 5/5 Bulkreads */




void read_input();
int main() {
    /*printf("Hello, World!\n");*/
    read_input();
    return 0;
}




void read_input() {


    int index1, index2, empty,last,upper_bound;
    char command, point, *quit, *action, dummy, buffer[ROWSIZE];
    rbtree *T;
    int i, size, count, j;
    int out, repetition;
    rbnode *tmp, *max, *s;
    list_commands *done = NULL;
    list_commands *undone = NULL;
    list_commands *ptr;


    T = tree_create();
    assert(T);
 last = 0;
    action = (char *) malloc(sizeof(char) * ACTION);
    assert(fgets(action, ACTION, stdin));
    sscanf(action, "%d,%d%c", &index1, &index2, &command);
    /*fgets(action,ACTION,stdin);
    strtok(action, "\n");
    sscanf(action,"%d,%d%c",&index1,&index2,&command);*/
    while (1) {
        if (command == 'c') {
            /* save the rows in a RB tree */


            list_commands *operation;
            operation = create_op(index1, index2, command);
            for (j = 0, i = index1; i <= index2; i++, j++) {
                assert(fgets(buffer, ROWSIZE, stdin));
                size = strlen(buffer);
                char *row = (char *) malloc(size + 1);
                strcpy(row, buffer);
                ///operation->text[j].added = malloc(sizeof(size+1)); /// adding to the stack the rows
                operation->text[j].added = row; /// adding to the stack the rows
                tmp = rbsearch(T, i); /// we can limit the number of search later ///*/
                if (tmp) {
                    /// rbdelete(T, tmp); need to reorder the keys ///
                    ///operation->text[j].prev = malloc(strlen(tmp->row)+1);
                    operation->text[j].prev = tmp->row;
                    rbsubstitute(T, i, row);
                } else {
                    rbnode *row_elem = make_rbnode(T, BLACK, i, row);
                    operation->text[j].prev = NULL;
                    rbinsert(T, row_elem);
                }




            }
            if(last==1) 
                    free_list(&undone);
         
            push(&done, operation);
            last = 0;
            getchar();
        }


        if (command == 'p') {


            /* with the strategy i can search in the tree only one time but with O(n) cost instead of
             * doing void searches in the tree */
            /* max = T->root;
            max = tree_maximum(T, max);
            empty = index2 - max->key;
            if (empty > 0)
                upper_bound = max->key;
            upper_bound = index2; */
            for (i = index1; i <= index2; i++)
                rbprint(T, i);
            /* if (empty > 0)
                for (i = 0; i < empty; i++)
                    printf(".\n"); */
        }
        if (command == 'd') {


            list_commands *operation;
            operation = create_op(index1, index2, command);
            tmp = rbsearch(T, index2);
            if (tmp)
                s = rb_successor(T, tmp);
            count = index2 - index1 + 1;
            for (j = 0, i = index1; i <= index2; i++, j++) {
                tmp = rbsearch(T, i);
                if (tmp) {
                    rbdelete(T, tmp);
                    operation->text[j].prev = tmp->row;
                    operation->text[j].added = NULL;
                    /// free(tmp->row);
                    free(tmp);
                }
            }




            /// if the first command is a delete it will break the program, you should initialize s
            if (s != T->nil)
                rb_fixkeys(T, s, count);
                
         if(last == 1) 
                    free_list(&undone);
                  


            push(&done, operation);
            last = 0;
        }


        assert(fgets(action, ACTION, stdin));
        if (out = sscanf(action, "%c", &command) == 1 && command == 'q')
            break;
        else if (out = sscanf(action, "%d%c", &repetition, &command) == 2 && command == 'u') {
               last = 1;


                while (repetition > 0 && done) {


                    list_commands *operation;
                    operation = extract(&done);
                    if (operation) {


                        if (operation->c == 'c') {
                            for (j = 0, i = operation->index1; i <= operation->index2; i++, j++) {
                                tmp = rbsearch(T, i);
                                if (tmp)
                                    tmp->row = operation->text[j].prev; /// i don't know if i have to check when the research fails


                            }
                            push(&undone, operation);
                        }
                        if (operation->c == 'd') {


                            count = operation->index1 - operation->index2 + 1;
                            for (j = 0, i = operation->index1; i <= operation->index2; i++, j++) {
                                tmp = rbsearch(T, i);
                                /* if you find a node with this key it means that the deleted rows had a successor
                                so you have to do a reverse rbfixkeys */
                                if (tmp) {
                                    rb_fixtext(T,tmp,operation->text[j].prev);
                                    /// since i free the node from the tree (but i save the row element) i have
                                    /// to insert a new node in the tree
                                    /*rbnode *row_elem = make_rbnode(T, BLACK, i, operation->text[j].prev);
                                    rbinsert(T, row_elem);*/
                                } else {
                                    rbnode *row_elem = make_rbnode(T, BLACK, i, operation->text[j].prev);
                                    rbinsert(T, row_elem);
                                }
                            }
                            push(&undone, operation);


                        }
                        repetition--;
                    }




                }
        } else if (out = sscanf(action, "%d%c", &repetition, &command) == 2 && command == 'r') {


            if (undone)
                while (repetition > 0) {


                    list_commands *operation;
                    operation = extract(&undone);
                    if (operation) {


                        if (operation->c == 'c') {


                            for (j = 0, i = operation->index1; i <= operation->index2; i++, j++) {
                                tmp = rbsearch(T, i);
                                if (tmp)
                                    tmp->row = operation->text[j].added; /// i don't know if i have to check when the research fails


                            }
                            push(&done, operation);
                        }
                        if (operation->c == 'd') {


                            tmp = rbsearch(T, operation->index2);
                            if (tmp)
                                s = rb_successor(T, tmp);
                            count = operation->index2 - operation->index1 + 1;
                            for (i = operation->index1; i <= operation->index2; i++) {
                                tmp = rbsearch(T, i);
                                if (tmp) {
                                    rbdelete(T, tmp);
                                    /// free(tmp->row);
                                    free(tmp);
                                }
                            }
                            /// if the first command is a delete it will break the program, you should initialize s
                            if (s != T->nil)
                                rb_fixkeys(T, s, count);
                            push(&done, operation);
                        }
                    }
                    repetition--;
                }
        }
        else
            sscanf(action, "%d,%d%c", &index1, &index2, &command);


    }
    free_tree(T, T->root);
    free(T->nil);
    free(T);
    free(action);
    free_list(&undone);
    free_list(&done);


}

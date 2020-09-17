typedef struct rows{


    char *prev;
    char *added;
}rows;




typedef struct list_commands{


    char c;
    int index1,index2;
    rows *text;
    struct list_commands *next;
}list_commands;


list_commands *see_top(list_commands *head);
void free_list(list_commands **head);
void *push(list_commands **head,list_commands *node);
list_commands *create_op(int index1,int index2,char c);


list_commands *create_op(int index1,int index2,char c){


    list_commands *node;
    node = malloc(sizeof(list_commands));
    assert(node);
    node->text = malloc(sizeof(rows)*(index2 - index1 + 1));
    node->index1 = index1;
    node->index2 = index2;
    node->c = c;
    node->next = NULL;
    return node;
}


void *push(list_commands **head,list_commands *node){


    node->next = *head;
    *head = node;
}


list_commands *extract(list_commands **head){


    list_commands *element = NULL;
    if(!*head)
        return element;
    element = *head;
    *head = (*head)->next;
    return element;
}


void free_list(list_commands **head){


    list_commands *ptr;
    ptr = *head;
    while(ptr != NULL) {
        *head = (*head)->next;
        free(ptr->text);
        free(ptr);
        ptr = *head;
    }
}


list_commands *see_top(list_commands *head){


    list_commands *ptr = NULL;
    if(head) {
        ptr = head;
        return ptr;
    }
    return ptr;
}







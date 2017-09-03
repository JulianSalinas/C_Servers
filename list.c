#include "list.h"


Node * new_node(long content){

    Node * n = malloc(sizeof(Node));
    n->next = n->prev = NULL;
    n->content = content;
    return (long) n;

}

List * new_list(){

    List * l = malloc(sizeof(List));
    l->start = l->end = NULL;
    l->size = 0;
    return l;

}

void add(List * l, long content){

    Node * n = new_node(content);
    if(l->size == 0)
        l->start = l->end = n;
    else{
        l->end->next = n;
        n->prev = l->end;
        l->end = n;
    }
    l->size++;
}

void push(List * l, long content){

    Node * n = new_node(content);
    if(l->size == 0)
        l->start = l->end = n;
    else{
        n->next = l->start;
        n->next->prev = n;
        l->start = n;
    }
    l->size++;

}

long peek(List * l){

    if(l->size == 0)
        return 0;
    else{
        Node * node = l->start;
        return node->content;
    }

}

long pop_aux(List * l){

    Node * pop = l->start;
    if(l->size == 1)
        l->start = l->end = NULL;
    else{
        l->start = l->start->next;
        pop->next->prev = NULL;
        pop->next = NULL;
    }
    l->size--;
    return pop->content;

}

long pop(List * l){

    if(l->size == 0)
        return 0;
    else
        return pop_aux(l);

}

void print_list(List * l , char * format){

    Node * tmp = l->start;
    printf("[ ");
    if(tmp == NULL) printf("Ø");
    while(tmp != NULL){
        printf(format, tmp->content);
        tmp = tmp->next;
        if(tmp != NULL) printf(", ");
    }
    printf(" ]");


}











































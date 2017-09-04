#include <stdlib.h>
#include <stdio.h>
#include "list.h"

int main(int argc, char *argv[])
{

    //No borrar las pruebas porfa...

    void * a = "fewdwedwe";
    long b = 123456;
    int c = 2332;
    printf("%zu", sizeof(a));
    printf("%zu", sizeof(b));
    printf("%zu", sizeof(c));

    List * l1 = new_list();

    //Imprime conjunto vacio ya que no tiene nada
    printf("\nLista vacia: ");
    print_list(l1, printf);

    //Pruebas de la funcion add
    printf("\nPrueba de add (3 veces): ");
    add(l1, "1");
    add(l1, "2");
    add(l1, "3");
    print_list(l1, printf);

    //Pruebas de la funcion pop
    char * taken = pop(l1);
    printf("\nPrueba pop: %s", taken);
    printf("\nEstado actual: ");
    print_list(l1, printf);

    //Prueba de la funcion push
    char * n = "4";
    push(l1, n);
    printf("\nPrueba de push: %s ", n);
    printf("\nEstado actual: ");
    print_list(l1, printf);

    //Prueba de peek
    char * p = peek(l1);
    printf("\nPrueba peek: %s", p);
    printf("\nEstado actual: ");
    print_list(l1, printf);

    //Probando el pop hasta vaciar la lista
    for(int i=0; i<5; i++)
        pop(l1);
    printf("\nVaciado de la lista");
    printf("\nEstado actual: ");
    print_list(l1, printf);

    //Lista con listas
    printf("\nCreando lista con listas: ");
    List * l2 = new_list();
    add(l1, "Hola");
    add(l1, "Mundo");
    push(l2, l1);
    add(l2, l1);

    Node * tmp = l2->start;
    while(tmp != NULL){
        print_list(tmp->content, printf);
        tmp = tmp->next;
    }

    printf("\n");
    return 0;
}

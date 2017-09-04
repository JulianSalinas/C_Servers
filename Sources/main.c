#include <stdlib.h>
#include <stdio.h>
#include "../Headers/list.h"

int main(int argc, char *argv[])
{

    //No borrar las pruebas porfa...
    printf("Hello World!");

    //Se pueden agregar ints, longs, chars o punteros a la lista
    //sin embargo, al hacer pop se debe castear el resultado

    List * strlist = new_list();

    //Imprime conjunto vacio ya que no tiene nada
    printf("\nLista vacia: ");
    print_list(strlist, "%s");

    //Pruebas de la funcion add
    //No es necesario castear a long
    printf("\nPrueba de add: ");
    add(strlist, "1");
    add(strlist, "20");
    add(strlist, "3");
    print_list(strlist, "%s");

    //Pruebas de la funcion pop
    char * taken = pop(strlist);
    printf("\nPrueba pop: %s", taken);
    printf("\nEstado actual: ");
    print_list(strlist, "%s");

    //Prueba de la funcion push
    char * n = "4";
    push(strlist, n);
    printf("\nPrueba de push: %s ", n);
    printf("\nEstado actual: ");
    print_list(strlist, "%s");

    //Prueba de peek
    char * p = peek(strlist);
    printf("\nPrueba peek: %s", p);
    printf("\nEstado actual: ");
    print_list(strlist, "%s");

    //Probando el pop hasta vaciar la lista
    //Se asegura de no caerse si la lista ya esta vacia
    for(int i=0; i<5; i++)
        pop(strlist);
    printf("\nVaciado de la lista");
    printf("\nEstado actual: ");
    print_list(strlist, "%s");


    printf("\n");
    return 0;
}

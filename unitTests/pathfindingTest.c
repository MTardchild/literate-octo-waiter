//
// Created by tscharntke on 15.12.2017.
//


#include <stdio.h>

#include "pathfinding.h"


void print_path() {
    int i = 0;
    char c;
    while (c = pf_path[i++]) {
        if (c == n)
            printf("n, ");
        else if (c == s)
            printf("s, ");
        else if (c == w)
            printf("w, ");
        else if (c == e)
            printf("e, ");
    }
    printf("\n");

}

int main(){
    parse();
    find_path(10, 7, start);
    print_path();
    find_path(1, 1, table_3);
    print_path();
    find_path(1, 6, table_3);
    print_path();
    find_path(10, 7, table_2);
    print_path();
    find_path(1, 6, table_1);
    print_path();
    find_path(1, 6, table_2);

    print_path();


}


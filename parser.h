//
// Created by tscharntke on 18.12.2017.
//

#ifndef OCTA_PARSER_H
#define OCTA_PARSER_H



/*
* Takes a_map and parses it to mu_parsed_map 
*/
void parse();



void parse() {
    byte i;
    for(i=0; i<ARENA_SIZE; i++){
        switch (a_map[i]) {
            case '#':
                mu_parsed_map[i] = border;
                break;
            case ' ':
                mu_parsed_map[i] = empty;
                break;
            case 'S':
                mu_parsed_map[i] = starting_square;
                break;
            case '1':
                mu_parsed_map[i] = table_1;
                break;
            case '2':
                mu_parsed_map[i] = table_2;
                break;
            case '3':
                mu_parsed_map[i] = table_3;
                break;
        }
    }
}

#endif //OCTA_PARSER_H

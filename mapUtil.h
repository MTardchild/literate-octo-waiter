//
// Created by tscharntke on 18.12.2017.
//

#ifndef OCTA_MAP_UTIL_H
#define OCTA_MAP_UTIL_H

#include "arenas.h"
#include "octaUtil.h"

#define direction_t byte
#define no_dir 0

#define n 1
#define e 2
#define s 3
#define w 4

#define STARTING_DIRECTION s

#define starting_square 4
#define border 8
#define empty 0
#define table_1 1
#define table_2 2
#define table_3 3

byte adjust_row(byte row, direction_t dir);
byte adjust_col(byte col, direction_t dir);
bool is_border(byte row, byte col, direction_t dir);
bool visited(byte row, byte col, direction_t dir);
bool is_table(byte row, byte col, direction_t dir);
byte next_to_goal(byte row, byte col, byte goal);

#define opposite_dir(dir) (((((dir) + 1) % 4) + 1))
#define map_val(row, col) mu_parsed_map[(col) + (row) * 14]


#define get_row(num) (((num) & 0xf0) >> 4)
//#define get_row get_top_4

#define get_col(num) ((num) & 0x0f)
//#define get_col get_bot_4

#define get_dir(num) (((num) & 0xe0) >> 5)
//#define get_dir get_top_3

#define get_turns(num) ((num) & 0x1f)
//#define get_turns get_bot_5

#define set_row(num, val) (((num) & 0x0f) | ((val) << 4))
//#define set_row set_top_4

#define set_col(num, val) (((num) & 0xf0) | (val))
//#define set_col set_bot_4

#define set_dir(num, val) (((num) & 0x1f) | ((val) << 5))
//#define set_dir set_top_3

#define set_turns(num, val) (((num) & 0xe0) | (val))
//#define set_turns set_bot_5


byte mu_parsed_map[ARENA_SIZE] = {0};


byte adjust_row(byte row, direction_t dir){
    if (dir == n)
        return row - 1;
    if (dir == s)
        return row + 1;
    return row;
}


byte adjust_col(byte col, direction_t dir){
    if (dir == w)
        return col - 1;
    if (dir == e)
        return col + 1;
    return col;
}


bool is_border(byte row, byte col, direction_t dir){
    row = adjust_row(row, dir);
    col = adjust_col(col, dir);
    if (row == 0 || row == 13 || col == 0 || col == 13)
        return true;
    return false;
}


bool visited(byte row, byte col, direction_t dir){
    row = adjust_row(row, dir);
    col = adjust_col(col, dir);
    if (map_val(row, col) != 0)
        return true;
    return false;
}


bool is_table(byte row, byte col, direction_t dir){
    row = adjust_row(row, dir);
    col = adjust_col(col, dir);
    byte val = map_val(row, col);
    if (val == table_1 || val == table_2 || val == table_3)
        return true;
    return false;
}

byte next_to_goal(byte row, byte col, byte goal){
    direction_t dir;
    for (dir = n; dir <=w; dir++){
        byte r = adjust_row(row, dir);
        byte c = adjust_col(col, dir);
        if (map_val (r, c) == goal)
            return dir;
    }
    return no_dir;
}





#endif //OCTA_MAP_UTIL_H

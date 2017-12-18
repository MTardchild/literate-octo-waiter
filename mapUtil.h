//
// Created by tscharntke on 18.12.2017.
//

#ifndef OCTA_MAP_UTIL_H
#define OCTA_MAP_UTIL_H

#include "arenas.h"
#include "octa_util.h"

#define direction_t byte
#define no_dir 0

#define n 1
#define e 2
#define s 3
#define w 4

#define STARTING_DIRECTION s

#define start 4
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


#define get_top_4(num) (((num) & 0xf0) >> 4)
#define get_row get_top_4

#define get_bot_4(num) ((num) & 0x0f)
#define get_col get_bot_4

#define get_top_3(num) (((num) & 0xe0) >> 5)
#define get_dir get_top_3

#define get_bot_5(num) ((num) & 0x1f)
#define get_turns get_bot_5

#define set_top_4(num, val) (((num) & 0x0f) | ((val) << 4))
#define set_row set_top_4

#define set_bot_4(num, val) (((num) & 0xf0) | (val))
#define set_col set_bot_4

#define set_top_3(num, val) (((num) & 0x1f) | ((val) << 5))
#define set_dir set_top_3

#define set_bot_5(num, val) (((num) & 0xe0) | (val))
#define set_turns set_bot_5


byte mu_parsed_map[ARENA_SIZE];




#endif //OCTA_MAP_UTIL_H

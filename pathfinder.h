//
// Created by tscharntke on 18.12.2017.
//

#ifndef OCTA_PATHFINDING_H
#define OCTA_PATHFINDING_H

#include "mapUtil.h"
#include "parser.h"

#define start_of_search 5

#define QUEUE_SIZE 36
#define MAX_PATH_SIZE 100


/*
 * Takes mu_parsed_map and finds the shortest path to goal with
 * the lowest number of turns necessary.
 * Start is at row, col facing STARTING_DIRECTION
 *
 * The resulting path is written to pf_path.
 */
void find_path(byte row, byte col, byte goal);


//private implementation:


byte pf_path[MAX_PATH_SIZE];


void trace_back_path(byte row, byte col, byte prev_dir) {
    direction_t dir;
    byte val, i = 0;
    for (i = 0; i<MAX_PATH_SIZE; i++)
        pf_path[i] = 0;
    i = 0;
    while (true){
        val = map_val(row, col);
        if (get_dir(val) == start_of_search) {
            if (prev_dir != STARTING_DIRECTION)
                pf_path[i] = prev_dir;
            break;
        }
        dir = opposite_dir(get_dir(val));
        if (prev_dir != dir)
            pf_path[i++] = prev_dir;
        pf_path[i++] = dir;
        prev_dir = dir;
        row = adjust_row(row, opposite_dir(dir));
        col = adjust_col(col, opposite_dir(dir));
    }
}

void turn_path(){
    byte i, tail, head;
    for (i = 0; i<MAX_PATH_SIZE; i++) {
        if (pf_path[i] == 0)
            break;
    }
    if (i == 0)
        return;
    tail = i - 1;
    head = 0;
    while (tail > head) {
        byte temp = pf_path[head];
        pf_path[head++] = pf_path[tail];
        pf_path[tail--] = temp;
    }
}


void goal_found(byte row, byte col, direction_t dir){
    trace_back_path(row, col, dir);
    turn_path();
    parse();
}


void less_turns(byte r, byte c, byte turns, direction_t prev_dir, direction_t dir) {
    byte prev_turns = get_turns(map_val(r, c));
    byte val;
    if (prev_dir == dir && turns < prev_turns){
        val = set_dir(turns, opposite_dir(dir));
        map_val(r, c) = val;
    }else if (turns + 1 < prev_turns) {
        val = set_dir(turns + 1, opposite_dir(dir));
        map_val(r, c) = val;
    }
}


void b_first_search(byte row, byte col, byte goal) {
	byte queue[QUEUE_SIZE];
	byte i;
	for (i=0;i<QUEUE_SIZE;i++)
		queue[i] = 0;
    byte head, tail;
    queue[0] = set_row(col, row);
    head = 0;
    tail = 1;
	byte test;

    map_val(row, col) = set_dir(map_val(row, col), start_of_search);
	
    while (head != tail) {
		row = get_row(queue[head]);
		col = get_col(queue[head]);
        
		head = (head + 1) % QUEUE_SIZE;
	
        direction_t dir_to_goal = next_to_goal(row, col, goal);

        if (dir_to_goal) {
            goal_found(row, col, dir_to_goal);
            break;
        }
        direction_t prev_dir = opposite_dir(get_dir(map_val(row, col)));
        byte turns = get_turns(map_val(row, col));

        direction_t dir;
		
        for (dir = n; dir<=w; dir++) {
			
            byte r = adjust_row(row, dir);
            byte c = adjust_col(col, dir);
            byte val = map_val(r, c);
			
            if (val == empty){
				
                turns = turns + ((dir != prev_dir) ? 1 : 0);
                map_val(r, c) = set_dir(turns, opposite_dir(dir));
                queue[tail] = set_row(c, r);
                tail = (tail + 1) % QUEUE_SIZE;
            }else if (get_dir(val) != empty && get_dir(val) != start_of_search) {
				
                less_turns(r, c, turns, prev_dir, dir);
            }
			
        }
    }
	
} 

void find_path(byte row, byte col, byte goal){
	
    if (row < 14 && row >= 0 && col < 14 && col >= 0)
        b_first_search(row, col, goal);
    //else
    //    exit(1);
}


#endif //OCTA_PATHFINDING_H

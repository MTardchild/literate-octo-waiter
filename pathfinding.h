//
// Created by tscharntke on 18.12.2017.
//

#ifndef OCTA_PATHFINDING_H
#define OCTA_PATHFINDING_H

#include "map_util.h"
#include "parser.h"

#define start_of_search 5

#define QUEUE_SIZE 36
#define MAX_PATH_SIZE 100

byte pf_path[MAX_PATH_SIZE];

/*
 * Takes mu_parsed_map and finds the shortest path to goal with
 * the lowest number of turns necessary.
 * Start is at row, col facing STARTING_DIRECTION
 *
 * The resulting path is written to pf_path.
 */
void find_path(byte row, byte col, byte goal);


#endif //OCTA_PATHFINDING_H

#ifndef MINIMAX_H
#define MINIMAX_H

#include "gamemap.h"
#include "math.h"

#define MAX_DEPTH 6
#define HEURISTICS_TAB_SIZE

class minimax
{
public:
    minimax(int max_depth);
    ~minimax();
    int play();

private:
    typedef int heuristics_t;
    typedef int col_idx_t;
    gamemap current_game;

    heuristics_t get_heuristics(state_t player = FREE);
    int iterative_move_search(int depth, state_t player);
    col_idx_t decide_move();

    const int max_depth;
    col_idx_t best_move_idx;

//    move_descriptor_t move_descriptor_tab[7];

    bool if_X_move_possible[7];
    bool if_O_move_possible[7];

//    int current_depth;
//    move_descriptor_t *find_minimum_move(bool parity);
//    move_descriptor_t *find_maximum_move();

//    typedef struct {
//        heuristics_t heuristics;
//        bool        if_move_possible;
//    } move_descriptor_t;
};

#endif // MINIMAX_H

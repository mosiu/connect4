#ifndef MINIMAX_H
#define MINIMAX_H

#include "gamemap.h"
#include "math.h"

#define MAX_DEPTH 6
#define HEURISTICS_TAB_SIZE

class minimax
{
public:
    minimax();
    ~minimax();
    int play();

private:
    typedef int heuristics_t;
    gamemap future_game[MAX_DEPTH];
    heuristics_t predict(int* move_sequence);
    int current_move_sequence[MAX_DEPTH];
    int current_depth;  // current_depth % 2 means negative heuristics sign...? (oponent move)

};

#endif // MINIMAX_H

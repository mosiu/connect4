#ifndef FIELD_H
#define FIELD_H

#include "direction.h"
#include <iostream>
using namespace std;

typedef enum {
    FREE = 0,
    O_PLAYER = 1,
    X_PLAYER = 2
} state_t;

typedef enum {
    CONTINUE,
    O_PLAYER_WINS,
    X_PLAYER_WINS,
    DRAW
} game_result_t;

class field
{
public:
    field();
    ~field();

    void set_player(state_t player);
    void set_position(int row, int col)
    {
        row_idx = row;
        col_idx = col;
    }
    int get_row()
    {
        return row_idx;
    }
    int get_col()
    {
        return col_idx;
    }
    bool check_win();

    state_t state;

    direction direction_tab[4]/* = {
        direction(VERTICAL),
        direction(HORIZONTAL),
        direction(DIAGONAL_PLUS),
        direction(DIAGONAL_MINUS)
    }*/;


private:
    int row_idx;
    int col_idx;


};


ostream &
operator<<(ostream & display_stream, field & to_display);

#endif // FIELD_H
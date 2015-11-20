#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "field.h"  // musi znac enum z tego pliku



class gamemap
{
public:
    gamemap();
    int wait_for_move(int column_input = 0);
    void set_first_player(char player)
    {
        (player == 'o') ? (current_player = O_PLAYER) : (current_player = X_PLAYER);
    }
    ~gamemap();

    gamemap& copy_gamestate(const gamemap& arg);

    int move_counter;

    void reprint();

    state_t current_player;
private:

    int get_heuristics();

    int get_player_move();
    int update_field_counters(direction_t dir, field* center_field);

    void switch_current_player();


    void put_mark(int column);

    inline field* get_current_field_ptr(int col_idx);
    inline int get_row_idx(int col_idx);

    field field_tab[6][7];

    field* current_field;

    bool column_full[7];
    int rows_occupied[7];


    bool win_lock;
};


#endif // GAMEMAP_H

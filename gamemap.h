#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "field.h"  // musi znac enum z tego pliku



class gamemap
{
public:
    gamemap();
    int move(int column_input = -1, state_t player = FREE);
    void retract_move();
    void set_player(char player)
    {
        (player == 'o') ? (current_player = O_PLAYER) : (current_player = X_PLAYER);
    }
    ~gamemap();

    gamemap& copy_gamestate(const gamemap& arg);

    int move_counter;

    state_t current_player;

    bool column_full[7];

    void reprint();


private:

    int get_single_heuristics();

    int read_player_move();
    int analyze_direction(direction_t dir, field* center_field);

    void switch_player();

    void set_current_field_ptr_and_save_previous(field *arg);

    void put_mark(int column);

    inline field* get_ptr_to_field_on_top_of_column(int col_idx);
    inline int get_row_idx(int col_idx);
    inline void set_current_field_ptr_and_save_previous();
    inline field* read_last_field();
    field field_tab[6][7];

    field* current_field;

    field* field_history[42];

    int rows_occupied[7];


    bool win_lock;
};


#endif // GAMEMAP_H

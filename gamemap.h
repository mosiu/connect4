#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "field.h"  // musi znac enum z tego pliku



class gamemap
{
public:
    gamemap();
    game_result_t wait_for_move();
    int predict_move(int column_input);
    ~gamemap();

    state_t current_player;
private:
    void set_first_player(char player)
    {
        (player == 'o') ? (current_player = O_PLAYER) : (current_player = X_PLAYER);
    }

    int update_field_counters(direction_t dir, field* center_field);

    void switch_current_player();
    void switch_predict_player();

    void predict_reset();

    void put_mark(int column);

    inline field* get_current_field_ptr(int col_idx);
    inline field* get_predict_field_ptr(int col_idx);
    inline int get_row_idx(int col_idx);
    inline int get_predict_row_idx(int col_idx);


    void reprint();

    field field_tab[6][7];
    field predict_tab[6][7];


    state_t predict_player;

    field* current_field;
    field* predict_field;

    bool column_full[7];
    bool predict_column_full[7];

    int rows_occupied[7];
    int predict_rows_occupied[7];

    int move_counter;
    int predict_move_counter;
};


#endif // GAMEMAP_H

#include "gamemap.h"

#include <iostream>
using namespace std;

#define MAX_COL_HEIGHT  6
#define MAP_CAPACITY    42

gamemap::gamemap()
{

    move_counter = 0;
    predict_move_counter = 0;
    for (int col_idx=0; col_idx<7; col_idx++)
    {
        rows_occupied[col_idx] = 0;
        predict_rows_occupied[col_idx] = 0;
        column_full[col_idx] = false;
        predict_column_full[col_idx] = false;
    }

    for(int row_idx = 0; row_idx < 6; row_idx++)
    {
        for (int col_idx = 0; col_idx < 7; col_idx++)
        {
            field_tab[row_idx][col_idx].set_position(row_idx, col_idx);
            predict_tab[row_idx][col_idx].set_position(row_idx, col_idx);
        }
    }

    char player;
    cout << "Which player shall begin? (o/x)" << endl;
    cin >> player;
    set_first_player(player);
    reprint();
}

// returns false if map full
game_result_t
gamemap::wait_for_move()
{
    if (move_counter >= MAP_CAPACITY)
        return DRAW;   // full map = no move

    if(current_player == O_PLAYER)
    {
        cout << 'O';
    }
    else
    {
        cout << 'X';
    }
    cout << ": Enter column number:";

    int column_input;

    // keep reading input until correct number occurs
    do
    {
        cin >> column_input;
    }
    while((((column_input >= 1) && (column_input <= 7)) == false)   // input invalid
        ||  // or
        (column_full[column_input-1]));  // column full

    // GRUPA PRZYDATNYCH DLA ALGORYTMU
    put_mark(column_input-1);
    move_counter++;

    update_field_counters(VERTICAL, current_field);    // necessary before checking win
    update_field_counters(HORIZONTAL, current_field);
    update_field_counters(DIAGONAL_PLUS, current_field);
    update_field_counters(DIAGONAL_MINUS, current_field);

    if ( (current_field->check_win() == true) && (current_player == O_PLAYER) )
        return O_PLAYER_WINS;

    if ( (current_field->check_win() == true) && (current_player == X_PLAYER) )
        return X_PLAYER_WINS;

    switch_current_player();

    reprint();

    return CONTINUE;
}

// travel around current_field and update counters, then return current field heuristics
int
gamemap::update_field_counters(direction_t dir, field* center_field)
{
    int interval;
    switch (dir)
    {
    case VERTICAL:
        interval = -7;
        break;
    case HORIZONTAL:
        interval = 1;
        break;
    case DIAGONAL_PLUS:
        interval = -6;
        break;
    case DIAGONAL_MINUS:
        interval = 8;
        break;
    }
    int my_disc_counter = 1;
    int my_row_counter = 1;
    int free_counter = 0;
    int width_counter = 1;
    field* row_beginning_pointer = NULL;
    bool row_flag;

    // reset pointer
    field* pointer = center_field;
    row_flag = true;
    // GO RIGHT/straight UP
    for (int i=0; i<7; i++)
    {
        // czy moge isc w prawo?
        switch (dir)
        {
        case VERTICAL:
            if(pointer->get_row() == 0)
            goto gtfo_1;
            break;
        case HORIZONTAL:
            if(pointer->get_col() == 6)
                goto gtfo_1;
            break;
        case DIAGONAL_PLUS:
            if((pointer->get_col() == 6) || (pointer->get_row() == 0))
                goto gtfo_1;
            break;
        case DIAGONAL_MINUS:
            if((pointer->get_col() == 6) || (pointer->get_row() == 5))
                goto gtfo_1;
            break;
        }

        // ide w prawo
        pointer += interval;
        width_counter ++;

        if(pointer->state == FREE)
        {
            free_counter++;
            row_flag = false;
        }
        else if (pointer->state == current_player)
        {
            my_disc_counter++;
            if(row_flag)
                my_row_counter++;
        }
        else
        {
            // jednak nie moge isc w prawo
            width_counter --;
            pointer -= interval;
            break;
        }
    }
gtfo_1:
    // reset pointer
    pointer = center_field;
    row_flag = true;
    // GO LEFT/straight DOWN
    for (int i=0; i<7; i++)
    {
        // czy moge isc w lewo?
        switch (dir)
        {
        case VERTICAL:
            if(pointer->get_row() == 5)
                goto gtfo_2;
            break;
        case HORIZONTAL:
            if(pointer->get_col() == 0)
                goto gtfo_2;
            break;
        case DIAGONAL_PLUS:
            if ((pointer->get_col() == 0) || (pointer->get_row() == 5))
                goto gtfo_2;
            break;
        case DIAGONAL_MINUS:
            if ((pointer->get_col() == 0) || (pointer->get_row() == 0))
                goto gtfo_2;
            break;
        }

        // ide w lewo
        pointer -= interval;
        width_counter ++;

        if(pointer->state == FREE)
        {
            free_counter++;
            row_flag = false;
            row_beginning_pointer = pointer + interval;
        }
        else if (pointer->state == current_player)
        {
            my_disc_counter++;
            if(row_flag)
                my_row_counter++;
        }
        else
        {
            // jednak nie moge isc w lewo
            width_counter --;
            pointer += interval;
            break;
        }
    }
gtfo_2:
    // UPDATE FIELDS
    //row_flag = false;    // don't do it, cuz if the flag remained true so far, row beginning pointer is unset, and still the || in if(...) below returns true. That's good.
    for (int i=0; i<width_counter; i++)
    {
        if( (pointer+i*interval)->state == current_player )
        {
            (pointer+i*interval)->direction_tab[dir].free_counter = free_counter;
            (pointer+i*interval)->direction_tab[dir].my_disc_counter = my_disc_counter;
            (pointer+i*interval)->direction_tab[dir].width_counter = width_counter;
            if (((pointer+i*interval) == row_beginning_pointer) || row_flag)
            {
                //row_beginning_flag = true; // not necessary (updating fields separated by free space with row counter value
                // from the current update does not influence the game)
                row_flag = true;    // set on first entry
                (pointer+i*interval)->direction_tab[dir].my_row_counter = my_row_counter;
            }
        }
    }
}

// returns single move heuristics (to be added to general)
int
gamemap::predict_move(int column_input)  // CAUTION! Don't input full columns!
{
    int single_heuristics_value;

    int draw_return_value;
    int win_value;
    if(predict_player == X_PLAYER)
    {
        draw_return_value = 10;
        win_value = 100;
    }
    else
    {
        draw_return_value = -10;
        win_value = -100;
    }
    if (predict_move_counter >= MAP_CAPACITY)
        return draw_return_value;   // full map = no move


    // GRUPA PRZYDATNYCH DLA ALGORYTMU
    put_mark(column_input-1);
    predict_move_counter++;

    update_field_counters(VERTICAL, predict_field);    // necessary before checking win
    update_field_counters(HORIZONTAL, predict_field);
    update_field_counters(DIAGONAL_PLUS, predict_field);
    update_field_counters(DIAGONAL_MINUS, predict_field);

    if ( (predict_field->check_win() == true))
        single_heuristics_value += win_value;

    switch_predict_player();

    reprint();

    return single_heuristics_value;
}

void
gamemap::predict_reset()
{
    for(int row_idx = 0; row_idx < 6; row_idx++)
    {
        for (int col_idx = 0; col_idx < 7; col_idx++)
        {
            predict_tab[row_idx][col_idx] = field_tab[row_idx][col_idx];
        }
    }
    predict_player = current_player;
    predict_field = current_field;

    for (int col_idx=0; col_idx<7; col_idx++)
    {
        predict_rows_occupied[col_idx] = rows_occupied[col_idx];
        predict_column_full[col_idx] = column_full[col_idx];
    }
    predict_move_counter = move_counter;
}

// determines and returns the current field (in reference form)
void
gamemap::put_mark(int col_idx)
{

    current_field = get_current_field_ptr(col_idx);

    current_field -> set_player(current_player);

    rows_occupied[col_idx] ++;

    // marking columns as full
    if (rows_occupied[col_idx] >= MAX_COL_HEIGHT)
        column_full[col_idx] = true;

}

void
gamemap::reprint()
{
    cout << "\n- 1  2  3  4  5  6  7 -\n";

    for(int row_idx = 0; row_idx < 6; row_idx++)
    {
        cout << '|';

        for (int col_idx = 0; col_idx < 7; col_idx++)
        {
            cout << field_tab[row_idx][col_idx];
        }
        cout << "|\n";
    }
}

void
gamemap::switch_current_player()
{
    if(current_player == O_PLAYER)
    {
        current_player = X_PLAYER;
    }
    else
    {
        current_player = O_PLAYER;
    }
}

void
gamemap::switch_predict_player()
{
    if(predict_player == O_PLAYER)
    {
        predict_player = X_PLAYER;
    }
    else
    {
        predict_player = O_PLAYER;
    }
}

inline
field*
gamemap::get_current_field_ptr(int col_idx)
{
    return (&field_tab[get_row_idx(col_idx)][col_idx]);
}

inline
int
gamemap::get_row_idx(int col_idx)
{
    return (MAX_COL_HEIGHT-1)-rows_occupied[col_idx];
}


inline
field*
gamemap::get_predict_field_ptr(int col_idx)
{
    return (&predict_tab[get_predict_row_idx(col_idx)][col_idx]);
}

inline
int
gamemap::get_predict_row_idx(int col_idx)
{
    return (MAX_COL_HEIGHT-1)-predict_rows_occupied[col_idx];
}

gamemap::~gamemap()
{

}


/*

    for (int row_idx=0; row_idx<MAX_COL_HEIGHT; row_idx++)
    {
        for (int col_idx=0; col_idx<7; col_idx++)
        {
            state_tab[row_idx][col_idx] = FREE;
        }
    }
    */

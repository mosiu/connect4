#include "gamemap.h"

#include <iostream>
#include "math.h"

using namespace std;

#define MAX_COL_HEIGHT  6
#define MAP_CAPACITY    42

gamemap::gamemap()
{
    move_counter = 0;
    // fist player by default
    current_player = O_PLAYER;
    for (int col_idx=0; col_idx<7; col_idx++)
    {
        rows_occupied[col_idx] = 0;
        column_full[col_idx] = false;
    }

    for(int row_idx = 0; row_idx < 6; row_idx++)
    {
        for (int col_idx = 0; col_idx < 7; col_idx++)
        {
            field_tab[row_idx][col_idx].set_position(row_idx, col_idx);
        }
    }
}

// returns heuristics
int
gamemap::wait_for_move(int column_input)
{
    if (!column_input)
    {
        column_input = get_player_move();
    }

    put_mark(column_input-1);
    move_counter++;

    int result = get_heuristics();

    switch_current_player();

    reprint();

    return result;
}

int
gamemap::get_heuristics()
{

    int move_heuristics = 0;
    int vertical_component = 0;
    int horizontal_component = 0;
    int diagonal_plus_component = 0;
    int diagonal_minus_component = 0;
    win_lock = false;   // prevents multiple win score addition in different directions
    vertical_component = update_field_counters(VERTICAL, current_field);    // necessary before checking win
    horizontal_component = update_field_counters(HORIZONTAL, current_field);
    diagonal_plus_component = update_field_counters(DIAGONAL_PLUS, current_field);
    diagonal_minus_component = update_field_counters(DIAGONAL_MINUS, current_field);

    if (current_player == X_PLAYER)
    {
        move_heuristics = vertical_component + horizontal_component
                + diagonal_minus_component + diagonal_plus_component;
    }
    else
    {
        move_heuristics = -vertical_component - horizontal_component
                - diagonal_minus_component - diagonal_plus_component;
    }
    return move_heuristics;
}

int
gamemap::get_player_move()
{
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
    return column_input;
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

    int width_component = 0;
    int row_component = 0;

    if(width_counter >= 4)  // score for row makes sense only if there's enough space for win!
    {
        row_component       = 8 + pow(8,my_row_counter);  // [16, 520]
        width_component     = pow(2, width_counter);  // [16, 128]
    }

    int win_component = 0;
    if ((my_row_counter >= 4) && !win_lock)
    {
        win_component = 10000;
        win_lock = true;    // thou shall win in only one direction
    }

    int heuristics = width_component + row_component + win_component;
    return heuristics;

    //row_flag = false;    // don't do it, cuz if the flag remained true so far, row beginning pointer is unset, and still the || in if(...) below returns true. That's good.
//    for (int i=0; i<width_counter; i++)
//    {
//        if( (pointer+i*interval)->state == current_player )
//        {
//            (pointer+i*interval)->direction_tab[dir].free_counter = free_counter;
//            (pointer+i*interval)->direction_tab[dir].my_disc_counter = my_disc_counter;
//            (pointer+i*interval)->direction_tab[dir].width_counter = width_counter;
//            if (((pointer+i*interval) == row_beginning_pointer) || row_flag)
//            {
//                //row_beginning_flag = true; // not necessary (updating fields separated by free space with row counter value
//                // from the current update does not influence the game)
//                row_flag = true;    // set on first entry
//                (pointer+i*interval)->direction_tab[dir].my_row_counter = my_row_counter;
//            }
//        }
//    }
}
/*
// returns single move heuristics (to be added to general)
int
gamemap::predict_move(int column_input)  // CAUTION! Don't input full columns!
{
    int single_heuristics_value;

    int draw_return_value;
    int win_value;
    if(current_player == X_PLAYER)
    {
        draw_return_value = 10;
        win_value = 100;
    }
    else
    {
        draw_return_value = -10;
        win_value = -100;
    }
    if (move_counter >= MAP_CAPACITY)
        return draw_return_value;   // full map = no move


    // GRUPA PRZYDATNYCH DLA ALGORYTMU
    put_mark(column_input-1);
    move_counter++;

    update_field_counters(VERTICAL, current_field);    // necessary before checking win
    update_field_counters(HORIZONTAL, current_field);
    update_field_counters(DIAGONAL_PLUS, current_field);
    update_field_counters(DIAGONAL_MINUS, current_field);

    if ( (current_field->check_win() == true))
        single_heuristics_value += win_value;

    switch_current_player();

    reprint();

    return single_heuristics_value;
}
*/
gamemap&
gamemap::copy_gamestate(const gamemap& arg)
{
    for(int row_idx = 0; row_idx < 6; row_idx++)
    {
        for (int col_idx = 0; col_idx < 7; col_idx++)
        {
            this->field_tab[row_idx][col_idx] = arg.field_tab[row_idx][col_idx];
        }
    }
    this->current_player = arg.current_player;
    this->current_field = arg.current_field;

    for (int col_idx=0; col_idx<7; col_idx++)
    {
        this->rows_occupied[col_idx] = arg.rows_occupied[col_idx];
        this->column_full[col_idx] = arg.column_full[col_idx];
    }
    this->move_counter = arg.move_counter;
    return *this;
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

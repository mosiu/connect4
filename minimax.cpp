#include "minimax.h"
#include "math.h"

#define O_PLAYER_TAB_IDX 0
#define X_PLAYER_TAB_IDX 1

minimax::minimax(int depth) : max_depth(depth)
{
    for (int col_idx = 0; col_idx < 7; col_idx ++)
    {
//        for(int depth_idx = 0; depth_idx < max_depth - 1; depth_idx ++)
//        {
//            move_descriptor_tab[depth_idx][col_idx].col_idx = col_idx; // TODO:: sth wrong here...?
//            move_descriptor_tab[depth_idx][col_idx].heuristics = 0;
//            move_descriptor_tab[depth_idx][col_idx].if_move_possible = false;
//        }
        if_X_move_possible[col_idx] = true;
    }
}

int
minimax::play()
{
    char ans;

    do
    {
        char player;
        cout << "Which player shall begin? (o/x)" << endl;
        cin >> player;

        gamemap current_game;

        current_game.set_first_player(player);
        current_game.reprint();


        while(true)
        {
            int single_move_heuristics;

            if(current_game.current_player == X_PLAYER)
            {
                single_move_heuristics = current_game.move(decide_move(), X_PLAYER);
                current_game.reprint();
            }
            else if (current_game.current_player == O_PLAYER)
            {
                single_move_heuristics = current_game.move();
            }
            else
            {
                break;
            }

            // if somebody won
            if (single_move_heuristics <= -100000)
            {
                cout << "\n --- You win, O_PLAYER, you clever bastard! --- \n\n\n";
                break;
            }
            else if (single_move_heuristics >= 100000)
            {
                cout << "\n --- You win, X_PLAYER, you clever bastard! --- \n\n\n";
                break;
            }

            // if nobody won
            if (current_game.move_counter >= 42)
            {
                cout << "\n -------- I T ' S   A   D R A W! -------- \n\n\n";
                break;
            }
        }
        cout << "Next game? (y/n):";
        cin >> ans;

    } while (ans == 'y');

    return 0;   // status: success
}

minimax::col_idx_t
minimax::decide_move()
{
    int best_move_heuristics = -0x7FFFFFFF;
    int best_move_idx;
    for(int move_idx=0; move_idx<7; move_idx++)
    {
        int val = iterative_move_search(max_depth - 1, X_PLAYER);
        if(val > best_move_heuristics)
        {
            best_move_heuristics = val;
            best_move_idx = move_idx;
        }
    }
    return best_move_idx;
}

/*minimax::col_idx_t
minimax::find_best_move()
{
    // detect first possible move
    int col_idx;
    for(col_idx = 0; col_idx < 7; col_idx ++)
    {
        if ( !current_game.column_full[col_idx] )
        {
            break;
        }
    }

    // now we have an idx of first free column
    current.move(col_idx);
    current_depth ++;   // first entry behind us
    if (current_depth <= max_depth)
        find_best_move();     // execute second entry

    // now depth almost maximum


//    move_descriptor_t some_move_tab[7];

//    for(int move_idx = 0; move_idx < 7; move_idx ++)
//    {
//        get_heuristics();   // now the tab is prepared! NiahahahahAHAHAHAAAA!!
//        if (future_game.current_player == O_PLAYER)
//        {
//            some_move_tab[move_idx] = find_maximum_move()->heuristics;    // cause this is MY move
//        }
//        else
//        {
//            some_move_tab[move_idx] = find_minimum_move()->heuristics;    // cause he will try to DESTROY me.
//        }
//    }
//    future_game.retract_move();


//    if (future_game.current_player == O_PLAYER)
//    {
//        some_move_tab[move_idx] = find_maximum_move()->heuristics;    // cause this is MY move
//    }
//    else
//    {
//        some_move_tab[move_idx] = find_minimum_move()->heuristics;    // cause he will try to DESTROY me.
//    }

}*/

int
minimax::iterative_move_search(int depth, state_t player)
{
    if (depth == 0)
    {
        current_game.reprint();
        return get_heuristics(player);
    }
    if (player == X_PLAYER) // maximizing player
    {
        int best_val = -0x7FFFFFFF;
        int val = 0;
        for (int move_idx = 0; move_idx < 7; move_idx++)
        {
//            if (if_X_move_possible[move_idx])
            {
                current_game.move(move_idx, player);

                val = iterative_move_search(depth-1, O_PLAYER);
                best_val = max(best_val, val);

                current_game.retract_move();
            }
        }
        return best_val;
    }
    else
    {
        int best_val = 0x7FFFFFFF;
        int val = 0;
        for (int move_idx = 0; move_idx < 7; move_idx++)
        {
//            if (if_O_move_possible[move_idx])
            {
                current_game.move(move_idx, player);

                val = iterative_move_search(depth-1, X_PLAYER);
                best_val = min(best_val, val);

                current_game.retract_move();
            }
        }
        return best_val;
    }
}

// write heuristics to subsequent move_tab descriptors
minimax::heuristics_t
minimax::get_heuristics(state_t player)
{
    // SIMULATION OF ALL POSSIBLE X_PLAYER MOVES
    // result will be sum of all possible moves' heruistics
    heuristics_t result = 0;
    for (int col_idx=0; col_idx < 7; col_idx ++)
    {
        if(current_game.column_full[col_idx])
        {
            if (player == X_PLAYER)
                if_X_move_possible[col_idx] = false;
        }
        else
        {
            if (player == X_PLAYER)
                if_X_move_possible[col_idx] = true;
            result += abs(current_game.move(col_idx, X_PLAYER));
            current_game.retract_move();
        }
    }
    // SIMULATION OF ALL POSSIBLE X_PLAYER MOVES
    // result will be sum of all possible moves' heruistics
    for (int col_idx=0; col_idx < 7; col_idx ++)
    {
        if(current_game.column_full[col_idx])
        {
            if (player == O_PLAYER)
                if_O_move_possible[col_idx] = false;
        }
        else
        {
            if (player == O_PLAYER)
                if_O_move_possible[col_idx] = true;
            result -= abs(current_game.move(col_idx, O_PLAYER));
            current_game.retract_move();
        }
    }
    return result;  // result bigger than 0 means it's all right
}

// on given depth, compare map heuristics. Make sure you have something to compare.
/*
minimax::move_descriptor_t*
minimax::find_minimum_move(bool parity)
{
    heuristics_t minimum_value = 0x7FFFFFFF;
    move_descriptor_t* result;
    for (col_idx = 0; col_idx <7; col_idx ++)
    {
        if(move_descriptor_tab[col_idx].if_move_possible)
        {
            if (move_descriptor_tab[col_idx].heuristics < minimum_value)
            {
                result = &move_descriptor_tab[col_idx];
            }
        }
    }
    return result;
}

minimax::move_descriptor_t*
minimax::find_maximum_move(bool parity)
{
    heuristics_t maximum_value = -0x7FFFFFFF;
    move_descriptor_t* result;
    for (col_idx = 0; col_idx <7; col_idx ++)
    {
        if(move_descriptor_tab[col_idx].if_move_possible)
        {
            if (move_descriptor_tab[col_idx].heuristics < maximum_value)
            {
                result = &move_descriptor_tab[col_idx];
            }
        }
    }
    return result;
}

// on given depth, compare map heuristics. Make sure you have something to compare.
minimax::move_descriptor_t*
minimax::find_minimum(move_descriptor_t *move_tab)
{
    heuristics_t minimum_value = 0x7FFFFFFF;
    move_descriptor_t* result;
    for (col_idx = 0; col_idx <7; col_idx ++)
    {
        if(move_tab[col_idx].if_move_possible)
        {
            if (move_tab[col_idx].heuristics < minimum_value)
            {
                result = &move_tab[col_idx];
            }
        }
    }
    return result;
}

minimax::move_descriptor_t*
minimax::find_maximum(move_descriptor_t *move_tab)
{
    heuristics_t maximum_value = -0x7FFFFFFF;
    move_descriptor_t* result;
    for (col_idx = 0; col_idx <7; col_idx ++)
    {
        if(move_tab[col_idx].if_move_possible)
        {
            if (move_tab[col_idx].heuristics < maximum_value)
            {
                result = &move_tab[col_idx];
            }
        }
    }
    return result;
}
*/


minimax::~minimax()
{

}


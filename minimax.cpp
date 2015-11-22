#include "minimax.h"
#include "math.h"

#define O_PLAYER_TAB_IDX 0
#define X_PLAYER_TAB_IDX 1

//#define HEURISTICS_TEST

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
        current_game = gamemap();

        char player;
        cout << "Which player shall begin? (o/x)" << endl;
        cin >> player;

        current_game.set_player(player);
        current_game.reprint();


        while(true)
        {
            int single_move_heuristics;
#ifdef HEURISTICS_TEST
            single_move_heuristics = current_game.move();
            cout << endl << get_heuristics() << endl;
#else
            if(current_game.current_player == X_PLAYER)
            {
                iterative_move_search(max_depth, X_PLAYER);
                single_move_heuristics = current_game.move(best_move_idx, X_PLAYER);
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
#endif

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

//minimax::col_idx_t
//minimax::decide_move()
//{
//    int best_move_heuristics = -0x7FFFFFFF;
//    int best_move_idx;
//    for(int move_idx=0; move_idx<7; move_idx++)
//    {
//        int val = iterative_move_search(max_depth - 1, X_PLAYER);
//        if(val > best_move_heuristics)
//        {
//            best_move_heuristics = val;
//            best_move_idx = move_idx;
//        }
//    }
//    return best_move_idx;
//}

int
minimax::iterative_move_search(int depth, state_t player)
{
    if (depth == 0)
    {
//        current_game.reprint();
        return get_heuristics();
    }
    if (player == X_PLAYER) // maximizing player
    {
        int best_maxval = -0x7FFFFFFF;
        int val = 0;
        for (int move_idx = 0; move_idx < 7; move_idx++)
        {
            if (!current_game.column_full[move_idx])
            {
                val = current_game.move(move_idx, player);

                // all above val operations are only meant to add win score to considered move
                val += iterative_move_search(depth-1, O_PLAYER);
                if(val > best_maxval)
                {
                    best_maxval = val;
                    best_move_idx = move_idx;
                }

                current_game.retract_move();
            }
        }
        return best_maxval;
    }
    else
    {
        int best_minval = 0x7FFFFFFF;
        int val = 0;
        for (int move_idx = 0; move_idx < 7; move_idx++)
        {
            if (!current_game.column_full[move_idx])
            {
                val = current_game.move(move_idx, player);

                // all above val operations are only meant to add win score to considered move
                val += iterative_move_search(depth-1, X_PLAYER);
                if(val < best_minval)
                    best_minval = val;

                current_game.retract_move();
            }
        }
        return best_minval;
    }
}

// write heuristics to subsequent move_tab descriptors
minimax::heuristics_t
minimax::get_heuristics()
{
    // SIMULATION OF ALL POSSIBLE X_PLAYER MOVES
    // sum of these moves' heruistics is added to result
    heuristics_t result = 0;

    for (int col_idx=0; col_idx < 7; col_idx ++)
    {
        heuristics_t single_score;
        if(!current_game.column_full[col_idx])
        {
            single_score = abs(current_game.move(col_idx, X_PLAYER));

            if (single_score >= 100000) // win score available only via iterative_move_search.
                single_score -= 100000; // walking this way you won't get win score!

            result += single_score;
            current_game.retract_move();
        }
    }
    // SIMULATION OF ALL POSSIBLE O_PLAYER MOVES
    // sum of these moves' heuristics is subtracted from result
    for (int col_idx=0; col_idx < 7; col_idx ++)
    {
        heuristics_t single_score;
        if(!current_game.column_full[col_idx])
        {
            single_score = abs(current_game.move(col_idx, O_PLAYER));

            if (single_score >= 100000) // win score available only via iterative_move_search.
                single_score -= 100000; // walking this way you won't get win score!

            result -= single_score;
            current_game.retract_move();
        }
    }
    return result;  // result bigger than 0 means it's all right
}

minimax::~minimax()
{

}


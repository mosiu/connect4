#include "minimax.h"

minimax::minimax()
{

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

        int single_move_heuristics;
        while(true)
        {
            single_move_heuristics = current_game.wait_for_move();

            if (current_game.move_counter >= 42)
            {
                cout << "\n -------- I T ' S   A   D R A W! -------- \n\n\n";
                break;
            }

            if (single_move_heuristics <= -10000)
            {
                cout << "\n --- You win, O_PLAYER, you clever bastard! --- \n\n\n";
                break;
            }
            else if (single_move_heuristics >= 10000)
            {
                cout << "\n --- You win, X_PLAYER, you clever bastard! --- \n\n\n";
                break;
            }
            else
            {
                if(current_game.current_player == X_PLAYER)
                {
                    //TODO:: launch prediction

                }
                continue;
            }
        }
        cout << "Next game? (y/n):";
        cin >> ans;

    } while (ans == 'y');

    return 0;   // status: success
}

minimax::~minimax()
{

}


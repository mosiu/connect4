#include <QCoreApplication>
#include "gamemap.h"

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    char ans;
    do
    {
        game_result_t game_result;
        gamemap game;

        while(true)
        {
            if (game.current_player == O_PLAYER)
            {
                game_result = game.wait_for_move();
            }
            else
            {
                game_result = game.
            }

            if (game_result == DRAW)
            {
                cout << "\n -------- I T ' S   A   D R A W! -------- \n\n\n";
                break;
            }
            else if (game_result == O_PLAYER_WINS)
            {
                cout << "\n --- You win, O_PLAYER, you clever bastard! --- \n\n\n";
                break;
            }
            else if (game_result == X_PLAYER_WINS)
            {
                cout << "\n --- You win, X_PLAYER, you clever bastard! --- \n\n\n";
                break;
            }
            else
            {
                continue;
            }
        }
        cout << "Next game? (y/n):";
        cin >> ans;
    }
    while (ans == 'y');
    a.exit();

    return a.exec();
}

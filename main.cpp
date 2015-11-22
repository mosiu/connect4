#include <QCoreApplication>
#include "gamemap.h"
#include "minimax.h"

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    char ans;

    minimax ai_game(5);
    ai_game.play();

    a.quit();

    return a.exec();
}

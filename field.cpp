#include "field.h"

field::field()
{
    state = FREE;
}

void
field::set_player(state_t player)
{
    state = player;
}

// returns true if won
bool
field::check_win()
{
    for (int i=0; i<4; i++)
    {
        if(direction_tab[i].my_row_counter >= 4)   // if won
            return true;
    }

    return false;
}

ostream &
operator<<(ostream & display_stream, field & to_display)
{
    if(to_display.state == FREE)
    {
        display_stream << " - ";
    }
    else if(to_display.state == O_PLAYER)
    {
        display_stream << " O ";
    }
    else
    {
        display_stream << " X ";
    }
    return display_stream;
}

field::~field()
{

}


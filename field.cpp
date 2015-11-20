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


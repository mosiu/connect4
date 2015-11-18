#ifndef FIELD_PTR_H
#define FIELD_PTR_H

#include "field.h"

class field_ptr
{
public:
    field_ptr(direction_t dir);
    ~field_ptr();
    field_ptr operator++ (int)  // postfix ++
    {
        switch(current_direction)
        {
        case VERTICAL:
            pointer -= 7;
            break;
        case HORIZONTAL:
            pointer++;
            break;
        case DIAGONAL_MINUS:
            pointer += 8;
            break;
        case DIAGONAL_PLUS:
            pointer -= 6;
        }
        return *this;
    }
    void set_direction(direction_t dir)
    {
        current_direction = dir;
    }
    void set(field* ptr)
    {

    }
private:
    // heart of the pointer
    field* pointer;

    // affects the ++ operator
    direction_t current_direction;
};

#endif // FIELD_PTR_H

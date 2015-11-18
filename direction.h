#ifndef DIRECTION_H
#define DIRECTION_H

typedef enum {
    VERTICAL = 0,
    HORIZONTAL = 1,
    DIAGONAL_PLUS = 2,
    DIAGONAL_MINUS = 3
} direction_t;

// it's a struct, actually
class direction
{
public:
    direction();
    ~direction();


    int free_counter;
    int my_disc_counter;
    int my_row_counter;
    int width_counter;
private:
};

#endif // DIRECTION_H

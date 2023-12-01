#include <stdio.h>
#include <stddef.h>

typedef struct {
    float mLength;
    float mDirection;
} Line;

struct Square {
    float mSide;
};

#define TAGGED_UNION \
    AS(Shape, \
        WITH(LINE, Line) \
        WITH(SQUARE, struct Square) \
        WITH(NUMBER, float) \
        WITH(TRIANGLE, struct { float mA, mB, mC; }) \
    )
#include "tagged_union.h"
#undef TAGGED_UNION

int main(void)
{
    size_t i;
    Shape lShapes[] = {
        tu_new(Shape, LINE,     { 1, 2 }    ),
        tu_new(Shape, TRIANGLE, { 3, 5, 6 } ),
        tu_new(Shape, NUMBER,   7           ),
        tu_new(Shape, SQUARE,   { 8 }       )
    };
    
    for (i = 0; i < sizeof(lShapes) / sizeof(*lShapes); i++)
    {
        tu_match (Shape, lShapes + i)
            on_tag (LINE, line,
                line->mDirection = 0;
                printf("len: %f dir: %f\n", line->mLength, line->mDirection);
            )
            on_tag (TRIANGLE, triangle,
                printf("sides: %f %f %f\n", triangle->mA, triangle->mB, triangle->mC);
            )
            on_tag (NUMBER, num,
                printf("%f\n", *num);
            )
        tu_end
    }
    return 0;
}
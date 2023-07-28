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
    USING (Shape, \
        ENTRY(LINE, Line) \
        ENTRY(SQUARE, struct Square) \
        ENTRY(NUMBER, float) \
        ENTRY(TRIANGLE, struct { float mA, mB, mC; }) \
    )
#include "tagged_union.h"
#undef TAGGED_UNION

int main(void)
{
    size_t i;
    Shape lShapes[] = {
        new_tagged_enum(Shape, LINE,    { 1, 2 }),
        new_tagged_enum(Shape, TRIANGLE,  { 3, 5, 6 }),
        new_tagged_enum(Shape, NUMBER,  7),
        new_tagged_enum(Shape, SQUARE,  { 8 })
    };
    
    for (i = 0; i < sizeof(lShapes) / sizeof(*lShapes); i++)
    {
        tagged_switch(Shape, lShapes + i)
            accept(LINE, line,
                line->mDirection = 0;
                printf("len: %f dir: %f\n", line->mLength, line->mDirection);
            )
            accept(TRIANGLE, triangle,
                printf("sides: %f %f %f\n", triangle->mA, triangle->mB, triangle->mC);
            )
            accept(NUMBER, num,
                printf("%f\n", *num);
            )
        tagged_end
    }
    return 0;
}

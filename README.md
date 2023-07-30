# tagged_union.h
A single header C library for safe construction and use of [tagged-unions](https://en.wikipedia.org/wiki/Tagged_union).

## Tagged-Union Type Declaration
To declare a type use the header as a template using `TAGGED_UNION` macro.
This macro can have a following form:
```c
#define TAGGED_UNION \
    USING(Shape, \
        ENTRY(LINE, Line) \
        ENTRY(SQUARE, struct Square) \
        ENTRY(NUMBER, float) \
        ENTRY(TRIANGLE, struct { float mA, mB, mC; }) \
    )
#include "tagged_union.h"
#undef TAGGED_UNION
```
`TAGGED_UNION` now contains call to `USING` macro which is used inside the header to generate code for the tagged-union.

First we specify the name of the tagged-union type - `Shape`,
then using pattern called [x-macros](https://en.wikipedia.org/wiki/X_macro) with each `ENTRY` we specify label for the tag enum and respective union member type.

Notice we can pass the member types in any type form, since the header will make a `typedef` for each member type _(the reason for that is we can later get the type name just from the tag enum label)_.

## Tagged-Union Initialization
For type-safe tagged-union initialization the header provides `new_tagged_enum` macro.
To create an array it can be used like this:
```c
Shape lShapes[] = {
    new_tagged_enum(Shape, LINE,      { 1, 2 }    ),
    new_tagged_enum(Shape, TRIANGLE,  { 3, 5, 6 } ),
    new_tagged_enum(Shape, NUMBER,      7         ),
    new_tagged_enum(Shape, SQUARE,    { 8 }       )
};
```
First we need to specify the tagged-union type,
then the tag enum label will tell the both set the right tag and make sure the correct union member is used,
lastly we initialize the member.

## Working with an Instances of a Tagged-Union
Header provides `tagged_switch` macro which helps to process individual cases.
```c
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
```
This macro **stores a pointer** to the tagged-union for later use _(compiler should be able to optimize this out)_,
that is why the macro takes type of the tagged-union and a pointer to it.
Each `accept` handles the specific union member,
for that you need to specify tag enum label _(from which the right member and its type is derrived)_,
name of a variable where pointer to the member will be taken **from previously stored** tagged-union _(compiler should be able to optimize this out)_ and 
what should be done with the variable holding the correctly cast union member.

Then the whole thing needs to be closed with `tagged_end` macro.

## Example File
You can check the `example.c`, to see what is going on try following command:
```bash
gcc -D_STDIO_H -D_STDDEF_H example.c -E | cat -s | sed '/^#/d' > example.i
```
This will do just the preprocessing step of gcc and also remove all the extra junk.

---
_Note: This library is just a fun experiment and exists only to share knowledge about fun features of C._

# tagged_union.h
A single header C library for safe construction and use of [tagged-unions](https://en.wikipedia.org/wiki/Tagged_union).

## Tagged-Union Type Declaration
To define a tagged-union just `#define` a macro that takes 2 arguments:
_2nd<sup>nd</sup> arg. can be ignored_, but the 1<sup>st</sup> one is used to define members:
```c
#define Shape(entry, ...) \
    entry(LINE, Line) \
    entry(SQUARE, struct Square) \
    entry(NUMBER, float) \
    entry(TRIANGLE, struct { float mA, mB, mC; })
```
_Note that the arguments can have any name._

Then you just pass it to `tagged_union` macro:
```c
tagged_union(Shape)
```
which will create all the necessary type declarations.

## Tagged-Union Initialization
For type-safe tagged-union initialization the header provides `tu_new` macro.
To create an array it can be used like this:
```c
Shape lShapes[] = {
    tu_new(LINE,      { 1, 2 }    ),
    tu_new(TRIANGLE,  { 3, 5, 6 } ),
    tu_new(NUMBER,      7         ),
    tu_new(SQUARE,    { 8 }       )
};
```
First we need to specify the tagged-union type,
then the tag enum label will tell the both set the right tag and make sure the correct union member is used,
lastly we initialize the member.

## Working with an Instances of a Tagged-Union
Header provides `tu_match` macro which helps to process individual cases.
```c
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
    tagged_end
}
```
This macro is a wrapper around `switch` statement and <span id="stored-tu-ptr">**stores a pointer**</span> to the tagged-union for later use _(compiler should be able to optimize this out)_,
that is why the macro requires the type of the tagged-union and a pointer to it.
Each `on_tag` handles the specific union member,
for that you need to specify tag enum **label** _(from which the right member and its type is derrived)_,
name of a variable to which a pointer to the member will be stored [from previously stored](#stored-tu-ptr) tagged-union _(compiler should be able to optimize this out)_ and 
what should be done with the variable holding the correctly cast union member _(this code will have its own `{ /*scope*/ break; }`, so there is no "fallthrough" for individual "cases")_.

Then the whole thing needs to be closed with `tagged_end` macro.

## Example File
You can check the `example.c`, to see what is going on try following command:
```bash
gcc -D_STDIO_H -D_STDDEF_H example.c -E | cat -s | sed '/^#/d' > example.i
```
This will do just the preprocessing step of gcc and also remove all the extra junk.

---
_Note: This library is just a fun experiment and exists only to share knowledge about fun features of C._

# tagged_union.h
This single header library provides a way to construct and use tagged unions safely.

Let's say you have following types:
```c
typedef struct {
    float mLength;
    float mDirection;
} Line;

typedef struct {
    float mSide;
} Square;

typedef float Number;

typedef struct { float mA, mB, mC; } Triangle;
```
And now would like to implement some polymorphism,
but one mistake and you are working on 🍎s instead of 🍐s.

Wouldn't it be cool if the opportunity to overlook something like that just wouldn't exist?
This is just how **tagged unions** should work! They let you access the right member **only** when you are sure you have the right one.

And this is also exactly what this library provides - a way to construct and access tagged unions in a way that is safe!

## How did it achieve that?
### First we need a way to construct the tagged union
For that we can use a well-known pattern called [x-macros](https://en.wikipedia.org/wiki/X_macro):
```c
#define UNION_TAGS
    ENTRY(LINE, Line) \
    ENTRY(SQUARE, struct Square) \
    ENTRY(NUMBER, float) \
    ENTRY(TRIANGLE, struct { float mA, mB, mC; })

/* This will create typedef for each tag enum label `tag` and union member type `...`.
 * Not only this lets us make ENTRies with unnamed types,
 * it also will later let us be able to get the type just from the tag enum label. */
#define ENTRY(tag, ...) typedef __VA_ARGS__ tag##_t;
UNION_TAGS
#undef ENTRY
/* Note that we are using `...` to capture all following tokens. */
```

### Now let us construct the actual tagged union!

```c
typedef struct {
#define ENTRY(tag, ...) tag,
    enum {
        UNION_TAGS
    } tag_value;
#undef ENTRY
#define ENTRY(tag, ...) tag##_t tag;
    union {
        UNION_TAGS
    } untagged_union;
#undef ENTRY
} MyUnion;
/* Notice the macro is discarding the rest of the tokens because we don't need them anymore. */ 
```

### But that's a lot of manual work!
So let the header take care of that. 
You might ask, but how will the header know the types?
Simple, let's just template the header!

Then creating such union is just these couple of lines with some small formalities.
```c
#define TAGGED_UNION \
    USING (Shape, \
        ENTRY(LINE, Line) \
        ENTRY(SQUARE, struct Square) \
        ENTRY(NUMBER, float) \
        ENTRY(TRIANGLE, struct { float mA, mB, mC; }) \
    )
#include "tagged_union.h"
#undef TAGGED_UNION
/* Note we used the same trick like we did with x-macros for putting everything into 1 macro definition
 * and that is we will be discarding the argument that we don't need in the header. */
```

### How do we even make an instance?
All we need is to set right tag for the right member. That is now easy, since the member bears the same name as the tag.
```c
#define new_tagged_enum(tagged_union_type, tag, ...) (tagged_union_type){ .tag_value = tag, .untagged_union.tag = __VA_ARGS__ }
```

### That's all cool but how do we even access it?
_I am assuming that the structure is going to be passed by reference in most cases from this point onwards (I will be relying on the compiler in cases when this is not true)._
We will use the most powerful statement in C language: `switch`!
But to get the right union member based on the case, we need a guaranteed way to get the union, so for that let's just store it as a pointer before we start the switch.
And after we match the right case we will get the right union member with the tag enum label.

<table>
<tr><th>macro call</th><th>actual code</th></tr>
<tr>
  <td>

```c
tagged_switch(Shape, lShapes + i)
```

  </td>
  <td>

```c
{
    Shape* const __tu = (lShapes + i);
    switch((lShapes + i)->tag_value) 
    {
```

  </td>
</tr>
<tr>
  <td>

```c
    accept(LINE, line,
        line->mDirection = 0;
        printf("len: %f dir: %f\n", line->mLength, line->mDirection);
    )
```

  </td>
  <td>

```c
    case LINE:
    {
        LINE_t* line = &__tu->untagged_union.LINE;
        line->mDirection = 0;
        printf("len: %f dir: %f\n", line->mLength, line->mDirection);
        break;
    }
```

  </td>
</tr>
<tr>
  <td>

```c
tagged_end
```

  </td>
  <td>

```c
    }
}
```

  </td>
</tr>
</table>

With this, even if you copy-paste code of a different member, as long as you don't touch the internal members by hand, the compiler shouldn't let you go away with working with 🍎s instead of 🍐s.
---
_Note: This library is just a fun experiment and exists only to share knowledge about fun features of C._

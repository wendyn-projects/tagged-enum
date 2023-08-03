/*
 *       __|\/\/|__
 *       \   '    /
 * |/ |\ < ' |) ' > | |\  |\| T 
 * '  ' `/_ .'`. _\ ' ' ` ' ' '
 *         |/\/\|
 */

/* Library Side */
#ifndef TAGGED_UNION_H_
#define TAGGED_UNION_H_
/*
 * Constructs the tagged-union instance.
 * @param tagged_union_type type of the tagged-union
 * @param tag enum label tag of the union member
 * @param ... initialization of the union member
 */
#define new_tagged_enum(tagged_union_type, tag, ...) (tagged_union_type){ .tag_value = tag, .untagged_union.tag = __VA_ARGS__ }

/**
 * `switch` wrapper that stores the pointer to the union for later use, which compiler should be able to optimize it out.
 * @param tagged_union_type type of the tagged-union to store the pointer.
 * @param tagged_union_ptr pointer to the tagged-union that is stored and which `switch` gets the tag from.
 */
#define tagged_pick(tagged_union_type, tagged_union_ptr) \
{ \
    tagged_union_type* const __tu = (tagged_union_ptr); \
    switch((tagged_union_ptr)->tag_value) {
        
/**
 * Case wrapper that safely casts the right union member (from previously stored pointer) based on the tag.
 * @param tag tag enum **label**
 * @param item_ptr_name pointer that will be created for the correct union member, compiler should be able to optimize it out.
 * @param code for the union member
 */
#define on_tag(tag, member_ptr_name, code) \
case tag: \
{ \
    tag##_t* const member_ptr_name = &__tu->untagged_union.tag; \
    code \
    break; \
}

/**
 * end of the wrapped `switch` statement
 */
#define tagged_end } }

#endif

#define AS(_, ...) __VA_ARGS__

/* Templated Header Side */
#ifdef TAGGED_UNION
/* Creating custom typedefs based on the tag enum labels so that we can later get it just from the enum label itself. */
#define MEMBER(tag, ...)  typedef __VA_ARGS__ tag##_t;
TAGGED_UNION
#undef MEMBER

/* Constructing the tagged-union type. */
typedef struct {
#define MEMBER(tag, ...)  tag,
    enum { TAGGED_UNION } tag_value;
#undef MEMBER
#define MEMBER(tag, ...)  tag##_t tag;
    union { TAGGED_UNION } untagged_union;
#undef MEMBER

#undef AS
#define AS(tagged_union_type, _) tagged_union_type
} TAGGED_UNION;
#undef AS
#endif

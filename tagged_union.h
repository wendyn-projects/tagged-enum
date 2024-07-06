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
 * @param tag enum label tag of the union member
 * @param ... initialization of the union member
 */
#define tu_new(tag, ...) { .tag_value = tag, .untagged_union.tag = __VA_ARGS__ }

/**
 * `switch` wrapper that stores the pointer to the union for later use, which compiler should be able to optimize it out.
 * @param tagged_union_type type of the tagged-union to store the pointer.
 * @param tagged_union_ptr pointer to the tagged-union that is stored and which `switch` gets the tag from.
 */
#define tu_match(tagged_union_type, tagged_union_ptr) \
{ \
    tagged_union_type* const __tu = (tagged_union_ptr); \
    switch ((tagged_union_ptr)->tag_value) \
    {

/**
 * `case` wrapper that safely casts the right union member (from previously stored pointer) based on the tag.
 * @param tag tag enum **label**
 * @param member_ptr_name pointer that will be created for the correct union member, compiler should be able to optimize it out.
 * @param ... code for the union member
 */
#define on_tag(tag, member_ptr_name, ...) \
case tag: \
{ \
    tag##_t* const member_ptr_name = &__tu->untagged_union.tag; \
    __VA_ARGS__ \
    break; \
}

/**
 * end of the wrapped `switch` statement
 */
#define tu_end } }

#define __tu_create_typedefs(tag, ...) typedef __VA_ARGS__ tag##_t;
#define __tu_create_tags(tag, ...) tag,
#define __tu_create_union_members(tag, ...) tag##_t tag;

#define tagged_union(entries) \
entries(__tu_create_typedefs,) \
typedef struct { \
    enum { entries(__tu_create_tags,) } tag_value; \
    union { entries(__tu_create_union_members,) } untagged_union; \
} entries;

#endif

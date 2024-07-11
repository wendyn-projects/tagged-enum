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
 * @param tag_label enum tag label of the union member
 * @param ... initialization of the union member
 */
#define tu_new(tag_label, ...) { .tag_value = tag_label, .untagged_union.tag_label = __VA_ARGS__ }

/**
 * `switch` wrapper that stores the pointer to the union for later use, which compiler should be able to optimize it out.
 * @param tagged_union_type type of the tagged-union to store the pointer.
 * @param tagged_union_ptr pointer to the tagged-union that is stored and which `switch` gets the tag from.
 */
#define tu_resolve(tagged_union_type, tagged_union_ptr) \
{ \
    tagged_union_type* const __tu = (tagged_union_ptr); \
    switch ((tagged_union_ptr)->tag_value) \
    { \
        {

/**
 * `case` wrapper that safely casts the right union member (from previously stored pointer) based on the tag.
 * @param tag_label enum tag **label**
 * @param member_ptr_name pointer that will be created for the correct union member, compiler should be able to optimize it out.
 */
#define tu_matches(tag_label, member_ptr_name) \
    break; \
} \
case tag_label: \
{ \
    tag_label##_t* const member_ptr_name = &__tu->untagged_union.tag_label;

/**
 * `default` wrapper
 */
#define tu_no_match \
    break; \
} \
default: \
{

/**
 * end of the wrapped `switch` statement
 */
#define tu_resolved } } }

#define tu_check(tagged_union, tag) ((tagged_union).tag_value == (tag))
#define tu_check_ptr(tagged_union_ptr, tag) ((tagged_union_ptr)->tag_value == (tag))

#define tu_cast(tag_label, tagged_union) (assert(tu_check((tagged_union), tag_label)), (tagged_union).untagged_union.tag_label)
#define tu_cast_ptr(tag_label, tagged_union_ptr) (tu_check_ptr(tagged_union_ptr, tag_label)? &(tagged_union_ptr)->untagged_union.tag_label : NULL)

#define __tu_create_typedefs(tag_label, ...) typedef __VA_ARGS__ tag_label##_t;
#define __tu_create_tags(tag_label, ...) tag_label,
#define __tu_create_union_members(tag_label, ...) tag_label##_t tag_label;

#define tagged_union(entries) \
entries(__tu_create_typedefs,) \
typedef struct { \
    enum { entries(__tu_create_tags,) } tag_value; \
    union { entries(__tu_create_union_members,) } untagged_union; \
} entries;

#endif

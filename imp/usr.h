#pragma once
/** @file
user data
=========

@verbatim
+--------------------+------------------------+
| .     61 bits value                   | 100 |
+-|------------------+------------------------+
  |    +----------------+------------+          +----------------+------------+
  `->  |  voba_value_t  *        .---|--------> |    .      size_t size       |
       +----------------+------------+          +----------------+------------+
       |       data ...  unknow size | \        |  const char * name          |
       +-----------------------------+  |       +-----------------------------+
       |     but 8 bytes alignment   |  |  (size)
       +-----------------------------+ /
@endverbatim

User data is garbage collected when unused. This is not a problem for
c, but might be a problem for C++, because deconstructor is automatic
invoked, a solution is needed. GC supports to register a finalizer.

\a size of user_data does not including the room for \a cls object, e.g.

@code{.c}
       struct point { int x; int y}; 
       voba_make_user_data(NIL, sizeof(struct point)) 
       ((struct point*) user_data_base(v))->x = 100; 
       ((struct point*) user_data_base(v))->y = 100;
@endcode

see cls.h, which is itself a user defined object.

*/
/** create a user defined object, whose class is \a cls*/
VOBA_VALUE_INLINE voba_value_t voba_make_user_data(voba_value_t cls);
/** @return the class object associated with the user defined object \a v*/
VOBA_VALUE_INLINE voba_value_t voba_user_data_class(voba_value_t v);
/** @return the pointer to the user defined object \a v*/
VOBA_VALUE_INLINE void*   voba_user_data_base(voba_value_t v);

#define VOBA_USER_DATA_AS(type,v) ((type)(voba_user_data_base(v)))
/** test whether a user defined object \a v is an instances of class \a cls*/
VOBA_VALUE_INLINE int voba_is_a(voba_value_t v, voba_value_t cls);




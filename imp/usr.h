#pragma once
/* user data */
/*
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


the user data is garbage collected when unused, this is not a problem
for c, but might be a problem for C++, because deconstructor is
automatic invoked. a solution is needed. TODO.

when voba_class_t is NIL, the default user class `vova_cls_user_data`
is used. this function regards user data as a opaque data area.

`size` of user_data, not including the room for cls function. E.g.
 
       struct point { int x; int y}; 
       voba_make_user_data(NIL, sizeof(struct point)) 
       ((struct point*) user_data_base(v))->x = 100; 
       ((struct point*) user_data_base(v))->y = 100;

*/
INLINE voba_value_t voba_make_user_data(voba_value_t cls);
INLINE voba_value_t voba_user_data_class(voba_value_t v);
INLINE void*   voba_user_data_base(voba_value_t v);
#define VOBA_USER_DATA_AS(type,v) ((type)(voba_user_data_base(v)))

INLINE int voba_is_a(voba_value_t v, voba_value_t cls);




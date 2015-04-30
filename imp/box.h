#pragma once
/** @file */
/** \file 
box
======

\verbatim
+--------------------+------------------------+
| .     61 bits value                   | 001 |
+-|------------------+------------------------+
  |                           voba_box_t
  |                           +----------------+------------+       
  `------------------------>  |  voba_value_t* data         |       
                              +----------------+------------+       

\endverbatim

A box object is a point to voba_value_t object.

 */
//
/*** @brief the class object associated with box object
 */
extern voba_value_t  voba_cls_box;
/** @brief create a box, containing the value `v` */
INLINE voba_value_t  voba_box(voba_value_t value);
/** @brief return the content of a box*/
INLINE voba_value_t  voba_unbox(voba_value_t box);




/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <raph@rhs.se> wrote this file. As long as you retain this notice you can
 * do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Raphael Poss
 * ----------------------------------------------------------------------------
 */

#ifndef NSC_VALUE_HXX
#define NSC_VALUE_HXX

#include "nsc_value.hh"

namespace nsc
{
  value::type_t value::type() const
  { return my_type_; }

  value::value(int x)
    : my_type_(integer)
  { my_data_.int_val = x; }

  value::value(const value& other)
    : my_type_(integer)
  { *this = other; }

  value::value()
    : my_type_(integer)
  { my_data_.int_val = 0; }

  value::value(const char *str)
    : my_type_(integer)
  { as_str() = str; }

  value::value(const std::string& str)
    : my_type_(integer)
  { as_str() = str; }
  
  value::value(void *p)
    : my_type_(ptr)
  { as_ptr() = p; } 

  value& value::operator=(const char *str)
  { as_str() = str; return *this; }
  value& value::operator=(int x)
  { as_int() = x; return *this; }
  value& value::operator=(const std::string& s)
  { as_str() = s; return *this; }
  value& value::operator=(void *p)
  { as_ptr() = p; return *this; }

  value::operator int() const
  { return as_int(); }
  value::operator const std::string() const
  { return as_str(); }

}


#endif

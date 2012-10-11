/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <raph@rhs.se> wrote this file. As long as you retain this notice you can
 * do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Raphael Poss
 * ----------------------------------------------------------------------------
 */

#ifndef NSC_VALUE_HH
#define NSC_VALUE_HH

#include <string>
#include <iostream>

namespace nsc
{

  struct value
  {
  public:
    enum type_t { integer, str, ptr };

    inline type_t type() const;

    inline value();
    inline value(const value&);
    inline value(int);
    inline value(const char*);
    inline value(const std::string&);
    inline value(void *ptr);

    value& operator=(const value&);
    inline value& operator=(int);
    inline value& operator=(const char*);
    inline value& operator=(const std::string&);
    inline value& operator=(void *);

    int as_int() const;
    int& as_int();
    const std::string as_str() const;
    std::string& as_str();
    void * as_ptr() const;
    void *& as_ptr();
    void * release();

    inline operator int() const;
    inline operator const std::string() const;
    
  protected:
    type_t my_type_;
    union {
      int int_val;
      std::string *str_val;
      void *ptr_val;
    } my_data_;
  };

  std::ostream& operator<<(std::ostream&, const value&);
  bool operator==(const value&, const value&);
  bool operator<(const value&, const value&);

}

#include "nsc_value.hxx"

#endif

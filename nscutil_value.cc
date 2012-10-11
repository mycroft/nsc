/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <raph@rhs.se> wrote this file. As long as you retain this notice you can
 * do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Raphael Poss
 * ----------------------------------------------------------------------------
 */

#include <iostream>
#include <sstream>

#include "nsc_value.hh"

namespace nsc
{

  void *value::as_ptr() const
  {
    switch(type())
      {
      case ptr:
	return my_data_.ptr_val;
      default:
	std::cerr << "WARNING: retrieving ptr of const non-ptr value at "
		  << this << std::endl;
	return 0;
      }
  }
  void *& value::as_ptr()
  {
    switch(type())
      {
      case ptr:
	break;
      case str:
	delete my_data_.str_val;
      case integer:
	my_type_ = ptr;
	my_data_.ptr_val = 0;
	break;
      }
    return my_data_.ptr_val;
  }

  int value::as_int() const
  {
    switch(type())
      {
      case ptr:
	std::cerr << "WARNING: retrieving int of ptr value at "
		  << this << std::endl;
	return 0;
      case str:
	{
	  std::istringstream i(*my_data_.str_val);
	  int v;
	  i >> v;
	  return v;
	}
      case integer:
	return my_data_.int_val;
      }
    return *((int*)NULL); // UNREACHABLE
  }

  int& value::as_int() 
  {
    switch(type())
      {
      case ptr:
	std::cerr << "WARNING: retrieving int value of ptr drops ptr at "
		  << this << std::endl;
	release();
	break;
      case str:
	{
	  int v;
	  {
	    std::istringstream i(*my_data_.str_val);
	    i >> v;
	  }
	  delete my_data_.str_val;
	  my_data_.int_val = v;
	}
	my_type_ = integer;
      default:
	break;
      }
    return my_data_.int_val;
  }

  const std::string value::as_str() const
  {
    switch(type())
      {
      case ptr:
	std::cerr << "WARNING: retrieving str of ptr value at "
		  << this << std::endl;
	return "";
      case integer:
	{
	  std::ostringstream o;
	  o << my_data_.int_val;
	  return o.str();
	}
      case str:
	return *my_data_.str_val;
      }
    return ""; // UNREACHABLE
  }

  std::string& value::as_str()
  {
    switch(type())
      {
      case ptr:
	std::cerr << "WARNING: retrieving str value of ptr drops ptr at "
		  << this << std::endl;
	release();
	my_type_ = str;
	my_data_.str_val = new std::string("");
	break;
      case integer:
	{
	  std::ostringstream o;
	  o << my_data_.int_val;
  	  my_type_ = str;
	  my_data_.str_val = new std::string(o.str());
	}
      default:
	break;
      }
    return *my_data_.str_val;
  }
  


  void *value::release()
  {
    void *r = as_ptr();
    my_data_.int_val = 0;
    my_type_ = integer;
    return r;
  }

  value& value::operator=(const value& other)
  {
    if (my_type_ == ptr && release())
      std::cerr << "WARNING: unreleased ptr value at "
		<< this << std::endl;

    switch(other.type())
      {
      case ptr:
	as_ptr() = other.as_ptr();
	break;
      case str:
	as_str() = other.as_str();
	break;
      case integer:
	as_int() = other.as_int();
	break;
      }
    return *this;
  }

  std::ostream& operator<<(std::ostream& o, const value& v)
  {
    if (v.type() == value::ptr)
      {
	std::cerr << "WARNING: printing ptr value at "
		  << &v << std::endl;
	return o << "PTR(" << v.as_ptr() << ')';
      }
    return o << v.as_str();
  }

  bool operator==(const value& v1, const value& v2)
  {
    return (v1.type() == v2.type() &&
	    ((v1.type() == value::integer && v1.as_int() == v2.as_int()) ||
	     (v1.type() == value::str && v1.as_str() == v2.as_str()) ||
	     (v1.type() == value::ptr && v1.as_ptr() == v2.as_ptr())));
  }

  bool operator<(const value& v1, const value& v2)
  {
    value::type_t v1t = v1.type(), v2t = v2.type();
    if (v1t == v2t && v1t == value::ptr)
      return (v1.as_ptr() < v2.as_ptr());
    if (v2t == value::ptr)
      return true;
    if (v1t == value::ptr)
      return false;
    int i1 = v1.as_int();
    int i2 = v2.as_int();
    if (i1 || i2)
      return i1 < i2;
    return v1.as_str() < v2.as_str();
  }

}

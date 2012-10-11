/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <raph@rhs.se> wrote this file. As long as you retain this notice you can
 * do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Raphael Poss
 * ----------------------------------------------------------------------------
 */

#include <iostream>
#include <fstream>
#include <sstream>

#include <stdlib.h>

#include "nsc_conf.hh"

namespace nsc
{
  conf_base::strs_t conf_base::keys() const
  {
    strs_t ret;
    for (data_t::const_iterator i = data.begin(); 
	 i != data.end(); 
	 ++i)
      ret.insert(i->first);
    return ret;
  }

  conf_base::vals_t conf_base::values(const std::string& key) const
  {
    vals_t ret;
    for (data_t::const_iterator i = data.lower_bound(key); 
	 i != data.upper_bound(key);
	 ++i)
      ret.push_back(i->second);
    return ret;
  }

  value conf_base::val(const std::string& key) const
  { 
    data_t::const_iterator i = data.find(key);
    if (i != data.end())
      return i->second;
    return value();
  }

  void conf_base::add_key_data(const std::string& key, const conf_base::vals_t& vals)
  {
    for (vals_t::const_iterator i = vals.begin();
	 i != vals.end();
	 ++i)
      data.insert(std::make_pair(key, *i));
  }

  std::ostream& operator<<(std::ostream& o, const conf_base& c)
  {
    for (conf_base::data_t::const_iterator i = c.data.begin();
	 i != c.data.end();
	 ++i)
      o << i->first << ' ' << i->second << std::endl;
    o << "EOC" << std::endl;
    return o;
  }
  
  std::istream& operator>>(std::istream& i, conf_base& c)
  {
    while (42)
      {
	if (!i)
	  break ;
	
	std::string input;
	i >> std::ws;
	getline(i, input);

	if (input == "EOC") // end of configuration ?
	  break ;
	if (input[0] == '#') // comment ?
	  continue ;

	// attempt and find "key value".
	std::istringstream is(input);
	std::string key, value;
	is >> key >> std::ws;
	if (key.empty())
	  break ;

	getline(is, value);
	c.data.insert(std::make_pair(key, value));
      }
    return i;
  }

  void conf_base::read(const char *filename)
  {
    std::ifstream i(filename);
    i >> *this;
  }

  void conf_base::write(const char *filename) const
  {
    std::ofstream o(filename);
    o << *this;
  }

  void conf::read()
  {
    std::string s = getenv("HOME");
    s += "/.nscrc";
    conf_base::read(s.c_str());
  }
  void conf::write() const
  {
    std::string s = getenv("HOME");
    s += "/.nscrc";
    conf_base::write(s.c_str());
  }

}


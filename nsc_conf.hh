/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <raph@rhs.se> wrote this file. As long as you retain this notice you can
 * do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Raphael Poss
 * ----------------------------------------------------------------------------
 */

#ifndef NSC_CONF_HH
#define NSC_CONF_HH

#include <map>
#include <list>
#include <set>
#include <string>
#include <iostream>
#include "nsc_value.hh"

namespace nsc
{
  struct conf_base
  {
    typedef std::set<std::string> strs_t;
    typedef std::list<value> vals_t;

    typedef std::multimap<std::string, value> data_t;
    data_t data;

    void write(const char *filename) const;
    void read(const char *filename);

    strs_t keys() const;
    vals_t values(const std::string& key) const;
    value val(const std::string& key) const;

    inline void clear_key(const std::string& key);
    inline void add_key_data(const std::string& key, const value& val);
    void add_key_data(const std::string& key, const vals_t& vals);
  };

  std::ostream& operator<<(std::ostream&, const conf_base&);
  std::istream& operator>>(std::istream&, conf_base&);


  struct conf : public conf_base {
    void write() const;
    void read();
  };

}

#include "nsc_conf.hxx"

#endif

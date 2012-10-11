/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <raph@rhs.se> wrote this file. As long as you retain this notice you can
 * do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Raphael Poss
 * ----------------------------------------------------------------------------
 */

#ifndef NSC_CONF_HXX
#define NSC_CONF_HXX

#include "nsc_conf.hh"

namespace nsc
{

  void conf_base::clear_key(const std::string& key)
  { data.erase(key); }
  void conf_base::add_key_data(const std::string& key, const value& val)
  { data.insert(std::make_pair(key, val)); }

}

#endif

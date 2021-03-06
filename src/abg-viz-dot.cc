// -*- mode: C++ -*-
//
// Copyright (C) 2013-2015 Red Hat, Inc.
//
// This file is part of the GNU Application Binary Interface Generic
// Analysis and Instrumentation Library (libabigail).  This library is
// free software; you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License as published by the
// Free Software Foundation; either version 3, or (at your option) any
// later version.

// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this program; see the file COPYING-LGPLV3.  If
// not, see <http://www.gnu.org/licenses/>.

#include "abg-viz-dot.h"
#include <stdexcept>
#include <fstream>

namespace abigail
{

using std::ostream;
using std::ostringstream;

// Constants.
const style parent_sty = { color::white, color::black, "" };
const style child_sty = { color::white, color::gray75, "" };

// Define.
units_type node_base::_M_count_total;

void
dot::write() 
{
  try
    {
      std::string filename(_M_title + ".gv");
      std::ofstream f(filename);
      if (!f.is_open() || !f.good())
	throw std::runtime_error("abigail::dot::write fail");
	  
      f << _M_sstream.str() << std::endl; 
    }
  catch(std::exception& e)
    {
      throw e;
    }
}

// DOT element beginning boilerplate.
void
dot::start_element() 
{ 
  // Open up digraph.
  _M_sstream << "digraph ";
  add_title();
  _M_sstream << "{" << std::endl;

  // Defaults.
  // XXX typo control
  const std::string edge_default = R"_delimiter_(edge [fontname="FreeSans",fontsize="9",labelfontname="FreeSans",labelfontsize="9"];)_delimiter_";
  const std::string node_default = R"_delimiter_(node [fontname="FreeSans",fontsize="9",shape=record];)_delimiter_";

   _M_sstream << edge_default << std::endl;
   _M_sstream << node_default << std::endl;
}

void
dot::finish_element() 
{
  _M_sstream << "}" << std::endl;
}

void
dot::add_title() 
{

  _M_sstream << '"' << _M_title << '"' << std::endl;
}

// See test 19, class-decl to base-class
// Variable: height, width
void
dot::add_node(const node_base& __node) 
{ 
  _M_sstream << "Node" << __node._M_count << " ";
  
  const std::string label("__label");
  const std::string height("__height");
  const std::string width("__width");

  std::string strip = R"_delimiter_([label="__label", height=__height, width=__width, color="black", fillcolor="white", style="filled"];)_delimiter_";

  string_replace(strip, label, __node._M_id);
  string_replace(strip, height, std::to_string(__node._M_y_space));
  string_replace(strip, width, std::to_string(__node._M_x_space));
  
  _M_sstream << strip << std::endl;
}

void
dot::add_edge(const node_base& __parent, const node_base& __child) 
{ 
  // XX typo control
  std::string style = R"_delimiter_([dir="forward",color="midnightblue",fontsize="9",style="solid",fontname="FreeSans"];)_delimiter_";

  _M_sstream << "Node" << __parent._M_count << "->";
  _M_sstream << "Node" << __child._M_count;
  _M_sstream << style << std::endl;
}

void
dot::add_parent(const parent_node& __p) 
{ 
  add_node(__p);
}

void
dot::add_child_to_node(const child_node& __c, const node_base& __p) 
{ 
  // XX remove duplicates
  add_node(__c);
  add_edge(__p, __c);
}

}//end namespace abigail

// -*- Mode: C++ -*-
//
// Copyright (C) 2013-2016 Red Hat, Inc.
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
//
// Author: Dodji Seketeli

/// @file
///
/// This file contains the declarations of the entry points to
/// de-serialize an instance of @ref abigail::corpus from a file in
/// elf format, containing dwarf information.

#include <ostream>
#include <elfutils/libdwfl.h>
#include "abg-corpus.h"

#ifndef __ABG_DWARF_READER_H__
#define __ABG_DWARF_READER_H__

namespace abigail
{

/// The namespace for the DWARF reader.
namespace dwarf_reader
{

using namespace abigail::ir;

/// The status of the @ref read_corpus_from_elf() call.
enum status
{
  /// The status is in an unknown state
  STATUS_UNKNOWN = 0,

  /// This status is for when the call went OK.
  STATUS_OK = 1,

  /// This satus is for when the debug info could not be read.
  STATUS_DEBUG_INFO_NOT_FOUND = 1 << 1,

  /// This status is for when the symbols of the ELF binaries could
  /// not be read.
  STATUS_NO_SYMBOLS_FOUND = 1 << 2,
};

status
operator|(status, status);

status
operator&(status, status);

status&
operator|=(status&, status);

status&
operator&=(status&, status);

/// The kind of ELF file we are looking at.
enum elf_type
{
  /// A normal executable binary
  ELF_TYPE_EXEC,
  /// A Position Independant Executable binary
  ELF_TYPE_PI_EXEC,
  /// A dynamic shared object, a.k.a shared library binrary.
  ELF_TYPE_DSO,
  /// A relocatalbe binary.
  ELF_TYPE_RELOCATABLE,
  /// An unknown kind of binary.
  ELF_TYPE_UNKNOWN
};

class read_context;

/// A convenience typedef for a smart pointer to a
/// dwarf_reader::read_context.
typedef shared_ptr<read_context> read_context_sptr;

read_context_sptr
create_read_context(const std::string&	elf_path,
		    char**		debug_info_root_path,
		    ir::environment*	environment,
		    bool		read_all_types = false);

corpus_sptr
read_corpus_from_elf(read_context&	ctxt,
		     status&);

corpus_sptr
read_corpus_from_elf(const std::string& elf_path,
		     char**		debug_info_root_path,
		     ir::environment*	environment,
		     bool		load_all_types,
		     status&);

bool
lookup_symbol_from_elf(const environment*		env,
		       const string&			elf_path,
		       const string&			symbol_name,
		       bool				demangle,
		       vector<elf_symbol_sptr>&	symbols);

bool
lookup_public_function_symbol_from_elf(const environment*		env,
				       const string&			path,
				       const string&			symname,
				       vector<elf_symbol_sptr>&	func_syms);

status
has_alt_debug_info(read_context&	elf_path,
		   bool&		has_alt_di,
		   string&		alt_debug_info_path);

status
has_alt_debug_info(const string&	elf_path,
		   char**		debug_info_root_path,
		   bool&		has_alt_di,
		   string&		alt_debug_info_path);

bool
get_soname_of_elf_file(const string& path, string& soname);

bool
get_type_of_elf_file(const string& path, elf_type& type);


void
set_debug_info_root_path(read_context& ctxt,
			 char** path);

char**
get_debug_info_root_path(read_context& ctxt,
			 char**& path);

bool
get_show_stats(read_context& ctxt);

void
set_show_stats(read_context& ctxt,
	       bool f);

void
set_do_log(read_context& ctxt, bool f);

void
set_environment(read_context& ctxt,
		ir::environment*);

const environment_sptr&
get_environment(const read_context& ctxt);

environment_sptr&
get_environment(read_context& ctxt);
}// end namespace dwarf_reader

}// end namespace abigail

#endif //__ABG_DWARF_READER_H__

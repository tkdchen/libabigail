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

// Author: Dodji Seketeli

/// @file
///
/// This program runs a diff between input ELF files containing DWARF
/// debugging information and compares the resulting report with a
/// reference report.  If the resulting report is different from the
/// reference report, the test has failed.  Note that the comparison
/// is done using the abidiff command line comparison tool.
///
/// The set of input files and reference reports to consider should be
/// present in the source distribution.

#include <sys/wait.h>
#include <cassert>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "abg-workers.h"
#include "abg-tools-utils.h"
#include "test-utils.h"

using std::string;
using std::cerr;

/// This is an aggregate that specifies where a test shall get its
/// input from and where it shall write its ouput to.
struct InOutSpec
{
  const char* in_elfv0_path;
  const char* in_elfv1_path;
  const char* abidiff_options;
  const char* in_report_path;
  const char* out_report_path;
}; // end struct InOutSpec;

InOutSpec in_out_specs[] =
{
  {
    "data/test-diff-filter/test0-v0.o",
    "data/test-diff-filter/test0-v1.o",
    "--no-default-suppression --no-linkage-name --no-show-locs --no-redundant",
    "data/test-diff-filter/test0-report.txt",
    "output/test-diff-filter/test0-report.txt",
  },
  {
    "data/test-diff-filter/test0-v0.o",
    "data/test-diff-filter/test0-v1.o",
    "--no-default-suppression --harmless --no-linkage-name "
    "--no-show-locs --no-redundant",
    "data/test-diff-filter/test01-report.txt",
    "output/test-diff-filter/test01-report.txt",
  },
  {
    "data/test-diff-filter/test1-v0.o",
    "data/test-diff-filter/test1-v1.o",
    "--no-default-suppression --no-linkage-name --no-show-locs --no-redundant",
    "data/test-diff-filter/test1-report.txt",
    "output/test-diff-filter/test1-report.txt",
  },
  {
    "data/test-diff-filter/test2-v0.o",
    "data/test-diff-filter/test2-v1.o",
    "--no-default-suppression --no-linkage-name --no-show-locs --no-redundant",
    "data/test-diff-filter/test2-report.txt",
    "output/test-diff-filter/test2-report.txt",
  },
  {
    "data/test-diff-filter/test3-v0.o",
    "data/test-diff-filter/test3-v1.o",
    "--no-default-suppression --no-linkage-name --no-show-locs --no-redundant",
    "data/test-diff-filter/test3-report.txt",
    "output/test-diff-filter/test3-report.txt",
  },
  {
    "data/test-diff-filter/test4-v0.o",
    "data/test-diff-filter/test4-v1.o",
    "--no-default-suppression --no-linkage-name --no-show-locs --no-redundant",
    "data/test-diff-filter/test4-report.txt",
    "output/test-diff-filter/test4-report.txt",
  },
  {
    "data/test-diff-filter/test5-v0.o",
    "data/test-diff-filter/test5-v1.o",
    "--no-default-suppression --no-linkage-name --no-show-locs --no-redundant",
    "data/test-diff-filter/test5-report.txt",
    "output/test-diff-filter/test5-report.txt",
  },
  {
    "data/test-diff-filter/test6-v0.o",
    "data/test-diff-filter/test6-v1.o",
    "--no-default-suppression --no-linkage-name --no-show-locs --no-redundant",
    "data/test-diff-filter/test6-report.txt",
    "output/test-diff-filter/test6-report.txt",
  },
  {
    "data/test-diff-filter/test7-v0.o",
    "data/test-diff-filter/test7-v1.o",
    "--no-default-suppression --no-linkage-name --no-show-locs --no-redundant",
    "data/test-diff-filter/test7-report.txt",
    "output/test-diff-filter/test7-report.txt",
  },
  {
    "data/test-diff-filter/test8-v0.o",
    "data/test-diff-filter/test8-v1.o",
    "--no-default-suppression --no-linkage-name --no-show-locs --no-redundant",
    "data/test-diff-filter/test8-report.txt",
    "output/test-diff-filter/test8-report.txt",
  },
  {
    "data/test-diff-filter/test9-v0.o",
    "data/test-diff-filter/test9-v1.o",
    "--no-default-suppression --no-linkage-name --no-show-locs --no-redundant",
    "data/test-diff-filter/test9-report.txt",
    "output/test-diff-filter/test9-report.txt",
  },
  {
    "data/test-diff-filter/test10-v0.o",
    "data/test-diff-filter/test10-v1.o",
    "--no-default-suppression --no-linkage-name --no-show-locs --no-redundant",
    "data/test-diff-filter/test10-report.txt",
    "output/test-diff-filter/test10-report.txt",
  },
  {
    "data/test-diff-filter/test11-v0.o",
    "data/test-diff-filter/test11-v1.o",
    "--no-default-suppression --no-linkage-name --no-show-locs --no-redundant",
    "data/test-diff-filter/test11-report.txt",
    "output/test-diff-filter/test11-report.txt",
  },
  {
    "data/test-diff-filter/test12-v0.o",
    "data/test-diff-filter/test12-v1.o",
    "--no-default-suppression --no-linkage-name --no-show-locs --no-redundant",
    "data/test-diff-filter/test12-report.txt",
    "output/test-diff-filter/test12-report.txt",
  },
  {
    "data/test-diff-filter/test13-v0.o",
    "data/test-diff-filter/test13-v1.o",
    "--no-default-suppression --no-linkage-name --no-show-locs --no-redundant",
    "data/test-diff-filter/test13-report.txt",
    "output/test-diff-filter/test13-report.txt",
  },
  {
    "data/test-diff-filter/test14-v0.o",
    "data/test-diff-filter/test14-v1.o",
    "--no-default-suppression --no-show-locs --no-redundant",
    "data/test-diff-filter/test14-0-report.txt",
    "output/test-diff-filter/test14-0-report.txt",
  },
  {
    "data/test-diff-filter/test14-v0.o",
    "data/test-diff-filter/test14-v1.o",
    "--no-default-suppression --no-show-locs --redundant",
    "data/test-diff-filter/test14-1-report.txt",
    "output/test-diff-filter/test14-1-report.txt",
  },
  {
    "data/test-diff-filter/test15-v0.o",
    "data/test-diff-filter/test15-v1.o",
    "--no-default-suppression --no-show-locs --no-redundant",
    "data/test-diff-filter/test15-0-report.txt",
    "output/test-diff-filter/test15-0-report.txt",
  },
  {
    "data/test-diff-filter/test15-v0.o",
    "data/test-diff-filter/test15-v1.o",
    "--no-default-suppression --no-show-locs --redundant",
    "data/test-diff-filter/test15-1-report.txt",
    "output/test-diff-filter/test15-1-report.txt",
  },
  {
    "data/test-diff-filter/test16-v0.o",
    "data/test-diff-filter/test16-v1.o",
    "--no-default-suppression --no-show-locs --no-redundant",
    "data/test-diff-filter/test16-report.txt",
    "output/test-diff-filter/test16-report.txt",
  },
  {
    "data/test-diff-filter/test16-v0.o",
    "data/test-diff-filter/test16-v1.o",
    "--no-default-suppression --no-show-locs --redundant",
    "data/test-diff-filter/test16-report-2.txt",
    "output/test-diff-filter/test16-report-2.txt",
  },
  {
    "data/test-diff-filter/test17-v0.o",
    "data/test-diff-filter/test17-v1.o",
    "--no-default-suppression --no-show-locs --no-redundant",
    "data/test-diff-filter/test17-0-report.txt",
    "output/test-diff-filter/test17-0-report.txt",
  },
  {
    "data/test-diff-filter/test17-v0.o",
    "data/test-diff-filter/test17-v1.o",
    "--no-default-suppression --no-show-locs --redundant",
    "data/test-diff-filter/test17-1-report.txt",
    "output/test-diff-filter/test17-1-report.txt",
  },
  {
    "data/test-diff-filter/test18-v0.o",
    "data/test-diff-filter/test18-v1.o",
    "--no-default-suppression --no-show-locs --no-redundant",
    "data/test-diff-filter/test18-report.txt",
    "output/test-diff-filter/test18-report.txt",
  },
  {
    "data/test-diff-filter/test19-enum-v0.o",
    "data/test-diff-filter/test19-enum-v1.o",
    "--no-default-suppression --no-show-locs --no-redundant",
    "data/test-diff-filter/test19-enum-report-0.txt",
    "output/test-diff-filter/test19-enum-report-0.txt",
  },
  {
    "data/test-diff-filter/test19-enum-v0.o",
    "data/test-diff-filter/test19-enum-v1.o",
    "--no-default-suppression --no-show-locs --harmless",
    "data/test-diff-filter/test19-enum-report-1.txt",
    "output/test-diff-filter/test19-enum-report-1.txt",
  },
  {
    "data/test-diff-filter/test20-inline-v0.o",
    "data/test-diff-filter/test20-inline-v1.o",
    "--no-default-suppression --no-show-locs --no-redundant",
    "data/test-diff-filter/test20-inline-report-0.txt",
    "output/test-diff-filter/test20-inline-report-0.txt",
  },
  {
    "data/test-diff-filter/test20-inline-v0.o",
    "data/test-diff-filter/test20-inline-v1.o",
    "--no-default-suppression --no-show-locs --harmless",
    "data/test-diff-filter/test20-inline-report-1.txt",
    "output/test-diff-filter/test20-inline-report-1.txt",
  },
  {
    "data/test-diff-filter/libtest21-compatible-vars-v0.so",
    "data/test-diff-filter/libtest21-compatible-vars-v1.so",
    "--no-default-suppression --no-show-locs --harmless",
    "data/test-diff-filter/test21-compatible-vars-report-0.txt",
    "output/test-diff-filter/test21-compatible-vars-report-0.txt",
  },
  {
    "data/test-diff-filter/libtest21-compatible-vars-v0.so",
    "data/test-diff-filter/libtest21-compatible-vars-v1.so",
    "--no-default-suppression --no-show-locs --no-redundant",
    "data/test-diff-filter/test21-compatible-vars-report-1.txt",
    "output/test-diff-filter/test21-compatible-vars-report-1.txt",
  },
  {
    "data/test-diff-filter/libtest22-compatible-fns-v0.so",
    "data/test-diff-filter/libtest22-compatible-fns-v1.so",
    "--no-default-suppression --no-show-locs --harmless",
    "data/test-diff-filter/test22-compatible-fns-report-0.txt",
    "output/test-diff-filter/test22-compatible-fns-report-0.txt",
  },
  {
    "data/test-diff-filter/libtest22-compatible-fns-v0.so",
    "data/test-diff-filter/libtest22-compatible-fns-v1.so",
    "--no-default-suppression --no-show-locs --no-redundant",
    "data/test-diff-filter/test22-compatible-fns-report-1.txt",
    "output/test-diff-filter/test22-compatible-fns-report-1.txt",
  },
  {
    "data/test-diff-filter/libtest23-redundant-fn-parm-change-v0.so",
    "data/test-diff-filter/libtest23-redundant-fn-parm-change-v1.so",
    "--no-default-suppression --no-show-locs",
    "data/test-diff-filter/test23-redundant-fn-parm-change-report-0.txt ",
    "output/test-diff-filter/test23-redundant-fn-parm-change-report-0.txt ",
  },
  {
    "data/test-diff-filter/libtest24-compatible-vars-v0.so",
    "data/test-diff-filter/libtest24-compatible-vars-v1.so",
    "--no-default-suppression --no-show-locs",
    "data/test-diff-filter/test24-compatible-vars-report-0.txt ",
    "output/test-diff-filter/test24-compatible-vars-report-0.txt ",
  },
  {
    "data/test-diff-filter/libtest24-compatible-vars-v0.so",
    "data/test-diff-filter/libtest24-compatible-vars-v1.so",
    "--no-default-suppression --no-show-locs --harmless",
    "data/test-diff-filter/test24-compatible-vars-report-1.txt ",
    "output/test-diff-filter/test24-compatible-vars-report-1.txt ",
  },
  {
    "data/test-diff-filter/libtest25-cyclic-type-v0.so",
    "data/test-diff-filter/libtest25-cyclic-type-v1.so",
    "--no-default-suppression --no-show-locs",
    "data/test-diff-filter/test25-cyclic-type-report-0.txt ",
    "output/test-diff-filter/test25-cyclic-type-report-0.txt "
  },
  {
    "data/test-diff-filter/libtest25-cyclic-type-v0.so",
    "data/test-diff-filter/libtest25-cyclic-type-v1.so",
    "--no-default-suppression --no-show-locs --redundant",
    "data/test-diff-filter/test25-cyclic-type-report-1.txt ",
    "output/test-diff-filter/test25-cyclic-type-report-1.txt "
  },
  {
    "data/test-diff-filter/libtest26-qualified-redundant-node-v0.so",
    "data/test-diff-filter/libtest26-qualified-redundant-node-v1.so",
    "--no-default-suppression --no-show-locs",
    "data/test-diff-filter/test26-qualified-redundant-node-report-0.txt",
    "output/test-diff-filter/test26-qualified-redundant-node-report-0.txt"
  },
  {
    "data/test-diff-filter/libtest26-qualified-redundant-node-v0.so",
    "data/test-diff-filter/libtest26-qualified-redundant-node-v1.so",
    "--no-default-suppression --no-show-locs --redundant",
    "data/test-diff-filter/test26-qualified-redundant-node-report-1.txt",
    "output/test-diff-filter/test26-qualified-redundant-node-report-1.txt"
  },
  {
    "data/test-diff-filter/libtest27-redundant-and-filtered-children-nodes-v0.so",
    "data/test-diff-filter/libtest27-redundant-and-filtered-children-nodes-v1.so",
    "--no-default-suppression --no-linkage-name --no-show-locs --no-redundant",
    "data/test-diff-filter/test27-redundant-and-filtered-children-nodes-report-0.txt",
    "output/test-diff-filter/test27-redundant-and-filtered-children-nodes-report-0.txt"
  },
  {
    "data/test-diff-filter/libtest27-redundant-and-filtered-children-nodes-v0.so",
    "data/test-diff-filter/libtest27-redundant-and-filtered-children-nodes-v1.so",
    "--no-default-suppression --no-linkage-name --no-show-locs --redundant",
    "data/test-diff-filter/test27-redundant-and-filtered-children-nodes-report-1.txt",
    "output/test-diff-filter/test27-redundant-and-filtered-children-nodes-report-1.txt"
  },
  {
    "data/test-diff-filter/libtest27-redundant-and-filtered-children-nodes-v0.so",
    "data/test-diff-filter/libtest27-redundant-and-filtered-children-nodes-v1.so",
    "--no-default-suppression --no-linkage-name --redundant --no-show-locs --harmless",
    "data/test-diff-filter/test27-redundant-and-filtered-children-nodes-report-2.txt",
    "output/test-diff-filter/test27-redundant-and-filtered-children-nodes-report-2.txt"
  },
  {
    "data/test-diff-filter/libtest28-redundant-and-filtered-children-nodes-v0.so",
    "data/test-diff-filter/libtest28-redundant-and-filtered-children-nodes-v1.so",
    "--no-default-suppression --no-linkage-name --no-show-locs --no-redundant",
   "data/test-diff-filter/test28-redundant-and-filtered-children-nodes-report-0.txt",
    "output/test-diff-filter/test28-redundant-and-filtered-children-nodes-report-0.txt",
  },
  {
    "data/test-diff-filter/libtest28-redundant-and-filtered-children-nodes-v0.so",
    "data/test-diff-filter/libtest28-redundant-and-filtered-children-nodes-v1.so",
    "--no-default-suppression --no-linkage-name --redundant --no-show-locs --harmless",
   "data/test-diff-filter/test28-redundant-and-filtered-children-nodes-report-1.txt",
    "output/test-diff-filter/test28-redundant-and-filtered-children-nodes-report-1.txt",
  },
  {
    "data/test-diff-filter/test29-finer-redundancy-marking-v0.o",
    "data/test-diff-filter/test29-finer-redundancy-marking-v1.o",
    "--no-default-suppression --no-linkage-name --no-show-locs --no-redundant",
    "data/test-diff-filter/test29-finer-redundancy-marking-report-0.txt",
    "output/test-diff-filter/test29-finer-redundancy-marking-report-0.txt",
  },
  {
    "data/test-diff-filter/test30-pr18904-rvalueref-liba.so",
    "data/test-diff-filter/test30-pr18904-rvalueref-libb.so",
    "--no-default-suppression --no-linkage-name --no-show-locs --no-redundant",
    "data/test-diff-filter/test30-pr18904-rvalueref-report0.txt",
    "output/test-diff-filter/test30-pr18904-rvalueref-report0.txt",
  },
  { // Just like the previous test, but emit loc info.
    "data/test-diff-filter/test30-pr18904-rvalueref-liba.so",
    "data/test-diff-filter/test30-pr18904-rvalueref-libb.so",
    "--no-default-suppression --no-linkage-name --no-redundant",
    "data/test-diff-filter/test30-pr18904-rvalueref-report1.txt",
    "output/test-diff-filter/test30-pr18904-rvalueref-report1.txt",
  },
  {
    "data/test-diff-filter/test31-pr18535-libstdc++-4.8.3.so",
    "data/test-diff-filter/test31-pr18535-libstdc++-4.9.2.so",
    "--no-default-suppression --no-linkage-name --no-show-locs --no-redundant",
    "data/test-diff-filter/test31-pr18535-libstdc++-report-0.txt",
    "output/test-diff-filter/test31-pr18535-libstdc++-report-0.txt",
  },
  { // Just like the previous test, but emit loc info.
    "data/test-diff-filter/test31-pr18535-libstdc++-4.8.3.so",
    "data/test-diff-filter/test31-pr18535-libstdc++-4.9.2.so",
    "--no-default-suppression --no-linkage-name --no-redundant",
    "data/test-diff-filter/test31-pr18535-libstdc++-report-1.txt",
    "output/test-diff-filter/test31-pr18535-libstdc++-report-1.txt",
  },
  {
    "data/test-diff-filter/libtest32-struct-change-v0.so",
    "data/test-diff-filter/libtest32-struct-change-v1.so",
    "--no-default-suppression --no-linkage-name --no-show-locs --no-redundant",
    "data/test-diff-filter/test32-ppc64le-struct-change-report0.txt",
    "output/test-diff-filter/test32-ppc64le-struct-change-report0.txt",
  },
  // This should be the last entry
  {NULL, NULL, NULL, NULL, NULL}
};

/// A task which launches abidiff on the binaries passed to the
/// constructor of the task.  The test also launches gnu diff on the
/// result of the abidiff to compare it against a reference abidiff
/// result.
struct test_task : public abigail::workers::task
{
  InOutSpec spec;
  bool is_ok;
  string diff_cmd;
  string error_message;

  test_task(const InOutSpec& s)
    : spec(s),
      is_ok(true)
  {}

  /// This virtual function overload actually performs the job of the
  /// task.
  ///
  /// It actually launches abidiff on the binaries passed to the
  /// constructor of the task.  It also launches gnu diff on the
  /// result of the abidiff to compare it against a reference abidiff
  /// result.
  virtual void
  perform()
  {
    using abigail::tests::get_src_dir;
    using abigail::tests::get_build_dir;
    using abigail::tools_utils::ensure_parent_dir_created;
    using abigail::tools_utils::abidiff_status;

    string in_elfv0_path, in_elfv1_path,
      abidiff_options, abidiff, cmd,
      ref_diff_report_path, out_diff_report_path;

    in_elfv0_path = string(get_src_dir()) + "/tests/" + spec.in_elfv0_path;
    in_elfv1_path = string(get_src_dir()) + "/tests/" + spec.in_elfv1_path;
    abidiff_options = spec.abidiff_options;
    ref_diff_report_path =
      string(get_src_dir()) + "/tests/" + spec.in_report_path;
    out_diff_report_path =
      string(get_build_dir()) + "/tests/" + spec.out_report_path;

    if (!ensure_parent_dir_created(out_diff_report_path))
      {
	error_message = string("could not create parent directory for ")
	  + out_diff_report_path;
	is_ok = false;
	return;
      }

    abidiff = string(get_build_dir()) + "/tools/abidiff";
    abidiff += " " + abidiff_options;

    cmd = abidiff + " " + in_elfv0_path + " " + in_elfv1_path;
    cmd += " > " + out_diff_report_path;

    bool abidiff_ok = true;
    int code = system(cmd.c_str());
    if (!WIFEXITED(code))
      abidiff_ok = false;
    else
      {
	abidiff_status status =
	  static_cast<abidiff_status>(WEXITSTATUS(code));
	if (abigail::tools_utils::abidiff_status_has_error(status))
	  abidiff_ok = false;
      }

    if (abidiff_ok)
      {
	cmd = "diff -u " + ref_diff_report_path
	  + " " + out_diff_report_path;

	string cmd_no_out = cmd + " > /dev/null";
	if (system(cmd_no_out.c_str()))
	  {
	    is_ok = false;
	    diff_cmd = cmd;
	  }
      }
    else
      is_ok = false;
  }
}; //end struct test_task.

/// A convenience typedef for shared
typedef shared_ptr<test_task> test_task_sptr;

int
main()
{
  using std::vector;
  using std::tr1::dynamic_pointer_cast;
  using abigail::workers::queue;
  using abigail::workers::task;
  using abigail::workers::task_sptr;
  using abigail::workers::get_number_of_threads;

  /// Create a task queue.  The max number of worker threads of the
  /// queue is the number of the concurrent threads supported by the
  /// processor of the machine this code runs on.
  const size_t num_tests = sizeof(in_out_specs) / sizeof (InOutSpec) - 1;
  size_t num_workers = std::min(get_number_of_threads(), num_tests);
  queue task_queue(num_workers);

  bool is_ok = true;

  for (InOutSpec* s = in_out_specs; s->in_elfv0_path; ++s)
    {
      test_task_sptr t(new test_task(*s));
      assert(task_queue.schedule_task(t));
    }

  /// Wait for all worker threads to finish their job, and wind down.
  task_queue.wait_for_workers_to_complete();

  // Now walk the results and print whatever error messages need to be
  // printed.

  const vector<task_sptr>& completed_tasks =
    task_queue.get_completed_tasks();

  assert(completed_tasks.size() == num_tests);

  for (vector<task_sptr>::const_iterator ti = completed_tasks.begin();
       ti != completed_tasks.end();
       ++ti)
    {
      test_task_sptr t = dynamic_pointer_cast<test_task>(*ti);
      if (!t->is_ok)
	{
	  is_ok = false;
	  if (!t->diff_cmd.empty())
	    system(t->diff_cmd.c_str());
	  if (!t->error_message.empty())
	    cerr << t->error_message << '\n';
	}
    }

  return !is_ok;
}

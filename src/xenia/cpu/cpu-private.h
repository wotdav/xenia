/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2013 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#ifndef XENIA_CPU_PRIVATE_H_
#define XENIA_CPU_PRIVATE_H_

#include <gflags/gflags.h>

DECLARE_bool(trace_function_generation);
DECLARE_bool(trace_kernel_calls);
DECLARE_bool(trace_user_calls);
DECLARE_bool(trace_instructions);
DECLARE_bool(trace_registers);
DECLARE_string(load_module_map);

DECLARE_string(dump_path);
DECLARE_bool(dump_module_map);

DECLARE_bool(debug);
DECLARE_bool(always_disasm);

DECLARE_bool(validate_hir);

DECLARE_uint64(break_on_instruction);
DECLARE_uint64(break_on_memory);
DECLARE_bool(break_on_debugbreak);

#endif  // XENIA_CPU_PRIVATE_H_
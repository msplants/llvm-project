//===-- ldexp_differential_fuzz.cpp ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// Differential fuzz test for llvm-libc ldexp implementation.
///
//===----------------------------------------------------------------------===//

#include "fuzzing/math/LdExpDiff.h"
#include "src/math/ldexp.h"

#include <math.h>
#include <stddef.h>
#include <stdint.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  LdExpDiff<double>(&__llvm_libc::ldexp, &::ldexp, data, size);
  return 0;
}

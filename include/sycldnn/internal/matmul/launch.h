/*
 * Copyright Codeplay Software Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef SYCLDNN_INCLUDE_INTERNAL_MATMUL_LAUNCH_H_
#define SYCLDNN_INCLUDE_INTERNAL_MATMUL_LAUNCH_H_

#include <CL/sycl.hpp>

#include "sycldnn/mem_object.h"
#include "sycldnn/status.h"

#include "sycldnn/matmul/params.h"

#include "sycldnn/export.h"

namespace sycldnn {
namespace matmul {
namespace internal {

/**
 * The internal matrix multiply launcher.
 *
 * Implemented in the compiled SYCL DNN library.
 */
template <typename T, bool TransposeLHS, bool TransposeRHS>
SNN_EXPORT SNNStatus launch(BaseMemObject<T const>& lhs,
                            BaseMemObject<T const>& rhs,
                            BaseMemObject<T>& output,
                            MatmulParams const& params, cl::sycl::queue& queue);

}  // namespace internal
}  // namespace matmul
}  // namespace sycldnn

#endif  // SYCLDNN_INCLUDE_INTERNAL_MATMUL_LAUNCH_H_

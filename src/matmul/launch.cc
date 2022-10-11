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
#include "sycldnn/internal/matmul/launch.h"
#include "sycldnn/matmul/params.h"

#include "sycldnn/mem_object.h"

#include "src/matmul/queue_kernel.h"

namespace sycldnn {
namespace matmul {
namespace internal {
namespace {

// Launch the kernel specified by the template parameters.
template <typename T, bool TransposeLHS, bool TransposeRHS, int RowTile,
          int AccTile, int ColTile>
SNNStatus launch_with_tiles(BaseMemObject<T const>& lhs,
                            BaseMemObject<T const>& rhs,
                            BaseMemObject<T>& output,
                            MatmulParams const& params, cl::sycl::queue& queue,
                            size_t wg_rows, size_t wg_cols, size_t wg_batch) {
  auto kernel = ((params.m % RowTile == 0) && (params.k % AccTile == 0) &&
                 (params.n % ColTile == 0))
                    ? queue_kernel<T, int, TransposeLHS, TransposeRHS, RowTile,
                                   AccTile, ColTile, false>
                    : queue_kernel<T, int, TransposeLHS, TransposeRHS, RowTile,
                                   AccTile, ColTile, true>;
  return kernel(lhs, rhs, output, params, queue, wg_rows, wg_cols, wg_batch);
}

}  // namespace

// Launch the matrix multiply kernel for the passed parameters.
template <typename T, bool TransposeLHS, bool TransposeRHS>
SNNStatus launch(BaseMemObject<T const>& lhs, BaseMemObject<T const>& rhs,
                 BaseMemObject<T>& output, MatmulParams const& params,
                 cl::sycl::queue& queue) {
  return launch_with_tiles<T, TransposeLHS, TransposeRHS, 4, 4, 4>(
      lhs, rhs, output, params, queue, 8, 4, 1);
}

#define INSTANTIATE_LAUNCHER(DTYPE, TLHS, TRHS)                                \
  template SNN_EXPORT SNNStatus launch<DTYPE, TLHS, TRHS>(                     \
      BaseMemObject<DTYPE const> & input, BaseMemObject<DTYPE const> & filter, \
      BaseMemObject<DTYPE> & output, MatmulParams const& params,               \
      cl::sycl::queue& queue);

#define INSTANTIATE_FOR_TYPE(DTYPE)        \
  INSTANTIATE_LAUNCHER(DTYPE, true, true)  \
  INSTANTIATE_LAUNCHER(DTYPE, false, true) \
  INSTANTIATE_LAUNCHER(DTYPE, true, false) \
  INSTANTIATE_LAUNCHER(DTYPE, false, false)

INSTANTIATE_FOR_TYPE(float);

#ifdef SNN_USE_DOUBLE
INSTANTIATE_FOR_TYPE(double);
#endif  // SNN_USE_DOUBLE

#ifdef SNN_USE_HALF
INSTANTIATE_FOR_TYPE(cl::sycl::half);
#endif  // SNN_USE_HALF

#undef INSTANTIATE_FOR_TYPE
#undef INSTANTIATE_LAUNCHER

}  // namespace internal
}  // namespace matmul
}  // namespace sycldnn

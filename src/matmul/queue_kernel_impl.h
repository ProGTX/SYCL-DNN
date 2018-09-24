/*
 * Copyright 2018 Codeplay Software Ltd
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
#ifndef SYCLDNN_SRC_MATMUL_QUEUE_KERNEL_IMPL_H_
#define SYCLDNN_SRC_MATMUL_QUEUE_KERNEL_IMPL_H_

#include "sycldnn/accessor_types.h"
#include "sycldnn/status.h"

#include "sycldnn/helpers/ratio.h"

#include "src/matmul/kernels.h"
#include "src/matmul/queue_kernel.h"

namespace sycldnn {
namespace matmul {
namespace internal {

template <typename T, typename Index, bool TransposeLHS, bool TransposeRHS,
          int RowTile, int AccTile, int ColTile>
SNNStatus queue_kernel(ReadAccessor<T const> lhs, ReadAccessor<T const> rhs,
                       ReadWriteAccessor<T> output, int batches, int m, int k,
                       int n, T beta, cl::sycl::queue& queue) {
  using Functor = MatmulKernel<T, Index, TransposeLHS, TransposeRHS, RowTile,
                               AccTile, ColTile>;
  Index constexpr pow_two_multiple = 8;

  Index const output_size_row = helpers::round_ratio_up(m, RowTile);
  Index const output_size_col = helpers::round_ratio_up(n, ColTile);
  size_t const n_row_threads =
      helpers::round_up_to_nearest_multiple(output_size_row, pow_two_multiple);
  size_t const n_col_threads =
      helpers::round_up_to_nearest_multiple(output_size_col, pow_two_multiple);
  size_t const n_batch_threads = static_cast<size_t>(batches);

  auto event = queue.submit([&](cl::sycl::handler& cgh) {
    cgh.require(lhs);
    cgh.require(rhs);
    cgh.require(output);

    Index const lhs_offset = lhs.get_offset().get(0);
    Index const rhs_offset = rhs.get_offset().get(0);
    Index const out_offset = output.get_offset().get(0);

    Functor functor{lhs, rhs,  output,     batches,    m,         k,
                    n,   beta, lhs_offset, rhs_offset, out_offset};

    cgh.parallel_for(
        cl::sycl::range<3>{n_row_threads, n_col_threads, n_batch_threads},
        functor);
  });
  return {event, StatusCode::OK};
}

}  // namespace internal
}  // namespace matmul
}  // namespace sycldnn
#endif  // SYCLDNN_SRC_MATMUL_QUEUE_KERNEL_IMPL_H_
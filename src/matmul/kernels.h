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
#ifndef SYCLDNN_SRC_MATMUL_KERNELS_H_
#define SYCLDNN_SRC_MATMUL_KERNELS_H_

#include "sycldnn/accessor_types.h"
#include "sycldnn/status.h"

#include "src/matmul/blocks.h"

namespace sycldnn {
namespace matmul {
template <typename T, typename Index, bool TransposeLHS, bool TransposeRHS,
          int RowTile, int AccTile, int ColTile>
struct MatmulKernel {
  MatmulKernel(ReadAccessor<T const> const& lhs,
               ReadAccessor<T const> const& rhs,
               ReadWriteAccessor<T> const& output, Index batches, Index m,
               Index k, Index n, T beta, Index lhs_offset, Index rhs_offset,
               Index out_offset)
      : lhs_{lhs},
        rhs_{rhs},
        output_{output},
        batches_{batches},
        m_{m},
        k_{k},
        n_{n},
        beta_{beta},
        lhs_offset_{lhs_offset},
        rhs_offset_{rhs_offset},
        out_offset_{out_offset} {};

  void SNN_ALWAYS_INLINE operator()(cl::sycl::item<3> item) {
    Index row = item.get_id(0) * RowTile;
    Index col = item.get_id(1) * ColTile;
    Index batch = item.get_id(2);

    if (row < m_ && col < n_) {
      auto lhs_ptr = lhs_.get_pointer() + batch * m_ * k_ + lhs_offset_;
      auto rhs_ptr = rhs_.get_pointer() + batch * k_ * n_ + rhs_offset_;
      auto out_ptr = output_.get_pointer() + batch * m_ * n_ + out_offset_;

      auto out_block = VectorBlock<T, RowTile, ColTile>{};
      if (beta_ != static_cast<T>(0)) {
        // Convert out_ptr from multi_ptr<T> to multi_ptr<T const>
        auto const_out_ptr =
            cl::sycl::multi_ptr<T const,
                                cl::sycl::access::address_space::global_space>{
                out_ptr.get()};
        out_block =
            load<RowTile, ColTile, false>(const_out_ptr, row, col, m_, n_);
        scalar_multiply(out_block, beta_);
      }

      for (Index acc_idx = 0; acc_idx < k_; acc_idx += AccTile) {
        auto lhs_block =
            load<RowTile, AccTile, TransposeLHS>(lhs_ptr, row, acc_idx, m_, k_);
        auto rhs_block =
            load<AccTile, ColTile, TransposeRHS>(rhs_ptr, acc_idx, col, k_, n_);
        block_mmacc(lhs_block, rhs_block, out_block);
      }
      store_block(out_block, out_ptr, row, col, n_, m_, n_);
    }
  }

 private:
  ReadAccessor<T const> lhs_;
  ReadAccessor<T const> rhs_;
  ReadWriteAccessor<T> output_;
  Index const batches_;
  Index const m_;
  Index const k_;
  Index const n_;
  T const beta_;
  Index const lhs_offset_;
  Index const rhs_offset_;
  Index const out_offset_;
};

}  // namespace matmul
}  // namespace sycldnn
#endif  // SYCLDNN_SRC_MATMUL_KERNELS_H_
/*
 * Copyright 2018 Codeplay Software Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use these files except in compliance with the License.
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
#ifndef SYCLDNN_BENCH_CONV2D_SNN_CONV2D_EXECUTOR_H_
#define SYCLDNN_BENCH_CONV2D_SNN_CONV2D_EXECUTOR_H_

#include "sycldnn/conv2d/launch.h"
#include "sycldnn/conv2d/params.h"
#include "sycldnn/conv2d/selector/selector.h"

namespace sycldnn {
namespace bench {

/** Executor to perform the Conv2d benchmark using SYCL-DNN.  */
template <typename Benchmark, typename ConvType>
struct SNNConv2DExecutor {
 private:
  using State = ::benchmark::State;
  using Conv2DParams = conv2d::Conv2DParams;
  using Selector = conv2d::Selector;

  /** Get a reference to the underlying benchmark fixture. */
  Benchmark& underlying_benchmark() { return static_cast<Benchmark&>(*this); }

 public:
  /** Execute a conv2d benchmark with the given parameters and selector. */
  void execute(State& state, Conv2DParams const& params, Selector& selector) {
    auto& benchmark = underlying_benchmark();
    auto backend = benchmark.get_backend();

    auto conv_sizes = sycldnn::conv2d::get_sizes<ConvType>(params);

    auto inp_gpu = benchmark.template allocate<float>(conv_sizes.input_size);
    auto fil_gpu = benchmark.template allocate<float>(conv_sizes.filter_size);
    auto out_gpu = benchmark.template allocate<float>(conv_sizes.output_size);

    {  // Ensure the kernel is built before benchmarking
      auto status = sycldnn::conv2d::launch<float, ConvType>(
          inp_gpu, fil_gpu, out_gpu, params, selector, backend);
      status.event.wait();

      if (sycldnn::StatusCode::OK != status.status) {
        state.SkipWithError(
            "Invalid or unsupported benchmark configuration. "
            "This may be expected behaviour and does not indicate a problem.");
        return;
      }
    }

    for (auto _ : state) {
      auto start = std::chrono::high_resolution_clock::now();
      auto status = sycldnn::conv2d::launch<float, ConvType>(
          inp_gpu, fil_gpu, out_gpu, params, selector, backend);

      status.event.wait();
      auto end = std::chrono::high_resolution_clock::now();

      auto elapsed_seconds =
          std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                    start);

      state.SetIterationTime(elapsed_seconds.count());
    }

    benchmark.deallocate(out_gpu);
    benchmark.deallocate(fil_gpu);
    benchmark.deallocate(inp_gpu);

    benchmark.template set_items_processed<ConvType>(state, params);
    benchmark.add_param_counters(state, params);
    benchmark.template add_bandwidth_counters<float>(state, conv_sizes);
  }
};

}  // namespace bench
}  // namespace sycldnn

#endif  // SYCLDNN_BENCH_CONV2D_SNN_CONV2D_EXECUTOR_H_
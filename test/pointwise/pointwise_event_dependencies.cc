/*
 * Copyright Codeplay Software Ltd.
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

// DO NOT MODIFY BY HAND
// This file was automatically generated by generate_pointwise_tests.py.
// Results calculated using Tensorflow v2.8.0.

#include <gtest/gtest.h>

#include "sycldnn/pointwise/direction.h"
#include "sycldnn/pointwise/operators.h"

#include "test/helpers/dependency_check.h"
#include "test/pointwise/pointwise_event_dependencies_fixture.h"
#include "test/types/cartesian_product.h"
#include "test/types/kernel_data_types.h"
#include "test/types/test_backend_types.h"

#include <vector>

using namespace sycldnn;  // NOLINT(google-build-using-namespace)

template <typename Pair>
using FloorForward =
    PointwiseEventFixture<Pair, pointwise::Floor, pointwise::Forward>;
TYPED_TEST_SUITE(FloorForward, types::GTestKernelDataTypes);
TYPED_TEST(FloorForward, Shape_1x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(1, 1);
  this->test_pointwise(input);
}
TYPED_TEST(FloorForward, Shape_8x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(8, 8);
  this->test_pointwise(input);
}
TYPED_TEST(FloorForward, Shape_9x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(9, 9);
  this->test_pointwise(input);
}
TYPED_TEST(FloorForward, Shape_10x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(10, 10);
  this->test_pointwise(input);
}

template <typename Pair>
using LogForward =
    PointwiseEventFixture<Pair, pointwise::Log, pointwise::Forward>;
TYPED_TEST_SUITE(LogForward, types::GTestKernelDataTypes);
TYPED_TEST(LogForward, Shape_1x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(1, 1);
  this->test_pointwise(input);
}
TYPED_TEST(LogForward, Shape_8x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(8, 8);
  this->test_pointwise(input);
}
TYPED_TEST(LogForward, Shape_9x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(9, 9);
  this->test_pointwise(input);
}
TYPED_TEST(LogForward, Shape_10x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(10, 10);
  this->test_pointwise(input);
}

template <typename Pair>
using LogGrad =
    PointwiseEventFixture<Pair, pointwise::Log, pointwise::Gradient>;
TYPED_TEST_SUITE(LogGrad, types::GTestKernelDataTypes);
TYPED_TEST(LogGrad, Shape_1x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(1, 1);
  this->test_pointwise(input);
}
TYPED_TEST(LogGrad, Shape_8x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(8, 8);
  this->test_pointwise(input);
}
TYPED_TEST(LogGrad, Shape_9x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(9, 9);
  this->test_pointwise(input);
}
TYPED_TEST(LogGrad, Shape_10x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(10, 10);
  this->test_pointwise(input);
}

template <typename Pair>
using ReluForward =
    PointwiseEventFixture<Pair, pointwise::Relu, pointwise::Forward>;
TYPED_TEST_SUITE(ReluForward, types::GTestKernelDataTypes);
TYPED_TEST(ReluForward, Shape_1x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(1, 1);
  this->test_pointwise(input);
}
TYPED_TEST(ReluForward, Shape_8x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(8, 8);
  this->test_pointwise(input);
}
TYPED_TEST(ReluForward, Shape_9x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(9, 9);
  this->test_pointwise(input);
}
TYPED_TEST(ReluForward, Shape_10x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(10, 10);
  this->test_pointwise(input);
}

template <typename Pair>
using ReluGrad =
    PointwiseEventFixture<Pair, pointwise::Relu, pointwise::Gradient>;
TYPED_TEST_SUITE(ReluGrad, types::GTestKernelDataTypes);
TYPED_TEST(ReluGrad, Shape_1x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(1, 1);
  this->test_pointwise(input);
}
TYPED_TEST(ReluGrad, Shape_8x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(8, 8);
  this->test_pointwise(input);
}
TYPED_TEST(ReluGrad, Shape_9x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(9, 9);
  this->test_pointwise(input);
}
TYPED_TEST(ReluGrad, Shape_10x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(10, 10);
  this->test_pointwise(input);
}

template <typename Pair>
using SqrtForward =
    PointwiseEventFixture<Pair, pointwise::Sqrt, pointwise::Forward>;
TYPED_TEST_SUITE(SqrtForward, types::GTestKernelDataTypes);
TYPED_TEST(SqrtForward, Shape_1x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(1, 1);
  this->test_pointwise(input);
}
TYPED_TEST(SqrtForward, Shape_8x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(8, 8);
  this->test_pointwise(input);
}
TYPED_TEST(SqrtForward, Shape_9x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(9, 9);
  this->test_pointwise(input);
}
TYPED_TEST(SqrtForward, Shape_10x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(10, 10);
  this->test_pointwise(input);
}

template <typename Pair>
using SqrtGrad =
    PointwiseEventFixture<Pair, pointwise::Sqrt, pointwise::Gradient>;
TYPED_TEST_SUITE(SqrtGrad, types::GTestKernelDataTypes);
TYPED_TEST(SqrtGrad, Shape_1x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(1, 1);
  this->test_pointwise(input);
}
TYPED_TEST(SqrtGrad, Shape_8x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(8, 8);
  this->test_pointwise(input);
}
TYPED_TEST(SqrtGrad, Shape_9x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(9, 9);
  this->test_pointwise(input);
}
TYPED_TEST(SqrtGrad, Shape_10x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(10, 10);
  this->test_pointwise(input);
}

template <typename Pair>
using TanhForward =
    PointwiseEventFixture<Pair, pointwise::Tanh, pointwise::Forward>;
TYPED_TEST_SUITE(TanhForward, types::GTestKernelDataTypes);
TYPED_TEST(TanhForward, Shape_1x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(1, 1);
  this->test_pointwise(input);
}
TYPED_TEST(TanhForward, Shape_8x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(8, 8);
  this->test_pointwise(input);
}
TYPED_TEST(TanhForward, Shape_9x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(9, 9);
  this->test_pointwise(input);
}
TYPED_TEST(TanhForward, Shape_10x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(10, 10);
  this->test_pointwise(input);
}

template <typename Pair>
using TanhGrad =
    PointwiseEventFixture<Pair, pointwise::Tanh, pointwise::Gradient>;
TYPED_TEST_SUITE(TanhGrad, types::GTestKernelDataTypes);
TYPED_TEST(TanhGrad, Shape_1x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(1, 1);
  this->test_pointwise(input);
}
TYPED_TEST(TanhGrad, Shape_8x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(8, 8);
  this->test_pointwise(input);
}
TYPED_TEST(TanhGrad, Shape_9x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(9, 9);
  this->test_pointwise(input);
}
TYPED_TEST(TanhGrad, Shape_10x1) {
  using DataType = typename TestFixture::DataType;
  const std::vector<DataType> input = iota_initialised_data<DataType>(10, 10);
  this->test_pointwise(input);
}

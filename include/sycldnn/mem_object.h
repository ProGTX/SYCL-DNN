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
#ifndef SYCLDNN_INCLUDE_MEM_OBJECT_H_
#define SYCLDNN_INCLUDE_MEM_OBJECT_H_

/**
 * \file
 * Provides the \ref sycldnn::MemObject and \ref sycldnn::MemObject classes,
 * along with the \ref sycldnn::make_mem_object helper function.
 */
#include <CL/sycl.hpp>
#include <type_traits>

#include "sycldnn/accessor_types.h"
#include "sycldnn/helpers/macros.h"
#include "sycldnn/helpers/sycl_language_helpers.h"

namespace sycldnn {

/**
 * Forward decleration of MemObject
 */
template <typename T,
          typename Alloc =
              sycldnn::helpers::buffer_allocator<std::remove_const_t<T>>>
struct MemObject;

/**
 * Alias to maintain BaseMemObject struct
 */
template <typename T,
          typename Alloc =
              sycldnn::helpers::buffer_allocator<std::remove_const_t<T>>>
using BaseMemObject = MemObject<T, Alloc>;

/**
 * Helper function to create MemObjects.
 *
 * This is useful as it can automatically deduce the template types, enabling
 * MemObjects to be constructed as simply as:
 * \code
 *   auto mem_object = make_mem_object(buffer, size, offset);
 * \endcode
 *
 * \param buffer The SYCL buffer to use as the underlying memory object.
 * \param extent The overall number of elements in the buffer to provide
 *               access to.
 * \param offset The offset from the start of the buffer (in number of
 *               elements) to use as the initial index for the memory object.
 *
 * \return A MemObject that provides access to the given SYCL buffer.
 */
template <typename T, typename Alloc>
MemObject<T, Alloc> make_mem_object(cl::sycl::buffer<T, 1, Alloc> buffer,
                                    size_t extent, size_t offset = 0) {
  SNN_ASSERT(buffer.get_count() >= extent + offset,
             "Buffer must contain at least extent + offset elements");
  return MemObject<T, Alloc>{buffer, extent, offset};
}

/**
 * The implementation of MemObject for SYCL buffers.
 */
template <typename T, typename Alloc>
struct MemObject {
  /** The datatype stored in the memory object. */
  using DataType = T;
  /** The allocator type of the underlying SYCL buffer. */
  using AllocType = Alloc;
  /** Alias for the underlying SYCL buffer type. */
  using Buffer = cl::sycl::buffer<DataType, 1, AllocType>;
  /** Alias for the SYCL command group handler. */
  using Handler = cl::sycl::handler;

 public:
  /**
   * Construct a MemObject wrapper around the given SYCL buffer.
   *
   * \param buffer SYCL buffer to use as underlying memory.
   * \param extent The overall number of elements in the buffer to provide
   *               access to.
   * \param offset The offset from the start of the buffer (in number of
   *               elements) to use as the initial index for the memory
   * object.
   */
  MemObject(Buffer buffer, size_t extent, size_t offset)
      : buffer_{buffer}, extent_{extent}, offset_{offset} {}

  /**
   * Get a read only accessor to the underlying memory object.
   *
   * \param cgh The SYCL command group handler to bind the buffer accessor to.
   * \return A ReadAccessor wrapper containing a SYCL accessor.
   */
  ReadAccessor<DataType> read_accessor(Handler& cgh) {
    return {buffer_, cgh, extent_, offset_};
  }

  /**
   * Get a read-write accessor to the underlying memory object.
   *
   * \param cgh The SYCL command group handler to bind the buffer accessor to.
   * \return A ReadWriteAccessor wrapper containing a SYCL accessor.
   */
  template <typename U = DataType,
            typename = std::enable_if<std::is_same<U, DataType>::value>>
  typename std::enable_if_t<!std::is_const<U>::value, ReadWriteAccessor<U>>
  read_write_accessor(Handler& cgh) {
    return {buffer_, cgh, extent_, offset_};
  }

  /**
   * Get a write only accessor to the underlying memory object.
   *
   * \param cgh The SYCL command group handler to bind the buffer accessor to.
   * \return A WriteAccessor wrapper containing a SYCL accessor.
   */
  template <typename U = DataType,
            typename = std::enable_if<std::is_same<U, DataType>::value>>
  typename std::enable_if_t<!std::is_const<U>::value, WriteAccessor<U>>
  write_accessor(Handler& cgh) {
    return {buffer_, cgh, extent_, offset_};
  }

  /**
   * Get a reference to the SYCL buffer referred to by this MemObject.
   * \return A reference to the SYCL buffer.
   */
  Buffer const& get_buffer() const { return buffer_; }

  /**
   * Get the extent of this MemObject. This is the number of elements in the
   * SYCL buffer that are available to a user when a SYCL accessor is
   * requested.
   * \return The extent of this MemObject.
   */
  size_t get_extent() const { return extent_; }

  /**
   * Get the offset of this MemObject into its Buffer.
   * \return The number of elements offset from the start of the Buffer.
   */
  size_t get_offset() const { return offset_; }

  /**
   * Get the number of elements in a buffer.
   * \return number of elements.
   */
  size_t get_count() const { return buffer_.get_count(); }

  /**
   * Return the same MemObject as a read-only one.
   * \return Read-only MemObject.
   */
  MemObject<DataType const, AllocType> as_const() const {
    return MemObject<DataType const, AllocType>(
        buffer_.template reinterpret<DataType const>(), extent_, offset_);
  }

  /**
   * Return a new MemObject with a buffer casted to a new type.
   * \return Casted MemObject.
   */
  template <typename NewDataType,
            typename std::enable_if<sizeof(NewDataType) == sizeof(DataType),
                                    int>::type = 0>
  auto cast() const {
    return make_mem_object(buffer_.template reinterpret<NewDataType>(), extent_,
                           offset_);
  }

 private:
  /** The underlying SYCL buffer. */
  Buffer buffer_;
  /** The number of elements to expose in the SYCL buffer. */
  size_t extent_;
  /** The offset from the start of the buffer (in elements). */
  size_t offset_;
};

}  // namespace sycldnn

#endif  // SYCLDNN_INCLUDE_MEM_OBJECT_H_

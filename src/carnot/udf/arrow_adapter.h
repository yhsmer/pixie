#pragma once

#include <arrow/array.h>
#include <arrow/memory_pool.h>
#include <algorithm>
#include <numeric>
#include <string>

#include <memory>
#include <vector>

#include "src/carnot/udf/udf.h"
#include "src/common/common.h"

namespace pl {

/*
 * Status adapter for arrow.
 */
template <>
inline Status StatusAdapter<arrow::Status>(const arrow::Status& s) noexcept {
  return Status(error::UNKNOWN, s.message());
}

namespace carnot {
namespace udf {

// The functions convert vector of UDF values to an arrow representation on
// the given MemoryPool.
template <typename TUDFValue>
inline std::shared_ptr<arrow::Array> ToArrow(const std::vector<TUDFValue>& data,
                                             arrow::MemoryPool* mem_pool) {
  DCHECK(mem_pool != nullptr);

  typename UDFValueTraits<TUDFValue>::arrow_builder_type builder(mem_pool);
  PL_CHECK_OK(builder.Reserve(data.size()));
  for (const auto v : data) {
    builder.UnsafeAppend(v.val);
  }
  std::shared_ptr<arrow::Array> arr;
  PL_CHECK_OK(builder.Finish(&arr));
  return arr;
}

// Specialization of the above for strings.
template <>
inline std::shared_ptr<arrow::Array> ToArrow<StringValue>(const std::vector<StringValue>& data,
                                                          arrow::MemoryPool* mem_pool) {
  DCHECK(mem_pool != nullptr);
  arrow::StringBuilder builder(mem_pool);
  size_t total_size =
      std::accumulate(data.begin(), data.end(), 0ULL,
                      [](uint64_t sum, const std::string& str) { return sum + str.size(); });
  // This allocates space for null/ptrs/size.
  PL_CHECK_OK(builder.Reserve(data.size()));
  // This allocates space for the actual data.
  PL_CHECK_OK(builder.ReserveData(total_size));
  for (const auto val : data) {
    builder.UnsafeAppend(val);
  }
  std::shared_ptr<arrow::Array> arr;
  PL_CHECK_OK(builder.Finish(&arr));
  return arr;
}

/**
 * Find the carnot UDFDataType for a given arrow type.
 * @param arrow_type The arrow type.
 * @return The carnot UDFDataType.
 */
udf::UDFDataType ArrowToCarnotType(const arrow::Type::type& arrow_type);

arrow::Type::type CarnotToArrowType(const udf::UDFDataType& udf_type);

int64_t ArrowTypeToBytes(const arrow::Type::type& arrow_type);

/**
 * Make an arrow builder based on carnot UDFDataType and usng the passed in MemoryPool.
 * @param data_type The carnot UDFDataType.
 * @param mem_pool The MemoryPool to use.
 * @return a unique_ptr to an array builder.
 */
std::unique_ptr<arrow::ArrayBuilder> MakeArrowBuilder(const udf::UDFDataType& data_type,
                                                      arrow::MemoryPool* mem_pool);
}  // namespace udf
}  // namespace carnot
}  // namespace pl

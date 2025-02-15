/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
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

#pragma once

#include <type_traits>
#include <folly/Portability.h>
#include <thrift/lib/cpp2/FieldMask.h>
#include <thrift/lib/cpp2/protocol/BinaryProtocol.h>
#include <thrift/lib/cpp2/protocol/CompactProtocol.h>
#include <thrift/lib/cpp2/protocol/Object.h>
#include <thrift/lib/cpp2/protocol/detail/Object.h>
#include <thrift/lib/thrift/detail/protocol.h>

namespace apache {
namespace thrift {
namespace protocol {
namespace detail {

struct ApplyPatch {
  // Applies 'patch' to 'value' in-place.
  void operator()(const Object& patch, Value& value) const;
  void operator()(const Object& patch, bool& value) const;
  void operator()(const Object& patch, int8_t& value) const;
  void operator()(const Object& patch, int16_t& value) const;
  void operator()(const Object& patch, int32_t& value) const;
  void operator()(const Object& patch, int64_t& value) const;
  void operator()(const Object& patch, float& value) const;
  void operator()(const Object& patch, double& value) const;
  void operator()(const Object& patch, folly::IOBuf& value) const;
  void operator()(const Object& patch, std::vector<Value>& value) const;
  void operator()(const Object& patch, std::set<Value>& value) const;
  void operator()(const Object& patch, std::map<Value, Value>& value) const;
  void operator()(const Object& patch, Object& value) const;
};

} // namespace detail

/**
 * Takes protocol Object and targer protocol Value. Makes sure that Object
 * represents a Patch and tries to apply this patch to the target Value.
 *
 * @param patch Object
 * @param value to be patched
 */
FOLLY_INLINE_VARIABLE constexpr detail::ApplyPatch applyPatch{};

// Constructs a Mask that only contains fields that are modified by the Patch.
// It will construct nested Mask for map and object patches.
// For map, it uses the address of Value key as the key for the mask.
// Note that Mask contains pointer to `protocol::Value` in patch, so
// caller needs to make sure Patch has longer lifetime than the mask.
Mask extractMaskFromPatch(const protocol::Object& patch);

// Extracting mask from a temporary patch is dangerous and should be disallowed.
protocol::Mask extractMaskFromPatch(Object&& patch) = delete;

// TODO: move the function definition to .cpp file to avoid including expensive
// headers.
template <type::StandardProtocol Protocol>
std::unique_ptr<folly::IOBuf> applyPatchToSerializedData(
    const protocol::Object& patch, const folly::IOBuf& buf) {
  // TODO: create method for this operation
  static_assert(
      Protocol == type::StandardProtocol::Binary ||
      Protocol == type::StandardProtocol::Compact);
  using ProtocolReader = std::conditional_t<
      Protocol == type::StandardProtocol::Binary,
      BinaryProtocolReader,
      CompactProtocolReader>;
  using ProtocolWriter = std::conditional_t<
      Protocol == type::StandardProtocol::Binary,
      BinaryProtocolWriter,
      CompactProtocolWriter>;
  Mask mask = extractMaskFromPatch(patch);
  MaskedDecodeResult result = parseObject<ProtocolReader>(buf, mask);
  applyPatch(patch, result.included);
  return serializeObject<ProtocolWriter>(result.included, result.excluded);
}

} // namespace protocol
} // namespace thrift
} // namespace apache

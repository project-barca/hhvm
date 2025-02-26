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

#include <thrift/lib/cpp2/protocol/detail/Object.h>

namespace apache::thrift::protocol::detail {

ValueStruct makeValueStruct(Value value) {
  ValueStruct valueStruct;
  valueStruct.value() = value;
  return valueStruct;
}

int64_t getMaskKey(MaskRef ref, const Value& newKey) {
  if (ref.isMapMask()) {
    MapIdToMask map = ref.mask.includes_map_ref()
        ? ref.mask.includes_map_ref().value()
        : ref.mask.excludes_map_ref().value();
    for (auto& [key, next] : map) {
      if (*(reinterpret_cast<Value*>(key)) == newKey) {
        return key;
      }
    }
  }
  return 0;
}

} // namespace apache::thrift::protocol::detail

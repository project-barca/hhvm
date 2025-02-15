/**
 * Autogenerated by Thrift for src/module.thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated @nocommit
 */

#include "thrift/compiler/test/fixtures/sink/gen-cpp2/module_data.h"

#include <thrift/lib/cpp2/gen/module_data_cpp.h>

namespace apache {
namespace thrift {

const std::array<folly::StringPiece, 1> TStructDataStorage<::cpp2::InitialResponse>::fields_names = {{
  "content",
}};
const std::array<int16_t, 1> TStructDataStorage<::cpp2::InitialResponse>::fields_ids = {{
  1,
}};
const std::array<protocol::TType, 1> TStructDataStorage<::cpp2::InitialResponse>::fields_types = {{
  TType::T_STRING,
}};

const std::array<folly::StringPiece, 1> TStructDataStorage<::cpp2::FinalResponse>::fields_names = {{
  "content",
}};
const std::array<int16_t, 1> TStructDataStorage<::cpp2::FinalResponse>::fields_ids = {{
  1,
}};
const std::array<protocol::TType, 1> TStructDataStorage<::cpp2::FinalResponse>::fields_types = {{
  TType::T_STRING,
}};

const std::array<folly::StringPiece, 1> TStructDataStorage<::cpp2::SinkPayload>::fields_names = {{
  "content",
}};
const std::array<int16_t, 1> TStructDataStorage<::cpp2::SinkPayload>::fields_ids = {{
  1,
}};
const std::array<protocol::TType, 1> TStructDataStorage<::cpp2::SinkPayload>::fields_types = {{
  TType::T_STRING,
}};

const std::array<folly::StringPiece, 1> TStructDataStorage<::cpp2::CompatibleWithKeywordSink>::fields_names = {{
  "sink",
}};
const std::array<int16_t, 1> TStructDataStorage<::cpp2::CompatibleWithKeywordSink>::fields_ids = {{
  1,
}};
const std::array<protocol::TType, 1> TStructDataStorage<::cpp2::CompatibleWithKeywordSink>::fields_types = {{
  TType::T_STRING,
}};

const std::array<folly::StringPiece, 1> TStructDataStorage<::cpp2::InitialException>::fields_names = {{
  "reason",
}};
const std::array<int16_t, 1> TStructDataStorage<::cpp2::InitialException>::fields_ids = {{
  1,
}};
const std::array<protocol::TType, 1> TStructDataStorage<::cpp2::InitialException>::fields_types = {{
  TType::T_STRING,
}};

const std::array<folly::StringPiece, 1> TStructDataStorage<::cpp2::SinkException1>::fields_names = {{
  "reason",
}};
const std::array<int16_t, 1> TStructDataStorage<::cpp2::SinkException1>::fields_ids = {{
  1,
}};
const std::array<protocol::TType, 1> TStructDataStorage<::cpp2::SinkException1>::fields_types = {{
  TType::T_STRING,
}};

const std::array<folly::StringPiece, 1> TStructDataStorage<::cpp2::SinkException2>::fields_names = {{
  "reason",
}};
const std::array<int16_t, 1> TStructDataStorage<::cpp2::SinkException2>::fields_ids = {{
  1,
}};
const std::array<protocol::TType, 1> TStructDataStorage<::cpp2::SinkException2>::fields_types = {{
  TType::T_I64,
}};

} // namespace thrift
} // namespace apache

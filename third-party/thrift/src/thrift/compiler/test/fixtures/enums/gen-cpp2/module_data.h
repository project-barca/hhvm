/**
 * Autogenerated by Thrift for src/module.thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated @nocommit
 */
#pragma once

#include <thrift/lib/cpp2/gen/module_data_h.h>

#include "thrift/compiler/test/fixtures/enums/gen-cpp2/module_types.h"

namespace apache { namespace thrift {

template <> struct TEnumDataStorage<::test::fixtures::enums::Metasyntactic> {
  using type = ::test::fixtures::enums::Metasyntactic;
  static constexpr const std::size_t size = 5;
  static const std::array<type, size> values;
  static const std::array<folly::StringPiece, size> names;
};

template <> struct TEnumDataStorage<::test::fixtures::enums::MyEnum1> {
  using type = ::test::fixtures::enums::MyEnum1;
  static constexpr const std::size_t size = 6;
  static const std::array<type, size> values;
  static const std::array<folly::StringPiece, size> names;
};

template <> struct TEnumDataStorage<::test::fixtures::enums::MyEnum2> {
  using type = ::test::fixtures::enums::MyEnum2;
  static constexpr const std::size_t size = 3;
  static const std::array<type, size> values;
  static const std::array<folly::StringPiece, size> names;
};

template <> struct TEnumDataStorage<::test::fixtures::enums::MyEnum3> {
  using type = ::test::fixtures::enums::MyEnum3;
  static constexpr const std::size_t size = 6;
  static const std::array<type, size> values;
  static const std::array<folly::StringPiece, size> names;
};

template <> struct TEnumDataStorage<::test::fixtures::enums::MyEnum4> {
  using type = ::test::fixtures::enums::MyEnum4;
  static constexpr const std::size_t size = 5;
  static const std::array<type, size> values;
  static const std::array<folly::StringPiece, size> names;
};

template <> struct TEnumDataStorage<::test::fixtures::enums::MyBitmaskEnum1> {
  using type = ::test::fixtures::enums::MyBitmaskEnum1;
  static constexpr const std::size_t size = 4;
  static const std::array<type, size> values;
  static const std::array<folly::StringPiece, size> names;
};

template <> struct TEnumDataStorage<::test::fixtures::enums::MyBitmaskEnum2> {
  using type = ::test::fixtures::enums::MyBitmaskEnum2;
  static constexpr const std::size_t size = 4;
  static const std::array<type, size> values;
  static const std::array<folly::StringPiece, size> names;
};

template <> struct TStructDataStorage<::test::fixtures::enums::SomeStruct> {
  static constexpr const std::size_t fields_size = 4;
  static const std::array<folly::StringPiece, fields_size> fields_names;
  static const std::array<int16_t, fields_size> fields_ids;
  static const std::array<protocol::TType, fields_size> fields_types;
};

template <> struct TStructDataStorage<::test::fixtures::enums::MyStruct> {
  static constexpr const std::size_t fields_size = 4;
  static const std::array<folly::StringPiece, fields_size> fields_names;
  static const std::array<int16_t, fields_size> fields_ids;
  static const std::array<protocol::TType, fields_size> fields_types;
};

}} // apache::thrift

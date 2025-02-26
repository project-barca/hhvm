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

#include <thrift/lib/cpp2/protocol/Object.h>

#include <set>

#include <folly/io/IOBufQueue.h>
#include <folly/portability/GMock.h>
#include <folly/portability/GTest.h>
#include <thrift/conformance/cpp2/AnyRegistry.h>
#include <thrift/conformance/cpp2/AnyStructSerializer.h>
#include <thrift/conformance/cpp2/Protocol.h>
#include <thrift/conformance/cpp2/Testing.h>
#include <thrift/conformance/cpp2/internal/AnyStructSerializer.h>
#include <thrift/conformance/data/ValueGenerator.h>
#include <thrift/conformance/if/gen-cpp2/conformance_types_custom_protocol.h>
#include <thrift/conformance/if/gen-cpp2/protocol_types.h>
#include <thrift/conformance/if/gen-cpp2/protocol_types_custom_protocol.h>
#include <thrift/lib/cpp/protocol/TType.h>
#include <thrift/lib/cpp2/BadFieldAccess.h>
#include <thrift/lib/cpp2/protocol/BinaryProtocol.h>
#include <thrift/lib/cpp2/protocol/Serializer.h>
#include <thrift/lib/cpp2/protocol/detail/Object.h>
#include <thrift/lib/cpp2/test/gen-cpp2/ObjectTest_types.h>
#include <thrift/lib/cpp2/type/BaseType.h>
#include <thrift/lib/cpp2/type/Tag.h>
#include <thrift/lib/cpp2/type/ThriftType.h>
#include <thrift/lib/thrift/gen-cpp2/protocol_types.h>
#include <thrift/lib/thrift/gen-cpp2/standard_types.h>
#include <thrift/test/testset/Testset.h>
#include <thrift/test/testset/gen-cpp2/testset_types_custom_protocol.h>

// TODO: Remove this. Specify namespace explicitly instead.
using namespace ::apache::thrift::conformance;

using detail::protocol_reader_t;
using detail::protocol_writer_t;

namespace apache::thrift::protocol {
namespace {

namespace testset = apache::thrift::test::testset;

template <typename C>
decltype(auto) at(C& container, size_t i) {
  auto itr = container.begin();
  std::advance(itr, i);
  return *itr;
}

TEST(ObjectTest, Example) {
  using facebook::thrift::lib::test::Bar;

  Bar bar;
  bar.field_3() = {"foo", "bar", "baz"};
  bar.field_4()->field_1() = 42;
  bar.field_4()->field_2() = "Everything";

  auto serialized = CompactSerializer::serialize<folly::IOBufQueue>(bar).move();

  // We can parse arbitrary serialized thrift blob into Protocol Object
  Object obj = parseObject<CompactSerializer::ProtocolReader>(*serialized);

  // We can re-serialize it back to Protocol Object
  // Note: there is no guarantee that serialized data is byte-wise idential.
  serialized = serializeObject<CompactSerializer::ProtocolWriter>(obj);

  // Test round-trip.
  EXPECT_EQ(CompactSerializer::deserialize<Bar>(serialized.get()), bar);

  // Test constructing the same Object manually
  Object foo;
  foo[FieldId{1}].emplace_i32() = 42;
  foo[FieldId{2}].emplace_binary() = *folly::IOBuf::copyBuffer("Everything");

  Object obj2;
  obj2[FieldId{10}] = asValueStruct<type::list<type::binary_t>>(*bar.field_3());
  obj2[FieldId{20}].emplace_object() = foo;

  EXPECT_EQ(obj, obj2);
}

TEST(ObjectTest, TypeEnforced) {
  // Always a bool when bool_t is used, without ambiguity.
  // Pointers implicitly converts to bools.
  Value value = asValueStruct<type::bool_t>("");
  ASSERT_EQ(value.getType(), Value::Type::boolValue);
  EXPECT_TRUE(value.get_boolValue());
}

TEST(ObjectTest, Bool) {
  Value value = asValueStruct<type::bool_t>(20);
  ASSERT_EQ(value.getType(), Value::Type::boolValue);
  EXPECT_TRUE(value.get_boolValue());

  value = asValueStruct<type::bool_t>(0);
  ASSERT_EQ(value.getType(), Value::Type::boolValue);
  EXPECT_FALSE(value.get_boolValue());
}

TEST(ObjectTest, Byte) {
  Value value = asValueStruct<type::byte_t>(7u);
  ASSERT_EQ(value.getType(), Value::Type::byteValue);
  EXPECT_EQ(value.get_byteValue(), 7);
}

TEST(ObjectTest, I16) {
  Value value = asValueStruct<type::i16_t>(7u);
  ASSERT_EQ(value.getType(), Value::Type::i16Value);
  EXPECT_EQ(value.get_i16Value(), 7);
}

TEST(ObjectTest, I32) {
  Value value = asValueStruct<type::i32_t>(7u);
  ASSERT_EQ(value.getType(), Value::Type::i32Value);
  EXPECT_EQ(value.get_i32Value(), 7);
}

TEST(ObjectTest, I64) {
  Value value = asValueStruct<type::i64_t>(7u);
  ASSERT_EQ(value.getType(), Value::Type::i64Value);
  EXPECT_EQ(value.get_i64Value(), 7);
}

TEST(ObjectTest, Enum) {
  enum class MyEnum { kValue = 7 };
  Value value = asValueStruct<type::enum_c>(MyEnum::kValue);
  ASSERT_EQ(value.getType(), Value::Type::i32Value);
  EXPECT_EQ(value.get_i32Value(), 7);

  value = asValueStruct<type::enum_c>(static_cast<MyEnum>(2));
  ASSERT_EQ(value.getType(), Value::Type::i32Value);
  EXPECT_EQ(value.get_i32Value(), 2);

  value = asValueStruct<type::enum_c>(21u);
  ASSERT_EQ(value.getType(), Value::Type::i32Value);
  EXPECT_EQ(value.get_i32Value(), 21);
}

TEST(ObjectTest, Float) {
  Value value = asValueStruct<type::float_t>(1.5);
  ASSERT_EQ(value.getType(), Value::Type::floatValue);
  EXPECT_EQ(value.get_floatValue(), 1.5f);
}

TEST(ObjectTest, Double) {
  Value value = asValueStruct<type::double_t>(1.5f);
  ASSERT_EQ(value.getType(), Value::Type::doubleValue);
  EXPECT_EQ(value.get_doubleValue(), 1.5);
}

TEST(ObjectTest, String) {
  Value value = asValueStruct<type::string_t>("hi");
  ASSERT_EQ(value.getType(), Value::Type::stringValue);
  EXPECT_EQ(value.get_stringValue(), "hi");
}

TEST(ObjectTest, Binary) {
  Value value = asValueStruct<type::binary_t>("hi");
  ASSERT_EQ(value.getType(), Value::Type::binaryValue);
  EXPECT_EQ(toString(value.get_binaryValue()), "hi");
}

TEST(ObjectTest, List) {
  std::vector<int> data = {1, 4, 2};
  Value value = asValueStruct<type::list<type::i16_t>>(data);
  ASSERT_EQ(value.getType(), Value::Type::listValue);
  ASSERT_EQ(value.get_listValue().size(), data.size());
  for (size_t i = 0; i < data.size(); ++i) {
    EXPECT_EQ(value.get_listValue()[i], asValueStruct<type::i16_t>(data[i]));
  }

  // Works with other containers
  value = asValueStruct<type::list<type::i16_t>>(
      std::set<int>(data.begin(), data.end()));
  std::sort(data.begin(), data.end());
  ASSERT_EQ(value.getType(), Value::Type::listValue);
  ASSERT_EQ(value.get_listValue().size(), data.size());
  for (size_t i = 0; i < data.size(); ++i) {
    EXPECT_EQ(value.get_listValue()[i], asValueStruct<type::i16_t>(data[i]));
  }
}

TEST(ObjectTest, List_Move) {
  // Validate the premise of the test.
  std::string s1 = "hi";
  std::string s2 = std::move(s1);
  EXPECT_EQ(s1, "");
  EXPECT_EQ(s2, "hi");

  std::vector<std::string> data;
  data.emplace_back("hi");
  data.emplace_back("bye");

  Value value = asValueStruct<type::list<type::string_t>>(data);
  // The strings are unchanged
  EXPECT_THAT(data, ::testing::ElementsAre("hi", "bye"));
  ASSERT_EQ(value.getType(), Value::Type::listValue);
  ASSERT_EQ(value.get_listValue().size(), 2);
  EXPECT_EQ(value.get_listValue()[0].get_stringValue(), "hi");
  EXPECT_EQ(value.get_listValue()[1].get_stringValue(), "bye");

  value = asValueStruct<type::list<type::string_t>>(std::move(data));

  // The strings have been moved.
  EXPECT_THAT(data, ::testing::ElementsAre("", ""));
  ASSERT_EQ(value.getType(), Value::Type::listValue);
  ASSERT_EQ(value.get_listValue().size(), 2);
  EXPECT_EQ(value.get_listValue()[0].get_stringValue(), "hi");
  EXPECT_EQ(value.get_listValue()[1].get_stringValue(), "bye");
}

TEST(ObjectTest, Set) {
  std::set<int> data = {1, 4, 2};
  Value value = asValueStruct<type::set<type::i16_t>>(data);
  ASSERT_EQ(value.getType(), Value::Type::setValue);
  ASSERT_EQ(value.get_setValue().size(), data.size());
  for (size_t i = 0; i < data.size(); ++i) {
    EXPECT_EQ(
        at(value.get_setValue(), i), asValueStruct<type::i16_t>(at(data, i)));
  }

  // Works with other containers
  value = asValueStruct<type::set<type::i16_t>>(
      std::vector<int>(data.begin(), data.end()));
  ASSERT_EQ(value.getType(), Value::Type::setValue);
  ASSERT_EQ(value.get_setValue().size(), data.size());
  for (size_t i = 0; i < data.size(); ++i) {
    EXPECT_EQ(
        at(value.get_setValue(), i), asValueStruct<type::i16_t>(at(data, i)));
  }
}

TEST(ObjectTest, Map) {
  std::map<std::string, int> data = {{"one", 1}, {"four", 4}, {"two", 2}};
  Value value = asValueStruct<type::map<type::string_t, type::byte_t>>(data);
  ASSERT_EQ(value.getType(), Value::Type::mapValue);
  ASSERT_EQ(value.get_mapValue().size(), data.size());
  for (const auto& entry : data) {
    auto itr =
        value.get_mapValue().find(asValueStruct<type::string_t>(entry.first));
    ASSERT_NE(itr, value.get_mapValue().end());
    EXPECT_EQ(itr->second, asValueStruct<type::byte_t>(entry.second));
  }

  // Works with other containers.
  std::vector<std::pair<std::string, int>> otherData(data.begin(), data.end());
  value = asValueStruct<type::map<type::string_t, type::byte_t>>(otherData);
  ASSERT_EQ(value.getType(), Value::Type::mapValue);
  ASSERT_EQ(value.get_mapValue().size(), data.size());
  for (const auto& entry : data) {
    auto itr =
        value.get_mapValue().find(asValueStruct<type::string_t>(entry.first));
    ASSERT_NE(itr, value.get_mapValue().end());
    EXPECT_EQ(itr->second, asValueStruct<type::byte_t>(entry.second));
  }
}

TEST(ObjectTest, Struct) {
  // TODO(afuller): Use a struct that covers more cases.
  auto protocol = ::apache::thrift::conformance::Protocol("hi").asStruct();
  Value value = asValueStruct<type::union_c>(protocol);
  ASSERT_EQ(value.getType(), Value::Type::objectValue);
  const Object& object = value.get_objectValue();
  EXPECT_EQ(object.members_ref()->size(), 2);
  EXPECT_EQ(
      object.members_ref()->at(1),
      asValueStruct<type::enum_c>(
          ::apache::thrift::conformance::StandardProtocol::Custom));
  EXPECT_EQ(object.members_ref()->at(2), asValueStruct<type::binary_t>("hi"));
}

TEST(ObjectTest, StructWithList) {
  testset::struct_with<type::list<type::i32_t>> s;
  std::vector<int> listValues = {1, 2, 3};
  s.field_1_ref() = listValues;
  Value value = asValueStruct<type::struct_c>(s);
  ASSERT_EQ(value.getType(), Value::Type::objectValue);
  const Object& object = value.get_objectValue();
  EXPECT_EQ(object.members_ref()->size(), 1);
  EXPECT_EQ(
      object.members_ref()->at(1),
      asValueStruct<type::list<type::i32_t>>(listValues));
}

TEST(ObjectTest, StructWithMap) {
  testset::struct_with<type::map<type::string_t, type::i32_t>> s;
  std::map<std::string, int> mapValues = {{"one", 1}, {"four", 4}, {"two", 2}};
  s.field_1_ref() = mapValues;
  Value value = asValueStruct<type::struct_c>(s);
  ASSERT_EQ(value.getType(), Value::Type::objectValue);
  const Object& object = value.get_objectValue();
  EXPECT_EQ(object.members_ref()->size(), 1);
  auto val = asValueStruct<type::map<type::binary_t, type::i32_t>>(mapValues);
  EXPECT_EQ(object.members_ref()->at(1), val);
}

TEST(ObjectTest, StructWithSet) {
  testset::struct_with<type::set<type::i64_t>> s;
  std::set<long> setValues = {1, 2, 3};
  s.field_1_ref() = setValues;
  Value value = asValueStruct<type::struct_c>(s);
  ASSERT_EQ(value.getType(), Value::Type::objectValue);
  const Object& object = value.get_objectValue();
  EXPECT_EQ(object.members_ref()->size(), 1);
  EXPECT_EQ(
      object.members_ref()->at(1),
      asValueStruct<type::set<type::i64_t>>(setValues));
}

TEST(ObjectTest, parseObject) {
  folly::IOBufQueue iobufQueue;
  testset::struct_with<type::set<type::i64_t>> thriftStruct;
  std::set<long> setValues = {1, 2, 3};
  thriftStruct.field_1_ref() = setValues;
  BinarySerializer::serialize(thriftStruct, &iobufQueue);
  auto serialized = iobufQueue.move();
  auto object = parseObject<BinarySerializer::ProtocolReader>(*serialized);
  EXPECT_EQ(object.members_ref()->size(), 1);
  EXPECT_EQ(
      object.members_ref()->at(1),
      asValueStruct<type::set<type::i64_t>>(setValues));
}

TEST(ObjectTest, serializeObject) {
  folly::IOBufQueue iobufQueue;
  testset::struct_with<type::set<type::i64_t>> thriftStruct;
  std::set<long> setValues = {1, 2, 3};
  thriftStruct.field_1_ref() = setValues;
  BinarySerializer::serialize(thriftStruct, &iobufQueue);
  auto expected = iobufQueue.move();
  auto object = parseObject<BinarySerializer::ProtocolReader>(*expected);
  auto actual = serializeObject<BinarySerializer::ProtocolWriter>(object);
  EXPECT_TRUE(folly::IOBufEqualTo{}(*actual, *expected));
}

TEST(ObjectTest, ValueUnionTypeMatch) {
  EXPECT_EQ(
      static_cast<type::BaseType>(Value::Type::boolValue),
      type::BaseType::Bool);
  EXPECT_EQ(
      static_cast<type::BaseType>(Value::Type::byteValue),
      type::BaseType::Byte);
  EXPECT_EQ(
      static_cast<type::BaseType>(Value::Type::i16Value), type::BaseType::I16);
  EXPECT_EQ(
      static_cast<type::BaseType>(Value::Type::i32Value), type::BaseType::I32);
  EXPECT_EQ(
      static_cast<type::BaseType>(Value::Type::i64Value), type::BaseType::I64);
  EXPECT_EQ(
      static_cast<type::BaseType>(Value::Type::floatValue),
      type::BaseType::Float);
  EXPECT_EQ(
      static_cast<type::BaseType>(Value::Type::doubleValue),
      type::BaseType::Double);
  EXPECT_EQ(
      static_cast<type::BaseType>(Value::Type::stringValue),
      type::BaseType::String);
  EXPECT_EQ(
      static_cast<type::BaseType>(Value::Type::binaryValue),
      type::BaseType::Binary);
  EXPECT_EQ(
      static_cast<type::BaseType>(Value::Type::listValue),
      type::BaseType::List);
  EXPECT_EQ(
      static_cast<type::BaseType>(Value::Type::setValue), type::BaseType::Set);
  EXPECT_EQ(
      static_cast<type::BaseType>(Value::Type::mapValue), type::BaseType::Map);
  EXPECT_EQ(
      static_cast<type::BaseType>(Value::Type::objectValue),
      type::BaseType::Struct);
}

template <typename ParseObjectTestCase>
class TypedParseObjectTest : public testing::Test {};

template <::apache::thrift::conformance::StandardProtocol Protocol, typename T>
std::unique_ptr<folly::IOBuf> serialize(T& s) {
  folly::IOBufQueue iobufQueue;
  protocol_writer_t<Protocol> writer;
  writer.setOutput(&iobufQueue);
  s.write(&writer);
  auto iobuf = iobufQueue.move();
  return iobuf;
}

template <
    ::apache::thrift::conformance::StandardProtocol Protocol,
    typename Tag,
    typename T>
void testParseObject() {
  T testsetValue;
  for (const auto& val : data::ValueGenerator<Tag>::getKeyValues()) {
    SCOPED_TRACE(val.name);
    testsetValue.field_1_ref() = val.value;
    auto valueStruct = asValueStruct<type::struct_c>(testsetValue);
    const Object& object = valueStruct.get_objectValue();

    auto iobuf = serialize<Protocol, T>(testsetValue);
    auto objFromParseObject = parseObject<protocol_reader_t<Protocol>>(*iobuf);
    EXPECT_EQ(objFromParseObject, object);
  }
}

type::StandardProtocol convertStandardProtocol(
    conformance::StandardProtocol prot) {
  return prot == conformance::StandardProtocol::Binary
      ? type::StandardProtocol::Binary
      : type::StandardProtocol::Compact;
}

template <
    ::apache::thrift::conformance::StandardProtocol Protocol,
    typename Tag,
    typename T>
void testWithMask(bool testSerialize) {
  T testsetValue;
  for (const auto& val : data::ValueGenerator<Tag>::getKeyValues()) {
    SCOPED_TRACE(val.name);
    testsetValue.field_1_ref() = val.value;
    auto valueStruct = asValueStruct<type::struct_c>(testsetValue);
    const Object& object = valueStruct.get_objectValue();

    auto reserialize = [&](MaskedDecodeResult& result) {
      auto reserialized = serializeObject<protocol_writer_t<Protocol>>(
          result.included, result.excluded);
      Object finalObj = parseObject<protocol_reader_t<Protocol>>(*reserialized);
      EXPECT_EQ(finalObj, object);
    };

    auto iobuf = serialize<Protocol, T>(testsetValue);
    {
      // parseObject with allMask should parse the entire object.
      auto result = parseObject<protocol_reader_t<Protocol>>(*iobuf, allMask());
      if (testSerialize) {
        reserialize(result);
      } else { // manually check the result
        EXPECT_EQ(result.included, object);
        MaskedProtocolData expected;
        expected.protocol() = convertStandardProtocol(Protocol);
        EXPECT_EQ(result.excluded, expected);
      }
    }
    {
      // parseObject with noneMask should parse nothing.
      auto result =
          parseObject<protocol_reader_t<Protocol>>(*iobuf, noneMask());
      if (testSerialize) {
        reserialize(result);
      } else { // manually check the result
        EXPECT_EQ(result.included, Object{});
        EXPECT_EQ(
            *result.excluded.protocol(), convertStandardProtocol(Protocol));
        auto& values = *result.excluded.values();
        auto& encodedValue =
            detail::getByValueId(values, *result.excluded.data()->full_ref());
        auto objFromExcluded =
            parseObject<protocol_reader_t<Protocol>>(*encodedValue.data());
        EXPECT_EQ(objFromExcluded, object);
      }
    }
    {
      // parseObject with Mask = includes{1: allMask()}
      Mask mask;
      mask.includes_ref().emplace()[1] = allMask();
      auto result = parseObject<protocol_reader_t<Protocol>>(*iobuf, mask);
      if (testSerialize) {
        reserialize(result);
      } else { // manually check the result
        EXPECT_EQ(result.included.size(), 1);
        EXPECT_EQ(result.included.at(FieldId{1}), object.at(FieldId{1}));
        EXPECT_EQ(
            *result.excluded.protocol(), convertStandardProtocol(Protocol));
      }
    }
  }
}

template <typename Tag>
bool hasEmptyContainer(const type::standard_type<Tag>& value) {
  if constexpr (type::is_a_v<Tag, type::container_c>) {
    if (value.size() == 0) {
      return true;
    }
  }
  if constexpr (type::is_a_v<Tag, type::map<type::all_c, type::container_c>>) {
    for (const auto& [mapkey, mapval] : value) {
      if (mapval.size() == 0) {
        return true;
      }
    }
  }
  return false;
}

template <
    ::apache::thrift::conformance::StandardProtocol Protocol,
    typename Tag,
    typename T>
void testSerializeObject() {
  T testsetValue;
  for (const auto& val : data::ValueGenerator<Tag>::getKeyValues()) {
    SCOPED_TRACE(val.name);
    testsetValue.field_1_ref() = val.value;
    auto valueStruct = asValueStruct<type::struct_c>(testsetValue);
    const Object& object = valueStruct.get_objectValue();
    auto schemalessSerializedData =
        serializeObject<protocol_writer_t<Protocol>>(object);

    auto schemaBasedSerializedData = serialize<Protocol, T>(testsetValue);

    // FIXME: skip validation for structs with empty list, set, map.
    if (hasEmptyContainer<Tag>(val.value)) {
      continue;
    }
    EXPECT_TRUE(folly::IOBufEqualTo{}(
        *schemalessSerializedData, *schemaBasedSerializedData));
  }
}

template <
    ::apache::thrift::conformance::StandardProtocol Protocol,
    typename Tag,
    typename T>
void testSerializeObjectAny() {
  AnyRegistry registry;
  registry.registerType<T>(
      createThriftTypeInfo({"facebook.com/thrift/conformance/struct_with"}));
  registry.registerSerializer<T>(&getAnyStandardSerializer<T, Protocol>());
  for (const auto& val : data::ValueGenerator<Tag>::getKeyValues()) {
    SCOPED_TRACE(val.name);
    RoundTripResponse anyValue;
    T testsetValue;
    testsetValue.field_1_ref() = val.value;
    anyValue.value_ref() = registry.store<Protocol>(testsetValue);

    auto schemaBasedSerializedData = serialize<Protocol>(anyValue);
    auto objFromParseObject =
        parseObject<protocol_reader_t<Protocol>>(*schemaBasedSerializedData);

    auto schemalessSerializedData =
        serializeObject<protocol_writer_t<Protocol>>(objFromParseObject);

    EXPECT_TRUE(folly::IOBufEqualTo{}(
        *schemalessSerializedData, *schemaBasedSerializedData));
  }
}

// The tests cases to run.
using ParseObjectTestCases = ::testing::Types<
    type::bool_t,
    type::byte_t,
    type::i16_t,
    type::i32_t,
    type::i64_t,
    type::float_t,
    type::double_t,
    type::binary_t,
    type::string_t,
    type::list<type::i64_t>,
    type::list<type::string_t>,
    type::set<type::i64_t>,
    type::set<type::string_t>,
    type::map<type::string_t, type::i64_t>,
    type::map<type::i64_t, type::double_t>,
    type::map<type::i64_t, type::set<type::string_t>>>;

TYPED_TEST_SUITE(TypedParseObjectTest, ParseObjectTestCases);

TYPED_TEST(TypedParseObjectTest, ParseSerializedSameAsDirectObject) {
  testParseObject<
      ::apache::thrift::conformance::StandardProtocol::Binary,
      TypeParam,
      testset::struct_with<TypeParam>>();
  testParseObject<
      ::apache::thrift::conformance::StandardProtocol::Compact,
      TypeParam,
      testset::struct_with<TypeParam>>();
  testParseObject<
      ::apache::thrift::conformance::StandardProtocol::Binary,
      TypeParam,
      testset::union_with<TypeParam>>();
  testParseObject<
      ::apache::thrift::conformance::StandardProtocol::Compact,
      TypeParam,
      testset::union_with<TypeParam>>();
}

TYPED_TEST(TypedParseObjectTest, ParseObjectWithMask) {
  testWithMask<
      ::apache::thrift::conformance::StandardProtocol::Binary,
      TypeParam,
      testset::struct_with<TypeParam>>(false);
  testWithMask<
      ::apache::thrift::conformance::StandardProtocol::Compact,
      TypeParam,
      testset::struct_with<TypeParam>>(false);
}

TYPED_TEST(TypedParseObjectTest, SerializeObjectWithMask) {
  testWithMask<
      ::apache::thrift::conformance::StandardProtocol::Binary,
      TypeParam,
      testset::struct_with<TypeParam>>(true);
  testWithMask<
      ::apache::thrift::conformance::StandardProtocol::Compact,
      TypeParam,
      testset::struct_with<TypeParam>>(true);
}

TYPED_TEST(TypedParseObjectTest, SerializeObjectSameAsDirectSerialization) {
  testSerializeObject<
      ::apache::thrift::conformance::StandardProtocol::Binary,
      TypeParam,
      testset::struct_with<TypeParam>>();
  testSerializeObject<
      ::apache::thrift::conformance::StandardProtocol::Compact,
      TypeParam,
      testset::struct_with<TypeParam>>();
  testSerializeObject<
      ::apache::thrift::conformance::StandardProtocol::Binary,
      TypeParam,
      testset::union_with<TypeParam>>();
  testSerializeObject<
      ::apache::thrift::conformance::StandardProtocol::Compact,
      TypeParam,
      testset::union_with<TypeParam>>();
}

TYPED_TEST(TypedParseObjectTest, SerializeObjectSameAsDirectSerializationAny) {
  testSerializeObjectAny<
      ::apache::thrift::conformance::StandardProtocol::Binary,
      TypeParam,
      testset::struct_with<TypeParam>>();
  testSerializeObjectAny<
      ::apache::thrift::conformance::StandardProtocol::Compact,
      TypeParam,
      testset::struct_with<TypeParam>>();
  testSerializeObjectAny<
      ::apache::thrift::conformance::StandardProtocol::Binary,
      TypeParam,
      testset::union_with<TypeParam>>();
  testSerializeObjectAny<
      ::apache::thrift::conformance::StandardProtocol::Compact,
      TypeParam,
      testset::union_with<TypeParam>>();
}

TEST(Object, invalid_object) {
  {
    Object obj;
    obj[FieldId{0}].emplace_list() = {
        asValueStruct<type::i32_t>(1), asValueStruct<type::i64_t>(1)};
    EXPECT_THROW(
        serializeObject<CompactSerializer::ProtocolWriter>(obj),
        TProtocolException);
  }
  {
    Object obj;
    obj[FieldId{0}].emplace_set() = {
        asValueStruct<type::i32_t>(1), asValueStruct<type::i64_t>(1)};
    EXPECT_THROW(
        serializeObject<CompactSerializer::ProtocolWriter>(obj),
        TProtocolException);
  }
  {
    Object obj;
    obj[FieldId{0}].emplace_map() = {
        {asValueStruct<type::i32_t>(1), asValueStruct<type::i32_t>(1)},
        {asValueStruct<type::i32_t>(2), asValueStruct<type::i64_t>(1)}};
    EXPECT_THROW(
        serializeObject<CompactSerializer::ProtocolWriter>(obj),
        TProtocolException);
  }
  {
    Object obj;
    obj[FieldId{0}].emplace_map() = {
        {asValueStruct<type::i32_t>(1), asValueStruct<type::i32_t>(1)},
        {asValueStruct<type::i64_t>(1), asValueStruct<type::i32_t>(1)}};
    EXPECT_THROW(
        serializeObject<CompactSerializer::ProtocolWriter>(obj),
        TProtocolException);
  }
}

TEST(Object, uri) {
  EXPECT_EQ(uri<Object>(), "facebook.com/thrift/protocol/Object");
  EXPECT_EQ(uri<Value>(), "facebook.com/thrift/protocol/Value");
}

TEST(Object, Wrapper) {
  Object object;
  EXPECT_TRUE(object.empty());
  object.members()[0];
  EXPECT_FALSE(object.empty());
  object.members()[2];
  EXPECT_EQ(object.size(), 2);
  EXPECT_EQ(&object[FieldId{0}], &object.members()[0]);
  EXPECT_EQ(&object[FieldId{2}], &object.members()[2]);
  EXPECT_EQ(&object.at(FieldId{0}), &object.members()[0]);
  EXPECT_EQ(&object.at(FieldId{2}), &object.members()[2]);
  EXPECT_EQ(object.if_contains(FieldId{0}), &object.members()[0]);
  EXPECT_EQ(object.if_contains(FieldId{2}), &object.members()[2]);

  EXPECT_EQ(object.contains(FieldId{0}), true);
  EXPECT_EQ(object.contains(FieldId{1}), false);
  EXPECT_EQ(object.contains(FieldId{2}), true);
  EXPECT_THROW(object.at(FieldId{1}), std::out_of_range);

  std::vector<int16_t> ids;
  std::vector<Value*> values;
  for (auto&& i : object) {
    ids.push_back(i.first);
    values.push_back(&i.second);
  }

  EXPECT_EQ(ids.size(), 2);
  EXPECT_EQ(ids[0], 0);
  EXPECT_EQ(ids[1], 2);
  EXPECT_EQ(values.size(), 2);
  EXPECT_EQ(values[0], &object.members()[0]);
  EXPECT_EQ(values[1], &object.members()[2]);

  EXPECT_EQ(object.erase(FieldId{0}), 1);
  EXPECT_EQ(object.contains(FieldId{0}), false);
  EXPECT_EQ(object.contains(FieldId{2}), true);
  EXPECT_EQ(object.size(), 1);

  EXPECT_EQ(object.erase(FieldId{1}), 0);
  EXPECT_EQ(object.size(), 1);
  EXPECT_FALSE(object.empty());

  EXPECT_EQ(object.erase(FieldId{2}), 1);
  EXPECT_EQ(object.size(), 0);
  EXPECT_TRUE(object.empty());
}

TEST(Value, Wrapper) {
  Object obj;
  obj.members()[100] = asValueStruct<type::string_t>("200");

  const std::vector<Value> l = {
      asValueStruct<type::i32_t>(10), asValueStruct<type::i32_t>(20)};

  const std::set<Value> s = {
      asValueStruct<type::i32_t>(30), asValueStruct<type::i32_t>(40)};

  const std::map<Value, Value> m = {
      {asValueStruct<type::i32_t>(50), asValueStruct<type::i32_t>(60)},
      {asValueStruct<type::i32_t>(70), asValueStruct<type::i32_t>(80)}};

  Value value;

#define FBTHRIFT_TEST_THRIFT_VALUE_TYPE(TYPE, VALUE)                   \
  do {                                                                 \
    EXPECT_FALSE(value.is_##TYPE());                                   \
    EXPECT_FALSE(value.TYPE##Value_ref());                             \
    EXPECT_THROW(value.as_##TYPE(), apache::thrift::bad_field_access); \
    EXPECT_EQ(value.if_##TYPE(), nullptr);                             \
    EXPECT_FALSE(value.TYPE##Value_ref());                             \
    value.emplace_##TYPE() = VALUE;                                    \
    EXPECT_TRUE(value.is_##TYPE());                                    \
    EXPECT_EQ(value.as_##TYPE(), VALUE);                               \
    EXPECT_EQ(*value.if_##TYPE(), VALUE);                              \
    EXPECT_EQ(value.TYPE##Value_ref(), VALUE);                         \
  } while (false)

  FBTHRIFT_TEST_THRIFT_VALUE_TYPE(bool, true);
  FBTHRIFT_TEST_THRIFT_VALUE_TYPE(byte, 1);
  FBTHRIFT_TEST_THRIFT_VALUE_TYPE(i16, 2);
  FBTHRIFT_TEST_THRIFT_VALUE_TYPE(i32, 3);
  FBTHRIFT_TEST_THRIFT_VALUE_TYPE(i64, 4);
  FBTHRIFT_TEST_THRIFT_VALUE_TYPE(float, 5);
  FBTHRIFT_TEST_THRIFT_VALUE_TYPE(double, 6);
  FBTHRIFT_TEST_THRIFT_VALUE_TYPE(string, "7");
  FBTHRIFT_TEST_THRIFT_VALUE_TYPE(object, obj);
  FBTHRIFT_TEST_THRIFT_VALUE_TYPE(list, l);
  FBTHRIFT_TEST_THRIFT_VALUE_TYPE(set, s);
  FBTHRIFT_TEST_THRIFT_VALUE_TYPE(map, m);

#undef FBTHRIFT_VALUE_TEST_TYPE

  // `binary` type requires special code since IOBuf doesn't have operator==
  const auto buf = *folly::IOBuf::copyBuffer("90");
  EXPECT_FALSE(value.is_binary());
  EXPECT_FALSE(value.binaryValue_ref());
  EXPECT_THROW(value.as_binary(), apache::thrift::bad_field_access);
  EXPECT_EQ(value.if_binary(), nullptr);
  EXPECT_FALSE(value.binaryValue_ref());
  value.emplace_binary() = buf;
  EXPECT_TRUE(value.is_binary());
  EXPECT_TRUE(folly::IOBufEqualTo{}(value.as_binary(), buf));
  EXPECT_TRUE(folly::IOBufEqualTo{}(*value.if_binary(), buf));
  EXPECT_TRUE(folly::IOBufEqualTo{}(value.binaryValue_ref().value(), buf));
}

TEST(Value, IsIntrinsicDefaultTrue) {
  EXPECT_TRUE(isIntrinsicDefault(asValueStruct<type::bool_t>(false)));
  EXPECT_TRUE(isIntrinsicDefault(asValueStruct<type::byte_t>(0)));
  EXPECT_TRUE(isIntrinsicDefault(asValueStruct<type::i16_t>(0)));
  EXPECT_TRUE(isIntrinsicDefault(asValueStruct<type::i32_t>(0)));
  EXPECT_TRUE(isIntrinsicDefault(asValueStruct<type::i64_t>(0)));
  EXPECT_TRUE(isIntrinsicDefault(asValueStruct<type::float_t>(0.0)));
  EXPECT_TRUE(isIntrinsicDefault(asValueStruct<type::double_t>(0.0)));
  EXPECT_TRUE(isIntrinsicDefault(asValueStruct<type::string_t>("")));
  EXPECT_TRUE(isIntrinsicDefault(asValueStruct<type::binary_t>("")));
  EXPECT_TRUE(isIntrinsicDefault(
      asValueStruct<type::list<type::string_t>>(std::vector<std::string>{})));
  EXPECT_TRUE(isIntrinsicDefault(
      asValueStruct<type::set<type::i64_t>>(std::set<int>{})));
  EXPECT_TRUE(
      isIntrinsicDefault(asValueStruct<type::map<type::i32_t, type::string_t>>(
          std::map<int, std::string>{})));
  testset::struct_with<type::map<type::string_t, type::i32_t>> s;
  s.field_1_ref() = std::map<std::string, int>{};
  Value objectValue = asValueStruct<type::struct_c>(s);
  EXPECT_TRUE(isIntrinsicDefault(objectValue));
  EXPECT_TRUE(isIntrinsicDefault(objectValue.as_object()));
  EXPECT_TRUE(isIntrinsicDefault(Value{}));
}

TEST(Value, IsIntrinsicDefaultFalse) {
  EXPECT_FALSE(isIntrinsicDefault(asValueStruct<type::bool_t>(true)));
  EXPECT_FALSE(isIntrinsicDefault(asValueStruct<type::byte_t>(1)));
  EXPECT_FALSE(isIntrinsicDefault(asValueStruct<type::i16_t>(1)));
  EXPECT_FALSE(isIntrinsicDefault(asValueStruct<type::i32_t>(1)));
  EXPECT_FALSE(isIntrinsicDefault(asValueStruct<type::i64_t>(1)));
  EXPECT_FALSE(isIntrinsicDefault(asValueStruct<type::float_t>(0.5)));
  EXPECT_FALSE(isIntrinsicDefault(asValueStruct<type::double_t>(0.5)));
  EXPECT_FALSE(isIntrinsicDefault(asValueStruct<type::string_t>("foo")));
  EXPECT_FALSE(isIntrinsicDefault(asValueStruct<type::binary_t>("foo")));
  EXPECT_FALSE(isIntrinsicDefault(asValueStruct<type::list<type::string_t>>(
      std::vector<std::string>{"foo"})));
  EXPECT_FALSE(isIntrinsicDefault(
      asValueStruct<type::set<type::i64_t>>(std::set<int>{1, 2, 3})));
  EXPECT_FALSE(
      isIntrinsicDefault(asValueStruct<type::map<type::i32_t, type::string_t>>(
          std::map<int, std::string>{{1, "foo"}, {2, "bar"}})));
  testset::struct_with<type::map<type::string_t, type::i32_t>> s;
  s.field_1_ref() = std::map<std::string, int>{{"foo", 1}, {"bar", 2}};
  Value objectValue = asValueStruct<type::struct_c>(s);
  EXPECT_FALSE(isIntrinsicDefault(objectValue));
  EXPECT_FALSE(isIntrinsicDefault(objectValue.as_object()));
}

template <typename ProtocolReader>
Value parseValue(const EncodedValue& encodedValue, TType type) {
  EXPECT_EQ(encodedValue.wireType().value(), type::toBaseType(type));
  ProtocolReader prot;
  prot.setInput(&encodedValue.data().value());
  return detail::parseValue(prot, type, false);
}

// Tests parseObject (and serializeObject if testSerialize) with mask.
template <::apache::thrift::conformance::StandardProtocol Protocol>
void testParseObjectWithMask(bool testSerialize) {
  Object obj, foo, bar, expected;
  // obj{1: 3,
  //     2: {1: "foo"}
  //     3: {5: {1: "foo"},
  //         6: true}3}
  foo[FieldId{1}].stringValue_ref() = "foo";
  bar[FieldId{5}].objectValue_ref() = foo;
  bar[FieldId{6}].boolValue_ref() = true;
  obj[FieldId{1}].i16Value_ref() = 3;
  obj[FieldId{2}].objectValue_ref() = foo;
  obj[FieldId{3}].objectValue_ref() = bar;

  // masks obj[2] and obj[3][6]
  Mask mask;
  auto& includes = mask.includes_ref().emplace();
  includes[2] = allMask();
  includes[3].excludes_ref().emplace()[5] = allMask();

  // expected{2: {1: "foo"}
  //          3: {6: true}}
  expected[FieldId{2}].objectValue_ref() = foo;
  expected[FieldId{3}].objectValue_ref().emplace()[FieldId{6}].boolValue_ref() =
      true;

  // serialize the object and deserialize with mask
  auto serialized = protocol::serializeObject<protocol_writer_t<Protocol>>(obj);
  MaskedDecodeResult result =
      parseObject<protocol_reader_t<Protocol>>(*serialized, mask, false);

  if (testSerialize) {
    // test serializeObject with mask
    auto reserialized = protocol::serializeObject<protocol_writer_t<Protocol>>(
        result.included, result.excluded);
    Object finalObj =
        parseObject<protocol_reader_t<Protocol>>(*reserialized, false);
    EXPECT_EQ(finalObj, obj);
    return;
  }

  // manually check the result
  EXPECT_EQ(*result.excluded.protocol(), convertStandardProtocol(Protocol));
  EXPECT_EQ(result.included, expected);
  auto& values = *result.excluded.values();
  EXPECT_EQ(values.size(), 2);
  // Excluded should contain obj[1] and obj[3][5].
  auto& excludedFields = result.excluded.data()->fields_ref().value();
  EXPECT_EQ(excludedFields.size(), 2);
  auto& i16Encoded = detail::getByValueId(
      values, excludedFields.at(FieldId{1}).full_ref().value());
  EXPECT_EQ(
      parseValue<protocol_reader_t<Protocol>>(i16Encoded, T_I16).as_i16(), 3);
  auto& nestedExcludedFields =
      excludedFields.at(FieldId{3}).fields_ref().value();
  EXPECT_EQ(nestedExcludedFields.size(), 1);
  auto& objectEncoded = detail::getByValueId(
      values, nestedExcludedFields.at(FieldId{5}).full_ref().value());
  EXPECT_EQ(
      parseValue<protocol_reader_t<Protocol>>(objectEncoded, T_STRUCT)
          .as_object(),
      foo);
}

template <::apache::thrift::conformance::StandardProtocol Protocol>
void testSerializeObjectWithMask() {
  Object obj, foo;
  // obj{1: {1: "foo",
  //         2: "bar"},
  //     2: 2,
  //     3: 3}
  foo[FieldId{1}].stringValue_ref() = "foo";
  foo[FieldId{2}].stringValue_ref() = "bar";
  obj[FieldId{1}].objectValue_ref() = foo;
  obj[FieldId{2}].i32Value_ref() = 2;
  obj[FieldId{3}].i32Value_ref() = 3;

  // masks obj[1][1] and obj[2]
  Mask mask;
  auto& includes = mask.includes_ref().emplace();
  includes[1].includes_ref().emplace()[1] = allMask();
  includes[2] = allMask();

  // serialize the object and deserialize with mask
  auto serialized = protocol::serializeObject<protocol_writer_t<Protocol>>(obj);
  MaskedDecodeResult result =
      parseObject<protocol_reader_t<Protocol>>(*serialized, mask, false);
  {
    Object expected, bar;
    // expected{1: {1: "foo"},
    //          2: 2}
    bar[FieldId{1}].stringValue_ref() = "foo";
    expected[FieldId{1}].objectValue_ref() = bar;
    expected[FieldId{2}].i32Value_ref() = 2;
    EXPECT_EQ(result.included, expected);
  }

  {
    // reserialize with the unmodified object
    auto reserialized = protocol::serializeObject<protocol_writer_t<Protocol>>(
        result.included, result.excluded);
    Object finalObj =
        parseObject<protocol_reader_t<Protocol>>(*reserialized, false);
    EXPECT_EQ(finalObj, obj);
  }

  {
    // reserialize with the modified object
    Object modified, baz;
    // modified{1: {3: "baz"},
    //          4: 4}
    baz[FieldId{3}].stringValue_ref() = "baz";
    modified[FieldId{1}].objectValue_ref() = baz;
    modified[FieldId{4}].i32Value_ref() = 4;

    Object expected, bar;
    // expected{1: {2: "bar",
    //              3: "baz"},
    //          3: 3,
    //          4: 4}
    bar[FieldId{2}].stringValue_ref() = "bar";
    bar[FieldId{3}].stringValue_ref() = "baz";
    expected[FieldId{1}].objectValue_ref() = bar;
    expected[FieldId{3}].i32Value_ref() = 3;
    expected[FieldId{4}].i32Value_ref() = 4;

    auto reserialized = protocol::serializeObject<protocol_writer_t<Protocol>>(
        modified, result.excluded);
    Object finalObj =
        parseObject<protocol_reader_t<Protocol>>(*reserialized, false);
    EXPECT_EQ(finalObj, expected);
  }
}

template <::apache::thrift::conformance::StandardProtocol Protocol>
void testSerializeObjectWithMaskError() {
  Object obj, foo;
  // obj{1: {1: "foo"}}
  foo[FieldId{1}].stringValue_ref() = "foo";
  obj[FieldId{1}].objectValue_ref() = foo;

  {
    // MaskedData[1] is full, which should be fields.
    MaskedProtocolData protocolData;
    protocolData.protocol() = convertStandardProtocol(Protocol);
    MaskedData& maskedData = protocolData.data_ref().value();
    maskedData.fields_ref().ensure()[FieldId{1}].full_ref() = type::ValueId{1};

    EXPECT_THROW(
        protocol::serializeObject<protocol_writer_t<Protocol>>(
            obj, protocolData),
        std::runtime_error);
  }
  {
    // MaskedData[2] is fields, which should be full.
    MaskedProtocolData protocolData;
    protocolData.protocol() = convertStandardProtocol(Protocol);
    MaskedData& maskedData = protocolData.data_ref().value();
    maskedData.fields_ref().ensure()[FieldId{2}].fields_ref().ensure();

    EXPECT_THROW(
        protocol::serializeObject<protocol_writer_t<Protocol>>(
            obj, protocolData),
        std::runtime_error);
  }
}

TEST(Object, ParseObjectWithMask) {
  testParseObjectWithMask<
      ::apache::thrift::conformance::StandardProtocol::Compact>(false);
  testParseObjectWithMask<
      ::apache::thrift::conformance::StandardProtocol::Binary>(false);
}

TEST(Object, SerializeObjectWithMask) {
  testParseObjectWithMask<
      ::apache::thrift::conformance::StandardProtocol::Compact>(true);
  testParseObjectWithMask<
      ::apache::thrift::conformance::StandardProtocol::Binary>(true);
  testSerializeObjectWithMask<
      ::apache::thrift::conformance::StandardProtocol::Compact>();
  testSerializeObjectWithMask<
      ::apache::thrift::conformance::StandardProtocol::Binary>();
  testSerializeObjectWithMaskError<
      ::apache::thrift::conformance::StandardProtocol::Compact>();
  testSerializeObjectWithMaskError<
      ::apache::thrift::conformance::StandardProtocol::Binary>();
}

// called by testParseObjectWithMapMask when testSerialize=true
template <::apache::thrift::conformance::StandardProtocol Protocol>
void testSerializeObjectWithMapMask(MaskedDecodeResult& result, Object& obj) {
  {
    // test serializeObject with mask
    // reserialize with the unmodified object
    auto reserialized = protocol::serializeObject<protocol_writer_t<Protocol>>(
        result.included, result.excluded);
    Object finalObj =
        parseObject<protocol_reader_t<Protocol>>(*reserialized, false);
    EXPECT_EQ(finalObj, obj);
  }
  {
    // reserialize with the modified object
    Object modified;
    // modified{1: map{10: {},
    //                 30: {"foo": 1}}}
    // This tests when map is empty and types are determined from MaskedData.
    std::map<int, std::map<std::string, int>> modifiedMap = {
        {10, {}}, {30, {{"foo", 1}}}};
    modified[FieldId{1}] = asValueStruct<
        type::map<type::i16_t, type::map<type::string_t, type::i32_t>>>(
        modifiedMap);

    Object expected;
    // expected{1: map{10: {"bar": 2},
    //                 20: {"baz": 3},
    //                 30: {"foo": 1}}}
    std::map<int, std::map<std::string, int>> expectedMap = {
        {10, {{"bar", 2}}}, {20, {{"baz", 3}}}, {30, {{"foo", 1}}}};
    expected[FieldId{1}] = asValueStruct<
        type::map<type::i16_t, type::map<type::string_t, type::i32_t>>>(
        expectedMap);

    auto reserialized = protocol::serializeObject<protocol_writer_t<Protocol>>(
        modified, result.excluded);
    Object finalObj =
        parseObject<protocol_reader_t<Protocol>>(*reserialized, false);
    EXPECT_EQ(finalObj, expected);
  }
}

template <::apache::thrift::conformance::StandardProtocol Protocol>
void testParseObjectWithMapMask(bool testSerialize) {
  Object obj;
  // obj{1: map{10: {"foo": 1,
  //                 "bar": 2},
  //            20: {"baz": 3}},
  //     2: set{1, 2, 3}}
  std::map<int, std::map<std::string, int>> map = {
      {10, {{"foo", 1}, {"bar", 2}}}, {20, {{"baz", 3}}}};
  obj[FieldId{1}] = asValueStruct<
      type::map<type::i16_t, type::map<type::string_t, type::i32_t>>>(map);
  std::set<int> set = {1, 2, 3};
  obj[FieldId{2}] = asValueStruct<type::set<type::byte_t>>(set);
  auto serialized = protocol::serializeObject<protocol_writer_t<Protocol>>(obj);

  // masks obj[1][10]["foo"] and obj[2]
  Mask mask;
  Value key10 = asValueStruct<type::i16_t>(10);
  Value key20 = asValueStruct<type::i16_t>(20);
  Value keyFoo = asValueStruct<type::string_t>("foo");
  Value keyBar = asValueStruct<type::string_t>("bar");
  auto& includes = mask.includes_ref().emplace();
  includes[1]
      .includes_map_ref()
      .emplace()[(int64_t)&key10]
      .includes_map_ref()
      .emplace()[(int64_t)&keyFoo] = allMask();
  // This is treated as allMask() as the type is set. It tests the edge case
  // that a set field may have a map mask, since extractMaskFromPatch cannot
  // determine if a patch is for map or set for some operators.
  includes[2].excludes_map_ref().emplace()[99] = allMask();

  Object expected;
  // expected{1: map{10: {"foo": 1}},
  //          2: set{1, 2, 3}}
  std::map<int, std::map<std::string, int>> expectedMap = {{10, {{"foo", 1}}}};
  expected[FieldId{1}] = asValueStruct<
      type::map<type::i16_t, type::map<type::string_t, type::i32_t>>>(
      expectedMap);
  expected[FieldId{2}] = asValueStruct<type::set<type::byte_t>>(set);

  // serialize the object and deserialize with mask
  MaskedDecodeResult result =
      parseObject<protocol_reader_t<Protocol>>(*serialized, mask, false);

  if (testSerialize) {
    testSerializeObjectWithMapMask<Protocol>(result, obj);
    return;
  }

  // manually check the result
  EXPECT_EQ(result.included, expected);
  EXPECT_EQ(*result.excluded.protocol(), convertStandardProtocol(Protocol));
  auto& values = *result.excluded.values();
  EXPECT_EQ(values.size(), 2); // map[10]["bar"] and map[20]
  auto& keys = *result.excluded.keys();
  EXPECT_EQ(keys.size(), 3); // 10, 20, and "bar"

  auto getKeyValueId = [&](Value& key) {
    auto it = std::find(keys.begin(), keys.end(), detail::makeValueStruct(key));
    EXPECT_NE(it, keys.end()); // It should find the value.
    return type::ValueId{apache::thrift::util::i32ToZigzag(it - keys.begin())};
  };

  // Excluded should contain map[10]["bar"] and map[20]
  auto& excludedKeys =
      result.excluded.data()->fields_ref()->at(FieldId{1}).values_ref().value();
  EXPECT_EQ(excludedKeys.size(), 2);
  // check map[20]
  {
    auto& mapEncoded = detail::getByValueId(
        values, excludedKeys.at(getKeyValueId(key20)).full_ref().value());
    EXPECT_EQ(
        parseValue<protocol_reader_t<Protocol>>(mapEncoded, T_MAP).as_map(),
        obj[FieldId{1}].as_map()[key20].as_map());
  }
  // check map[10]["bar"]
  {
    auto& nestedExcludedKeys =
        excludedKeys.at(getKeyValueId(key10)).values_ref().value();
    EXPECT_EQ(nestedExcludedKeys.size(), 1);
    auto& i32Encoded = detail::getByValueId(
        values,
        nestedExcludedKeys.at(getKeyValueId(keyBar)).full_ref().value());
    EXPECT_EQ(
        parseValue<protocol_reader_t<Protocol>>(i32Encoded, T_I32).as_i32(), 2);
  }
}

template <::apache::thrift::conformance::StandardProtocol Protocol>
void testSerializeObjectWithMapMaskError() {
  Object obj;
  // obj{1: map{1: "foo"}}
  std::map<int, std::string> map = {{1, "foo"}};
  obj[FieldId{1}] = asValueStruct<type::map<type::i32_t, type::string_t>>(map);

  {
    // MaskedData[1] is full, which should be values.
    MaskedProtocolData protocolData;
    protocolData.protocol() = convertStandardProtocol(Protocol);
    MaskedData& maskedData = protocolData.data_ref().value();
    maskedData.fields_ref().ensure()[FieldId{1}].full_ref() = type::ValueId{1};

    EXPECT_THROW(
        protocol::serializeObject<protocol_writer_t<Protocol>>(
            obj, protocolData),
        std::runtime_error);
  }
  {
    // MaskedData[1][2] is values, which should be full.
    MaskedProtocolData protocolData;
    protocolData.protocol() = convertStandardProtocol(Protocol);
    MaskedData& maskedData = protocolData.data_ref().value();
    auto& keys = protocolData.keys().ensure();
    keys.push_back(makeValueStruct(asValueStruct<type::i32_t>(2)));
    type::ValueId keyValueId =
        type::ValueId{apache::thrift::util::i32ToZigzag(keys.size() - 1)};
    maskedData.fields_ref()
        .ensure()[FieldId{1}]
        .values_ref()
        .ensure()[keyValueId]
        .values_ref()
        .ensure();

    EXPECT_THROW(
        protocol::serializeObject<protocol_writer_t<Protocol>>(
            obj, protocolData),
        std::runtime_error);
  }
}

TEST(Object, ParseObjectWithMapMask) {
  testParseObjectWithMapMask<
      ::apache::thrift::conformance::StandardProtocol::Compact>(false);
  testParseObjectWithMapMask<
      ::apache::thrift::conformance::StandardProtocol::Binary>(false);
}

TEST(Object, SerializeObjectWithMapMask) {
  testParseObjectWithMapMask<
      ::apache::thrift::conformance::StandardProtocol::Compact>(true);
  testParseObjectWithMapMask<
      ::apache::thrift::conformance::StandardProtocol::Binary>(true);
  testSerializeObjectWithMapMaskError<
      ::apache::thrift::conformance::StandardProtocol::Compact>();
  testSerializeObjectWithMapMaskError<
      ::apache::thrift::conformance::StandardProtocol::Binary>();
}

template <::apache::thrift::conformance::StandardProtocol Protocol>
void testParseObjectWithTwoMasks() {
  Object obj, foo;
  // obj{1: {1: "foo",
  //         2: "bar"},
  //     2: 2,
  //     3: 3,
  //     4: map{10: {"foo": 1,
  //                 "bar": 2},
  //            20: {"baz": 3}}}
  foo[FieldId{1}].stringValue_ref() = "foo";
  foo[FieldId{2}].stringValue_ref() = "bar";
  obj[FieldId{1}].objectValue_ref() = foo;
  obj[FieldId{2}].i32Value_ref() = 2;
  obj[FieldId{3}].i32Value_ref() = 3;
  std::map<int, std::map<std::string, int>> map = {
      {10, {{"foo", 1}, {"bar", 2}}}, {20, {{"baz", 3}}}};
  obj[FieldId{4}] = asValueStruct<
      type::map<type::i16_t, type::map<type::string_t, type::i32_t>>>(map);

  Value key10 = asValueStruct<type::i16_t>(10);
  Value key20 = asValueStruct<type::i16_t>(20);
  Value keyFoo = asValueStruct<type::string_t>("foo");
  Value keyBaz = asValueStruct<type::string_t>("baz");

  // masks obj[2] and obj[4][10]["foo"]
  Mask readMask;
  {
    auto& includes = readMask.includes_ref().emplace();
    includes[2] = allMask();
    includes[4]
        .includes_map_ref()
        .emplace()[(int64_t)&key10]
        .includes_map_ref()
        .emplace()[(int64_t)&keyFoo] = allMask();
  }

  // masks obj[1][1], obj[3], obj[4][10], and obj[4][20]["baz"]
  Mask writeMask;
  {
    auto& includes = writeMask.includes_ref().emplace();
    includes[1].includes_ref().emplace()[1] = allMask();
    includes[3] = allMask();
    auto& includes_map = includes[4].includes_map_ref().emplace();
    includes_map[(int64_t)&key10] = allMask();
    includes_map[(int64_t)&key20]
        .includes_map_ref()
        .emplace()[(int64_t)&keyBaz] = allMask();
  }

  // serialize the object and deserialize with mask
  auto serialized = protocol::serializeObject<protocol_writer_t<Protocol>>(obj);
  MaskedDecodeResult result = parseObject<protocol_reader_t<Protocol>>(
      *serialized, readMask, writeMask, false);
  {
    Object expected;
    // expected{1: {},
    //          2: 2,
    //          4: map{10: {"foo": 1}
    //                 20: {}}}
    expected[FieldId{1}].objectValue_ref().emplace();
    expected[FieldId{2}].i32Value_ref() = 2;
    std::map<int, std::map<std::string, int>> expectedMap = {
        {10, {{"foo", 1}}}, {20, {}}};
    expected[FieldId{4}] = asValueStruct<
        type::map<type::i16_t, type::map<type::string_t, type::i32_t>>>(
        expectedMap);
    EXPECT_EQ(result.included, expected);
  }

  {
    // reserialize with the object and MaskedData
    Object expected, bar;
    // expected{1: {2: "bar"},
    //          2: 2,
    //          4: map{10: {"foo": 1},
    //                 20: {}}}
    bar[FieldId{2}].stringValue_ref() = "bar";
    expected[FieldId{1}].objectValue_ref() = bar;
    expected[FieldId{2}].i32Value_ref() = 2;
    std::map<int, std::map<std::string, int>> expectedMap = {
        {10, {{"foo", 1}}}, {20, {}}};
    expected[FieldId{4}] = asValueStruct<
        type::map<type::i16_t, type::map<type::string_t, type::i32_t>>>(
        expectedMap);
    auto reserialized = protocol::serializeObject<protocol_writer_t<Protocol>>(
        result.included, result.excluded);
    Object finalObj =
        parseObject<protocol_reader_t<Protocol>>(*reserialized, false);
    EXPECT_EQ(finalObj, expected);
  }
}

TEST(Object, ParseObjectWithTwoMasks) {
  testParseObjectWithTwoMasks<
      ::apache::thrift::conformance::StandardProtocol::Compact>();
  testParseObjectWithTwoMasks<
      ::apache::thrift::conformance::StandardProtocol::Binary>();
}

} // namespace
} // namespace apache::thrift::protocol

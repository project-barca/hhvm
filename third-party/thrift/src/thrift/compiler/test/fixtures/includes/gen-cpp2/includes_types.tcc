/**
 * Autogenerated by Thrift for src/includes.thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated @nocommit
 */
#pragma once

#include "thrift/compiler/test/fixtures/includes/gen-cpp2/includes_types.h"

#include <thrift/lib/cpp2/gen/module_types_tcc.h>
#include <thrift/lib/cpp2/op/Clear.h>
#include <thrift/lib/cpp2/op/Get.h>


namespace apache {
namespace thrift {
namespace detail {

template <>
struct TccStructTraits<::cpp2::Included> {
  static void translateFieldName(
      folly::StringPiece _fname,
      int16_t& fid,
      apache::thrift::protocol::TType& _ftype) noexcept;
};

} // namespace detail
} // namespace thrift
} // namespace apache

namespace cpp2 {

template <class Protocol_>
void Included::readNoXfer(Protocol_* iprot) {
  __fbthrift_clear_terse_fields();

  apache::thrift::detail::ProtocolReaderStructReadState<Protocol_> _readState;

  _readState.readStructBegin(iprot);

  using apache::thrift::TProtocolException;


  if (UNLIKELY(!_readState.advanceToNextField(
          iprot,
          0,
          1,
          apache::thrift::protocol::T_I64))) {
    goto _loop;
  }
_readField_MyIntField:
  {
    ::apache::thrift::detail::pm::protocol_methods<::apache::thrift::type_class::integral, ::std::int64_t>::readWithContext(*iprot, this->__fbthrift_field_MyIntField, _readState);
    
  }
 this->__isset.set(0, true);

  if (UNLIKELY(!_readState.advanceToNextField(
          iprot,
          1,
          2,
          apache::thrift::protocol::T_STRUCT))) {
    goto _loop;
  }
_readField_MyTransitiveField:
  {
    _readState.beforeSubobject(iprot);
    ::apache::thrift::detail::pm::protocol_methods<::apache::thrift::type_class::structure, ::cpp2::Foo>::readWithContext(*iprot, this->__fbthrift_field_MyTransitiveField, _readState);
    _readState.afterSubobject(iprot);
    
  }
 this->__isset.set(1, true);

  if (UNLIKELY(!_readState.advanceToNextField(
          iprot,
          2,
          0,
          apache::thrift::protocol::T_STOP))) {
    goto _loop;
  }

_end:
  _readState.readStructEnd(iprot);

  return;

_loop:
  _readState.afterAdvanceFailure(iprot);
  if (_readState.atStop()) {
    goto _end;
  }
  if (iprot->kUsesFieldNames()) {
    _readState.template fillFieldTraitsFromName<apache::thrift::detail::TccStructTraits<Included>>();
  }

  switch (_readState.fieldId) {
    case 1:
    {
      if (LIKELY(_readState.isCompatibleWithType(iprot, apache::thrift::protocol::T_I64))) {
        goto _readField_MyIntField;
      } else {
        goto _skip;
      }
    }
    case 2:
    {
      if (LIKELY(_readState.isCompatibleWithType(iprot, apache::thrift::protocol::T_STRUCT))) {
        goto _readField_MyTransitiveField;
      } else {
        goto _skip;
      }
    }
    default:
    {
_skip:
      _readState.skip(iprot);
      _readState.readFieldEnd(iprot);
      _readState.readFieldBeginNoInline(iprot);
      goto _loop;
    }
  }
}

template <class Protocol_>
uint32_t Included::serializedSize(Protocol_ const* prot_) const {
  uint32_t xfer = 0;
  xfer += prot_->serializedStructSize("Included");
  {
    xfer += prot_->serializedFieldSize("MyIntField", apache::thrift::protocol::T_I64, 1);
    xfer += ::apache::thrift::detail::pm::protocol_methods<::apache::thrift::type_class::integral, ::std::int64_t>::serializedSize<false>(*prot_, this->__fbthrift_field_MyIntField);
  }
  {
    xfer += prot_->serializedFieldSize("MyTransitiveField", apache::thrift::protocol::T_STRUCT, 2);
    xfer += ::apache::thrift::detail::pm::protocol_methods<::apache::thrift::type_class::structure, ::cpp2::Foo>::serializedSize<false>(*prot_, this->__fbthrift_field_MyTransitiveField);
  }
  xfer += prot_->serializedSizeStop();
  return xfer;
}

template <class Protocol_>
uint32_t Included::serializedSizeZC(Protocol_ const* prot_) const {
  uint32_t xfer = 0;
  xfer += prot_->serializedStructSize("Included");
  {
    xfer += prot_->serializedFieldSize("MyIntField", apache::thrift::protocol::T_I64, 1);
    xfer += ::apache::thrift::detail::pm::protocol_methods<::apache::thrift::type_class::integral, ::std::int64_t>::serializedSize<false>(*prot_, this->__fbthrift_field_MyIntField);
  }
  {
    xfer += prot_->serializedFieldSize("MyTransitiveField", apache::thrift::protocol::T_STRUCT, 2);
    xfer += ::apache::thrift::detail::pm::protocol_methods<::apache::thrift::type_class::structure, ::cpp2::Foo>::serializedSize<true>(*prot_, this->__fbthrift_field_MyTransitiveField);
  }
  xfer += prot_->serializedSizeStop();
  return xfer;
}

template <class Protocol_>
uint32_t Included::write(Protocol_* prot_) const {
  uint32_t xfer = 0;
  xfer += prot_->writeStructBegin("Included");
  bool previousFieldHasValue = true;
  {
    constexpr int16_t kPrevFieldId = 0;
    xfer += ::apache::thrift::detail::writeFieldBegin<apache::thrift::protocol::T_I64, 1, kPrevFieldId>(*prot_, "MyIntField", previousFieldHasValue);
    previousFieldHasValue = true;
    xfer += ::apache::thrift::detail::pm::protocol_methods<::apache::thrift::type_class::integral, ::std::int64_t>::write(*prot_, this->__fbthrift_field_MyIntField);
    xfer += prot_->writeFieldEnd();
  }
  {
    constexpr int16_t kPrevFieldId = 1;
    xfer += ::apache::thrift::detail::writeFieldBegin<apache::thrift::protocol::T_STRUCT, 2, kPrevFieldId>(*prot_, "MyTransitiveField", previousFieldHasValue);
    previousFieldHasValue = true;
    xfer += ::apache::thrift::detail::pm::protocol_methods<::apache::thrift::type_class::structure, ::cpp2::Foo>::write(*prot_, this->__fbthrift_field_MyTransitiveField);
    xfer += prot_->writeFieldEnd();
  }
  xfer += prot_->writeFieldStop();
  xfer += prot_->writeStructEnd();
  return xfer;
}

extern template void Included::readNoXfer<>(apache::thrift::BinaryProtocolReader*);
extern template uint32_t Included::write<>(apache::thrift::BinaryProtocolWriter*) const;
extern template uint32_t Included::serializedSize<>(apache::thrift::BinaryProtocolWriter const*) const;
extern template uint32_t Included::serializedSizeZC<>(apache::thrift::BinaryProtocolWriter const*) const;
extern template void Included::readNoXfer<>(apache::thrift::CompactProtocolReader*);
extern template uint32_t Included::write<>(apache::thrift::CompactProtocolWriter*) const;
extern template uint32_t Included::serializedSize<>(apache::thrift::CompactProtocolWriter const*) const;
extern template uint32_t Included::serializedSizeZC<>(apache::thrift::CompactProtocolWriter const*) const;


} // cpp2

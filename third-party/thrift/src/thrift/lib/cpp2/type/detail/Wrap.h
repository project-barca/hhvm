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

#include <stdexcept>
#include <type_traits>

#include <folly/Portability.h>
#include <folly/Traits.h>
#include <thrift/lib/cpp2/op/Clear.h>
#include <thrift/lib/cpp2/op/Compare.h>
#include <thrift/lib/cpp2/op/Get.h>
#include <thrift/lib/cpp2/op/detail/Compare.h>
#include <thrift/lib/cpp2/type/Tag.h>
#include <thrift/lib/cpp2/type/TagUtil.h>

namespace apache {
namespace thrift {
namespace type {
namespace detail {

// A base class the "curiously reoccurring template pattern" with helpers that
// propagate const&-ness.
template <typename D>
class BaseDerived {
 protected:
  constexpr D& derived() & noexcept { return static_cast<D&>(*this); }
  constexpr D&& derived() && noexcept { return static_cast<D&&>(*this); }
  constexpr const D& derived() const& noexcept {
    return static_cast<const D&>(*this);
  }
  constexpr const D&& derived() const&& noexcept {
    return static_cast<const D&&>(*this);
  }
};

// A base class for a wrapper around a Thrift value.
//
// Specifically designed to work with thrift::InlineAdapter.
//
// TODO(afuller): Consider expanding features and making public.
template <typename T, typename Tag = structured_tag<T>>
class Wrap {
 public:
  using underlying_type = T;
  using underlying_tag = Tag;

  Wrap() = default;
  explicit Wrap(const underlying_type& data) : data_(data) {}
  explicit Wrap(underlying_type&& data) noexcept : data_(std::move(data)) {}

  FOLLY_NODISCARD underlying_type& toThrift() & { return data_; }
  FOLLY_NODISCARD const underlying_type& toThrift() const& { return data_; }
  FOLLY_NODISCARD underlying_type&& toThrift() && { return std::move(data_); }
  FOLLY_NODISCARD const underlying_type&& toThrift() const&& {
    return std::move(data_);
  }

  void reset() { op::clear<underlying_tag>(data_); }
  bool empty() const {
    return op::isEmpty<underlying_tag>(data_)
        // TODO(afuller): Remove when terse_write is enabled:
        || data_ == T{};
  }

 protected:
  T data_;

  // Gets the field reference, for the given id, in the given structured data.
  template <typename Id, typename U>
  constexpr static decltype(auto) get(Id, U&& data) {
    return op::get<folly::remove_cvref_t<U>, Id>(std::forward<U>(data));
  }

  // Gets the field reference, for the given id, in the underlying data.
  template <typename Id>
  constexpr decltype(auto) get(Id id = Id{}) & {
    return get(id, data_);
  }
  template <typename Id>
  constexpr decltype(auto) get(Id id = Id{}) && {
    return get(id, std::move(data_));
  }
  template <typename Id>
  constexpr decltype(auto) get(Id id = Id{}) const& {
    return get(id, data_);
  }
  template <typename Id>
  constexpr decltype(auto) get(Id id = Id{}) const&& {
    return get(id, std::move(data_));
  }

  ~Wrap() = default; // abstract base class

  // A fluent version of 'reset()'.
  FOLLY_NODISCARD T& resetAnd() { return (reset(), data_); }
};

template <typename Derived, typename T, typename Tag = structured_tag<T>>
class EqWrap : public Wrap<T, Tag>, protected BaseDerived<Derived> {
  using Base = Wrap<T, Tag>;

 public:
  using Base::Base;

 protected:
  // Down-cast to access protected member.
  static const T& data(const Derived& val) {
    return static_cast<const EqWrap&>(val).data_;
  }
  static T& data(Derived& val) { return static_cast<EqWrap&>(val).data_; }
  static T&& data(Derived&& val) { return std::move(data(val)); }

  ~EqWrap() = default; // abstract base class

 private:
  friend class BaseDerived<Derived>;

  friend constexpr bool operator==(const Derived& lhs, const Derived& rhs) {
    return op::equal<Tag>(data(lhs), data(rhs));
  }
  friend constexpr bool operator!=(const Derived& lhs, const Derived& rhs) {
    return !(lhs == rhs);
  }
};

template <typename Derived, typename T, typename Tag = structured_tag<T>>
class CompareWrap : public EqWrap<Derived, T, Tag> {
  using Base = EqWrap<Derived, T, Tag>;

 public:
  using Base::Base;

 protected:
  using Base::data;
  ~CompareWrap() = default; // abstract base class

  constexpr static folly::ordering cmp(const Derived& lhs, const Derived& rhs) {
    return op::compare<Tag>(data(lhs), data(rhs));
  }

 private:
  friend constexpr bool operator<(const Derived& lhs, const Derived& rhs) {
    return op::detail::is_lt(cmp(lhs, rhs));
  }
  friend constexpr bool operator<=(const Derived& lhs, const Derived& rhs) {
    return op::detail::is_lteq(cmp(lhs, rhs));
  }
  friend constexpr bool operator>(const Derived& lhs, const Derived& rhs) {
    return op::detail::is_gt(cmp(lhs, rhs));
  }
  friend constexpr bool operator>=(const Derived& lhs, const Derived& rhs) {
    return op::detail::is_gteq(cmp(lhs, rhs));
  }
};

} // namespace detail
} // namespace type
} // namespace thrift
} // namespace apache

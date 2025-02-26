/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */

#pragma once
#if __has_include(<src/gen-cpp2/MyService.h>)
#include <src/gen-cpp2/MyService.h>
#else
#include <src/gen-cpp2/service_clients.h>
#endif

#include <folly/futures/Future.h>
#include <folly/futures/Promise.h>
#include <folly/Unit.h>
#include <thrift/lib/py3/clientcallbacks.h>
#include <thrift/lib/py3/client_wrapper.h>

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <vector>

namespace cpp2 {

class MyServiceClientWrapper : public ::thrift::py3::ClientWrapper {
  public:
    using ::thrift::py3::ClientWrapper::ClientWrapper;

    folly::Future<folly::Unit> query(
      apache::thrift::RpcOptions& rpcOptions,
      ::cpp2::MyStruct arg_s,
      ::cpp2::Included arg_i);
    folly::Future<folly::Unit> has_arg_docs(
      apache::thrift::RpcOptions& rpcOptions,
      ::cpp2::MyStruct arg_s,
      ::cpp2::Included arg_i);
};


} // namespace cpp2

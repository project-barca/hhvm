/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */

#pragma once
#if __has_include(<src/gen-cpp2/NestedContainers.h>)
#include <src/gen-cpp2/NestedContainers.h>
#else
#include <src/gen-cpp2/module_clients.h>
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

class NestedContainersClientWrapper : public ::thrift::py3::ClientWrapper {
  public:
    using ::thrift::py3::ClientWrapper::ClientWrapper;

    folly::Future<folly::Unit> mapList(
      apache::thrift::RpcOptions& rpcOptions,
      std::map<int32_t,std::vector<int32_t>> arg_foo);
    folly::Future<folly::Unit> mapSet(
      apache::thrift::RpcOptions& rpcOptions,
      std::map<int32_t,std::set<int32_t>> arg_foo);
    folly::Future<folly::Unit> listMap(
      apache::thrift::RpcOptions& rpcOptions,
      std::vector<std::map<int32_t,int32_t>> arg_foo);
    folly::Future<folly::Unit> listSet(
      apache::thrift::RpcOptions& rpcOptions,
      std::vector<std::set<int32_t>> arg_foo);
    folly::Future<folly::Unit> turtles(
      apache::thrift::RpcOptions& rpcOptions,
      std::vector<std::vector<std::map<int32_t,std::map<int32_t,std::set<int32_t>>>>> arg_foo);
};


} // namespace cpp2

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

package com.facebook.thrift.adapter.common;

import com.facebook.thrift.protocol.ByteBufTProtocol;
import io.netty.buffer.ByteBuf;

/**
 * Use this type adapter if you want a to get a zero copy slice of the binary field. This will
 * create a retained slice so you must free the ByteBuf when you're not longer interested in using
 * it to prevent memory leaks. Use with caution.
 */
public class SlicedPooledByteBufTypeTypeAdapter extends AbstractByteBufTypeAdapter {
  @Override
  public ByteBuf fromThrift(ByteBufTProtocol protocol) {
    return protocol.readBinaryAsSlice().retain();
  }
}

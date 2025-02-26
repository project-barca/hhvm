# Copyright (c) Meta Platforms, Inc. and affiliates.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


from __future__ import annotations

from typing import Any, NamedTuple, Optional

from thrift.python.adapter import Adapter
from thrift.python.types import Struct

# pyre-fixme[2]: Parameter annotation cannot be `Any`.
class Wrapped(NamedTuple):
    # pyre-fixme[4]: Attribute annotation cannot be `Any`.
    obj: Any


class Wrapper(Adapter[Any, Wrapped]):
    @classmethod
    def from_thrift(
        cls,
        # pyre-fixme[2]: Parameter annotation cannot be `Any`.
        original: Any,
        *,
        transitive_annotation: Optional[Struct] = None,
    ) -> Wrapped:
        return Wrapped(obj=original)

    @classmethod
    # pyre-fixme[3]: Return annotation cannot be `Any`.
    def to_thrift(
        cls,
        adapted: Wrapped,
        *,
        transitive_annotation: Optional[Struct] = None,
    ) -> Any:
        return adapted.obj

////////////////////////////////////////////////////////////////////////////////
/// DISCLAIMER
///
/// Copyright 2014-2021 ArangoDB GmbH, Cologne, Germany
/// Copyright 2004-2014 triAGENS GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is ArangoDB GmbH, Cologne, Germany
///
/// @author Simon Grätzer
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstddef>
#include <cstdint>

#include "Basics/ErrorCode.h"

namespace arangodb {
namespace encoding {

template <typename T>
[[nodiscard]] ErrorCode gzipUncompress(uint8_t const* compressed, size_t compressedLength,
                                       T& uncompressed);

template <typename T>
[[nodiscard]] ErrorCode gzipInflate(uint8_t const* compressed, size_t compressedLength,
                                    T& uncompressed);

template <typename T>
[[nodiscard]] ErrorCode gzipDeflate(uint8_t const* uncompressed, size_t uncompressedLength,
                                    T& compressed);

}  // namespace encoding
}  // namespace arangodb

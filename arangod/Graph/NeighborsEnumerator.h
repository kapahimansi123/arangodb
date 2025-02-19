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
/// @author Michael Hackstein
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Basics/Common.h"
#include "Containers/FlatHashSet.h"
#include "Graph/PathEnumerator.h"

#include <velocypack/Slice.h>

namespace arangodb {
namespace graph {
// @brief Enumerator optimized for neighbors. Does not allow edge access

class NeighborsEnumerator final : public arangodb::traverser::PathEnumerator {
  containers::FlatHashSet<arangodb::velocypack::StringRef> _allFound;
  containers::FlatHashSet<arangodb::velocypack::StringRef> _currentDepth;
  containers::FlatHashSet<arangodb::velocypack::StringRef> _lastDepth;
  containers::FlatHashSet<arangodb::velocypack::StringRef>::iterator _iterator;
  containers::FlatHashSet<arangodb::velocypack::StringRef> _toPrune;

  uint64_t _searchDepth;

 public:
  NeighborsEnumerator(arangodb::traverser::Traverser* traverser,
                      arangodb::traverser::TraverserOptions* opts);

  ~NeighborsEnumerator() = default;

  void setStartVertex(arangodb::velocypack::StringRef startVertex) override;

  /// @brief Get the next Path element from the traversal.
  bool next() override;

  aql::AqlValue lastVertexToAqlValue() override;

  aql::AqlValue lastEdgeToAqlValue() override;

  aql::AqlValue pathToAqlValue(arangodb::velocypack::Builder& result) override;

 private:
  void swapLastAndCurrentDepth();

  bool shouldPrune(arangodb::velocypack::StringRef v);
};

}  // namespace graph
}  // namespace arangodb


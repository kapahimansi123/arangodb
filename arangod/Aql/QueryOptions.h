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
/// @author Jan Steemann
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include "Aql/types.h"
#include "Basics/Common.h"
#include "Transaction/Options.h"

namespace arangodb {
namespace velocypack {
class Builder;
class Slice;
}  // namespace velocypack

namespace aql {

enum class ProfileLevel : uint8_t {
  /// no profiling information
  None = 0,
  /// Output timing for query stages
  Basic = 1,
  /// Enable instrumentation for execute calls
  Blocks = 2,
  /// Log tracing info for execute calls
  TraceOne = 3,
  /// Log tracing information including execute results
  TraceTwo = 4
};

enum class TraversalProfileLevel : uint8_t {
  /// no profiling information
  None = 0,
  /// include traversal tracing
  Basic = 1
};

struct QueryOptions {
  QueryOptions();
  explicit QueryOptions(arangodb::velocypack::Slice);
  TEST_VIRTUAL ~QueryOptions() = default;

  void fromVelocyPack(arangodb::velocypack::Slice slice);
  void toVelocyPack(arangodb::velocypack::Builder& builder, bool disableOptimizerRules) const;
  TEST_VIRTUAL ProfileLevel getProfileLevel() const { return profile; }
  TEST_VIRTUAL TraversalProfileLevel getTraversalProfileLevel() const { return traversalProfile; }

  size_t memoryLimit;
  size_t maxNumberOfPlans;
  size_t maxWarningCount;
  size_t maxNodesPerCallstack;
  double maxRuntime; // query has to execute within the given time or will be killed
  double satelliteSyncWait;
  double ttl; // time until query cursor expires - avoids coursors to
              // stick around for ever if client does not collect the data
  /// Level 0 nothing, Level 1 profile, Level 2,3 log tracing info
  ProfileLevel profile;
  TraversalProfileLevel traversalProfile;
  bool allPlans;
  bool verbosePlans;
  bool stream;
  bool silent;
  bool failOnWarning;
  bool cache;
  bool fullCount;
  bool count;
  bool verboseErrors;
  bool skipAudit; // skips audit logging - used only internally
  ExplainRegisterPlan explainRegisters;

  /// @brief shard key attribute value used to push a query down
  /// to a single server
  std::string forceOneShardAttributeValue;

  /// @brief optimizer rules to turn off/on manually
  std::vector<std::string> optimizerRules;
  
  /// @brief manual restriction to certain shards
  std::unordered_set<std::string> restrictToShards;

#ifdef USE_ENTERPRISE
  // TODO: remove as soon as we have cluster wide transactions
  std::unordered_set<std::string> inaccessibleCollections;
#endif

  transaction::Options transactionOptions;
  
  static size_t defaultMemoryLimit;
  static size_t defaultMaxNumberOfPlans;
  static size_t defaultMaxNodesPerCallstack;
  static double defaultMaxRuntime;
  static double defaultTtl;
  static bool defaultFailOnWarning;
  static bool allowMemoryLimitOverride;
};

}  // namespace aql
}  // namespace arangodb


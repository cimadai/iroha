/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IROHA_YAC_VOTE_STORAGE_HPP
#define IROHA_YAC_VOTE_STORAGE_HPP

#include <unordered_map>
#include <vector>
#include <nonstd/optional.hpp>

#include "consensus/yac/messages.hpp"
#include "consensus/yac/storage/yac_common.hpp"
#include "consensus/yac/storage/yac_proposal_storage.hpp"
#include "consensus/yac/storage/storage_result.hpp"

namespace iroha {
  namespace consensus {
    namespace yac {

      /**
       * Class provide storage for votes and useful methods for it.
       */
      class YacVoteStorage {
       public:

        /**
         * Insert vote in storage
         * @param msg - current vote message
         * @param peers_in_round - number of peers participated in round
         * @return structure with result of inserting
         */
        StorageResult storeVote(VoteMessage msg, uint64_t peers_in_round);

        StorageResult applyCommit(CommitMessage commit,
                                  uint64_t peers_in_round);

        StorageResult applyReject(RejectMessage reject,
                                  uint64_t peers_in_round);

        nonstd::optional<StorageResult> findProposal(YacHash hash);

       private:
        // --------| private api |--------

        /**
         * Find existed proposal storage or create new if required
         * @param msg - vote for finding
         * @param peers_in_round - number of peer required
         * for verify supermajority;
         * This parameter used on creation of proposal storage
         * @return - index of required proposal storage
         */
        uint64_t findProposalStorage(const VoteMessage &msg,
                                     uint64_t peers_in_round);

        /**
         * Active proposals
         */
        std::vector<YacProposalStorage> proposal_storages_;
      };

    } // namespace yac
  } // namespace consensus
} // namespace iroha
#endif //IROHA_YAC_VOTE_STORAGE_HPP

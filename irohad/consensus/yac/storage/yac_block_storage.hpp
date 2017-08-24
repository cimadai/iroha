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

#ifndef IROHA_YAC_BLOCK_VOTE_STORAGE_HPP
#define IROHA_YAC_BLOCK_VOTE_STORAGE_HPP

#include "consensus/yac/storage/storage_result.hpp"
#include "consensus/yac/storage/yac_common.hpp"

namespace iroha {
  namespace consensus {
    namespace yac {

      /**
       * Class provide storage of votes for one block
       */
      class YacBlockStorage {
       public:

        YacBlockStorage(YacHash hash,
                        uint64_t peers_in_round);

        /**
         * Try to insert vote to storage
         * @param msg - vote for insertion
         * @return actual state of storage
         */
        StorageResult insert(VoteMessage msg);

        /**
         * Insert commit to current storage
         * @param commit
         * @return
         */
        StorageResult insert(CommitMessage commit);

        /**
         * @return current block store state
         */
        StorageResult getState();

        /**
         * @return all votes attached to storage
         */
        std::vector<VoteMessage> getVotes();

        /**
         * @return attached proposal hash
         */
        ProposalHash getProposalHash();

        /**
         * @return attached block hash
         */
        BlockHash getBlockHash();

       private:
        // --------| private fields |--------

        /**
         * Try to invert new vote
         * @param msg - vote for insertion
         * @return true, if inserted
         */
        bool tryInsert(VoteMessage msg);

        /**
         * Return new status of state based on supermajority metrics
         * @return actual state status
         */
        CommitState updateSupermajorityState();

        /**
         * Verify uniqueness of vote in storage
         * @param msg - vote for verification
         * @return true if vote doesn't appear in storage
         */
        bool unique_vote(VoteMessage &msg);

        /**
         * Verify that commit message satisfy to block storage
         * @param commit - message for verification
         * @return true, if satisfied
         */
        bool checkCommitScheme(const CommitMessage &commit);

        /**
         * Common hash of all votes in storage
         */
        YacHash hash_;

        /**
         * All votes stored in block store
         */
        std::vector<VoteMessage> votes_;

        /**
         * Provide knowledge about state of block storage
         */
        StorageResult current_state_;

        /**
         * Number of peers in current round
         */
        uint64_t peers_in_round_;
      };

    } // namespace yac
  } // namespace consensus
} // namespace iroha
#endif //IROHA_YAC_BLOCK_VOTE_STORAGE_HPP

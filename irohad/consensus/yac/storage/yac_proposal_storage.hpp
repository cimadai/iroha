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

#ifndef IROHA_YAC_PROPOSAL_STORAGE_HPP
#define IROHA_YAC_PROPOSAL_STORAGE_HPP

#include <vector>
#include "consensus/yac/messages.hpp"
#include "consensus/yac/storage/yac_common.hpp"
#include "consensus/yac/storage/storage_result.hpp"
#include "consensus/yac/storage/yac_block_storage.hpp"

namespace iroha {
  namespace consensus {
    namespace yac {

      /**
       * Class for storing votes related to given proposal hash
       * and gain information about commits/rejects in system
       */
      class YacProposalStorage {
       public:

        YacProposalStorage(ProposalHash hash, uint64_t peers_in_round);

        /**
         * Try to insert vote to block
         * @param msg - vote
         * @return result, that contains actual state of storage
         */
        StorageResult insert(VoteMessage msg);

        /**
         * Try to apply commit for storage
         * @param commit - message for insertion
         * @return CommitState::not_committed - commit not achieved,
         *         CommitState::committed - commit achieve on vote
         *         from this message
         *         CommitState::committed_before - commit achieved before
         *         this commit message
         */
        StorageResult applyCommit(const CommitMessage &commit,
                                                                                     uint64_t peers_in_round);

        StorageResult applyReject(const RejectMessage &reject,
                                  uint64_t peers_in_round);

        /**
         * @return current stored proposal hash
         */
        ProposalHash getProposalHash() const;

        /**
         * @return current state of storage
         */
        StorageResult getState() const;

       private:
        // --------| private api |--------

        /**
         * Possible to insert vote
         * @param msg - vote for insertion
         * @return true if possible
         */
        bool shouldInsert(const VoteMessage &msg);

        /**
         * Is this vote vilid for insertion in proposal storage
         * @param vote_hash - hash for verification
         * @return true if it may be applied
         */
        bool checkProposalHash(ProposalHash vote_hash);

        /**
         * Is this peer first time appear in this proposal storage
         * @return true, if peer unique
         */
        bool checkPeerUniqueness(const VoteMessage &msg);

        /**
         * Method try to find proof of reject.
         * This computes as
         * sum of unvoted nodes + vote with maximal rete < supermajority
         * @return true, if prove exist
         */
        bool hasRejectProof();

        /**
         * Verify that reject message satisfy scheme
         * @param reject - message for verification
         * @return true, if satisfied
         */
        bool checkRejectScheme(const RejectMessage &reject) {
          auto votes = reject.votes;
          if (votes.size() == 0) return false;
          auto common_proposal = votes.at(0).hash.proposal_hash;
          for (auto &&vote:votes) {
            if (common_proposal != vote.hash.proposal_hash) {
              return false;
            }
          }
          return true;
        };

        /**
         * Find block index with provided parameters,
         * if those store absent - create new
         * @param proposal_hash - hash of proposal
         * @param block_hash - hash of block
         * @return index of storage
         */
        uint64_t findStore(ProposalHash proposal_hash, BlockHash block_hash);

        /**
         * flat map of all votes stored in this proposal storage
         * @return all votes with current proposal hash
         */
        std::vector<VoteMessage> aggregateAll();

        /**
         * Hash of proposal
         */
        ProposalHash hash_;

        /**
         * Current state of storage
         */
        StorageResult current_state_;

        /**
         * Provide number of peers participated in current round
         */
        uint64_t peers_in_round_;

        /**
         * Vector of blocks based on this proposal
         */
        std::vector<YacBlockStorage> block_votes_;
      };
    } // namespace yac
  } // namespace consensus
} // namespace iroha
#endif //IROHA_YAC_PROPOSAL_STORAGE_HPP

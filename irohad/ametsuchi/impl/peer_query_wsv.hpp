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

#ifndef IROHA_PEER_QUERY_WSV_HPP
#define IROHA_PEER_QUERY_WSV_HPP

#include <vector>
#include <memory>
#include "model/peer.hpp"
#include "ametsuchi/wsv_query.hpp"
#include "ametsuchi/peer_query.hpp"

namespace iroha {
  namespace ametsuchi {

    /**
     * Implementation of PeerQuery interface based on WsvQuery fetching
     */
    class PeerQueryWsv : public PeerQuery {
     public:
      explicit PeerQueryWsv(std::shared_ptr<WsvQuery> wsv);

      /**
       * Fetch peers stored in ledger
       * @return list of peers in insertion to ledger order
       */
      nonstd::optional<std::vector<model::Peer>> getLedgerPeers() override;

     private:
      std::shared_ptr<WsvQuery> wsv_;
    };

  }  // namespace ametsuchi
}  // namespace iroha
#endif //IROHA_PEER_QUERY_WSV_HPP

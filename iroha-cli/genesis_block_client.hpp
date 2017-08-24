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

#ifndef IROHA_GENESIS_BLOCK_CLIENT_HPP
#define IROHA_GENESIS_BLOCK_CLIENT_HPP

#include <endpoint.grpc.pb.h>
#include <model/block.hpp>

namespace iroha_cli {

  class GenesisBlockClient {
   public:
    virtual ~GenesisBlockClient() {}
    virtual void set_channel(const std::string &ip, const int port) = 0;
    virtual grpc::Status send_genesis_block(
        const iroha::model::Block &iroha_block,
        iroha::protocol::ApplyGenesisBlockResponse &response) = 0;
    virtual void send_abort_genesis_block(const iroha::model::Block &block) = 0;
  };

}  // namespace iroha_cli

#endif  // IROHA_GENESIS_BLOCK_CLIENT_HPP

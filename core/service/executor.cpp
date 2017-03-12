/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.
http://soramitsu.co.jp
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
     http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <infra/protobuf/api.pb.h>
#include <iostream>
#include <infra/config/peer_service_with_json.hpp>

#include <repository/domain/asset_repository.hpp>
#include <repository/domain/account_repository.hpp>
#include <util/logger.hpp>

namespace executor{

    using Api::Transaction;
    void add(const Transaction &tx) {
        if (tx.has_asset()) {
            // Add<Asset>
            const auto asset = tx.asset();
            repository::asset::add(tx.senderpubkey(),asset.name(),asset);
        } else if (tx.has_domain()) {
            // Add<Domain>
            const auto domain = tx.domain();
        } else if (tx.has_account()) {
            // Add<Account>
            const auto account = tx.account();
            repository::account::add(account.publickey(), account);

            // Add default asset
            auto asset =Api::Asset();
            auto base  =Api::BaseObject();
            base.set_valueint(100);
            asset.set_name("iroha");
            asset.set_domain("default");
            (*asset.mutable_value())["value"] = base;
            repository::asset::add(tx.senderpubkey(),asset.name(),asset);

            logger::info("executor") << "add account";
        } else if( tx.has_peer() ) {
            // Temporary - to operate peer service
            peer::Node query_peer(
                    tx.peer().address(),
                    tx.peer().publickey(),
                    tx.peer().trust().value(),
                    tx.peer().trust().isok()
            );
            if( tx.type() == "Add" ) {
                config::PeerServiceConfig::getInstance().addPeer( query_peer );
            } else if( tx.type() == "Remove" ) {
                config::PeerServiceConfig::getInstance().removePeer( query_peer.getPublicKey() );
            } else if( tx.type() == "Update" ) {
                config::PeerServiceConfig::getInstance().updatePeer( query_peer.getPublicKey(), query_peer );
            }
        }
    }

    void valueValidation(Api::Asset asset, std::vector<std::string> names){

    }

    void transfer(const Transaction& tx){
        if(tx.has_asset()){
            // Transfer<Asset>
            auto sender = tx.senderpubkey();
            auto receiver = tx.receivepubkey();
            const auto assetName = tx.asset().name();

            // **********************************************************************************
            // * This is Transfer<Asset>'s logic. multi message chat
            // **********************************************************************************
            if(tx.asset().value().find("targetName") != tx.asset().value().end()) {
                const auto targetName = tx.asset().value().at("targetName").valuestring();
                auto senderAsset = repository::asset::find(sender, assetName);
                auto receiverAsset = repository::asset::find(receiver, assetName);

                auto senderHasNum = (*senderAsset.mutable_value())[targetName].valueint();
                auto receiverHasNum = (*receiverAsset.mutable_value())[targetName].valueint();

                (*senderAsset.mutable_value())[targetName].set_valueint(senderHasNum - 1);
                (*receiverAsset.mutable_value())[targetName].set_valueint(receiverHasNum + 1);

                repository::asset::update(sender, assetName, senderAsset);
                repository::asset::update(receiver, assetName, receiverAsset);


                // **********************************************************************************
                // * This is Transfer<Asset>'s logic. multi message chat
                // **********************************************************************************
            }else if(tx.asset().value().find("targetName") != tx.asset().value().end()){
                const auto targetName = tx.asset().value().at("targetName").valuestring();
                auto senderAsset = repository::asset::find(sender, assetName);
                auto receiverAsset = repository::asset::find(receiver, assetName);

                auto senderHasNum = (*senderAsset.mutable_value())[targetName].valueint();
                auto receiverHasNum = (*receiverAsset.mutable_value())[targetName].valueint();

                (*senderAsset.mutable_value())[targetName].set_valueint(senderHasNum-1);
                (*receiverAsset.mutable_value())[targetName].set_valueint(receiverHasNum+1);

                repository::asset::update(sender, assetName, senderAsset);
                repository::asset::update(receiver, assetName, receiverAsset);

            // **********************************************************************************
            // * This is Transfer<Asset>'s logic. virtual currency
            // **********************************************************************************
            }else if(tx.asset().value().find("value") != tx.asset().value().end()) {
                const auto value = tx.asset().value().at("value").valueint();
                auto senderAsset = repository::asset::find(sender, assetName);
                auto receiverAsset = repository::asset::find(receiver, assetName);
                if(senderAsset.value().find("value") != senderAsset.value().end() &&
                    receiverAsset.value().find("value") != receiverAsset.value().end()){
                    auto senderValue = senderAsset.value().at("value").valueint();
                    auto receiverValue = receiverAsset.value().at("value").valueint();
                    if(senderValue > value){
                        (*senderAsset.mutable_value())["value"].set_valueint(senderValue - value);
                        (*receiverAsset.mutable_value())["value"].set_valueint(receiverValue + value);
                    }
                }
                repository::asset::update(sender, assetName, senderAsset);
                repository::asset::update(receiver, assetName, receiverAsset);
            }
        }else if(tx.has_domain()){
            // Transfer<Domain>
        }else if(tx.has_account()){
            // Transfer<Account>
            // Add<Account>
        }else if(tx.has_peer()){
            // Transfer<Peer>
        }
    }

    void update(const Transaction& tx){
        if(tx.has_asset()){
            // Update<Asset>
        }else if(tx.has_domain()){
            // Update<Domain>
        }else if(tx.has_account()){
            // Update<Account>
            const auto account = tx.account();
            repository::account::update(account.publickey(), account);
        }else if(tx.has_peer()){
            // Update<Peer>
        }
    }

    void remove(const Transaction& tx){
        if(tx.has_asset()){
            // Remove<Asset>
            const auto name = tx.account().name();
            repository::asset::remove(tx.senderpubkey(),name);
        }else if(tx.has_domain()){
            // Remove<Domain>
        }else if(tx.has_account()){
            // Remove<Account>
            const auto account = tx.account();
            repository::account::remove(account.publickey());
        }else if(tx.has_peer()){
            // Remove<Peer>
        }
    }

    void contract(const Transaction& tx){
        if(tx.has_asset()){
            // Contract<Asset>
        }else if(tx.has_domain()){
            // Contract<Domain>
        }else if(tx.has_account()){
            // Contract<Account>
        }else if(tx.has_peer()){
            // Contract<Peer>
        }
    }

    void execute(const Transaction& tx){
        logger::info("executor") << "Executor";
        logger::info("executor")  << "DebugString:"<< tx.DebugString();
        if(tx.type() == "add"){
            add(tx);
        }else if(tx.type() == "transfer"){
            transfer(tx);
        }else if(tx.type() == "update"){
            update(tx);
        }else if(tx.type() == "remove"){
            remove(tx);
        }else if(tx.type() == "contract"){
            contract(tx);
        }
    }

};

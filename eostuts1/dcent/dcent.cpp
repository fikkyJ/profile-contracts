#include <dcent.hpp>

/**
* This file describes a create profile function for decentralized design schema for EOS dev
*/

void dcent::create(
  account_name application,
  account_name username
){
  require_auth(username);//a single account's details requires validation

  profile_table profiles(_self,_self);

  auto iter = profiles.find(username);//check if username is in use

  eosio_assert(iter == profiles.end(),"account already exists"); //if user name exists stop everything

  profiles.emplace(application,[&](auto & row){//create a new user profile
    row.username = username;
  });

  print("profile created"); //pass message to client

}

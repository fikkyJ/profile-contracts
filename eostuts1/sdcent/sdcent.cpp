#include <sdcent.hpp>

/**
* This file describes a login and signup function for a semi decentralized design schema for EOS dev
*/

void sdcent::create(
  account_name application,
  account_name username,
  account_name email,
  string password
){
  require_auth(application);//a single account's details requires validation

  profile_table profiles(_self,_self);

  auto user_index = profiles.get_index<N(byUser)>();//check if username is in use
  auto iter = user_index.find(username);

  eosio_assert(iter == user_index.end(),"account name in use");

  auto email_index = profiles.get_index<N(byEmail)>();//check if email address is in use
  auto itr = email_index.find(email);

  eosio_assert(itr == email_index.end(),"email name in use");

  profiles.emplace(application,[&](auto & row){//create a new user profile
    row.user_id = profiles.available_primary_key();
    row.username = username;
    row.email = email;
    row.password = password;
  });

  print("account created"); //pass message to client
}


void sdcent::login(
  account_name application,
  account_name username,
  string password,
  account_name session_id,
  account_name platform
){

  profile_table profiles(_self,_self); //init profile table

  auto user_index = profiles.get_index<N(byUser)>();//check if username is in use
  auto iter = user_index.find(username);

  eosio_assert(iter != user_index.end(),"username does not exist");//assert error
  eosio_assert(iter->password!=password, "invalid password"); //assert error if password doesn't match up

  uint64_t user_id = iter->user_id;//init user id from search func

  session_table sessions(_self,_self);//init sessions table

  //the platform field in the sessions table allows for multi platfrom logins to be made by the same user
  auto platform_index = sessions.get_index<N(byPlatform)>();

  auto itr = platform_index.find(platform);

  for(; itr != platform_index.end() && itr->platform; ++iter)//check for pre existing sessions on the same platform
      eosio_assert(itr->user_id == user_id, ""+itr->session_id);//return existing session ti the client

  sessions.emplace(application,[&](auto & row){//create new blockchain session
      row.user_id = user_id;
      row.session_id = session_id;
      row.platform = platform;
  });

  print("session created");//pass message to client

}

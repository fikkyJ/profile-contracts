#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <string>

using namespace eosio;
using std::string;

class sdcent : public contract {

    using contract::contract;

    public:
        sdcent(account_name self) : contract(self){};

        // @abi action
        void create(
          const account_name application,
          const account_name username,
          const account_name email,
          const string password
        );

        // @abi action
        void login(
          const account_name application,
          const account_name username,
          const string password,
          const account_name session_id,
          const account_name platform
        );



    private:

      // @abi table profiles i64
      struct profile{
        uint64_t user_id;
        account_name username;
        string password;
        account_name email;
        string bio;
        string location;
        string dp;

        auto primary_key() const {return user_id;}
        account_name by_user() const {return username;}
        account_name by_email() const {return username;}
        EOSLIB_SERIALIZE(profile,(username)(password)(bio)(location)(dp));
      };

      typedef multi_index<N(profiles), profile,
          indexed_by <N( byUser ), const_mem_fun< profile, account_name, &profile::by_user>>,
          indexed_by <N( byEmail ), const_mem_fun< profile, account_name, &profile::by_email>>> profile_table;

      // @abi table sessions i64
      struct session{
        account_name session_id;
        uint64_t user_id;
        account_name platform;

        auto primary_key() const {return session_id;}
        account_name by_platform() const {return platform;}

        EOSLIB_SERIALIZE(session, (session_id)(user_id)(platform));
      };

      typedef multi_index<N(sessions), session,
          indexed_by <N( byPlatform ), const_mem_fun< session, account_name, &session::by_platform >>> session_table;

};

EOSIO_ABI(sdcent, (create)(login))

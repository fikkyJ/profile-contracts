#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <string>

using namespace eosio;
using std::string;

class dcent : public contract {

    using contract::contract;

    public:
        dcent(account_name self) : contract(self){};

        // @abi action
        void create(
        const account_name application,
        const account_name username
        );

    private:
      // @abi table profiles i64
      struct profile{
            account_name username;
            string bio;
            string location;
            string dp;

            auto primary_key() const {return username;}

            EOSLIB_SERIALIZE(profile,   (username)(bio)(location)(dp));
        };

        typedef multi_index<N(profiles), profile> profile_table;

};

EOSIO_ABI(dcent, (create))

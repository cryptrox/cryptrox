// Copyright (c) 2014-2017 The Dash Core developers
// Copyright (c) 2019 Cryptroxcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef CRYPTROX_SPORK_H
#define CRYPTROX_SPORK_H

#include <hash.h>
#include <net.h>
#include <utilstrencodings.h>

// CRYPTROX BEGIN
class CSporkDB;

/** Global variable that points to the spork database (protected by cs_main) */
extern std::unique_ptr<CSporkDB> pSporkDB;
//CRYPTROX END

class CSporkMessage;
class CSporkManager;

/*
    Don't ever reuse these IDs for other sporks
    - This would result in old clients getting confused about which spork is for what
*/
static const int SPORK_START                                            = 10001;
static const int SPORK_END                                              = 10013;
// CRYPTROX BEGIN
static const int SPORK_CRYPTROX_START                                    = 94680010;
static const int SPORK_CRYPTROX_END                                      = 94680031;
// CRYPTROX END

static const int SPORK_2_INSTANTSEND_ENABLED                            = 10001;
static const int SPORK_3_INSTANTSEND_BLOCK_FILTERING                    = 10002;
static const int SPORK_5_INSTANTSEND_MAX_VALUE                          = 10004;
static const int SPORK_8_MASTERNODE_PAYMENT_ENFORCEMENT                 = 10007;
static const int SPORK_9_SUPERBLOCKS_ENABLED                            = 10008;
static const int SPORK_10_MASTERNODE_PAY_UPDATED_NODES                  = 10009;
static const int SPORK_12_RECONSIDER_BLOCKS                             = 10011;
static const int SPORK_13_OLD_SUPERBLOCK_FLAG                           = 10012;
static const int SPORK_14_REQUIRE_SENTINEL_FLAG                         = 10013;
// CRYPTROX BEGIN
static const int SPORK_CRYPTROX_01_HANDBRAKE_HEIGHT                      = 94680010;
static const int SPORK_CRYPTROX_01_HANDBRAKE_FORCE_X16R                  = 94680016;

static const int SPORK_CRYPTROX_02_IGNORE_SLIGHTLY_HIGHER_COINBASE       = 94680021;
static const int SPORK_CRYPTROX_02_IGNORE_FOUNDER_REWARD_CHECK           = 94680022;
static const int SPORK_CRYPTROX_02_IGNORE_FOUNDER_REWARD_VALUE           = 94680023;
static const int SPORK_CRYPTROX_02_IGNORE_MASTERNODE_REWARD_VALUE        = 94680024;
static const int SPORK_CRYPTROX_02_IGNORE_MASTERNODE_REWARD_PAYEE        = 94680025;

static const int SPORK_CRYPTROX_03_BLOCK_REWARD_SMOOTH_HALVING_START     = 94680031;
// CRYPTROX END

static const int64_t SPORK_2_INSTANTSEND_ENABLED_DEFAULT                = 0;            // ON
static const int64_t SPORK_3_INSTANTSEND_BLOCK_FILTERING_DEFAULT        = 0;            // ON
static const int64_t SPORK_5_INSTANTSEND_MAX_VALUE_DEFAULT              = 1000;         // 1000 Cryptrox
static const int64_t SPORK_8_MASTERNODE_PAYMENT_ENFORCEMENT_DEFAULT     = 4070908800ULL;// OFF
static const int64_t SPORK_9_SUPERBLOCKS_ENABLED_DEFAULT                = 4070908800ULL;// OFF
static const int64_t SPORK_10_MASTERNODE_PAY_UPDATED_NODES_DEFAULT      = 4070908800ULL;// OFF
static const int64_t SPORK_12_RECONSIDER_BLOCKS_DEFAULT                 = 0;            // 0 BLOCKS
static const int64_t SPORK_13_OLD_SUPERBLOCK_FLAG_DEFAULT               = 4070908800ULL;// OFF
static const int64_t SPORK_14_REQUIRE_SENTINEL_FLAG_DEFAULT             = 4070908800ULL;// OFF
// CRYPTROX BEGIN
static const int64_t SPORK_CRYPTROX_01_HANDBRAKE_HEIGHT_DEFAULT             = 4070908800ULL;// OFF
static const int64_t SPORK_CRYPTROX_01_HANDBRAKE_FORCE_X16R_DEFAULT         = 1;            // 1x

static const int64_t SPORK_CRYPTROX_02_IGNORE_SLIGHTLY_HIGHER_COINBASE_DEFAULT  = 4070908800ULL;// OFF
static const int64_t SPORK_CRYPTROX_02_IGNORE_FOUNDER_REWARD_CHECK_DEFAULT      = 4070908800ULL;// OFF
static const int64_t SPORK_CRYPTROX_02_IGNORE_FOUNDER_REWARD_VALUE_DEFAULT      = 4070908800ULL;// OFF
static const int64_t SPORK_CRYPTROX_02_IGNORE_MASTERNODE_REWARD_VALUE_DEFAULT   = 4070908800ULL;// OFF
static const int64_t SPORK_CRYPTROX_02_IGNORE_MASTERNODE_REWARD_PAYEE_DEFAULT   = 4070908800ULL;// OFF

static const int64_t SPORK_CRYPTROX_03_BLOCK_REWARD_SMOOTH_HALVING_START_DEFAULT  = 4070908800ULL;// OFF
// CRYPTROX END

extern std::map<uint256, CSporkMessage> mapSporks;
extern CSporkManager sporkManager;

//
// Spork classes
// Keep track of all of the network spork settings
//

class CSporkMessage
{
private:
    std::vector<unsigned char> vchSig;

public:
    int nSporkID;
    int64_t nValue;
    int64_t nTimeSigned;

    CSporkMessage(int nSporkID, int64_t nValue, int64_t nTimeSigned) :
        nSporkID(nSporkID),
        nValue(nValue),
        nTimeSigned(nTimeSigned)
        {}

    CSporkMessage() :
        nSporkID(0),
        nValue(0),
        nTimeSigned(0)
        {}


    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(nSporkID);
        READWRITE(nValue);
        READWRITE(nTimeSigned);
        READWRITE(vchSig);
    }

    uint256 GetHash() const
    {
        CHashWriter ss(SER_GETHASH, PROTOCOL_VERSION);
        ss << nSporkID;
        ss << nValue;
        ss << nTimeSigned;
        return ss.GetHash();
    }

    bool Sign(std::string strSignKey);
    bool CheckSignature();
    void Relay(CConnman& connman);
};


class CSporkManager
{
private:
    std::vector<unsigned char> vchSig;
    std::string strMasterPrivKey;
    std::map<int, CSporkMessage> mapSporksActive;

public:

    CSporkManager() {}

    // CRYPTROX BEGIN
    void LoadSporksFromDB();
    // CRYPTROX END
    void ProcessSpork(CNode* pfrom, const std::string& strCommand, CDataStream& vRecv, CConnman& connman);
    void ExecuteSpork(int nSporkID, int nValue);
    bool UpdateSpork(int nSporkID, int64_t nValue, CConnman& connman);

    bool IsSporkActive(int nSporkID);
    int64_t GetSporkValue(int nSporkID);
    int GetSporkIDByName(std::string strName);
    std::string GetSporkNameByID(int nSporkID);

    bool SetPrivKey(std::string strPrivKey);
};

#endif // CRYPTROX_SPORK_H

// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Copyright (c) 2019 Cryptroxcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <primitives/block.h>

#include <hash.h>
#include <tinyformat.h>
#include <utilstrencodings.h>
#include <crypto/common.h>

#include <crypto/x16r.h>

uint256 CBlockHeader::GetHash() const
{
    return SerializeHash(*this);
}

uint256 CBlockHeader::GetPoWHash() const
{
    uint256 powHash = uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

    switch (nVersion & ALGO_VERSION_MASK)
    {
        case ALGO_X16R:    powHash = HashX16R(BEGIN(nVersion), END(nNonce), hashPrevBlock); break;
        default:           break; // CRYPTROX TODO: we should not be here
    }

    return powHash;
}

unsigned int CBlockHeader::GetAlgoEfficiency(int nBlockHeight) const
{
    switch (nVersion & ALGO_VERSION_MASK)
    {
        case ALGO_X16R:    return       1;
        default:           return       1; // CRYPTROX TODO: we should not be here
    }

    return 1; // CRYPTROX TODO: we should not be here
}

std::string CBlock::ToString() const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, ver=0x%08x, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
        GetHash().ToString(),
        nVersion,
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        nTime, nBits, nNonce,
        vtx.size());
    for (const auto& tx : vtx) {
        s << "  " << tx->ToString() << "\n";
    }
    return s.str();
}

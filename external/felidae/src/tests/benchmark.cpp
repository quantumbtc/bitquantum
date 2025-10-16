/*
Copyright (c) 2022, Haku Labs

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	* Neither the name of the copyright holder nor the
	  names of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "../felidae.h"
#include "../blake3/blake3_impl.h"
#include "stopwatch.hpp"
#include <iostream>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <atomic>

const uint8_t blockTemplate_[] = {
	0x07, 0x07, 0xf7, 0xa4, 0xf0, 0xd6, 0x05, 0xb3, 0x03, 0x26, 0x08, 0x16, 0xba, 0x3f, 0x10, 0x90, 0x2e, 0x1a, 0x14,
	0x5a, 0xc5, 0xfa, 0xd3, 0xaa, 0x3a, 0xf6, 0xea, 0x44, 0xc1, 0x18, 0x69, 0xdc, 0x4f, 0x85, 0x3f, 0x00, 0x2b, 0x2e,
	0xea, 0x00, 0x00, 0x00, 0x00, 0x77, 0xb2, 0x06, 0xa0, 0x2c, 0xa5, 0xb1, 0xd4, 0xce, 0x6b, 0xbf, 0xdf, 0x0a, 0xca,
	0xc3, 0x8b, 0xde, 0xd3, 0x4d, 0x2d, 0xcd, 0xee, 0xf9, 0x5c, 0xd2, 0x0c, 0xef, 0xc1, 0x2f, 0x61, 0xd5, 0x61, 0x09
};

int main() {
    std::atomic<uint32_t> atomicNonce(0);
    uint64_t hash[FELIDAE_HASH_SIZE / sizeof(uint64_t)];
    uint8_t blockTemplate[sizeof(blockTemplate_)];
    std::memcpy(blockTemplate, blockTemplate_, sizeof(blockTemplate_));
    void* noncePtr = blockTemplate + 39;
    auto nonce = atomicNonce.fetch_add(1);
    uint64_t noncesCount = 10000;

    Stopwatch sw(true);

    while(nonce < noncesCount) {
        store32(noncePtr, nonce);
        felidae_calculate_hash(blockTemplate, sizeof(blockTemplate), hash, 1);
        for (int i = 0; i < FELIDAE_HASH_SIZE; ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << ((int)hash[i] & 0xff);
        }
        std::cout << std::endl;
        nonce = atomicNonce.fetch_add(1);
    }

    double elapsed = sw.getElapsed();

    std::cout << "Found result in " << sw.getElapsed() << " s" << std::endl;
    std::cout << "Performance: " << noncesCount / elapsed << " hashes per second" << std::endl;
}
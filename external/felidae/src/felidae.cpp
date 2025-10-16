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

#include "felidae.h"
#include "k12_blake3.h"
#include <cassert>
#include <limits>
#include <cfenv>

extern "C" {
    void felidae_calculate_hash(const void *input, size_t inputSize, void *output, size_t extra_iterations) {
        assert(input != NULL);
        assert(output != NULL);
        assert(inputSize > 0);
        assert(extra_iterations > 0);
        assert(inputSize <= std::numeric_limits<uint64_t>::max());
        assert(extra_iterations <= std::numeric_limits<uint64_t>::max());
        uint64_t inputSize_ = static_cast<uint64_t>(inputSize);
        uint64_t extra_iterations_ = static_cast<uint64_t>(extra_iterations);

        for(uint64_t i = 0; i < extra_iterations_; i++) {
            k12_blake3_hash(input, inputSize_, output);
        }
    }
}
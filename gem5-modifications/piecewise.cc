/*
 * Copyright (c) 2014 The University of Wisconsin
 *
 * Copyright (c) 2006 INRIA (Institut National de Recherche en
 * Informatique et en Automatique  / French National Research Institute
 * for Computer Science and Applied Mathematics)
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* @file
 * Implementation of a piecewise linear branch predictor
 */

#include "cpu/pred/piecewise.hh"

#include "base/intmath.hh"
#include "base/logging.hh"
#include "base/random.hh"
#include "base/trace.hh"
#include "debug/Fetch.hh"
//#include "debug/Tage.hh"

#include <stdlib.h>     /* srand, rand */

#define SRAND_SEED 0xBEEF
piecewise::piecewise(const piecewiseParams *params) : BPredUnit(params) // add more to this list
{
    weights.assign(N, std::vector<int>(M, std::vector<int>(GHL + 1, 0)));
    GA.assign(GHL, 0);
    theta =  2.14 * (GHL + 1) + 20.58;
}


/**
* Looks up the given address in the branch predictor and returns
* a true/false value as to whether it is taken.
* @param branch_addr The address of the branch to look up.
* @param bp_history Pointer to any bp history state.
* @return Whether or not the branch is taken.
*/
bool piecewise::lookup(ThreadID tid, Addr branch_addr, void* &bp_history){
    int addr = branch_addr % N;
    sum = weights[addr][0][0];

    for (int i = 1; i < GHL; i++) {
        int j = GA[i] % M;

        if (GHR[i] == 1) { // if branch taken
            sum += weights[addr][j][i];
        }
        else {
            sum -= weights[addr][j][i];
        }
    }

    return sum >= 0;
}


/**
* Updates the branch predictor with the actual result of a branch.
* @param branch_addr The address of the branch to update.
* @param taken Whether or not the branch was taken.
*/
void piecewise::update(ThreadID tid, Addr branch_addr, bool taken, void *bp_history, bool squashed, const StaticInstPtr & inst, Addr corrTarget){

    assert(bp_history == NULL);
    // No state to restore, and we do not update on the wrong
    // path.
    if (squashed) {
        return;
    }

    bool prediction = (sum >= 0);
    int addr = branch_addr % N;

    if (prediction != taken || abs(sum) < theta) {
        if (taken) {
            weights[addr][0][0] += 1;
        }
        else {
            weights[addr][0][0] -= 1;
        }

        for (int i = 1; i < GHL; i++) {
            int j = GA[i] % M;
            if (GHR[i] == 1) { // branch taken
                weights[addr][j][i] += 1;
            }
            else {
                weights[addr][j][i] -= 1;
            }
        }
    }

    // update GA
    for (int i = GHL - 1; i >= 1; i--) {
        GA[i] = GA[i - 1];
    }
    GA[0] = branch_addr;

    // update GHR
	GHR <<= 1;
	if (taken)
		GHR.set(0, 1);
	else
		GHR.set(0, 0);
    
}



piecewise*
piecewiseParams::create()
{
    return new piecewise(this);
}
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
 * Implementation of a custom branch predictor
 */

#include "cpu/pred/custom.hh"

#include "base/intmath.hh"
#include "base/logging.hh"
#include "base/random.hh"
#include "base/trace.hh"
#include "debug/Fetch.hh"
//#include "debug/Tage.hh"

#include <stdlib.h>     /* srand, rand */

#define SRAND_SEED 0xBEEF
custom::custom(const customParams *params) : BPredUnit(params) // add more to this list
{

	weights.assign(NUM_PERCEPTRON, std::vector<int>(HISTORY_LEN, 0));
	theta = 1.93 * HISTORY_LEN + 14;

/*	srand(SRAND_SEED);
	for (int i=0; i < NUM_WEIGHTS; i++){
		weights_1[i] = int8_t(rand() % 256) - 128;
		global_history[i] = 0;
	} */
}


/**
* Looks up the given address in the branch predictor and returns
* a true/false value as to whether it is taken.
* @param branch_addr The address of the branch to look up.
* @param bp_history Pointer to any bp history state.
* @return Whether or not the branch is taken.
*/
bool custom::lookup(ThreadID tid, Addr branch_addr, void* &bp_history){
	/*pred_taken = inference();
	return pred_taken; */
	int idx = branch_addr % NUM_PERCEPTRON;
	int sum = weights[idx][0];

	for (int i = 1; i < HISTORY_LEN; i++) {
		if (GHR[i - 1] == 1) {  // if branch taken
			sum += weights[idx][i];
		}
		else {
			sum -= weights[idx][i];
		}
	}

	return sum >= 0;
}


/**
* Updates the branch predictor with the actual result of a branch.
* @param branch_addr The address of the branch to update.
* @param taken Whether or not the branch was taken.
*/
void custom::update(ThreadID tid, Addr branch_addr, bool taken, void *bp_history, bool squashed, const StaticInstPtr & inst, Addr corrTarget){

    assert(bp_history == NULL);
    // No state to restore, and we do not update on the wrong
    // path.
    if (squashed) {
        return;
    }
    
	int idx = branch_addr % NUM_PERCEPTRON;
	int sum = weights[idx][0];

	for (int i = 1; i < HISTORY_LEN; i++) {
		if (GHR[i - 1] == 1) {  // if branch taken
			sum += weights[idx][i];
		}
		else {
			sum -= weights[idx][i];
		}
	}
	
	pred_taken = (sum >= 0);

	if (pred_taken != taken || abs(sum) <= theta) {
		for (int i = 0; i < HISTORY_LEN; i++) {
			if (taken)
				weights[idx][i] += 1;
			else
				weights[idx][i] -= 1;
		}
	}

	// update GHR
	GHR <<= 1;
	if (taken)
		GHR.set(0, 1);
	else
		GHR.set(0, 0);
 //   backprop(pred_taken, taken);   
}

custom*
customParams::create()
{
    return new custom(this);
}


/* Implement Nueral Network Functionality Here  */
bool custom::inference(){

/*	int64_t sum = 0;
	for (int i = 0; i < NUM_WEIGHTS; i++){
		sum += (int16_t) weights_1[i] * (int16_t) global_history[i];
	}

	if (sum >= 0){
		return true;
	} */
	return false;
}

/* I don't remember how this works; it was in one of the papers */
void custom::backprop(bool pred_taken, bool taken){
/*	if (pred_taken && !taken){
		for (int i = 0; i < NUM_WEIGHTS; i++){
			weights_1[i]--;
		}
	}else if (!pred_taken && taken){
		for (int i = 0; i < NUM_WEIGHTS; i++){
			weights_1[i]++;
		}
	}*/
}


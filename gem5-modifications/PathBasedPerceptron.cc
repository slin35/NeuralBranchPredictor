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

#include "cpu/pred/PathBasedPerceptron.hh"

#include "base/intmath.hh"
#include "base/logging.hh"
#include "base/random.hh"
#include "base/trace.hh"
#include "debug/Fetch.hh"
//#include "debug/Tage.hh"

#include <stdlib.h>     /* srand, rand */
#include <string.h>

#define SRAND_SEED 0xBEEF
PathBasedPerceptron::PathBasedPerceptron(const PathBasedPerceptronParams *params) : BPredUnit(params) // add more to this list
{

	i = 0;
	SG = 0;
	G = 0;
	y_out = 0;
	memset(v, 0, (HISTORY_LEN+1)*sizeof(int8_t));
	memset(R, 0, (HISTORY_LEN+1)*sizeof(int16_t));
	memset(SR, 0, (HISTORY_LEN+1)*sizeof(int16_t));
	memset(H, false, (HISTORY_LEN+1)*sizeof(bool));
	memset(W, 0, N*(HISTORY_LEN+1)*sizeof(int8_t));

}


/**
* Looks up the given address in the branch predictor and returns
* a true/false value as to whether it is taken.
* @param branch_addr The address of the branch to look up.
* @param bp_history Pointer to any bp history state.
* @return Whether or not the branch is taken.
*/
bool PathBasedPerceptron::lookup(ThreadID tid, Addr branch_addr, void* &bp_history){


	pred_taken = inference(branch_addr);
	return pred_taken;
}


/**
* Updates the branch predictor with the actual result of a branch.
* @param branch_addr The address of the branch to update.
* @param taken Whether or not the branch was taken.
*/
void PathBasedPerceptron::update(ThreadID tid, Addr branch_addr, bool taken, void *bp_history, bool squashed, const StaticInstPtr & inst, Addr corrTarget){

    assert(bp_history == NULL);
    // No state to restore, and we do not update on the wrong
    // path.
    if (squashed) {
        return;
    }
    
    backprop(taken);   
}

PathBasedPerceptron*
PathBasedPerceptronParams::create()
{
    return new PathBasedPerceptron(this);
}


/* Implement Nueral Network Functionality Here  */
bool PathBasedPerceptron::inference(uint64_t pc){

	i = pc % N; // i = 0
	v[HISTORY_LEN] = i;
	int16_t y_out = SR[HISTORY_LEN] + W[i][0];
	
	if (y_out >= 0){
		pred_taken = true;
	}else{
		pred_taken = false;
	}

	// Insert R here
	memcpy(R,SR, (HISTORY_LEN+1)*sizeof(int16_t) );
	for (uint8_t j = 1; j < HISTORY_LEN; j++){
		uint8_t k = HISTORY_LEN-j;
		if (pred_taken == true){
			SR[k+1] = SR[k] + W[i][j];
		}else{
			SR[k+1] = SR[k] - W[i][j];
		}
	}
	//SR = SR'
	SR[0] = 0;
	SG = (SG << 1) || pred_taken;
	return pred_taken;
}

#define theta 1.93*HISTORY_LEN+14
//void PathBasedPerceptron::backprop(int8_t i, int16_t y_out, bool pred_taken, bool was_taken, int8_t* v, bool* H){
void PathBasedPerceptron::backprop(bool was_taken){

	if( (pred_taken != was_taken) || (y_out <= theta) ){
		if (was_taken){
			W[i][0] += 1;
		}else{
			W[i][0] -= 1;
		}
		
		for (uint8_t j = 1; j < HISTORY_LEN; j++){
		
			int8_t k = v[j];
			
			if (was_taken == H[j]){
				W[k][j] += 1;
			}else{
				W[k][j] -= 1;
			}
	
		}
	
	}

	/* Look Here */
	G = (G << 1) | was_taken;
	if (pred_taken != was_taken){
		SG = G;
		memcpy(SR, R, (HISTORY_LEN+1)*sizeof(int16_t));
	}

}


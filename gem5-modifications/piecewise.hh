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
 * Implementation of a TAGE branch predictor. TAGE is a global-history based
 * branch predictor. It features a PC-indexed bimodal predictor and N
 * partially tagged tables, indexed with a hash of the PC and the global
 * branch history. The different lengths of global branch history used to
 * index the partially tagged tables grow geometrically. A small path history
 * is also used in the hash.
 *
 * All TAGE tables are accessed in parallel, and the one using the longest
 * history that matches provides the prediction (some exceptions apply).
 * Entries are allocated in components using a longer history than the
 * one that predicted when the prediction is incorrect.
 */

#ifndef __CPU_PRED_PIECEWISE_
#define __CPU_PRED_PIECEWISE_

#include <vector>

#include "base/types.hh"
#include "cpu/pred/bpred_unit.hh"
#include "params/piecewise.hh"

#define GHL 26  // the global history length
#define N 8   // address range
#define M 118   // GA range

#define HASH_PRIME_1 511387U
#define HASH_PRIME_2 660509U
#define HASH_PRIME_3 1289381U

class piecewise: public BPredUnit
{

  /* private variables and methods */
  private:
    std::bitset<GHL> GHR; // global history length
    std::vector<int> GA;  // global array of previous branch addresses
    std::vector<std::vector<std::vector<int>>> weights; // n x m x (GHL + 1)
    float theta;  // threshold to decide whether the predictor needs more training
    int sum;

  public:
    piecewise(const piecewiseParams* params);

    // Base class methods.
     /**
     * Looks up the given address in the branch predictor and returns
     * a true/false value as to whether it is taken.
     * @param branch_addr The address of the branch to look up.
     * @param bp_history Pointer to any bp history state.
     * @return Whether or not the branch is taken.
     */
    bool lookup(ThreadID tid, Addr branch_addr, void* &bp_history) override;
    
     /**
     * Updates the branch predictor with the actual result of a branch.
     * @param branch_addr The address of the branch to update.
     * @param taken Whether or not the branch was taken.
     */
    void update(ThreadID tid, Addr branch_addr, bool taken, void *bp_history, bool squashed, const StaticInstPtr & inst, Addr corrTarget) override;
    
    /* Not necessary */
    void btbUpdate(ThreadID tid, Addr branch_addr, void* &bp_history){};
    void uncondBranch(ThreadID tid, Addr br_pc, void* &bp_history){};
    void squash(ThreadID tid, void *bp_history) { assert(bp_history == NULL); };
    
    
};

#endif // __CPU_PRED_PIECEWISE_

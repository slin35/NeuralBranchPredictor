#ifndef __PARAMS__piecewise__
#define __PARAMS__piecewise__

class piecewise;

#include "params/BranchPredictor.hh"


struct piecewiseParams
    : public BranchPredictorParams
{
    piecewise* create();
};

#endif // __PARAMS__piecewise__
#ifndef __PARAMS__custom__
#define __PARAMS__custom__

class custom;

#include "params/BranchPredictor.hh"


struct customParams
    : public BranchPredictorParams
{
    custom* create();
};

#endif // __PARAMS__custom__

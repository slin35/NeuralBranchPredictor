#ifndef __PARAMS__PathBasedPerceptron__
#define __PARAMS__PathBasedPerceptron__

class PathBasedPerceptron;

#include "params/BranchPredictor.hh"


struct PathBasedPerceptronParams
    : public BranchPredictorParams
{
    custom* create();
};

#endif // __PARAMS__PathBasedPerceptron__

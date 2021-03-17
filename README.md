# NeuralBranchPredictor


## setup gem5
on Ubuntu 20.04
```
sudo apt install build-essential git m4 scons zlib1g zlib1g-dev \
    libprotobuf-dev protobuf-compiler libprotoc-dev libgoogle-perftools-dev \
    python3-dev python3-six python-is-python3 libboost-all-dev pkg-config
```

## to build an X86 simulator
```
scons build/X86/gem5.opt -j {number of cpus}
```

## to run
to run any python scripts
```
build/X86/gem5.opt test_script.py
```

to print out list of branch predictors in gem5
```
build/X86/gem5.opt configs/example/se.py --list-bp-types
```

to run the executables with a specified branch predictor
```
build/X86/gem5.opt configs/example/se.py --cmd=[executables] --bp-type=[name_of_branch_predictor]
```

to get stats related to the performance of branch preditors
```
grep 'branch' m5out/stats.txt
```
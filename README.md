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
build/X86/gem5.opt test.py

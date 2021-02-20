build/X86/gem5.opt configs/example/se.py --cmd=tests/test-progs/hello/bin/x86/linux/hello --bp-type=custom
grep 'branch' m5out/stats.txt

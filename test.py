import m5
from m5.objects import *

# instance a system
system = System()

# set default clock domaina and voltage domain
system.clk_domain = SrcClockDomain()
system.clk_domain.clock = '1GHz'
system.clk_domain.voltage_domain = VoltageDomain()

# set mem mode and default address range
system.mem_mode = 'timing'
system.mem_ranges = [AddrRange('512MB')]

# create a CPU
system.cpu = TimingSimpleCPU()

# create a membus
system.membus = SystemXBar()

# connect cpu ports to membus
system.cpu.icache_port = system.membus.slave
system.cpu.dcache_port = system.membus.slave

# create interrupt controllers
system.cpu.createInterruptController()
system.cpu.interrupts[0].pio = system.membus.master
system.cpu.interrupts[0].int_master = system.membus.slave
system.cpu.interrupts[0].int_slave = system.membus.master

system.system_port = system.membus.slave

# setup mem controller
system.mem_ctrl = MemCtrl()
system.mem_ctrl.dram = DDR3_1600_8x8()
system.mem_ctrl.dram.range = system.mem_ranges[0]
system.mem_ctrl.port = system.membus.master

process = Process()
process.cmd = ['tests/test-progs/hello/bin/x86/linux/hello']
system.cpu.workload = process
system.cpu.createThreads()

root = Root(full_system = False, system = system)
m5.instantiate()

print("Beginning simulation!")
exit_event = m5.simulate()

print('Exiting @ tick %i because %s'.format(m5.curTick(), exit_event.getCause()))
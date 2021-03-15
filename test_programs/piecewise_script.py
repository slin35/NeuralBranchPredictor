import os
import re
import sys

args_rsa = ('rsa', '5000000000', 'kjv_part.txt', 'rsa_out.txt')
args_pi = ('pi', '5000000', 'pi_out.txt', 'pi_out.txt')
args_md5 = ('md5', '5000000000', 'kjv_part.txt', 'md5_out.txt')

cmd_args = (args_rsa, args_pi, args_md5)
bps = ('custom', 'PathBasedPerceptron', 'piecewise')

cmd_prefix = '../build/X86/gem5.opt ../configs/example/se.py --bp-type={} --cmd=../test_programs/{} --mem-size={} --input=../test_programs/{} --output=../../test_programs/{}'

table = [[0]*4,[0]*4]
table[1][0] = 'piecewise perceptron' 

table[0][1] = 'rsa'
table[0][2] = 'pi'
table[0][3] = 'md5'

i = 2
bp = bps[i]
for j in range(len(cmd_args)):
    args = cmd_args[j]
    run_cmd = cmd_prefix.format(bp, args[0], args[1], args[2], args[3])
    os.system(run_cmd)
    
    f = open('./m5out/stats.txt', 'r')
    stats_txt = f.read()
    #print(stats_txt)
    f.close()
    
    mispred_match = re.search( '(?<=BranchMispred)\s+\d+', stats_txt)
    total_match = re.search( '(?<=Branches)\s+\d+', stats_txt)
    
    if mispred_match == None or total_match == None:
        print(f'Program Failed to produce statistics: {args[0]}')
        accuracy = 0
        continue
    else:
        num_misses = int(mispred_match[0].strip())
        num_total = int(total_match[0].strip())
        accuracy = 1 - num_misses/num_total




    table[1][j+1] = (accuracy, num_total, num_misses)
		

print(table)

if len(sys.argv) < 2:
	f = open('table.csv','w')
else:
	f = open(sys.argv[1],'w')

for j in range(len(table[0])):
	for i in range(len(table)):
		if i == 0 and j == 0:
			f.write('')
		elif i == 0 or j == 0:
			f.write('"' + table[i][j] + '"')
		else:	
			f.write(str(table[i][j][0]))
		if i < len(table) - 1:
			f.write(', ')
	f.write('\n')
	
f.close()
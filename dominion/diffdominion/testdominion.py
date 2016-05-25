import subprocess
import sys

if len(sys.argv) != 4:
    print "You must supply two directories that contain a dominion.c file and a random seed\n"
dom = []
dom.append( sys.argv[1] + 'dominion.c')
dom.append( sys.argv[2] + 'dominion.c')
seed = sys.argv[3]

for i in range(2):
    subprocess.call("cp " + dom[i] + " ./dominion.c", shell=True)
    subprocess.call("make clean; make testdominion.o", shell=True)
    subprocess.call("timeout 10s ./testdominion " + str(seed) + " > dominion" + str(i) + ".out", shell=True)
    subprocess.call("gcov dominion.c > dominion" + str(i) + ".out.gcov", shell=True)

r = subprocess.call("diff dominion0.out dominion1.out > diff.out", shell=True)
if (r != 0):
    print "\nTEST FAILED! Check diff.out for details\n"
else:
    print "\nTEST PASSED!\n"

import subprocess
import glob
import os
import sys
FNULL = open(os.devnull, 'w')

#compile the original
subprocess.call("cp ./mutantsrc/original_dominion.c ./dominion.c", shell=True)
subprocess.call("make testdominion.o", shell=True)
subprocess.call("./testdominion 42 > original_dominion.c.out", shell=True)
subprocess.call("mv ./testdominion ./binaries/original_dominion", shell=True)


for mutantsrc in glob.iglob("./mutantsrc/mutant*"):
    duplicate = False
    subprocess.call("cp " + mutantsrc + " ./dominion.c", shell=True)
    r = subprocess.call("make testdominion.o", shell=True, stdout=FNULL)
    if (r != 0): #failed to compile
        print "MUTANT NOT COMPILABLE"
        subprocess.call("mv " + mutantsrc + " ./notcompiled/", shell=True)
        continue
    #for bin in glob.iglob("./binaries/*"):
        #this returns 1 if no difference was found, 0 if there is a difference
        #r = subprocess.call("cmp ./testdominion " + bin + " -l | grep -v -q -e 4[0-2][0-9] -e 61[8-9]6[0-9] -e 6285[7-9]", shell=True, stderr=sys.stdout.fileno())
        #if (r == 1):
            #print "DUPLICATE FOUND"
            #subprocess.call("mv " + mutantsrc + " ./duplicates/", shell=True)
            #duplicate = True
            #break
    if (duplicate == True):
        continue
    #it compiled and it's unique
    print "UNIQUE MUTANT FOUND"
    subprocess.call("timeout 2s ./testdominion 42 > mutant.out", shell=True)
    r = subprocess.call("diff -q ./original_dominion.c.out ./mutant.out", shell=True, stdout=FNULL)
    if (r == 1):
        print "MUTANT KILLED!"
        subprocess.call("mv " + mutantsrc + " ./killed/", shell=True)
    else:
        print "MUTANT NOT KILLED!"
        subprocess.call("mv " + mutantsrc + " ./notkilled/", shell=True)
    subprocess.call("mv ./testdominion ./binaries/" + mutantsrc.split('.')[1].split('/')[2], shell=True)
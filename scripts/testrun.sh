######################################################################################
# This scripts runs the two Test traces
# You will need to first compile your code in ../src.BC before launching this script
# the results are stored in the ../results/ folder 
######################################################################################

########## ---------------  B.1 ---------------- ################
../src.BC/sim -pipewidth 1 -schedpolicy 0 -loadlatency 1 ../traces/100gcc.ptr.gz > ../results/B1.100gcc.res
../src.BC/sim -pipewidth 1 -schedpolicy 0 -loadlatency 1 ../traces/1000gcc.ptr.gz > ../results/B1.1000gcc.res

########## ---------------  B.2 ---------------- ################
../src.BC/sim -pipewidth 1 -schedpolicy 1 -loadlatency 1 ../traces/100gcc.ptr.gz > ../results/B2.100gcc.res
../src.BC/sim -pipewidth 1 -schedpolicy 1 -loadlatency 1 ../traces/1000gcc.ptr.gz > ../results/B2.1000gcc.res

########## ---------------  B.3 ---------------- ################
../src.BC/sim -pipewidth 1 -schedpolicy 0 -loadlatency 4 ../traces/100gcc.ptr.gz > ../results/B3.100gcc.res
../src.BC/sim -pipewidth 1 -schedpolicy 0 -loadlatency 4 ../traces/1000gcc.ptr.gz > ../results/B3.1000gcc.res
########## ---------------  B.4 ---------------- ################
../src.BC/sim -pipewidth 1 -schedpolicy 1 -loadlatency 4 ../traces/100gcc.ptr.gz > ../results/B4.100gcc.res
../src.BC/sim -pipewidth 1 -schedpolicy 1 -loadlatency 4 ../traces/1000gcc.ptr.gz > ../results/B4.1000gcc.res

########## ---------------  C.1 ---------------- ################
../src.BC/sim -pipewidth 2 -schedpolicy 0 -loadlatency 1 ../traces/100gcc.ptr.gz > ../results/C1.100gcc.res
../src.BC/sim -pipewidth 2 -schedpolicy 0 -loadlatency 1 ../traces/1000gcc.ptr.gz > ../results/C1.1000gcc.res

########## ---------------  C.2 ---------------- ################
../src.BC/sim -pipewidth 2 -schedpolicy 1 -loadlatency 1 ../traces/100gcc.ptr.gz > ../results/C2.100gcc.res
../src.BC/sim -pipewidth 2 -schedpolicy 1 -loadlatency 1 ../traces/1000gcc.ptr.gz > ../results/C2.1000gcc.res

########## ---------------  C.3 ---------------- ################
../src.BC/sim -pipewidth 2 -schedpolicy 0 -loadlatency 4 ../traces/100gcc.ptr.gz > ../results/C3.100gcc.res
../src.BC/sim -pipewidth 2 -schedpolicy 0 -loadlatency 4 ../traces/1000gcc.ptr.gz > ../results/C3.1000gcc.res

########## ---------------  C.4 ---------------- ################
../src.BC/sim -pipewidth 2 -schedpolicy 1 -loadlatency 4 ../traces/100gcc.ptr.gz > ../results/C4.100gcc.res
../src.BC/sim -pipewidth 2 -schedpolicy 1 -loadlatency 4 ../traces/1000gcc.ptr.gz > ../results/C4.1000gcc.res

########## ---------------  GenReport ---------------- ################
grep LAB3_CPI ../results/B?.*.res > testresults.txt
grep LAB3_CPI ../results/C?.*.res >> testresults.txt
######### ------- Goodbye -------- ##################

echo "Done. Check testresults.txt, and .res files in ../results";

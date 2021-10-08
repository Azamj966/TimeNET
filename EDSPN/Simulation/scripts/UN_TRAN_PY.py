# 	Starten der unwissenden EDSPN Simulation unter Windows


import os
import string
import sys
import subprocess
import shutil

THOME = os.environ.get('TNETHOME')



currentdir=os.curdir

distr="null"
distr_string="null"
method="null"
meth_string="null"
trans="null"
trans_string="null"
rule="null"
rule_string="null"
transient_time="null"
no_sampling_points="null"
maxsamples="null"
maxtime="null"
max_execution_time="null"
csv_dat="null"
algo_no="null"
start="null"
nice="4"
Sep=os.sep
BIN_DIR=THOME + Sep + "EDSPN" + Sep + "Simulation" + Sep + "bin"
SIM_DIR=THOME + Sep + "EDSPN" + Sep + "Simulation"
SHARED_BIN_DIR=THOME + Sep + "EDSPN" + Sep + "Shared" + Sep + "bin"

usage="Usage : SIM_TRAN_win model -{D|S} -{Pon|Poff} <transient_time> <no_sampling_points> <conf.level> <epsilon> <seed> <max.real time> <csv Datei> <Algo NO>"
clem="bin hier"

if sys.argv[1] == "" :
  print usage
  END_SCRIPT()
else :
  NETNAME=sys.argv[1]


NET=MODELDIR + Sep + NETNAME

# check netname
if os.path.isfile('"'+NET +'.TN"') :
  print "No such net"
  END_SCRIPT()


# parameters
if sys.argv[2] == "" :
  distr=-S
else :
  distr=sys.argv[2]

if sys.argv[3] == "" :
  rule=-Pon
else :
  rule=sys.argv[3]

if sys.argv[4] == "" :
  transient_time=100.0
else :
  transient_time=sys.argv[4]

if sys.argv[5] == "" :
  no_sampling_points=100
else :
  no_sampling_points=sys.argv[5]

if sys.argv[6] == "" :
  level=95
else :
  level=sys.argv[6]

if sys.argv[7] == "" :
  epsilon=10
else :
  epsilon=sys.argv[7]

if sys.argv[8] == "" :
  seed=1
else :
  seed=sys.argv[8]

if sys.argv[9] == "" :
  max_execution_time=300.0
else :
  max_execution_time=sys.argv[9]


if sys.argv[10] == "" :
  csv_dat=Random
else :

  print sys.argv[10]
  csv_dat=sys.argv[10]


if sys.argv[11]== "" :
  algo_no=1
else :

  algo_no=sys.argv[11]
  print algo_no


if sys.argv[12]== "" :
  start=1
else :
  print sys.argv[12]
  start=sys.argv[12]



# Defaults
trans = "Toff"
method = "-STR"
# Dummys
maxsamples=0;
maxtime=0.0;

if distr == "-S" :
	distr_string="Sequential"
	distr="S"

if distr == "-D" :
	distr_string="Parallel"
	distr="D"


if rule == "-Pon" :
	rule_string="on."
	rule="Pon"

if rule == "-Toff" :
	rule_string="off."
	rule="Poff"


print
print distr_string + 'transient Simulation of net:'
print NET
print 'Percentage-rule is '+rule_string

os.system(SHARED_BIN_DIR + Sep + "proc_tn.exe -s "+NETNAME)
retcode = subprocess.call([sys.executable, "proc_tn.exe"])

if retcode==1 :

  print 'ERROR occurred while proc_TN execution.'
  print 'SOLUTION OF MODEL '+ NETNAME +' FAILED.'

  END_SCRIPT()


print '"'+NET+'_MDF.c" -I"'+SIM_DIR + Sep + 'include" -I"'+ SIM_DIR + Sep + 'sim_utils' + Sep + 'include" -o "'+NET+'_MDF.obj"'
print '"'+NET+'_ERC.c" -I"'+SIM_DIR + Sep + 'include" -I"'+SIM_DIR + Sep + 'sim_utils' + Sep + 'include" -o "'+NET+'_ERC.obj"'
print '"'+NET+'_RDY.c" -I"'+SIM_DIR+ + Sep + 'include" -I"'+SIM_DIR + Sep + 'sim_utils' + Sep + 'include" -o "'+NET+'_RDY.obj"'
print '"'+NET+'_slave.exe" "'+NET+'_MDF.obj" "'+NET+'_ERC.obj" "'+BIN_DIR + Sep + 'libsim_str_slave_win.a" -L"'+BIN_DIR+'" -lsim_utils_win -L"'+SHARED_BIN_DIR+'" -lsysdep_win -lnetio_win -lws2_32'
print '"'+NET+'_master.exe" "'+NET+'_RDY.obj" "'+BIN_DIR + Sep + 'libsim_str_master_win.a" -L"'+BIN_DIR+'" -lsim_utils_win -L"'+SHARED_BIN_DIR+'" -lsysdep_win -lnetio_win -lws2_32'


os.system('gcc -c "'+NET+'_MDF.c" -I"'+SIM_DIR + Sep + 'include" -I"'+ SIM_DIR + Sep + 'sim_utils' + Sep + 'include" -o "'+NET+'_MDF.obj"')
os.system('gcc -c "'+NET+'_ERC.c" -I"'+SIM_DIR + Sep + 'include" -I"'+SIM_DIR + Sep + 'sim_utils' + Sep + 'include" -o "'+NET+'_ERC.obj"')
os.system('gcc -c "'+NET+'_RDY.c" -I"'+SIM_DIR+ + Sep + 'include" -I"'+SIM_DIR + Sep + 'sim_utils' + Sep + 'include" -o "'+NET+'_RDY.obj"')

os.system('gcc -o "'+NET+'_slave.exe" "'+NET+'_MDF.obj" "'+NET+'_ERC.obj" "'+BIN_DIR + Sep + 'libsim_str_slave_win.a" -L"'+BIN_DIR+'" -lsim_utils_win -L"'+SHARED_BIN_DIR+'" -lsysdep_win -lnetio_win -lws2_32')
os.system('gcc -o "'+NET+'_master.exe" "'+NET+'_RDY.obj" "'+BIN_DIR + Sep + 'libsim_str_master_win.a" -L"'+BIN_DIR+'" -lsim_utils_win -L"'+SHARED_BIN_DIR+'" -lsysdep_win -lnetio_win -lws2_32')

# check executables
if  os.path.isfile('"'+NET+'_master.exe"') :
  print 'Could not build the simulator, for errors see above.'
  END_SCRIPT()

if  os.path.isfile('"'+NET+'_slave.exe"') :
  print 'Could not build the simulator, for errors see above.'
  END_SCRIPT()




os.system('"'+NET+'_master.exe "' +NETNAME+' '+ level+ ' '+ epsilon+ ' ' +seed+ ' ' +maxsamples+ ' ' +maxtime+ ' ' +max_execution_time+ ' ' +distr+ ' ' +trans+ ' ' +transient_time+ ' ' +no_sampling_points+ ' ' +rule+ ' ' +csv_dat+ ' ' +algo_no+ ' ' +start)

def END_SCRIPT():

    print 'Removing temporary files'

    if os.path.isfile('"'+NET+'.DEFINFO"') :
         os.remove('"'+NET+'.DEFINFO"')

    if os.path.isfile(' "'+NET+'_master.exe"') :
        os.remove( '"'+NET+'_master.exe"')

    if os.path.isfile('"'+NET+'_master.ilk"') :
        os.remove( '"'+NET+'_master.ilk"')

    if os.path.isfile(' "'+NET+'_master.pdb"') :
        os.remove( '"'+NET+'_master.pdb"')

    if os.path.isfile(' "'+NET+'_slave.exe"') :
        os.remove( '"'+NET+'_slave.exe"')

    if os.path.isfile(' "'+NET+'_slave.ilk"') :
        os.remove( '"'+NET+'_slave.ilk"')

    if os.path.isfile(' "'+NET+'_slave.pdb"') :
        os.remove( '"'+NET+'_slave.pdb"')

    if os.path.isfile(' "'+NET+'.sim_curves"') :
        os.remove('"'+NET+'.sim_curves"')

    if os.path.isfile(' "'+NET+'.temp_sim_curves"') :
        os.remove('"'+NET+'.temp_sim_curves"')


    if os.path.isfile(' "'+NET+'_ERC.c"') :
        os.remove('"'+NET+'_ERC.c"')

    if os.path.isfile(' "'+NET+'_RDY.c"') :
        os.remove('"'+NET+'_RDY.c"')

    if os.path.isfile(' "'+NET+'_MDF.c"') :
        os.remove('"'+NET+'_MDF.c"')

    if os.path.isfile(' "'+NET+'_erc.obj"') :
        os.remove('"'+NET+'_erc.obj"')

    if os.path.isfile(' "'+NET+'_rdy.obj"') :
        os.remove('"'+NET+'_rdy.obj"')

    if os.path.isfile(' "'+NET+'_mdf.obj"') :
        os.remove('"'+NET+'_mdf.obj"')

    if os.path.isfile(' "'+NET+'.dir' + Sep + csv_dat+'"') :
        os.remove('"'+NET+'.dir' + Sep + csv_dat+'"')


    if not os.path.ispath(' "'+NET+'.dir"') :
        os.mkdir(r'"'+NET+'.dir"')


    if os.path.isfile('"'+NET+'.pid"') :
        shutil.copy2('"'+NET+'.pid" "'+NET+'.dir"')
        os.remove('"'+NET+'.pid"')

    if os.path.isfile('"'+NET+'.RESULTS"') :
        shutil.copy2('"'+NET+'.RESULTS" "'+NET+'.dir"')
        os.remove('"'+NET+'.RESULTS"')
    if os.path.isfile('"'+NET+'.STAT_OUT"') :
        shutil.copy2('"'+NET+'.STAT_OUT" "'+NET+'.dir"')
        os.remove('"'+NET+'.STAT_OUT"')
    if os.path.isfile('"'+NET+'.TN"') :
        shutil.copy2('"'+NET+'.TN" "'+NET+'.dir"')
        os.remove('"'+NET+'.TN"')


    sys.exit(0)
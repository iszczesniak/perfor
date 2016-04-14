#PBS -q plgrid
#PBS -l walltime=71:00:00
#PBS -l nodes=1:ppn=12
#PBS -o ~/perfor/results
#PBS -j oe
module load plgrid/libs/boost/1.59.0 > /dev/null 2>&1
~/perfor/perfor > ~/perfor/results

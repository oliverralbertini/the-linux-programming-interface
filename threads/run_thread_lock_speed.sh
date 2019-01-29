#!/bin/sh
#
# run_thread_lock_speed.sh
#
# Run thread_lock_speed repeatedly, and calculate statistics
# on average real and CPU time

spin=""
errfnd=0
while getopts "s" opt; do
    case "$opt" in
    s)	spin="-s"
	;;
    *)  errfnd=1
	break
	;;
    esac
done
shift `expr $OPTIND - 1`

if test $# -ne 3 -o $errfnd -eq 1; then
    echo "Usage: $0 [-s] num-iter inner-loop-cnt outer-loop-cnt"
    exit 1
fi

num_iter=$1
inner_loop=$2
outer_loop=$3

iter=0
while test $iter -lt $num_iter; do
    echo
    echo "ITERATION:" $iter
    iter=$(expr $iter + 1)
    ( time -p ./thread_lock_speed $spin 2 $inner_loop $outer_loop ) 2>&1
done | tee /dev/tty | tee x | awk '
	BEGIN {
		cnt = 0
		real_max = user_max = sys_max = cpu_max = -1
		rmin = user_min = sys_min = cpu_min = 10000
	}	
	/^real/ {
    		cnt++
		real_tot += $2
		if ($2 < rmin)
		    rmin = $2
		if ($2 > real_max)
		    real_max = $2
    	}
	/^user/ {
	    	u = $2
		user_tot += $2
		if ($2 < user_min)
		    user_min = $2
		if ($2 > user_max)
		    user_max = $2
    	}
	/^sys/ {
		sys_tot += $2
		if ($2 < sys_min)
		    sys_min = $2
		if ($2 > sys_max)
		    sys_max = $2

		# The following relies on the fact that "tipe -p" prints
                # "user" time before "sys" time

		if (u + $2 < cpu_min)
		    cpu_min = u + $2
		if (u + $2 > cpu_max)
		    cpu_max = u + $2
    	}
	END {
	        print "cnt = " cnt
		printf("Avg real time       = %.2f  (range: %.2f, %.2f)\n",
			real_tot / cnt, rmin, real_max)
		printf("Avg CPU time        = %.2f  (range: %.2f, %.2f)\n",
			(user_tot + sys_tot) / cnt, cpu_min, cpu_max)
		printf("Avg user CPU time   = %.2f  (range: %.2f, %.2f)\n",
			user_tot / cnt, user_min, user_max)
		printf("Avg system CPU time = %.2f  (range: %.2f, %.2f)\n",
			sys_tot / cnt, sys_min, sys_max)
	}
'

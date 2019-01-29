#!/bin/sh

# Usage: sh view_v2_cgroups.sh cgroup-dir...
#
# Display cgroups in the cgroup v2 hierarchy, showing
# child cgroups, member processes, and cgroup.type

# (C) 2017 Michael Kerrisk. Licensed under the GNU GPL v2 or later

RED=$(tput setaf 1)
GREEN=$(tput setaf 10)
BLUE=$(tput setaf 4)
LIGHT_BLUE=$(tput setaf 51)
MAGENTA=$(tput setaf 5)
LIGHT_PURPLE=$(tput setaf 93)
BRIGHT=$(tput bold)
NORMAL=$(tput sgr0)
GRAY=$(tput setaf 240)
BRIGHT_YELLOW=$(tput setaf 11)
BLINK=$(tput blink)
REVERSE=$(tput smso)
UNDERLINE=$(tput smul)

INDENT="    "

display_list()
{
(
    prefix="$1"
    items="$2"
    indent="$3"
    color="$4"
    alt_color="$5"

    width=$(expr 72 - $(echo "$indent" | wc -c))
    if test $width -lt 50; then
        width=50
    fi

    items="$(echo $items | sed 's/^  *//')"
    if ! test -z "$items"; then
#        echo -n "${color}"
        echo "$prefix{$items}" | \
            fold -s -w $width | \
            sed 's/^ */'"$indent"'/;s/^\( *\)\([1-9]\)/\1       \2/' | \
            sed 's/ \([^ ]\)/ '"$color"'\1/g;
                 s/\([^ ]\) /\1'"$NORMAL"' /g;
                 s/$/'"$NORMAL"'/;
                 s/\*-/'"$alt_color"'*'"$NORMAL$color"'-/g;
                 '
#        echo "$NORMAL"
    fi
)
}

for tdir in $*; do
(
    cd $tdir/.. > /dev/null
    for dir in $(find $(basename $tdir) -type d | sort); do 
	f=$dir/cgroup.type
	if test -e $f; then
            gtype="$(cat $f)"
	else
	    gtype="root"
	fi
        ocolor=""
        ccolor=""
        show_pids=0

        if test "$gtype" = "domain threaded"; then
            ocolor="$UNDERLINE$BRIGHT$GREEN"
            type="[dt]"
            show_pids=1
        fi
        if test "$gtype" = "domain invalid"; then
            ocolor="$REVERSE$LIGHT_PURPLE"
            type="[inv]"
        fi
        if test "$gtype" = "threaded"; then
            ocolor="$GREEN"
            type="[t]"
        fi
        if test "$gtype" = "domain"; then
            ocolor=""
            type="[d]"
            show_pids=1
        fi
        if test "$gtype" = "root"; then
            ocolor=""
            type="[/]"
            show_pids=1
        fi

        if ! test -z "ocolor"; then
            ccolor="$NORMAL"
        fi

        name="$ocolor$(basename $dir)$ccolor"
        indent=$(echo $dir | sed 's%[^/]*/%'"$INDENT"'%g;s/[^ ]*$//')
        controllers=$(cat $dir/cgroup.subtree_control)
        echo -n "$indent$name $NORMAL$type$NORMAL"

        if ! test -z "$controllers"; then
            echo -n "    $BRIGHT_YELLOW($controllers)$NORMAL"
        fi
        echo

        width=$(expr 72 - $(echo "$indent$INDENT" | wc -c))
        if test $width -lt 50; then
            width=50
        fi

        if test $show_pids != 0; then
            tasks="$(echo $(sort -n $dir/cgroup.procs))"
            display_list "PIDs: " "$tasks" "$indent$INDENT" "$LIGHT_BLUE" "$RED"
        fi

	tlist="$(echo $(sort -n $dir/cgroup.threads))"
	tasks=""

	for t in $tlist; do
	    { tgid=$(awk '/^Tgid/ {print $2}' \
		< /proc/$t/status) ; } 2> /dev/null
	    { policy=$(awk '{print $41}' \
		< /proc/$t/stat) ; } 2> /dev/null
	    pchar=""

	    # SCHED_FIFO is 1, SCHED_RR is 2
	    if ! test -z "$policy" && 
		    ( test $policy -eq 1 || test $policy -eq 2); then
		pchar='*'
	    fi
	    if ! test -z "$tgid" && test $t != $tgid; then
		t="$t$pchar-[$tgid]"
	    fi
	    tasks="$tasks $t"
	done
	display_list "TIDs: " "$tasks" "$indent$INDENT" "$LIGHT_BLUE" "$RED$REVERSE"

    done
)
done 2>&1 | less -r

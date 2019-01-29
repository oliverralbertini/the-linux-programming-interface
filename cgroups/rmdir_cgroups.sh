#!/bin/sh

# Remove the cgroup trees named in the command-line arguments

rmdir $(find $* -type d | sort -r)

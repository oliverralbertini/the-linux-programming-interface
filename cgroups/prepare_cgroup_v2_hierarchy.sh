#!/bin/sh

# Make the cgroup hierarchy that is diagrammed in the cgroups v2 slides

cd /sys/fs/cgroup/unified
mkdir -p g1/t1/t1-a
mkdir -p g1/t1/t1-b/t1-c
mkdir -p g1/g1-d/g1-d2
mkdir -p g2/g2-x
mkdir -p g2/g2-y

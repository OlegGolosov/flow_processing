#!/bin/bash


path_to_input=$1
shift 1

output_dir=$(dirname $path_to_input)
path_to_processed="$output_dir/corr_processed.root"
path_to_profiles="$output_dir/profiles.root"

echo go!

root -l -q 'process.C ("'$path_to_input'","'$path_to_processed'")' || exit $?
mv -v $path_to_profiles $path_to_profiles"_old"
root -l -q 'profiles.C ("'$path_to_processed'","'$path_to_profiles'")'


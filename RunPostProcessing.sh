#!/bin/bash


path_to_corr=$1
shift 1

output_dir=$(dirname $path_to_input)
path_to_profiles="$output_dir/profiles.root"
path_to_graphs="$output_dir/graphs.root"
path_to_pics="$output_dir/pics.root"

echo go!

root -l -q 'process.C("'$path_to_corr'","'$path_to_profiles'")' || exit $?
mv -v $path_to_graphs $path_to_graphs"_old"
root -l -q 'profiles.C("'$path_to_profiles'","'$path_to_graphs'")'
root -l -q 'draw.C("'$path_to_graphs'","'$path_to_pics'")' 

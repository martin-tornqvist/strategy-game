#!/bin/bash

root_dir=$PWD

make debug -j 4 && cd target/debug && ./strategy_game

cd $root_dir

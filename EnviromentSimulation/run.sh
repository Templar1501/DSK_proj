#!bin/bash

clang NetworkSimulation.c ../ClientSimulation/ClientSimulation.c ../LamportAlgorithm/LamportAlgorithm.c ../BlockChainUtilities/BlockChain.c ../BlockChainUtilities/Riddles.c -lpthread

if test -f "a.out"; then
    ./a.out $1
    rm a.out
fi
#Specify the version being used aswell as the language
cmake_minimum_required(VERSION 2.6)
#Name your project here
project(australian_voting)

#Sends the -std=c99 flag to the gcc compiler
add_definitions(-std=c++11)
#This tells CMake to fib.c and name it fibonacci
add_executable(australian_voting voting.c++)

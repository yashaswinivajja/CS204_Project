# CS204_Project

================================================
Functional Simulator for RISCV Processor
================================================

Team Members' names:
Pedapudi Chaitanya    2021CSB1121
Peguda Akshitha       2021CSB1122
Shaik Darakshinda     2021CSB1130
Yashaswini Vajja      2021CSB1137
                        
README
Table of contents
1. Directory Structure
2. How to build
3. How to execute

Directory Structure:
--------------------
CS204-Project
  |
  |- bin
      |
      |- myRISCVSim
  |- include
      |
      |- myRISCVSim.h
  |- src
      |- main.c
      |- Makefile
      |- myRISCVSim.h
  |- test
      |- simple_add.txt
      |- fib.txt
      |- array_add.txt
      |- fact.txt
      |- bubble.txt
      
How to build
------------
For building:
	$cd src
	$make

For cleaning the project:
	$cd src
	$make clean

How to execute
--------------
./myRISCVSim test/simple_add.mc

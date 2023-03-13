# CS204_Project

================================================
Functional Simulator for RISCV Processor
================================================

Team Members' names:
Pedapudi Chaitanya    2021CSB1121
Peguda Akshitha       2019CSB1122
Shaik Darakshinda	   2019EEB1130
Yashaswini Vajja      2019CSB1137
                        
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
  |- doc
      |
      |- design-doc.docx
  |- include
      |
      |- myRISCVSim.h
  |- src
      |- main.c
      |- Makefile
      |- myRISCVSim.h
  |- test
      |- simple_add.mc
      |- fib.mc
      |- array_add.mc
      |- fact.mc
      |- bubble.mc
      
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


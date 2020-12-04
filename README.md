## PowerDEVS

PowerDEVS is a general-purpose software tool for discrete event system 
specification (DEVS) modeling and simulation oriented to the simulation of 
hybrid systems. It allows atomic DEVS models to be defined in C++ language 
that can then be coupled graphically in hierarchical block diagrams to 
create more complex systems. The environment automatically translates the 
graphically coupled models into a C++ code which executes the simulation. 

A feature of PowerDEVS is the possibility to perform simulations under a 
real-time operating system (RTAI) synchronizing with a real-time clock, 
which permits the design and automatic implementation of synchronous and 
asynchronous digital controllers. Combined with its continuous system 
simulation library, PowerDEVS is also an efficient tool for real-time 
simulation of physical systems. 
Another feature is the interconnection between PowerDEVS and the numerical 
package Scilab. PowerDEVS simulations can make use of Scilab workspace 
variables and functions, and the results can be sent back to Scilab for further 
processing and data analysis. In addition to describing the main features 
of the software tool, the article also illustrates its use with some examples 
which show its simplicity and efficiency.

## Installation

These are generic installation instructions.

## Dependences

In order to be able to install and compile ModelicaCC, 
the following  dependencies must be installed: 

   * make
   * qt5-default
   * g++ 
   * libqt5svg5-dev

In order to build the documentation:

   * texlive-latex-base
   * texlive-latex-extra

## Basic Installation

The simplest way to compile this package is:

  1. `cd` to the directory containing the package's source code and type
     `autoconf` to generate the configuration scripts.
    
  3. Type `make` to compile all the binaries. The different tools are located 
     in the bin folder. 

  4. You can remove the program binaries and object files from the
     source code directory by typing `make clean`.  


# cclibs
========

cclibs is a collection of C code that I wrote during the '90s. It is 
composed of various useful modules. Most of this code is written in
the style of libgpl.

## cfg

A module to handle windows .ini style of configuration files. 

## cmd

A module to emulate the VMS CLD library. CLD was used to parse the
command line, and could also be used to process input from within a
program. The cmd modules allows for the process input from within a
program.

## lpd

A module to handle the interaction with a LPD server. Written for a
in-house project to interact with Xerox printers.

## misc

A set of routine used for various miscellaneous things. You need to
convert a left over punched number from COBOL. There is a routine for 
that. How about a left over-punch number from DIBOL? There is a routine 
for  that. You just love those DEC BASIC string handling functions, 
there are routines for that. 

## que

A module to handle linked lists. Implemented as a double linked list.
Because a single linked list is a waste of time. Flexible enough so
that you can implement stacks, fifos and other such data structures.

## vms

Modules to interact with the VMS operating system. Not suitable for
Linux or Windows. But if you want to access a RMS index file, talk 
to a mailbox or LAT device. There is code to do so.

## windows

Modules to interact with a terminal. Based on the VMS implementation
of curses. It includes code  that allows it to also run on Linux, 
MS-DOS (pdcurses), and the Amiga (a ported version of ncurses 1.81 
which I did a long time ago). 


# An Experiment in creating objects

This is an experiment in creating objects. The private code and the
interface code is broken out from each other. Header files are used
to bind them together. A public header file is used so that other code 
can access the public methods.

Both static and dynamic libraries are created. The dynamic library can be
used for distribution. This should allow for easier updates to the main
code base.

Since the private methods are not marked as "static" they can be
accessed directly. So this uses the convention of function names with
a leading underscore are "private" and should not be accessed directly. 

This is the unfortunate byproduct of allowing private methods to be 
overridden. Like everything, there are trade offs on desirability. I
felt it was more desirable to override methods then to keep them
strictly private.



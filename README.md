
relexec
=======

relexec is a simple utility to execute an interpreter relative to a script's location.


Usage
-----

Compile relexec.c and install in a well-known place, e.g. /bin or /usr/bin.

In your scripts, use the following she-bang line:

    #!/usr/bin/relexec {relative-path-to-interpreter}

Example
-------

Assume the following directory layout:

    $TOP/
      +- bin/
           +- perl
           +- script.pl
      +- dir1/
           +- other.pl

Both scripts, script.pl and other.pl, are perl scripts which should be executed
by the perl interpreter at $TOP/bin/perl.

The she-bang line for script.pl:

    #!/usr/bin/relexec perl

The she-bang line for other.pl:

    #/usr/bin/relexec ../bin/perl

This makes the scripts location independent.


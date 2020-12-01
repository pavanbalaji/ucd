			UCD

UCD is a high-performance noncontiguous datatype engine that can be
used to express and manipulate noncontiguous data.  This release is an
experimental version of UCD that contains features related to
packing/unpacking, I/O vectors, and flattening noncontiguous
datatypes.

This README file should contain enough information to get you started
with UCD.  More information about UCD can be found at
https://github.com/pmodels/ucd.


1. Getting Started
2. Testing UCD
3. Reporting Problems
4. Alternate Configure Options
5. Compiler Flags
6. Developer Builds


-------------------------------------------------------------------------------

# Getting Started


The following instructions take you through a sequence of steps to get the
default configuration of UCD up and running.

1. You will need the following prerequisites.

    - REQUIRED: This tar file ucd-<version>.tar.gz

    - REQUIRED: A C compiler (gcc is sufficient)

  Also, you need to know what shell you are using since different shell has
  different command syntax.  Command "echo $SHELL" prints out the current shell
  used by your terminal program.

2. Unpack the tar file and go to the top level directory:

    tar xzf ucd-<version>.tar.gz
    cd ucd-<version>

3. Choose an installation directory, say /home/USERNAME/ucd-install,
which is assumed to be non-existent or empty.

4. Configure UCD specifying the installation directory:

      ./configure --prefix=/home/USERNAME/ucd-install 2>&1 | tee c.txt

5. Build UCD:

      make 2>&1 | tee m.txt

  This step should succeed if there were no problems with the preceding step.
  Check file m.txt.  If there were problems, do a "make clean" and then run
  make again with V=1.

    make V=1 2>&1 | tee m.txt   (for bash and sh)

  Then go to step 3 below, for reporting the issue to the UCD developers
  and other users.

6. Install UCD:

      make install 2>&1 | tee mi.txt

  This step collects all required executables and scripts in the bin
  subdirectory of the directory specified by the prefix argument to configure.

-------------------------------------------------------------------------------

# Testing UCD

To test UCD, we package the UCD test suite in the UCD
distribution.  You can run the test suite in the test directory using:

     make testing

If you run into any problems on running the test suite, please follow
step 3 below for reporting them to the UCD developers and other
users.

-------------------------------------------------------------------------------

# Reporting Problems

If you have problems with the installation or usage of UCD, please follow
these steps:

1. First visit the Frequently Asked Questions (FAQ) page at
https://github.com/pmodels/ucd/wiki/FAQ
to see if the problem you are facing has a simple solution.

2. If you cannot find an answer on the FAQ page, look through
previous issues filed (https://github.com/pmodels/ucd/issues).  It
is likely someone else had a similar problem, which has already been
resolved before.


-------------------------------------------------------------------------------

# Alternate Configure Options

UCD has a number of other features.  If you are exploring UCD as part
of a development project, you might want to tweak the UCD build with the
following configure options.  A complete list of configuration options can be
found using:

    ./configure --help

-------------------------------------------------------------------------------

# Compiler Flags

By default, UCD automatically adds certain compiler optimizations to
CFLAGS.  The currently used optimization level is -O2.

This optimization level can be changed with the --enable-fast option passed to
configure.  For example, to build UCD with -O3, one can simply do:

    ./configure --enable-fast=O3

Or to disable all compiler optimizations, one can do:

    ./configure --disable-fast

For more details of --enable-fast, see the output of "./configure --help".

For performance testing, we recommend the following flags:

    ./configure --enable-fast=O3

-------------------------------------------------------------------------------

# Developer Builds

For UCD developers who want to directly work on the primary version control
system, there are a few additional steps involved (people using the release
tarballs do not have to follow these steps).  Details about these steps can be
found here: https://github.com/pmodels/ucd/wiki/Getting-and-Building

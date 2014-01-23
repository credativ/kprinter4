Kprinter4
=========

Kprinter4 is a simple stand-alone document printer modelled after the KDE4
print dialog. It can be used in place of /bin/lpr in order to better control
the print setup of legacy applications.

It is modelled after the KDE3-based kprinter application.

### Prerequisites

Debian Packages: psutils, ghostscript

### Compile

mkdir build
cd build
cmake -DCMAKE_VERBOSE_MAKEFILE=ON -DCMAKE_INSTALL_PREFIX=/usr ..
make
(sudo) make install

### Run

build/kprinter --help
build/kprinter file1.ps [file2.ps] ..

KPrinter4
=========

KPrinter4 is a simple stand-alone PostScript document printer modelled after the
KDE 4 print dialog. It can be used in place of /bin/lpr in order to better
control the print setup of non-KDE applications.

Main features:

* Printing PostScript documents with KDE 4 print dialog.
* Scaling and positioning of documents.
* Poster printing (backported from the known KDE 3 KPrinter tool).

Known limitations so far:

* Poster printing with multi-page documents: Only first page will be taken.

### Prerequisites

Debian Packages: psutils, ghostscript, poster

### Compile

mkdir build
cd build
cmake -DCMAKE_VERBOSE_MAKEFILE=ON -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/usr ..
make
(sudo) make install

### Run

build/kprinter4 --help
build/kprinter4 [options] file.ps

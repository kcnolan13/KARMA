KARMA
=====

Kyle's AAMPS Real-time Map App


KARMA has been designed to help task-saturated AOC  flight crews make better, faster decisions.


All supporting source for the software package is contained in one root directory, KARMA, designed to reside in the Tile Server’s home folder. This ~/KARMA/ directory contains the two major facets of the project as it exists today:

~/KARMA/karma/: The C++ application that logs AAMPS data from the Data Distribution Server (DDS) to a PostgreSQL database (entitled flight_archive).

~/KARMA/server/: All sources related to the client-facing user interface suite.


In traditional AAMPS data system fashion, ~/KARMA/karma/ contains karma.cpp source code, supported by a handful of classic helper files (e.g. global.h, Message.cpp, Message.h, TLV.cpp, TLV.h, etc.). The ~/KARMA/karma/Release/ subdirectory contains a Makefile, the compiled source code, and several config files.

Meanwhile, ~/KARMA/server/ contains an assortment of routines and dynamically generated map assets necessary for proper User Interface (UI) operation. This directory is made network-accessible via Apache webserver through use of a symbolic link. This link, pointing to ~/KARMA/server/, resides in the default web content location -- either /var/www/ or /var/www/html/, depending on the individual Apache installation.

~/KARMA/server/ also has a subdirectory ~/KARMA/server/icons/,  containing a myriad of UI graphics, including a large folder of PHP-generated wind barb icons. 
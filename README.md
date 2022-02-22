XR-MCU

XR-MCU is a multithreading server capable of connecting to a set of players and constructing virtual scene with their pointclouds.
In order to do so this framework has been divided into three main modules:

1.- Acquisition: This module is devoted the acquisition of frame flows coming from of connected session players. Its main duties are then:

  .- Connection to session players. The technology to do so is by means of socket.io technology against an outer orchestrator.
  .- Reception of incoming frame flow. Again, we use socket.io as communication technology.
  .- Decoding of input frame flow. Current XR-MCU version only deals with cwi codecs but the implementation allow any other codec format assuming there is a conversion between frame formats.

2.-Storage: This module is devoted to the storage of incoming geometry. Every time a new player is connected to XR-MCU a new mcu player is created, which is responsible of processing player events (mainly player position). Each player comes with a geometry container which stores a fixed number of last frames. Because current codec implementation demmands wolrd coordinates, this geometry container stores transformed geometry into world coordinates.

3.-Scene Builiding: Every player (consumer) is attached to a scene builder, which is in charge of of collecting latest received frames of each visible player. For doing so we have the following steps:

  .- Resove visble players: given a set of session players, resolve those who are i nsight given a frustrum that characterizes our camera.
  .- Collect the latest frame of each visible player. Perform a fusion of the geometry set.
  .- Encode the resulting scene.

In order to generate XR-MCU project, you need CMake 3.13 or newer. You can download the latest version at this link:

https://cmake.org/download/[](https://github.com/jaumeMA/ddk#installing-perl)

Just launch the installer and follow the instructions.
Installing Perl

In order to generate CMake scripts, you need Perl. You can download the latest version at ActiveState website:

https://www.activestate.com/products/perl/downloads/[](https://github.com/jaumeMA/ddk#preconditions)[](https://github.com/jaumeMA/ddk#building-the-project)

Just launch the installer and follow the instructions.
Preconditions

This code compiles in windows visual studio 2019 (18.6 or later) and in linux gcc 7 or later.
Building the Project

Clone the repository
execute perl script genPrj-XXX.pl (where XXX stands for platform)

The MCUXXX.sln solution will be located in the build folder, while generated binaries will be generated in the bin folder.

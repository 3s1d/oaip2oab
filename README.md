# oaip2oab

To build on Ubuntu (18.04) - should work on any other debian based system as well:

`sudo apt install libboost-all-dev libkml-dev`

`cd Src`

`make`

After successful build:

`sudo make install`

Usage:
`oaip2oab <Path to openAIP files>`
  
The tool will generate a world.oab file in open air binary format in the current directory. It can be used on a compatible skytraxx variometer.
Get some aip-files with airspaces here: https://openaip.net

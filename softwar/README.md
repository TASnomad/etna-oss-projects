# SoftWar

ETNA C project SoftWar

A tiny multiplayer game played by AIs

__Status__
- Still in developpement
- WIP:
	- Integration of libjson-c for PUB/SUB messaging

- `Tested on GNU/Linux Debian stretch distro`
- `Partial support for Darwin machines (need more serious tests)`

# Dependencies
- __ZMQ__ lib
- __CZMQ__ lib
- __JSON-C__ lib

Debian installation:

	- `sudo apt-get install libzmq3-dev`

	- `sudo apt-get install libczmq-dev`

	- `sudo apt-get install `libjson-c-dev`

Darwin installation:

	- `brew install zmq`

	- `brew install czmq`

	- `brew install json-c`

Arch installation:

	- `aurman/yaourt -S czmq`

	- `aurman/yaourt -S zeromq`

	- `aurman/yaourt -S json-c`


	- `pacman -S python2-pip`

	- `sudo pip2 install --upgrade pip`

	- `sudo pip2 --install pyzmq`

# Build
	* Building stable binary `make re`

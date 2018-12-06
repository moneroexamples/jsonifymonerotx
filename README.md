## Makes complete json objects of transactions and blocks

Useful for producing json of txs and blocks for use in unit tests
in other projects. The will be used primarly in openmonero and the onion explorer
for generating transaction and block json representations for unit tests.

## Example compilation on Ubuntu 18.04

C++14 is required to run this code.

#### Monero download and compilation

Download and compile recent Monero into your home folder:

```bash
# first install monero dependecines
sudo apt update

sudo apt install git build-essential cmake libboost-all-dev miniupnpc libunbound-dev graphviz doxygen libunwind8-dev pkg-config libssl-dev libcurl4-openssl-dev libgtest-dev libreadline-dev libzmq3-dev libsodium-dev libhidapi-dev libhidapi-libusb0

# go to home folder
cd ~

git clone --recurse-submodules https://github.com/monero-project/monero

cd monero/

USE_SINGLE_BUILDDIR=1 make
```


#### Compilation of the xmregcore

```bash

# go to home folder if still in ~/monero
cd ~

git clone --recurse-submodules  https://github.com/moneroexamples/jsonifymonerotx.git

cd jsonifymonerotx

mkdir build && cd build

cmake ..

# altearnatively can use cmake -DMONERO_DIR=/path/to/monero_folder ..
# if monero is not in ~/monero

make

```

## Other examples

Other examples can be found on  [github](https://github.com/moneroexamples?tab=repositories).
Please know that some of the examples/repositories are not
finished and may not work as intended.

## How can you help?

Constructive criticism, code and website edits are always good. They can be made through github.

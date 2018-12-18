## Makes complete json objects of transactions and blocks

Useful for producing json of txs and blocks for use in unit tests
in other projects. The will be used primarly in openmonero and the onion explorer
for generating transaction and block json representations for unit tests.

## Example compilation on Ubuntu 18.04

C++14 is required to compile this code.

#### Monero download and compilation

Follow instructions in the following link:

https://github.com/moneroexamples/monero-compilation/blob/master/README.md

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

#### Examples 

For stagenet:

```bash
./jsonifyxmrtx 233281d06f745ff79213765112e0b4e34f9f833b151b846e6efcd202cf74d5e7 -s 57Hx8QpLUSMjhgoCNkvJ2Ch91mVyxcffESCprnRPrtbphMCv8iGUEfCUJxrpUWUeWrS9vPWnFrnMmTwnFpSKJrSKNuaXc5q,9595c2445cdd4c88d78f0af41ebdf52f68ae2e3597b9e7b99bc3d62e300df806,6fe76a5e4657695cbcc09fab93b70ac974c76e0f02afde71cb460423d1444b03 -r 78tUApnibpS26vFVpzvMN9VKgPaw2EUgU7ViswvKTfy1XJ4NhM6NQcZL6TUWm81sX7bgJJxLYe2MxDx4MLfYRLZJPAMMnon,b45e6f38b2cd1c667459527decb438cdeadf9c64d93c8bccf40a9bf98943dc09 78tUApnibpS26vFVpzvMN9VKgPaw2EUgU7ViswvKTfy1XJ4NhM6NQcZL6TUWm81sX7bgJJxLYe2MxDx4MLfYRLZJPAMMnon,b45e6f38b2cd1c667459527decb438cdeadf9c64d93c8bccf40a9bf98943dc09 55ZbQdMnZHPFS8pmrhHN5jMpgJwnnTXpTDmmM5wkrBBx4xD6aEnpZq7dPkeDeWs67TV9HunDQtT3qF2UGYWzGGxq3zYWCBE,c8a4d62e3c86de907bd84463f194505ab07fc231b3da753342d93fccb5d39203
```

Second example:

```bash
./jsonifyxmrtx c06df274acc273fbce0666b2c8846ac6925a1931fb61e3020b7cc5410d4646b1 -s 57Hx8QpLUSMjhgoCNkvJ2Ch91mVyxcffESCprnRPrtbphMCv8iGUEfCUJxrpUWUeWrS9vPWnFrnMmTwnFpSKJrSKNuaXc5q,9595c2445cdd4c88d78f0af41ebdf52f68ae2e3597b9e7b99bc3d62e300df806 -r 55ZbQdMnZHPFS8pmrhHN5jMpgJwnnTXpTDmmM5wkrBBx4xD6aEnpZq7dPkeDeWs67TV9HunDQtT3qF2UGYWzGGxq3zYWCBE,c8a4d62e3c86de907bd84463f194505ab07fc231b3da753342d93fccb5d39203 52yPC3h4aaPGhsocGyava38xfhAYMg2262r4SzXppUUxNHNi9JEoVVzKZdJf7BZ6nLeZf5RCiDT2wLK9ZsZ2iH7oDAHxGgb,917c975d30dcd71a60a334823d39934dedd7cb064ea56a2b1328c4a4a201b60c 7AEr46AkwDjaJw2nxrcjBWfX5YifuL7Sw2c3mCciPTHU5tzmmCrsnNvcy4xPTQmcgSSKmt295t8roSUxHLfVtcQoLgEmRN5,917c975d30dcd71a60a334823d39934dedd7cb064ea56a2b1328c4a4a201b60c
```

## Other examples

Other examples can be found on  [github](https://github.com/moneroexamples?tab=repositories).
Please know that some of the examples/repositories are not
finished and may not work as intended.

## How can you help?

Constructive criticism, code and website edits are always good. They can be made through github.

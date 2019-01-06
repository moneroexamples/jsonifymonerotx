## Makes complete json objects of transactions and blocks

Useful for producing json of txs and blocks for use in unit tests
in other projects. The will be used primarly in openmonero and the onion explorer
for generating transaction and block json representations for unit tests.

## Example compilation on Ubuntu 18.04

C++14 is required to compile this code.

#### Monero download and compilation

Follow instructions in the following link:

https://github.com/moneroexamples/monero-compilation/blob/master/README.md

#### Compilation of the jsonifymonerotx

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

#### Program options

```bash
jsonifymonerotx: construct complete json representaiton of monero tx or blk:
  -h [ --help ]                         Help
  --hash arg                            transaction or block hash to jsonify
  -n [ --nettype ] arg (=2)             network type: 0-MAINNET, 1-TESTNET,
                                        2-STAGENET
  -b [ --blockchain-path ] arg (=/home/mwo2/.bitmonero/stagenet/lmdb)
                                        path to lmdb folder containing the
                                        blockchain
  -s [ --sender ] arg                   optional sender's address,viewkey,spend
                                        key
  -m [ --message ] arg                  optional message to be saved in the
                                        json file
  -t [ --txprvkey ] arg                 transaction private key
  -r [ --recipients ] arg               optional recipients'
                                        address,viewkey,spendkey
  -w [ --save ]                         write json produced to a file
  -c [ --command ]                      save command used to generate the json
                                        file
  -d [ --display ]                      do not display json produced
```


#### Examples 

Some stagenet transactions for jsonification:

```bash
./jsonifyxmrtx 233281d06f745ff79213765112e0b4e34f9f833b151b846e6efcd202cf74d5e7 -s 57Hx8QpLUSMjhgoCNkvJ2Ch91mVyxcffESCprnRPrtbphMCv8iGUEfCUJxrpUWUeWrS9vPWnFrnMmTwnFpSKJrSKNuaXc5q,9595c2445cdd4c88d78f0af41ebdf52f68ae2e3597b9e7b99bc3d62e300df806,6fe76a5e4657695cbcc09fab93b70ac974c76e0f02afde71cb460423d1444b03 -r 78tUApnibpS26vFVpzvMN9VKgPaw2EUgU7ViswvKTfy1XJ4NhM6NQcZL6TUWm81sX7bgJJxLYe2MxDx4MLfYRLZJPAMMnon,b45e6f38b2cd1c667459527decb438cdeadf9c64d93c8bccf40a9bf98943dc09 78tUApnibpS26vFVpzvMN9VKgPaw2EUgU7ViswvKTfy1XJ4NhM6NQcZL6TUWm81sX7bgJJxLYe2MxDx4MLfYRLZJPAMMnon,b45e6f38b2cd1c667459527decb438cdeadf9c64d93c8bccf40a9bf98943dc09 55ZbQdMnZHPFS8pmrhHN5jMpgJwnnTXpTDmmM5wkrBBx4xD6aEnpZq7dPkeDeWs67TV9HunDQtT3qF2UGYWzGGxq3zYWCBE,c8a4d62e3c86de907bd84463f194505ab07fc231b3da753342d93fccb5d39203
```


```bash
./jsonifyxmrtx c06df274acc273fbce0666b2c8846ac6925a1931fb61e3020b7cc5410d4646b1 -s 57Hx8QpLUSMjhgoCNkvJ2Ch91mVyxcffESCprnRPrtbphMCv8iGUEfCUJxrpUWUeWrS9vPWnFrnMmTwnFpSKJrSKNuaXc5q,9595c2445cdd4c88d78f0af41ebdf52f68ae2e3597b9e7b99bc3d62e300df806,6fe76a5e4657695cbcc09fab93b70ac974c76e0f02afde71cb460423d1444b03 -r 55ZbQdMnZHPFS8pmrhHN5jMpgJwnnTXpTDmmM5wkrBBx4xD6aEnpZq7dPkeDeWs67TV9HunDQtT3qF2UGYWzGGxq3zYWCBE,c8a4d62e3c86de907bd84463f194505ab07fc231b3da753342d93fccb5d39203 52yPC3h4aaPGhsocGyava38xfhAYMg2262r4SzXppUUxNHNi9JEoVVzKZdJf7BZ6nLeZf5RCiDT2wLK9ZsZ2iH7oDAHxGgb,917c975d30dcd71a60a334823d39934dedd7cb064ea56a2b1328c4a4a201b60c 7AEr46AkwDjaJw2nxrcjBWfX5YifuL7Sw2c3mCciPTHU5tzmmCrsnNvcy4xPTQmcgSSKmt295t8roSUxHLfVtcQoLgEmRN5,917c975d30dcd71a60a334823d39934dedd7cb064ea56a2b1328c4a4a201b60c
```

```bash
./jsonifyxmrtx d7dcb2daa64b5718dad71778112d48ad62f4d5f54337037c420cb76efdd8a21c -s 56heRv2ANffW1Py2kBkJDy8xnWqZsSrgjLygwjua2xc8Wbksead1NK1ehaYpjQhymGK4S8NPL9eLuJ16CuEJDag8Hq3RbPV,b45e6f38b2cd1c667459527decb438cdeadf9c64d93c8bccf40a9bf98943dc09,df0f5720ae0b69454ca7db35db677272c7c19513cd0dc4147b0e00792a10f406 -r 55ZbQdMnZHPFS8pmrhHN5jMpgJwnnTXpTDmmM5wkrBBx4xD6aEnpZq7dPkeDeWs67TV9HunDQtT3qF2UGYWzGGxq3zYWCBE,c8a4d62e3c86de907bd84463f194505ab07fc231b3da753342d93fccb5d39203,db97fd6562de6ea59a04ee487dd3e8d73fca2e4348bf2a786071c32e29638808 78LbLrVuGpjWXFfazxJhP9RkEaKFoUgMvRhuAoEeeWvti4rQUQvNLRLW9NQyZAQ9KW3AzZfxYsfojFVJQbE8G1Kh7RxRPLW,c8a4d62e3c86de907bd84463f194505ab07fc231b3da753342d93fccb5d39203,db97fd6562de6ea59a04ee487dd3e8d73fca2e4348bf2a786071c32e29638808 75pv9rs1sVKcbqkH2iLhJEhCgMRhfnhjoetxDECyJNVN3SodRmdk9oNBAhK7dicQRfXmC2fZirLru8ac8RC9iejiA4pHTVz,c8a4d62e3c86de907bd84463f194505ab07fc231b3da753342d93fccb5d39203,db97fd6562de6ea59a04ee487dd3e8d73fca2e4348bf2a786071c32e29638808 57Hx8QpLUSMjhgoCNkvJ2Ch91mVyxcffESCprnRPrtbphMCv8iGUEfCUJxrpUWUeWrS9vPWnFrnMmTwnFpSKJrSKNuaXc5q,9595c2445cdd4c88d78f0af41ebdf52f68ae2e3597b9e7b99bc3d62e300df806,6fe76a5e4657695cbcc09fab93b70ac974c76e0f02afde71cb460423d1444b03
```

```bash
./jsonifyxmrtx ddff95211b53c194a16c2b8f37ae44b643b8bd46b4cb402af961ecabeb8417b2 -s 56heRv2ANffW1Py2kBkJDy8xnWqZsSrgjLygwjua2xc8Wbksead1NK1ehaYpjQhymGK4S8NPL9eLuJ16CuEJDag8Hq3RbPV,b45e6f38b2cd1c667459527decb438cdeadf9c64d93c8bccf40a9bf98943dc09,df0f5720ae0b69454ca7db35db677272c7c19513cd0dc4147b0e00792a10f406 -r 57Hx8QpLUSMjhgoCNkvJ2Ch91mVyxcffESCprnRPrtbphMCv8iGUEfCUJxrpUWUeWrS9vPWnFrnMmTwnFpSKJrSKNuaXc5q,9595c2445cdd4c88d78f0af41ebdf52f68ae2e3597b9e7b99bc3d62e300df806,6fe76a5e4657695cbcc09fab93b70ac974c76e0f02afde71cb460423d1444b03
```

```bash
./jsonifyxmrtx 140807b970e52b7c633d7ca0ba5be603922aa7a2a1213bdd16d3c1a531402bf6 -w -s 57hGLsqr6eLjUDoqWwP3Ko9nCJ4GFN5AyezdxNXwpa1PMt6M4AbsBgcHH21hVe2MJrLGSM9C7UTqcEmyBepdhvFE4eyW3Kd,abd38825f114988939b06b6d694fa1c82497c8276685e64a559370be5428260b,901e26d14fa03bad2295cd5a1f2106b8e8f27690d78db0d7245b190bf0d77201 -r 55ZbQdMnZHPFS8pmrhHN5jMpgJwnnTXpTDmmM5wkrBBx4xD6aEnpZq7dPkeDeWs67TV9HunDQtT3qF2UGYWzGGxq3zYWCBE,c8a4d62e3c86de907bd84463f194505ab07fc231b3da753342d93fccb5d39203,db97fd6562de6ea59a04ee487dd3e8d73fca2e4348bf2a786071c32e29638808 78LbLrVuGpjWXFfazxJhP9RkEaKFoUgMvRhuAoEeeWvti4rQUQvNLRLW9NQyZAQ9KW3AzZfxYsfojFVJQbE8G1Kh7RxRPLW,c8a4d62e3c86de907bd84463f194505ab07fc231b3da753342d93fccb5d39203,db97fd6562de6ea59a04ee487dd3e8d73fca2e4348bf2a786071c32e29638808 75pv9rs1sVKcbqkH2iLhJEhCgMRhfnhjoetxDECyJNVN3SodRmdk9oNBAhK7dicQRfXmC2fZirLru8ac8RC9iejiA4pHTVz,c8a4d62e3c86de907bd84463f194505ab07fc231b3da753342d93fccb5d39203,db97fd6562de6ea59a04ee487dd3e8d73fca2e4348bf2a786071c32e29638808 57Hx8QpLUSMjhgoCNkvJ2Ch91mVyxcffESCprnRPrtbphMCv8iGUEfCUJxrpUWUeWrS9vPWnFrnMmTwnFpSKJrSKNuaXc5q,9595c2445cdd4c88d78f0af41ebdf52f68ae2e3597b9e7b99bc3d62e300df806,6fe76a5e4657695cbcc09fab93b70ac974c76e0f02afde71cb460423d1444b03 -t 08ff71026b174e8e4829d60e7368f33b5b69628e7e6b42212f30333e52fcdc0c
```

```bash
./jsonifyxmrtx a7a4e3bdb305b97c43034440b0bc5125c23b24d0730189261151c0aa3f2a05fc -w -s 57hGLsqr6eLjUDoqWwP3Ko9nCJ4GFN5AyezdxNXwpa1PMt6M4AbsBgcHH21hVe2MJrLGSM9C7UTqcEmyBepdhvFE4eyW3Kd,abd38825f114988939b06b6d694fa1c82497c8276685e64a559370be5428260b,901e26d14fa03bad2295cd5a1f2106b8e8f27690d78db0d7245b190bf0d77201 -r 55ZbQdMnZHPFS8pmrhHN5jMpgJwnnTXpTDmmM5wkrBBx4xD6aEnpZq7dPkeDeWs67TV9HunDQtT3qF2UGYWzGGxq3zYWCBE,c8a4d62e3c86de907bd84463f194505ab07fc231b3da753342d93fccb5d39203,db97fd6562de6ea59a04ee487dd3e8d73fca2e4348bf2a786071c32e29638808 78LbLrVuGpjWXFfazxJhP9RkEaKFoUgMvRhuAoEeeWvti4rQUQvNLRLW9NQyZAQ9KW3AzZfxYsfojFVJQbE8G1Kh7RxRPLW,c8a4d62e3c86de907bd84463f194505ab07fc231b3da753342d93fccb5d39203,db97fd6562de6ea59a04ee487dd3e8d73fca2e4348bf2a786071c32e29638808 75pv9rs1sVKcbqkH2iLhJEhCgMRhfnhjoetxDECyJNVN3SodRmdk9oNBAhK7dicQRfXmC2fZirLru8ac8RC9iejiA4pHTVz,c8a4d62e3c86de907bd84463f194505ab07fc231b3da753342d93fccb5d39203,db97fd6562de6ea59a04ee487dd3e8d73fca2e4348bf2a786071c32e29638808 57Hx8QpLUSMjhgoCNkvJ2Ch91mVyxcffESCprnRPrtbphMCv8iGUEfCUJxrpUWUeWrS9vPWnFrnMmTwnFpSKJrSKNuaXc5q,9595c2445cdd4c88d78f0af41ebdf52f68ae2e3597b9e7b99bc3d62e300df806,6fe76a5e4657695cbcc09fab93b70ac974c76e0f02afde71cb460423d1444b03 -t 0b3b1a044b19078be720f9c5582da0148a5c72056391fe33b5546ae06dda130d
```


## Other examples

Other examples can be found on  [github](https://github.com/moneroexamples?tab=repositories).
Please know that some of the examples/repositories are not
finished and may not work as intended.

## How can you help?

Constructive criticism, code and website edits are always good. They can be made through github.

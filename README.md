# ising-model
### To do list
 - implement wolff algorithm
 - decide whether is good to use wolff basede on acceptance
 - print configurations at successive tau times
## Build
There is a `GNUmakefile` in the parent directory which can easily set up all the directory tree structure needed for running the programs. The whole project is then built using command
```
make
```
By default this compile the `C` program creating the `ising-model-c` executable, which in turn can be executed with
```
./ising-model-c <args>
```
If you want to change the main porgram name simply set `PROGRAM` envirnomental variable when making the project
```
PROGRAM=custom_name.fancy make
```
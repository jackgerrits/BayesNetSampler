## Build
Run `make` to compile all necessary files and produce a binary called `sampler`.

```
make
```

## Test
Uses the Google Test framework. https://github.com/google/googletest/

To run the included unit and integration tests contained in `test.cpp` :

```
make test
```

Note that several integration tests are non-deterministic given the nature of this assignment, however they have been given a large enough range and number of samples such that if it is working correctly it will pass.

## Run

```
./sampler [-r] networkDefinitionFile
```

- `-r` - use rejection sampling. Omitting `-r` uses the default likelihood sampling method.

- `networkDefinitonFile` - path to a file containing a network definition. An example definition can be seen in `burglarnetwork.txt`

The program then accepts a query from `stdin` and prints the probability distribution of the query variable to `stdout`. An example query is:

```
P(Burglar | Alarm=true, Earthquake=true)
```

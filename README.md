# C-ANN

An implementation of a neural network in pure C.

Points in 2D are assigned either `+1` or `-1` as their class.
The goal is to train a neural network to properly split points between these classes.

The data is from: [caxenie/gradient-descent-ann](https://github.com/caxenie/gradient-descent-ann).

Training data: input.csv

Test data: testing.csv

Expected labels: expected.csv

## Building

The project uses make. To build it, simply call make in the project root directory.

```
$ make
```

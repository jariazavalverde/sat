# Boolean satisfiability problem
## A simple SAT solver implemented in C

The **Boolean satisfiability problem** (**SAT**) is the problem of determining if there exists an interpretation that satisfies a given boolean formula.

## Compilation

```shell
gcc -I/usr/include -L/usr/lib main.c io.h io.c structures.c structures.h sat.h sat.c -o sat
```

## Usage

```shell
$ ./sat "path/to/file.cnf" [-t] [-i]
```

* **-t** - shows the execution time
* **-i** - shows the formula, the number of variables and the number of clauses

### DIMACS CNF file format

This format is used to define a bolean expression, written in **conjunctive normal form** (**CNF**), that may be used as an example of the **satisfiability problem**. A formula is in conjunctive normal form if it is a conjunction of one or more clauses, where a clause is a disjunction of literals.

The CNF file format is an ASCII file format:

1. The file may begin with comment lines. The first character of each comment line must be a lower case letter "c". Comment lines typically occur in one section at the beginning of the file, but are allowed to appear throughout the file.
1. The comment lines are followed by the "problem" line. This begins with a lower case "p" followed by a space, followed by the problem type, which for CNF files is "cnf", followed by the number of variables followed by the number of clauses.
1. The remainder of the file contains lines defining the clauses, one by one.
1. A clause is defined by listing the index of each positive literal, and the negative index of each negative literal. Indices are 1-based, and for obvious reasons the index 0 is not allowed.
1. The definition of a clause may extend beyond a single line of text.
1. The definition of a clause is terminated by a final value of "0".
1. The file terminates after the last clause is defined.

Here is the CNF file that corresponds to the simple formula `(x1 OR (NOT x3)) AND (x2 OR x3 OR (NOT x1))`:

```
c  simple_v3_c2.cnf
c
p cnf 3 2
1 -3 0
2 3 -1 0
```
*Reference: [https://people.sc.fsu.edu/~jburkardt/data/cnf/cnf.html](https://people.sc.fsu.edu/~jburkardt/data/cnf/cnf.html)*

## Example

```shell
$ ./sat "examples/par8-1-c.cnf" -t -i
```

```shell
( -2 1 )( -3 -2 )( -3 -2 -1 )( 3 2 -1 )( -3 2 1 )( 3 -2 1 )( -4 2 )( -5 -4 )( -5 -4 -2 )( 5 4 -2 )( -5 4 2 )( 5 -4 2 )( -6 4 )( -7 -6 )( -7 -6 -4 )( 7 6 -4 )( -7 6 4 )( 7 -6 4 )( -8 6 )( -9 -8 )( -9 -8 -6 )( 9 8 -6 )( -9 8 6 )( 9 -8 6 )( -10 8 )( -11 -10 )( -11 -10 -8 )( 11 10 -8 )( -11 10 8 )( 11 -10 8 )( -12 10 )( -13 -12 )( -13 -12 -10 )( 13 12 -10 )( -13 12 10 )( 13 -12 10 )( -14 12 )( -15 -14 )( -15 -14 -12 )( 15 14 -12 )( -15 14 12 )( 15 -14 12 )( -16 14 )( -17 -16 )( -17 -16 -14 )( 17 16 -14 )( -17 16 14 )( 17 -16 14 )( -18 16 )( -19 -18 )( -19 -18 -16 )( 19 18 -16 )( -19 18 16 )( 19 -18 16 )( -20 18 )( -21 -20 )( -21 -20 -18 )( 21 20 -18 )( -21 20 18 )( 21 -20 18 )( -22 20 )( -23 -22 )( -23 -22 -20 )( 23 22 -20 )( -23 22 20 )( 23 -22 20 )( -24 22 )( -25 -24 )( -25 -24 -22 )( 25 24 -22 )( -25 24 22 )( 25 -24 22 )( -26 24 )( -27 -26 )( -27 -26 -24 )( 27 26 -24 )( -27 26 24 )( 27 -26 24 )( -28 26 )( -29 -28 )( -29 -28 -26 )( 29 28 -26 )( -29 28 26 )( 29 -28 26 )( 28 -30 )( -31 -30 )( -31 -28 -30 )( 31 -28 30 )( -31 28 30 )( 31 28 -30 )( -33 -32 -3 )( 33 32 -3 )( -33 32 3 )( 33 -32 3 )( -35 -34 -32 )( 35 34 -32 )( -35 34 32 )( 35 -34 32 )( -37 -34 36 )( 37 -34 -36 )( -37 34 -36 )( 37 34 36 )( -39 -38 -5 )( 39 38 -5 )( -39 38 5 )( 39 -38 5 )( -35 -40 -38 )( 35 40 -38 )( -35 40 38 )( 35 -40 38 )( -42 -41 -40 )( 42 41 -40 )( -42 41 40 )( 42 -41 40 )( -36 -41 43 )( 36 -41 -43 )( -36 41 -43 )( 36 41 43 )( -44 -7 29 )( 44 -7 -29 )( 44 7 29 )( -44 7 -29 )( -33 -45 -44 )( 33 45 -44 )( -33 45 44 )( 33 -45 44 )( -37 -36 -45 )( 37 36 -45 )( -37 36 45 )( 37 -36 45 )( -37 -46 -9 )( 37 46 -9 )( -37 46 9 )( 37 -46 9 )( -36 -43 -46 )( 36 43 -46 )( -36 43 46 )( 36 -43 46 )( -39 -47 -11 )( 39 47 -11 )( -39 47 11 )( 39 -47 11 )( -33 -48 -47 )( 33 48 -47 )( -33 48 47 )( 33 -48 47 )( -37 -36 -48 )( 37 36 -48 )( -37 36 48 )( 37 -36 48 )( -39 -49 -13 )( 39 49 -13 )( -39 49 13 )( 39 -49 13 )( -33 -36 -49 )( 33 36 -49 )( -33 36 49 )( 33 -36 49 )( -50 -15 29 )( 50 -15 -29 )( 50 15 29 )( -50 15 -29 )( -35 -37 -50 )( 35 37 -50 )( -35 37 50 )( 35 -37 50 )( -39 -35 -17 )( 39 35 -17 )( -39 35 17 )( 39 -35 17 )( -39 -51 -19 )( 39 51 -19 )( -39 51 19 )( 39 -51 19 )( -35 -52 -51 )( 35 52 -51 )( -35 52 51 )( 35 -52 51 )( -37 -52 42 )( 37 -52 -42 )( -37 52 -42 )( 37 52 42 )( -53 -21 29 )( 53 -21 -29 )( 53 21 29 )( -53 21 -29 )( -33 -54 -53 )( 33 54 -53 )( -33 54 53 )( 33 -54 53 )( -35 -54 42 )( 35 -54 -42 )( -35 54 -42 )( 35 54 42 )( -33 -23 42 )( 33 -23 -42 )( -33 23 -42 )( 33 23 42 )( -55 -25 29 )( 55 -25 -29 )( 55 25 29 )( -55 25 -29 )( -33 -56 -55 )( 33 56 -55 )( -33 56 55 )( 33 -56 55 )( -35 -56 36 )( 35 -56 -36 )( -35 56 -36 )( 35 56 36 )( -39 -57 -27 )( 39 57 -27 )( -39 57 27 )( 39 -57 27 )( -58 -57 29 )( 58 -57 -29 )( 58 57 29 )( -58 57 -29 )( -35 -59 -58 )( 35 59 -58 )( -35 59 58 )( 35 -59 58 )( -37 -59 -36 )( 37 -59 36 )( -37 59 36 )( 37 59 -36 )( -37 -60 -31 )( 37 60 -31 )( -37 60 31 )( 37 -60 31 )( -42 -61 -60 )( 42 61 -60 )( -42 61 60 )( 42 -61 60 )( -36 -61 43 )( 36 -61 -43 )( -36 61 -43 )( 36 61 43 )( -39 -62 -30 )( 39 62 -30 )( -39 62 30 )( 39 -62 30 )( -33 -63 -62 )( 33 63 -62 )( -33 63 62 )( 33 -63 62 )( -42 -64 -63 )( 42 64 -63 )( -42 64 63 )( 42 -64 63 )( -36 -64 -43 )( 36 -64 43 )( -36 64 43 )( 36 64 -43 )
Number of clauses: 254
Number of variables: 64
Execution time: 0.000332 seconds.
SAT
1 -2 3 -4 -5 -6 -7 -8 -9 -10 -11 -12 -13 -14 -15 -16 -17 -18 -19 -20 -21 -22 -23 -24 -25 -26 -27 -28 -29 -30 -31 32 -33 -34 35 36 -37 38 39 -40 41 42 43 44 45 -46 47 48 49 50 51 -52 53 54 55 56 57 -58 59 -60 61 62 63 -64 
 ```

## License

The source code of this repository (located in the [**src** folder](src)) is released under the terms of the [BSD 3-Clause License](LICENSE). Sample files included in the [**examples** folder](examples) were originally taken from [here](https://www.cs.ubc.ca/~hoos/SATLIB/benchm.html).

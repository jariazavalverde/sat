# Boolean satisfiability problem

## Compilation

```shell
gcc -I/usr/include -L/usr/lib main.c io.h io.c structures.c structures.h sat.h sat.c -o sat
```

## Usage

```shell
$ ./sat [options] file
```

* **-st** - display statistics.
* **-cdcl** - use the conflict-driven clause learning algorithm.
* **-apsvall** - use the Apsvall-Plass-Tarjan algorithm (for 2-CNF instances).

#### DIMACS CNF file format

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
**Reference:** [https://people.sc.fsu.edu/~jburkardt/data/cnf/cnf.html](https://people.sc.fsu.edu/~jburkardt/data/cnf/cnf.html)

#### Example

```shell
$ ./sat -st "sample/parity/par8-1-c.cnf"
```

```shell
sat
1 -2 3 -4 -5 -6 -7 -8 -9 -10 -11 -12 -13 -14 -15 -16 -17 -18 -19 -20 -21 -22 -23 -24 -25 -26 -27 -28 -29 -30 -31 32 -33 -34 35 36 -37 38 39 -40 41 42 43 44 45 -46 47 48 49 50 51 -52 53 54 55 56 57 -58 59 -60 61 62 63 -64 
(:decisions    3
 :propagations 61
 :conflicts    0
 :total-time   0.00)
 ```

## License

The source code of this repository (located in the [**src** folder](src)) is released under the terms of the [BSD 3-Clause License](LICENSE). Sample files included in the [**sample** folder](sample) were originally taken from [here](https://www.cs.ubc.ca/~hoos/SATLIB/benchm.html).

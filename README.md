# Boolean satisfiability problem
A simple SAT solver implemented in Haskell.

## Usage

The function `sat` takes an string representing a formula in CNF and returns an interpretation that satisfies it, if there exists.

```haskell
sat :: String -> Maybe Interpretation
```
A formula in CNF is a conjunction of one or more clauses, where a clause is a disjunction of literals. The string `xyz x'y y'z'` represents the formula `(x or y or z) and ((not x) or y) and ((not y) or (not z))`.

## Examples

```haskell
ghci> sat "xyz x'y y'z'"
Just [('x',False),('y',False),('z',True)]
```

```haskell
ghci> sat "x x'"
Nothing
```

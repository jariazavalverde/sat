module SAT(sat) where

import Data.List(nub)
import Data.Maybe(fromJust, isJust)

data Literal = Neg { getVariable :: Char } | Pos { getVariable :: Char } deriving (Show, Ord, Eq)
type Variable = Char
type Clause = [Literal]
type Formula = [Clause]
type Interpretation = [(Variable,Bool)]

-- Sign of a literal
sign :: Literal -> Bool
sign (Pos _) = True
sign (Neg _) = False

-- Complementary of a literal
complementary :: Literal -> Literal
complementary (Pos x) = Neg x
complementary (Neg x) = Pos x

-- String to formula in CNF
readCNF :: String -> Formula
readCNF = map f . words
    where f (x:'\'':xs) = Neg x : f xs
          f (x:xs) = Pos x : f xs
          f [] = []

-- Check satisfiability of a formula in CNF
sat :: String -> Maybe Interpretation
sat = (`dpll` []) . simplifyFormula . readCNF

-- DPLL algorithm
dpll :: Formula -> Interpretation -> Maybe Interpretation
dpll [] i = Just i
dpll ([]:_) _ = Nothing
dpll f@((x:y):z) i = let var = getVariable x
                         mi = dpll (applyFormula (var,True) f) ((var,True):i)
                      in if isJust mi then mi
                         else dpll (applyFormula (var,False) f) ((var,False):i)

-- Replace variable with a value in a formula
applyFormula :: (Variable,Bool) -> Formula -> Formula
applyFormula s = map fromJust . filter isJust . map (applyClause s [])

-- Replace variable with a value in a clause
applyClause :: (Variable,Bool) -> Clause -> Clause -> Maybe Clause
applyClause _ p [] = Just p
applyClause (x,b) f (l:ls) = if x == getVariable l
                             then if b == sign l then Nothing else Just (f ++ ls)
                             else applyClause (x,b) (l:f) ls

-- Simplify a formula
simplifyFormula :: Formula -> Formula
simplifyFormula = nub . filter (not . null) . map simplifyClause

-- Simplify a clause
simplifyClause :: Clause -> Clause
simplifyClause = f [] . nub
    where f p [] = p
          f p (l:ls) = if elem (complementary l) ls then [] else f (l:p) ls


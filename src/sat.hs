import Data.List(nub)
import Data.Maybe(fromJust)

data Literal = Pos Char | Neg Char deriving (Show, Eq)
type Variable = Char
type Clause = [Literal]
type Formula = [Clause]
type Interpretation = [(Variable,Bool)]

-- Get variable from a literal
getVariable :: Literal -> Variable
getVariable (Pos x) = x
getVariable (Neg x) = x

-- Get variables from a Formula
getVariables :: Formula -> [Variable]
getVariables = nub . map getVariable . concat

-- String to formula
readCNF :: String -> Formula
readCNF = map f . words
    where f (x:'\'':xs) = Neg x : f xs
          f (x:xs) = Pos x : f xs
          f [] = []

-- Check satisfiability of a formula in CNF
sat :: String -> Maybe Interpretation
sat p = dpll (getVariables f) f []
    where f = simplifyFormula $ readCNF p

-- DPLL algorithm
dpll :: [Variable] -> Formula -> Interpretation -> Maybe Interpretation
dpll _ [] sub = Just sub
dpll [] _ sub = Nothing
dpll (x:xs) p sub = let true = dpll xs (applyFormula (x,True) p) ((x,True):sub)
                    in if true /= Nothing then true
                       else dpll xs (applyFormula (x,False) p) ((x,False):sub)

-- Apply link to a formula
applyFormula :: (Variable,Bool) -> Formula -> Formula
applyFormula s = map fromJust . filter (/= Nothing) . map (applyClause s [])

-- Apply link to a clause
applyClause :: (Variable,Bool) -> Clause -> Clause -> Maybe Clause
applyClause _ p [] = Just p
applyClause (x,b) p (Pos l:ls) = if x == l then if b then Nothing else Just (p ++ ls) else applyClause (x,b) (Pos l:p) ls
applyClause (x,b) p (Neg l:ls) = if x == l then if b then Just (p ++ ls) else Nothing else applyClause (x,b) (Neg l:p) ls

-- Simplify a formula
simplifyFormula :: Formula -> Formula
simplifyFormula = nub . filter (/= []) . map simplifyClause

-- Simplify a clause
simplifyClause :: Clause -> Clause
simplifyClause = f [] . nub
    where f p [] = p
          f p (Pos x:ls) = if elem (Neg x) ls then [] else f (Pos x:p) ls
          f p (Neg x:ls) = if elem (Pos x) ls then [] else f (Neg x:p) ls




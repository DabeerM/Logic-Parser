# Logic-Parser
This program works out whether a formula is true in a graph under the given variable assignment.
It parses first order formulas in a language with no function symbols and one binary predicate
(or, and, implies = V, ^, >) A formula is defined by φ ::= X[ts] | − φ | (φ ◦ φ) | Ez φ | Az φ
The variable assignment is of the following order: Num_of_nodes , num_of_edges , edges (in pairs 
E.g. 1 2 forms an edge) and finally, the last 3 are node assignments for the nodes x,y&z.

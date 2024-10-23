# Concepts implemented:
## LEXICAL ANALYSIS:
* Counting tokens
* Grouping keywords, identifiers, literals, operators and delimiters

## OPERATIONS: 
* FIRST
* FOLLOW

## SYNTAX ANALYSIS:
1. RECURSIVE DESCENT PARSER (for an example grammar)
   Grammar:
1. S → E  
2. E → E + T | E - T | T  
3. T → T * F | T / F | F  
4. F → ( E ) | - F | + F | N | V  
5. V → x | y | z  
6. N → D | D N  
7. D → 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
   
2. LL(1) parser for any grammar

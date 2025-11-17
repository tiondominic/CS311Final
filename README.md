# CS311 – Theory of Computation Final Project  
## Regex-to-NFA Engine, Approximate Matching, and XML Validation using PDA  
Author: Kurt André Olaer  
Course: CS311 – Automata Theory and Formal Languages  
Date: November 2025  

---

# 1. Introduction

This project implements a unified automata simulator that covers:

- Regex to NFA conversion using Thompson’s Construction  
- Exact NFA simulation for string acceptance  
- NFA to Regular Grammar conversion  
- Approximate matching over regular languages  
- XML structural validation using a Pushdown Automaton (PDA)

The goal is to demonstrate the differences in expressive power between:

- Regular languages (NFA, regex)  
- Extended finite automata (approximate acceptance)  
- Context-free languages (PDA for XML)  

All features are accessed through a simple CLI menu written in C++.

---

# 2. System Architecture

```
MAIN MENU
 ├── 1. Regex to NFA (Exact Matching)
 │     ├── Tokenize regex
 │     ├── Insert explicit concatenation operators
 │     ├── Convert to postfix using shunting yard
 │     ├── Build NFA using Thompson’s construction
 │     ├── Exact NFA simulation
 │     └── NFA to Regular Grammar conversion
 │
 ├── 2. Approximate Matching
 │     ├── Construct NFA
 │     ├── BFS on (state, position, errors)
 │     └── Accept if final state reachable with <= k errors
 │
 └── 3. XML Validation (PDA)
       ├── Tokenize XML tags
       ├── Stack push for start tags
       ├── Stack pop and compare for end tags
       └── Accept if stack is empty at end
```

---

# 3. Regular Languages Module

## 3.1 Regex to NFA Construction

This system fully implements Thompson’s Construction, composed of:

1. Character class expansion  
   Example:  
   `[ACGT]` becomes `(A|C|G|T)`

2. Tokenization and concatenation insertion  
   Example:  
   `AB` becomes `A . B`  
   `A(B|C)` becomes `A . ( B | C )`

3. Shunting Yard Algorithm (infix to postfix)  
   Operator precedence:  
   - Highest: `*`  
   - Middle: `.`  
   - Lowest: `|`

4. Thompson fragment construction  
   - Literal transitions  
   - Concatenation  
   - Alternation  
   - Kleene star  
   - Parentheses  
   - Escapes

### Supported Regex Features

| Feature | Example | Supported |
|--------|---------|-----------|
| Literal characters | A, b, 3 | Yes |
| Concatenation | AB | Yes |
| Alternation | A\|B | Yes |
| Kleene Star | A* | Yes |
| Grouping | (AB|C) | Yes |
| Character classes | [A-Z], [ACGT] | Yes |
| Escapes | \*, \( | Yes |

---

## 3.2 Exact NFA Simulation

Uses epsilon-closure based simulation:

1. Start with epsilon-closure(startState)  
2. For each input character, move to next states  
3. Apply epsilon-closure again  
4. If any final state is active after input → ACCEPT  

---

## 3.3 NFA to Regular Grammar

Each NFA state is renamed S0, S1, S2, …

Transitions become productions:

Example:

```
S0 -> aS1
S1 -> bS2
S2 -> $
```

---

# 4. Approximate Matching Module

## 4.1 Purpose

This module demonstrates approximate acceptance over **regular languages**, not strict Levenshtein distance over a single string.

Allowed operations:

- Substitution  
- Insertion  
- Deletion  

A BFS processes triples: `(state, position, errorCount)`.

Acceptance occurs when:

```
position == input_length
and final_state is reachable
and errors_used <= k
```

---

## 4.2 Behavior Note

In approximate matching over regular languages, patterns that contain `*` or alternations may accept strings with fewer errors than strict edit distance would indicate.

This is correct for automata-theoretic approximate acceptance.

---

# 5. XML Validation (PDA)

XML nesting requires a pushdown automaton.

## PDA Rules

| Input | PDA Action |
|--------|---------------|
| `<tag>` | push "tag" |
| `</tag>` | pop and compare |
| End of input | accept if stack empty |

Failure conditions:

- Mismatched tags  
- Extra closing tags  
- Unclosed tags  

Example valid XML:

```
<a><b></b></a>
```

Example invalid XML:

```
<a><b></a></b>
```

---

# 6. Program Usage

Upon running the program:

```
=============================
         MAIN MENU
=============================
1. Regex to NFA (Exact Matching)
2. Approximate Matching
3. XML Validation using PDA
4. Exit
```

## Option 1: Regex → NFA

- Generates the NFA  
- Tests exact matching  
- Prints alphabet  
- Prints grammar productions  

## Option 2: Approximate Matching

- Tests input against regex with error tolerance `k`

## Option 3: XML Validation

- Reads multi-line XML  
- Prints stack operations  
- Reports validity  

---

# 7. Sample Inputs and Outputs

## 7.1 Exact Matching Example

Regex:
```
A(B|C)*D
```

Input:
```
ABBCD
```

Output:
```
ACCEPTED
```

---

## 7.2 Approximate Matching Example

Regex:
```
ATG[ACGT]*TGA
```

Input:
```
ATGACCTTAGA
```

k:
```
2
```

Output:
```
MATCHED WITH APPROXIMATION
```

---

## 7.3 XML Validation Example

Input:
```
<root>
<to>A</to>
</root>
```

Output:
```
XML structure is VALID.
```

---

# 8. Complexity Analysis

| Module | Time Complexity | Space Complexity |
|--------|-----------------|------------------|
| Regex to NFA | O(n) | O(n) |
| Exact Matching | O(n * |states|) | O(|states|) |
| Approx Matching | O(n * |states| * k) | O(n * |states| * k) |
| XML PDA | O(n) | O(n) |

---

# 9. Limitations

- Regex does not support `+`, `?`, `{m,n}`, or negated classes.  
- XML parser handles structure only (no attributes).  
- Approximate matching is over regular languages, not strict Levenshtein.  

---

# 10. Future Extensions

- Add regex features `+`, `?`, `{m,n}`  
- DFA minimization  
- NFA visualization  
- XML attribute parsing  
- Performance optimization  

---

# 11. Conclusion

This project successfully demonstrates:

- The construction and simulation of NFAs from regular expressions  
- Approximate acceptance over regular languages  
- Pushdown automaton behavior for XML validation  
- Clear distinction between regular and context-free languages  
- Clean and maintainable C++ architecture  

The system is academically aligned with CS311 requirements and presents a unified view of language recognition across multiple automata classes.

---

# End of Documentation

# Text Search Engine and Bioinformatics Project

**Course:** CS311 – Automata Theory and Formal Languages

**Academic Term:** December 2025

**Authors:** James Dominic Tion, Kurt Andre Olaer, Nathaniel Beatisula

---

## 1. Abstract

This project demonstrates the practical application of Automata Theory in the domains of Information Retrieval and Computational Biology. By implementing a suite of formal language processors—including a Regex-to-NFA engine, an Approximate String Matching algorithm, and a Pushdown Automaton (PDA) for structural validation—this system illustrates the distinctions in expressive power between Regular Languages and Context-Free Languages (CFLs). The software serves as a comparative simulator, visualizing how finite state memory handles pattern matching and where stack-based memory becomes necessary for hierarchical data (XML and RNA).

## 2. Theoretical Framework

The system is designed around the **Chomsky Hierarchy**, explicitly distinguishing between Type-3 (Regular) and Type-2 (Context-Free) languages.

### 2.1. Regular Languages (Type-3)

The core search engine treats user queries as regular expressions. These are compiled into **Non-Deterministic Finite Automata (NFA)**.

* **Formal Definition:** M = (Q, \Sigma, \delta, q_0, F)
* **Construction Algorithm:** Thompson’s Construction is used to transform regular expressions into an equivalent NFA with \epsilon-transitions.
* **Application:** Used for exact DNA pattern matching and keyword search.

### 2.2. Approximate Matching (Extended Finite Automata)

While standard NFAs recognize exact regular languages, biological sequence analysis often requires error tolerance. This module extends the NFA simulation to allow transition errors (substitutions, insertions, deletions) up to a threshold k. This demonstrates how finite automata can be adapted for fuzzy logic applications without leaving the regular language domain.

### 2.3. Context-Free Languages (Type-2)

Regular languages cannot model nested structures (e.g., matching an arbitrary number of open and close tags). To address this, the system implements a **Pushdown Automaton (PDA)**.

* **Formal Definition:** M = (Q, \Sigma, \Gamma, \delta, q_0, Z, F) where \Gamma represents the stack alphabet.
* **Application:** Used for validating XML document structure and RNA secondary folding (Watson-Crick base pairing), which essentially follows the Dyck language model of balanced parentheses.

---

## 3. System Architecture & Algorithms

The solution is implemented in **C++** and modularized into three distinct engines:

### 3.1. The Regex Engine (`regextonfa.cpp`)

* **Preprocessing:** Expands character classes (e.g., `[A-C]` \to `(A|B|C)`) and inserts explicit concatenation operators.
* **Compilation:** Parses the postfix regex using a shunting-yard approach and builds the NFA using Thompson’s method.
* **Simulation:** utilizing an \epsilon-closure algorithm to track multiple active states simultaneously, simulating non-determinism on deterministic hardware.
* **Grammar Conversion:** Capable of reverse-engineering the NFA into a **Right-Linear Regular Grammar** (e.g., A \to aB).

### 3.2. The Approximate Matcher (`approximate.cpp`)

* **Algorithm:** Performs a Breadth-First Search (BFS) over the state space tuple (state, index, error\_count).
* **Acceptance Condition:** A string is accepted if a final state is reachable when index = |input| and error\_count \le k.
* **Bioinformatics Use Case:** Identifies gene sequences that may have mutated but remain functionally similar to the search pattern.

### 3.3. The Structural Validator (`xmlvalidate.cpp`)

* **Mechanism:** Implements a deterministic PDA using the C++ `std::stack`.
* **XML Validation:** Pushes opening tags onto the stack and pops them upon encountering closing tags, ensuring strict LIFO (Last-In, First-Out) nesting.
* **RNA Folding:** Validates secondary structures by treating base pairs (A-U, C-G) as matching delimiters, analogous to checking balanced parentheses in compiler design.

---

## 4. Usage Guidelines

### 4.1. Compilation

The project utilizes a standard `Makefile` (or can be compiled directly via g++).

```bash
g++ main.cpp regextonfa.cpp converter.cpp approximate.cpp xmlvalidate.cpp -o automata_engine

```

### 4.2. Execution Modes

The Command Line Interface (CLI) offers four primary modes:

1. **Regex to NFA (Exact Matching):**
* Input a Regex (supports `*`, `|`, `()`, `[]`).
* Input a test string (or DNA sequence).
* *Output:* Transition diagram, Regular Grammar, and match indices.


2. **Approximate Matching:**
* Input a Regex and a Sequence.
* Input tolerance k (max errors).
* *Output:* Substrings matching the pattern within the specified edit distance.


3. **XML Validation (PDA):**
* Input a multiline XML string.
* *Output:* Stack trace and validity verdict.


4. **RNA Secondary Structure (PDA):**
* Input an RNA sequence (e.g., `A.CG.U`).
* *Output:* Verification of valid loop folding based on complementary base pairs.

---

## 5. Complexity Analysis

| Module | Algorithm | Time Complexity | Space Complexity |
| --- | --- | --- | --- |
| **Regex Compilation** | Thompson's Construction | $O( | r |
| **Exact Simulation** | NFA Step w/ Set | $O( | s |
| **Approximate Matching** | BFS State Expansion | $O( | s |
| **PDA Validation** | Single Pass Stack | $O( | s |

*Where |r| is regex length, |s| is string length, |Q| is number of states, and k is error tolerance.*

---

## 6. Limitations and Future Work

1. **DFA Minimization:** While the NFA architecture is robust, the implementation of DFA minimization (Hopcroft’s Algorithm) is currently a planned extension to optimize search speed for large inputs.
2. **Extended Regex Operators:** Operators such as `+` and `?` are currently handled via expansion or manual implementation; native support could optimize the graph size.
3. **XML Attributes:** The current PDA validates tag nesting structure but ignores internal tag attributes.

---

## 7. Conclusion

This project successfully implements a comparative automata simulator. It highlights that while Regular Languages (NFAs) are highly efficient for pattern searching and information retrieval in flat sequences (DNA), they fail to capture the hierarchical dependencies found in data structures (XML) and biological folding (RNA). Consequently, the integration of Pushdown Automata is shown to be strictly necessary for these Context-Free domains.

# Satellite Network Coding & Hierarchy Engine

A C implementation of a priority-queue-based tree generator and encoding engine, developed for the Data Structures and Algorithms coursework. The system models a satellite network using min-heaps, Huffman-like tree construction, and LCA (Lowest Common Ancestor) tree traversal algorithms for encoding, decoding, and relationship queries.

## Overview

The engine organizes satellite communication nodes based on their broadcast frequencies and identifier metadata. It utilizes a dynamically resized min-heap to construct a hierarchical binary prefix tree, enabling lossless data encoding, message decoding, level-order hierarchy printing, and common ancestor discovery.

## Key Data Structures

* **Satellite Node (`nod`)**:
  * Represents an individual network entity or an aggregated parent router.
  * Stores node metadata: identifier name, frequency weighting, left child, right child, and a reference pointer to its parent node (for bottom-up traversals).
* **Binary Min-Heap (`heap`)**:
  * Implements dynamic priority queue semantics with runtime reallocation.
  * Uses a customizable comparator supporting compound criteria (primary by frequency, secondary by alphanumeric name).

## Core Functionality

### 1. Heap Operations
* `initializare_heap`: Allocates memory for the heap container, registers the custom comparator, and initializes the dynamic array buffer.
* `insereaza_element`: Appends a node, resizes the underlying buffer when necessary, and applies up-heap bubbling to maintain priority invariants.
* `extract_heap`: Removes and returns the root element, swaps the last element to the root position, and applies down-heap bubbling.

### 2. Prefix Tree Construction
* `construieste_arbore`: Implements tree building by iteratively extracting the two lowest-priority nodes from the heap, resolving lexicographical tie-breakers, generating a combined parent node, linking child-parent references, and reinserting the parent back into the priority queue until a single root remains.

### 3. Encoding & Decoding
* `codificare`: Computes the unique prefix binary path for a given satellite. Performs a bottom-up traversal using parent pointers, records branch directions (left vs. right), and reverses the sequence to produce the root-to-leaf binary code.
* `decodificare`: Consumes a binary string stream to traverse the tree downward. Upon reaching target terminal leaves, the decoded satellite identifier is output and traversal resets to the root.

### 4. Tree Traversal & Analysis
* `afisare_arbore`: Conducts depth computation across the topology and outputs nodes ordered by tree depth level.
* `gaseste_satelit`: Performs an exhaustive lookup to locate a satellite node by name.
* `parinte_comun`: Resolves the Lowest Common Ancestor (LCA) for two satellite nodes by leveling unequal node depths and stepping upward synchronously.
* `free_arbore`: Recursively deallocates all heap-allocated tree nodes and buffers to ensure zero memory leaks.

## Compilation and Execution

### Build

Compile using standard C99 flags:

```bash
gcc -Wall -Wextra -std=c99 main.c -o satellite_engine
```

### Run

```bash
./satellite_engine
```

Or redirect inputs from a test vector:

```bash
./satellite_engine < input.txt
```

## Author

* Miruna Enache (311CC)
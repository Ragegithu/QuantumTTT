# QuantumTTT

A qubit simulator built on a tic-tac-toe board. Each cell on the board is a qubit with real complex amplitudes, gate operations, and probabilistic collapse. Not a gimmick — the physics is actually correct.

Built with C++ and SFML 3.


## What it is

Each of the 9 cells holds a qubit in superposition. You manipulate qubits using quantum gates via a command line interface, then measure them to collapse their state to 0 or 1. The probabilities are real — amplitudes are stored as complex numbers, gates are matrix multiplications, and measurement samples from the actual probability distribution.


## Commands

```
select X Y      select a qubit at position X Y (1-indexed)
hadamard        apply Hadamard gate — puts qubit into 50/50 superposition
rotateY <deg>   apply Ry gate — shifts probability toward 0 or 1
rotateZ <deg>   apply Rz gate — rotates phase without changing probability
measure         collapse the qubit — locks it permanently
exit            close
```


## How the physics works

A qubit has two complex amplitudes — alpha for |0⟩ and beta for |1⟩. The probability of measuring 0 or 1 is the magnitude squared of each amplitude. Gates are unitary matrices applied to the state vector.

**Ry** directly shifts probabilities. Ry(180°) on |0⟩ gives 100% chance of measuring 1.

**Rz** rotates phase only — probabilities don't visibly change. But it changes how subsequent Ry gates behave, making it a trap/setup move.

**Hadamard** puts the qubit on the equator of the Bloch sphere — perfect 50/50. Applied twice it returns to the original state.

Once measured a qubit is locked. No further gates can be applied.

## Building

Requires CMake 3.16+ and GCC (MSYS2 UCRT64 on Windows). SFML 3 is fetched automatically.


# Windows (MSYS2)
```bash
mkdir build && cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
cmake --build .
```
# Linux
```bash
mkdir build && cd build
cmake ..
cmake --build .
```
## Why
Started as Quantum Tic-Tac-Toe, ended up as a sandbox for learning how qubits actually work. The gate math is implemented from scratch — no quantum libraries, just complex numbers and matrix multiplication

Started as Quantum Tic-Tac-Toe, ended up as a sandbox for learning how qubits actually work. The gate math is implemented from scratch — no quantum libraries, just complex numbers and matrix multiplication.

The plan is to strip the game out entirely and turn this into a proper n-qubit playground with entanglement support.

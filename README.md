# The Boys — Discrete Event Simulation Project

## Overview

The Boys is a discrete event simulation project developed in C, inspired by the TV series *The Boys*. The project simulates a world containing heroes, bases, and missions over the course of one simulated year.

Heroes travel between bases, wait in queues, form teams, complete missions, gain experience, and may even die during dangerous missions. The entire system evolves dynamically through scheduled events processed in chronological order using a Future Event List (priority queue).

The project was developed for the Computer Science course at :contentReference[oaicite:0]{index=0}.

---

# Main Concepts

## Heroes
Each hero has:

- Unique ID
- Skill set
- Patience level
- Travel speed
- Experience points
- Current base
- Alive/dead status

Heroes continuously move between bases and participate in missions.

---

## Bases
Bases are distributed throughout the simulated world and contain:

- Maximum capacity
- Waiting queue
- Heroes currently present
- Cartesian coordinates

Each base manages hero admission through a gatekeeper system.

---

## Missions
Missions appear randomly throughout the simulation and require specific skill combinations.

A mission can only be completed by a base whose heroes collectively possess all required skills. The closest capable base is selected to complete the mission.

Heroes participating in missions gain experience and may die depending on the mission danger level.

---

# Simulation Model

The project uses a **Discrete Event Simulation (DES)** model.

The simulation is driven by events processed in chronological order using a priority queue called the **Future Event List (FEL)**.

Implemented events include:

- `CHEGA` (arrival)
- `ESPERA` (wait in queue)
- `DESISTE` (give up)
- `AVISA` (gatekeeper notification)
- `ENTRA` (enter base)
- `SAI` (leave base)
- `VIAJA` (travel)
- `MISSAO` (mission)
- `MORRE` (death)
- `FIM` (end of simulation)

Each event may modify the simulation state and schedule new future events.

---

# Technical Features

- Developed entirely in C
- Event-driven architecture
- Priority queue implementation for event scheduling
- Dynamic simulation over 525,600 simulated minutes
- Randomized world generation
- Queue and set Abstract Data Types (ADTs)
- Extensive console logging for debugging and analysis
- Memory-safe implementation tested with Valgrind

---

# Skills and Concepts Practiced

This project provided practical experience with:

- Discrete event simulation
- Data structures
- Priority queues
- Queue management systems
- Modular programming in C
- Dynamic memory management
- Debugging large-scale systems
- Simulation modeling
- Event scheduling algorithms

---

# Development Experience

One of the biggest challenges of the project was designing the event system and ensuring that all interactions between heroes, bases, and missions behaved consistently over long simulation periods.

The project also required extensive debugging and testing due to the large number of generated events and the complexity of the simulation dynamics.

Through this project, I significantly improved my understanding of low-level programming, event-driven systems, and large-scale software organization in C.

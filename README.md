<div align="center">
  <h1>philosophers</h1>
  <p>A simulation of the dining philosophers problem as part of the 42 curriculum. This project explores concurrency, synchronization, and deadlock prevention techniques in C.</p>
</div>

# About the project

The dining philosophers problem is a classic synchronization problem. The challenge is to design a solution where philosophers can alternately think and eat without running into a deadlock situation, where no philosopher can eat, or a starvation situation, where a philosopher is perpetually unable to eat.

This project implements this problem in C, using threads and mutexes to handle the synchronization.

# Getting started

Start by cloning the repository:
```c
git clone https://github.com/fhuisman/minishell philosophers
```
Go into the philosophers directory and compile by using make:
```c
cd philosophers
make
```

# Usage

To run the simulation, use the following command:

```c
./philosophers [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat (optional)]
```

Example:
```c
./philosophers 5 800 200 200
```
This example starts a simulation with 5 philosophers, where each philosopher has:

    800ms before they die without eating,
    200ms to eat,
    200ms to sleep.

Optional argument specifies how many times each philosopher must eat before the simulation ends. If not provided, the simulation runs indefinitely.

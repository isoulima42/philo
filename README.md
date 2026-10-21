*This project has been created as part of the 42 curriculum by isoulima.*

# Philosophers

<img src="score.png" width="150" alt="Score 100/100">

![Language](https://img.shields.io/badge/Language-C-blue)
![Norminette](https://img.shields.io/badge/Norminette-OK-brightgreen)

## Description

Philosophers is a simulation of the classic dining philosophers problem.
A number of philosophers sit around a round table, with one fork between each pair of neighbours.
To eat, a philosopher must hold both the fork on their left and the fork on their right. They then sleep and think, and the simulation stops as soon as one of them starves.
Each philosopher is a separate thread and each fork is protected by a mutex.
The project covers the basics of multithreading: creating and joining threads, protecting shared data with mutexes, and preventing deadlocks and data races.

---

## Instructions

### Compilation

```bash
make
```

This produces the `philo` executable.

```bash
make clean    # removes the .o files
make fclean   # removes the .o files and philo
make re       # rebuilds everything
```

### Usage

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

All times are in milliseconds. Every argument must be a strictly positive integer that fits in an `int`. Otherwise, the program prints an error on stderr and exits with status `1`.

```bash
./philo 1 800 200 200      # a single philosopher has one fork only: dies at 800 ms
./philo 5 800 200 200      # nobody dies
./philo 5 800 200 200 7    # stops once every philosopher has eaten at least 7 times
./philo 4 410 200 200      # nobody dies
./philo 4 310 200 100      # one philosopher dies
```

Sample output:

```
0 1 has taken a fork
0 1 has taken a fork
0 1 is eating
0 4 is thinking
1 2 is thinking
1 3 has taken a fork
1 3 has taken a fork
1 3 is eating
201 3 is sleeping
201 1 is sleeping
201 5 has taken a fork
```

---

## Features

| Message | Meaning |
|---------|---------|
| `X has taken a fork` | philosopher `X` locked one of their forks |
| `X is eating` | `X` holds both forks; their last meal time is reset |
| `X is sleeping` | `X` put both forks down and sleeps for `time_to_sleep` |
| `X is thinking` | `X` waits before taking the forks again |
| `X died` | `X` did not start eating within `time_to_die` ms |

- One thread per philosopher, one mutex per fork
- Optional meal limit: the simulation stops once every philosopher has eaten at least the requested number of times
- Death message printed less than 10 ms after the actual death (1 ms measured in tests)
- No message printed after a death, and messages never overlap
- Single-philosopher case handled: they only have one fork and die

### Internal functions

| File | Functions |
|------|-----------|
| `main.c` | `cleanup_rules`, `join_philos`, `start_simulation`, `main` |
| `parse.c` | `ft_strlen`, `print_error`, `parse_number`, `parse_args` |
| `init.c` | `destroy_forks`, `init_forks`, `set_forks`, `init_philos`, `init_rules` |
| `time.c` | `get_time_us`, `get_time`, `ft_usleep`, `print_status` |
| `monitor.c` | `is_stopped`, `set_stop`, `philo_died`, `all_full`, `monitor` |
| `routine.c` | `lonely_philo`, `eat`, `think`, `philo_routine` |

---

## Technical choices

**Structures**: `t_rules` holds the shared simulation state (parameters, start time, `stop` flag, `stop_lock` and `print_lock` mutexes, array of forks). `t_philo` holds the state of each philosopher (`id`, `meals`, `last_meal`, `meal_lock`, pointers to their two forks).

**Deadlock prevention**: each philosopher always locks the lower-numbered fork first (`set_forks` in `init.c`). Since every philosopher takes the forks in the same global order, no circular wait can form, so a deadlock is impossible.

**Fairness**: even-numbered philosophers start by thinking, so two neighbours do not fight over the same fork at startup. After each meal, `think` waits for half of the time left before death, minus `time_to_eat` (capped at `MAX_THINK`). A philosopher who has just eaten therefore waits, and their hungrier neighbour gets the forks first. This is what keeps tight cases such as `5 610 200 200` or `199 610 200 200` alive.

**Death detection**: the main thread runs `monitor`. Every 0.5 ms, it reads each philosopher's `last_meal` under that philosopher's `meal_lock`. The philosopher updates `last_meal` under the same mutex, so the monitor can never check for a death at the exact moment the philosopher starts eating. As soon as the elapsed time exceeds `time_to_die`, the monitor takes `print_lock`, sets the `stop` flag and prints `died`. `print_status` checks `stop` while holding `print_lock`, so nothing is printed after a death.

**Precise sleeping**: `usleep` alone can oversleep by several milliseconds. `ft_usleep` therefore sleeps in 500 µs steps and checks a microsecond clock between steps. It also returns as soon as the simulation stops, so the threads end quickly.

**Constraints**: no global variables. All shared state goes through a single `t_rules` passed by pointer, and every shared field (`stop`, `last_meal`, `meals`) is only read or written under a mutex. All mutexes are destroyed and all memory is freed before exiting, including when initialisation or thread creation fails.

**Checks**: no data race reported by `valgrind --tool=helgrind`, `valgrind --tool=drd` or `-fsanitize=thread`. No leak reported by `valgrind --leak-check=full --show-leak-kinds=all`.

---

## Resources

- `man pthread_create`
- `man pthread_join`
- `man pthread_mutex_init`
- `man pthread_mutex_lock`
- `man gettimeofday`
- `man usleep`
- Dining philosophers problem (Wikipedia)
- POSIX Threads Programming (LLNL)
- Valgrind manual: Helgrind and DRD
- Official 42 subject

---

## AI usage

AI was used to :

- write this README
- understand some concepts


---

## Author

42 Lausanne
Login: isoulima

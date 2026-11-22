# Homework 9 · Threads basics

Run `make` on Linux, then run the programs separately.

| Command | What to check |
| --- | --- |
| `./basic_threads` | Three distinct workers print their pthread IDs. The main thread prints its final message after all joins. |
| `./array_sum` | Workers sum the two halves of the array and print 10 and 26. |
| `./print_numbers` | Each worker prints 1 through 5 in order. The relative order of different workers can vary. |
| `./parallel_squares` | Five workers print the squares 1, 4, 9, 16, and 25. |

Each worker receives its own stable argument in an array that stays alive until all joins have completed. The numeric display of `pthread_t` assumes Linux.

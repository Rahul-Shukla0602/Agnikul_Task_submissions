# Test Matrix for Multi-Process System

| Test Case | Description                        | Expected Result                                           | Actual Result | Pass/Fail |
|-----------|------------------------------------|-----------------------------------------------------------|---------------|-----------|
| TC1       | Start main process                 | Main process starts and creates P2, P3, and P4                   |               |           |
| TC2       | Task 1 (Increment) for P2          | P2 increments variable from 0 to 100 using 2 threads      |               |           |
| TC3       | Task 1 (Increment) for P3          | P3 increments variable from 0 to 100 using 2 threads      |               |           |
| TC4       | Task 2 (Message Queue) for P2      | P2 creates message queue, sends and receives message      |               |           |
| TC5       | Task 2 (Message Queue) for P3      | P3 creates message queue, sends and receives message      |               |           |
| TC6       | Task 3 (Base Conversion) for P2    | P2 converts base-4 input to decimal                       |               |           |
| TC7       | Task 3 (Base Conversion) for P3    | P3 converts base-4 input to decimal                       |               |           |
| TC8       | Verifier (P4) functionality        | P4 verifies and acknowledges task completion              |               |           |
| TC9       | Main process task switching        | P1 switches between P2 and P3 for task execution          |               |           |
| TC10      | Graceful termination               | System terminates gracefully on SIGINT                    |               |           |

## Test Execution Instructions

1. Compile the project using `make`.
2. Run `./main` and observe the output.
3. For **TC6** and **TC7**, enter base-4 numbers when prompted.
4. For **TC10**, use `Ctrl+C` to send SIGINT and observe graceful termination.

Fill in the "Actual Result" and "Pass/Fail" columns based on the observed behavior.

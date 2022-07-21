https://www.topcoder.com/challenges/81d26476-fb3c-4e4e-9ffe-7ee17c0438ab?tab=details


# Approach
## Overview
I solved this problem using simulated annealing.

Initial Solution
- A loop using only the edge cells
- Dice faces are: `V, V-1, V, V-2, V-1, max(V-3,2)`

Transition
- T1. Change a face value / swap face values of the dice (`0.1~1%`)
- T2. Shift start cell forward or backward by 1 (`1%`)
- T3. Remove part of the loop and reconnect it with randomlized DFS (`all the rest`)

## Things that work
- Change evaluation function from actual score
  - T1: `actual_score + expect_score * 0.5` where `expect_score` is  `(num. of cells whose filled number equal v) * v` (`v` is a face value of the dice)
  - T3: `actual_score + (num. of unused cells) * 0.5`

- In T3, allow transisions only when `(x, y, dice_pattern)` of source/target cells are same before/after the change
  - `dice_pattern`: Diffferent dice placements are considered different patterns.  (24 patterns)
  - It avoids drastically change score and were possible to evaluate faster
  - Exception: If the loop is `perfect` or the target cell equals goal cell, allow transisions if `dice_pattern` of the target cell differs before/after the change

- Speed-up
  - Calculate the differential score
  - Precomputing (e.g., transisions between dice placements)
  - As a result, the number of transisions of simmulated annealing is `~10M`

# Example Results
```
#1: 55.4
#2: 2003.5
#3: 1411.5
#4: 297.1
#5: 82.3
#6: 330.6
#7: 427.7
#8: 1542.7
#9: 1214.2
#10: 1498.3
```

# Links
- final submit: https://github.com/sash2104/mm138/blob/59b3af4a706b18f6ced50b0feefb2a6fef945796/src/main.cpp
- replay: https://twitter.com/sash277/status/1549810050319159296

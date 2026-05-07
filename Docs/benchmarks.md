// first
[PROFILER].forward : 0.0337 ms
[PROFILER].backward : 0.039 ms
[PROFILER].step : 0.07 ms
[PROFILER].Epoch 0 : 0.8964 ms

// step optimization

[PROFILER].forward : 0.033 ms
[PROFILER].backward : 0.0287 ms
[PROFILER].step : 0.0396 ms
[PROFILER].Epoch 0 : 0.8276 ms

// softmax

[PROFILER].Epoch 0 : 397.894 ms
[PROFILER].Epoch 1 : 399.389 ms
[PROFILER].Epoch 2 : 400.218 ms
[PROFILER].Epoch 3 : 383.795 ms
[PROFILER].Epoch 4 : 381.697 ms
[PROFILER].Epoch 5 : 383.606 ms
[PROFILER].Epoch 6 : 392.635 ms
[PROFILER].Epoch 7 : 386.296 ms
[PROFILER].Epoch 8 : 384.631 ms
[PROFILER].Epoch 9 : 398.598 ms
Accuracy : 0.9226

// reduced steps count & less learning_rate after 5th epoch

[PROFILER].Epoch 0 : 328.4 ms
[PROFILER].Epoch 1 : 330.159 ms
[PROFILER].Epoch 2 : 325.664 ms
[PROFILER].Epoch 3 : 325.383 ms
[PROFILER].Epoch 4 : 320.52 ms
[PROFILER].Epoch 5 : 311.736 ms
[PROFILER].Epoch 6 : 318.912 ms
[PROFILER].Epoch 7 : 319.199 ms
[PROFILER].Epoch 8 : 321.486 ms
[PROFILER].Epoch 9 : 325.062 ms
Accuracy : 0.9254

// 128 neurons

[PROFILER].Epoch 0 : 4597.4 ms
[PROFILER].Epoch 1 : 4560.44 ms
[PROFILER].Epoch 2 : 4582.15 ms
[PROFILER].Epoch 3 : 4568.57 ms
[PROFILER].Epoch 4 : 4596.2 ms
[PROFILER].Epoch 5 : 4571.09 ms
[PROFILER].Epoch 6 : 4569.16 ms
[PROFILER].Epoch 7 : 4584.83 ms
[PROFILER].Epoch 8 : 4570.46 ms
[PROFILER].Epoch 9 : 4581.62 ms
Accuracy : 0.9645

// enabled OpenMP

[PROFILER].Epoch 0 : 8363.96 ms
[PROFILER].Epoch 1 : 6630.58 ms
[PROFILER].Epoch 2 : 7360.77 ms
...

// tried OpenMP on epoches. Waited seven seconds only once

[PROFILER].Epoch 7 : 7266.83 ms
[PROFILER].Epoch 8 : 7332.91 ms
[PROFILER].Epoch 9 : 7413.47 ms
[PROFILER].Epoch 3 : 7478.25 ms
[PROFILER].Epoch 1 : 7496.26 ms
[PROFILER].Epoch 5 : 7507.61 ms
[PROFILER].Epoch 6 : 7556.86 ms
[PROFILER].Epoch 0 : 7571.97 ms
[PROFILER].Epoch 4 : 7614.85 ms
[PROFILER].Epoch 2 : 7651.5 ms
Accuracy : 0.365

// tried OpenMP on images

[PROFILER].Epoch 0 : 548.923 ms
[PROFILER].Epoch 1 : 533.514 ms
[PROFILER].Epoch 2 : 534.924 ms
[PROFILER].Epoch 3 : 541.85 ms
[PROFILER].Epoch 4 : 534.53 ms
[PROFILER].Epoch 5 : 537.035 ms
[PROFILER].Epoch 6 : 538.391 ms
[PROFILER].Epoch 7 : 534.408 ms
[PROFILER].Epoch 8 : 534.119 ms
[PROFILER].Epoch 9 : 538.638 ms
Accuracy : 0.2077

---

[PROFILER].Epoch 0 : 530.717 ms
[PROFILER].Epoch 1 : 532.224 ms
[PROFILER].Epoch 2 : 528.154 ms
[PROFILER].Epoch 3 : 530.406 ms
[PROFILER].Epoch 4 : 530.802 ms
[PROFILER].Epoch 5 : 536.721 ms
[PROFILER].Epoch 6 : 531.48 ms
[PROFILER].Epoch 7 : 534.917 ms
[PROFILER].Epoch 8 : 539.939 ms
[PROFILER].Epoch 9 : 530.924 ms
[PROFILER].Epoch 10 : 532.798 ms
[PROFILER].Epoch 11 : 532.245 ms
[PROFILER].Epoch 12 : 528.907 ms
[PROFILER].Epoch 13 : 529.433 ms
[PROFILER].Epoch 14 : 533.315 ms
[PROFILER].Epoch 15 : 531.954 ms
[PROFILER].Epoch 16 : 531.318 ms
[PROFILER].Epoch 17 : 533.896 ms
[PROFILER].Epoch 18 : 539.929 ms
[PROFILER].Epoch 19 : 529.473 ms
[PROFILER].Epoch 20 : 537.826 ms
[PROFILER].Epoch 21 : 531.102 ms
[PROFILER].Epoch 22 : 530.027 ms
[PROFILER].Epoch 23 : 533.928 ms
[PROFILER].Epoch 24 : 529.642 ms
[PROFILER].Epoch 25 : 538.383 ms
[PROFILER].Epoch 26 : 529.776 ms
[PROFILER].Epoch 27 : 528.783 ms
[PROFILER].Epoch 28 : 531.107 ms
[PROFILER].Epoch 29 : 530.453 ms
[PROFILER].Epoch 30 : 529.389 ms
[PROFILER].Epoch 31 : 536.16 ms
[PROFILER].Epoch 32 : 529.947 ms
[PROFILER].Epoch 33 : 529.602 ms
[PROFILER].Epoch 34 : 536.347 ms
[PROFILER].Epoch 35 : 531.097 ms
[PROFILER].Epoch 36 : 531.125 ms
[PROFILER].Epoch 37 : 548.853 ms
[PROFILER].Epoch 38 : 531.499 ms
[PROFILER].Epoch 39 : 531.599 ms
[PROFILER].Epoch 40 : 534.847 ms
[PROFILER].Epoch 41 : 529.568 ms
[PROFILER].Epoch 42 : 532.295 ms
[PROFILER].Epoch 43 : 533.593 ms
[PROFILER].Epoch 44 : 530.701 ms
[PROFILER].Epoch 45 : 531.533 ms
[PROFILER].Epoch 46 : 533.554 ms
[PROFILER].Epoch 47 : 530.039 ms
[PROFILER].Epoch 48 : 531.604 ms
[PROFILER].Epoch 49 : 532.499 ms
Accuracy : 0.2039

// 128 neurons, 4 layers, 10 epochs with new random

[PROFILER].Epoch 0 : 5634.03 ms
[PROFILER].Epoch 1 : 5881.77 ms
[PROFILER].Epoch 2 : 10600.9 ms
[PROFILER].Epoch 3 : 9419.92 ms
[PROFILER].Epoch 4 : 10306.3 ms
[PROFILER].Epoch 5 : 5454.95 ms
[PROFILER].Epoch 6 : 5593.5 ms
[PROFILER].Epoch 7 : 5521.26 ms
[PROFILER].Epoch 8 : 5475.02 ms
[PROFILER].Epoch 9 : 5446.15 ms
Accuracy : 0.1135

// 10, 30 epoches, used shuffle and smart learning_rate

[PROFILER].Epoch 0 : 309.429 ms
[PROFILER].Epoch 1 : 281.483 ms
[PROFILER].Epoch 2 : 282.491 ms
[PROFILER].Epoch 3 : 273.805 ms
[PROFILER].Epoch 4 : 258.774 ms
[PROFILER].Epoch 5 : 269.797 ms
[PROFILER].Epoch 6 : 270.787 ms
[PROFILER].Epoch 7 : 264.797 ms
[PROFILER].Epoch 8 : 263.859 ms
[PROFILER].Epoch 9 : 259.511 ms
[PROFILER].Epoch 10 : 255.806 ms
[PROFILER].Epoch 11 : 267.851 ms
[PROFILER].Epoch 12 : 264.724 ms
[PROFILER].Epoch 13 : 260.358 ms
[PROFILER].Epoch 14 : 261.328 ms
[PROFILER].Epoch 15 : 260.997 ms
[PROFILER].Epoch 16 : 262.935 ms
[PROFILER].Epoch 17 : 263.987 ms
[PROFILER].Epoch 18 : 263.242 ms
[PROFILER].Epoch 19 : 263.2 ms
[PROFILER].Epoch 20 : 274.933 ms
[PROFILER].Epoch 21 : 272.03 ms
[PROFILER].Epoch 22 : 277.465 ms
[PROFILER].Epoch 23 : 275.344 ms
[PROFILER].Epoch 24 : 275.894 ms
[PROFILER].Epoch 25 : 271.53 ms
[PROFILER].Epoch 26 : 262.217 ms
[PROFILER].Epoch 27 : 258.294 ms
[PROFILER].Epoch 28 : 268.811 ms
[PROFILER].Epoch 29 : 276.272 ms
Accuracy : 0.9459
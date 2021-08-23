# C++ implementation of Kalman Filter for Object Tracking

Two algorithms for object tracking based on Kalman Filter [1,2] are implemented using OpenCV C++ library [3]. First, a Constant Velocity Model [4], and second an Acceleration Model. The strengths and weaknesses of both models are discussed using toy and real video sequences. Each tracking experiment can be divided into 3 steps, (1) foreground mask is generated based on Background Subtraction, morphological opening is applied to filter noise from the background mask. (2) the main Blobs are extracted from the filtered mask using Connected Component Analysis (CCA). From the extracted blobs, the center of the biggest blob is used as input measurement to the Kalman filter. (3) with the main Blob as the input for Kalman Filter, Constant Velocity and Acceleration models for Kalman filter are implemented.



#### Toy series
---
<img src="./imgs/ToyVideo1.png" alt="drawing" width="300"/>
<img src="./imgs/324.png" alt="drawing" width="300"/>
<img src="./imgs/AM3.png" alt="drawing" width="300"/>


#### Real series
---
<img src="./imgs/3315.png" alt="drawing" width="300"/>
<img src="./imgs/3324.png" alt="drawing" width="300"/>
<img src="./imgs/3334.png" alt="drawing" width="300"/>
<img src="./imgs/RealVideo4a.png" alt="drawing" width="800"/>



## Cite this work
    J. Rico, M. Eyakep, (2020) Kalman Filter for object tracking
    [Source code](https://github.com/jvirico/KalmanTracker)

#### Authors
    J. Rico (jvirico@gmail.com)
    M. Eyakem (meazaeyakem1@gmail.com)
## References
[1] - [J. Rico, M. Eyakem. (2020) Kalman Filter for object tracking](./Kalman_Filter_JRico_report.pdf).\
[2] - Kalman, R.E.: A new approach to linear filtering and prediction problems (1960).\
[3] - Kaehler, A., Bradski, G.: Learning OpenCV 3: computer vision in C++ with the
OpenCV library. ” O’Reilly Media, Inc.” (2016).\
[4] - Sch¨oller, C., Aravantinos, V., Lay, F., Knoll, A.: What the constant velocity model
can teach us about pedestrian motion prediction. IEEE Robotics and Automation
Letters 5(2), 1696–1703 (2020).
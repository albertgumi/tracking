# Questions:

# where is the list of delta Z values?
# our SENSOR_RESOLUTION for the R matrix?
# the next point to be considered in the KF calculation is the extrapolated?
# What is the I matrix?
# What are the initial values used for the P matrix?
# Is the covariance considered in R the one used to see if the new point can be accepted in the track?
# Which is the P matrix in a parallel environment?
# Parallel implementation why 3?
# Forward, backward extrapolation
# What happens if there is a error to higher values and another to lowe values and it breaks the track?

function [x, P] =  kf(x1,y1,x2,y2, x_old, P_old)

    dt = 3;
    F = [1,0,dt,0;0,1,0,dt ;0,0,1,0; 0,0,0,1];
    H = [1,0,0,0;0,1,0,0];
    R = [1,0;0,1];      
    I = [1,0,0,0;0,1,0,0;0,0,1,0;0,0,0,1];  # Has the identity matrix sense?


    # 1. PREDICT
    # Predict (a priori) state estimate
    x_k_1 = x_old;
    x_k = F * x_k_1;
    
    # Predict (a priori) estimate covariance
    P_k_1 = P_old;
    P_k = F * P_k_1 * transpose(F);
    
    
    # 2. UPDATE
    # Innovation or measurement residual
    z = [x2;y2];
    y = z - H * x_k;
    
    # Innovation (or residual) covariance
    S = H * P_k * transpose(H) + R;
    
    # Optimal Kalman gain
    K = P_k * transpose(H) * inverse(S);
    
    # Updated (a posterior) state estimate
    x = x_k + K * y;
    
    # Updated (a posterior) estimate covariance
    P = (I - K * H) * P_k;
    
endfunction

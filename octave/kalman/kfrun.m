i = 2;          # Initial index

P = [3,0,0,0;
     0,3,0,0;
     0,0,3,0;
     0,0,0,3];  #Initial covariance matrix
     
a_x = 0.03;     # Initial slope

x = [1;1;
     a_x;
     a_x];      # initial track (state) matrix

l_x = [x(1)];         # Create the list of elements with X
l_y = [x(2)];         # Create the list of elements with Y

# First iteration
[x, P] = kf(1,1,2,2,x,P);
P
l_x(end+1) = x(1);    # Append x in the list
l_y(end+1) = x(2);    # Append y in the list

r_p = 0;              # Previous random number
real = [0,0];

# Consider an increment in (x,z) and (y,z) of (1,3)
while i <= 50
   # Introduce random variance
   r = (rand(1)-0.5) * 20;
   
   real(end+1) = i+r;
   
   # KF calculation
   [x, P] = kf(i + r,i + r,i+1 + r_p,i+1+r_p,x,P);
   r_p = r;
   
   # Print values
   i
   x
   P
   
   # Append x in the list
   l_x(end+1) = x(1); 
   
   # Append y in the list    
   l_y(end+1) = x(2);     
   i = i+1;

endwhile

l = 1:3:153;
plot(l,l_x)

title ("Kalman filter for a dX=1 dZ=3");
xlabel ("Z value");
ylabel ("X value");
     
hold on
plot(l, 1:1:51, 'color', 'red')
hold on
plot(l, real, 'color', 'green', 'o')
hold on

# Calculate te line equation
x
m = x(3);
z = 153;
b = x(1) - m*z;
line = m*l+b;
plot(l, line, 'color', 'black')
legend ("blue: kalman","red: reference", "green: real","black: line equation");
pause

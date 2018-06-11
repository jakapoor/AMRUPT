% Investigate array spacing > pi/4
dw = 10/4;
theta = [-pi/4 : pi/50 : pi/4];
cp = 2*pi*dw;
psi = cp*sin(theta);
theta_hat = asin(psi/cp);
plot(theta, theta_hat);


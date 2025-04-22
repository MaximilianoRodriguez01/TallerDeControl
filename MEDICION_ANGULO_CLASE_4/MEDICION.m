 
y_n_mas_2 = out.d1(3:(end));
y_n_mas_1 = out.d1(2:(end-1));
y_n = out.d1(1:end-2);
u_n = out.d2(1:(end-2));

X = [y_n_mas_1, y_n, u_n];

alpha = inv(X.' * X) * (X' * y_n_mas_2);

% alpha = [1.3920; -0.4710; -0.0296]

figure();
plot(u_n);















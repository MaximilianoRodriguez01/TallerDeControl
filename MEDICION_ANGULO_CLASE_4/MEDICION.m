y_n_mas_2 = out.d1(3:(end));
y_n_mas_1 = out.d1(2:(end-1));
y_n = out.d1(1:end-2);
x_n = out.d2(1:(end -2));

X = [y_n_mas_1, y_n, x_n];

alpha = inv(X.' * X) * (X' * y_n_mas_2);

% alpha:
% corresponden a y_n+1, y_n, x_n

% 1.4587822
% -0.51130295
% 0.022384644

% y_n+2 = y_n+1 * 1.458 - y_n * 0.511 + x_n * 0.0223

% Y(Z) * Z^-2 = 1.458 * Y(Z) * Z^-1 - 0.511 * Y(Z) + X_N * 0.223

% Y(Z)*(Z-2 - 1.458 Z-1 + 0.511) = X(Z) * 0.223

% Y(Z) / X(Z) = 0.223 / (Z-2 - 1.458 Z-1 + 0.511)



Ts = 0.020; % tiempo de muestreo

num = 0.0223;        % coeficientes del numerador (z^0, z^-1)
den = [0.511, -1.4857, 1];      % coeficientes del denominador (z^0, z^-1, z^-2)

H_discreto = tf(num, den, Ts);

H_continuo = d2c(H_discreto, "tustin");

s = tf('s');

H_continuo = tf(74.42, [1, -32.64, 84.43]);

figure();
% step(H_continuo);
step(H_continuo);
bode(H_continuo);
grid on
















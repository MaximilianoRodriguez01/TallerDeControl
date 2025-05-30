close all

    optionss=bodeoptions;
    optionss.MagVisible='on';
    optionss.PhaseMatching='on';
    optionss.PhaseMatchingValue=-180;
    optionss.PhaseMatchingFreq=1;
    optionss.Grid='on';

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

% y_n+2 = y_n+1 * alpha1 - y_n * |alpha2| + x_n * alpha3

% Y(Z) * Z^-2 = alpha1 * Y(Z) * Z^-1 + alpha2 * Y(Z) + X_N * alpha3

% Y(Z)*(Z-2 - alpha1 Z-1 -alpha2 ) = X(Z) * alpha3

% Y(Z) / X(Z) = alpha3 / (Z-2 - alpha1 Z-1 - alpha2)



Ts = 0.020; % tiempo de muestreo

num = alpha(3);        % coeficientes del numerador (z^0, z^-1)
den = [1, -alpha(1), -alpha(2)];      % coeficientes del denominador (z^0, z^-1, z^-2)

H_discreto = tf(num, den, Ts);

H_continuo = d2c(H_discreto, "tustin");

H_continuo = tf(43.15, [1, 38.84, 100.7]);

s = tf('s');

figure();
plot(x_n);
title("X_N");




figure();
% step(H_continuo);
step(H_continuo);

figure();
bode(H_continuo, optionss);
grid on


linsp = linspace(0, (length(x_n)-1)*Ts, length(x_n)).';

figure();
[y, tout] = lsim(H_continuo, x_n, linsp);
plot(tout, y);
title("salida simulada");
hold on
plot(tout, y_n);
title("Y_N");
hold off

C = 1/s;

L = minreal(C*H_continuo);

figure();
bode(L, optionss);
title("BODE DE L");

k = db2mag(15.6);

C = C*k;

L = minreal(C*H_continuo);

figure();
bode(L, optionss);
title("BODE DE L CON GANANCIA");

C_digital = c2d(C, 0.020,'tustin');



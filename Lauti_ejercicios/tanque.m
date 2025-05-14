s=tf('s');
optionss=bodeoptions;
optionss.MagVisible='on';
optionss.PhaseMatching='on';
optionss.PhaseMatchingValue=-180;
optionss.PhaseMatchingFreq=1;
optionss.Grid='on';

% Planta linealizada tanque de agua

num = [-0.004233];
den = [1 0.002397];

P = tf(num, den);

% Nucleo del controlador, con el integrador anulo el error en estado
% estacionario, permitiendo que el nivel del tanque se mantenga constante
% en el valor deseado para t -> inf

C = -1/s;

% La nueva "planta" P_nueva = C*P necesita compensar la fase, utilizo una
% red de adelanto, busco sus parametros para conseguir el mayor adelanto
% (60 grados) 

u = sin(60*(pi/180))
alfa = (1-u)/(1+u)

% Elijo la frecuencia de cruce a la que quiero que el adelanto sea maximo,
%opte por el el valor expresado mas abajo sin nigun criterio, asumiendo que
% para esa frecuencia es posible controlar el nivel del tanque

w_gc = 0.065; 
f = 2*pi/w_gc
T = 1/(sqrt(alfa)*w_gc);

% La red de adelanto resulta

lead_compensator = (s + 1/T)/(s + 1/(T*alfa));

% Busco la ganancia K de forma tal de compensar la atenuacion producida por
% la red de adelanto

k = 1/sqrt(alfa)

% Defino mi controlador como
C = C*k*lead_compensator

% La transferencia a lazo abierto L y lazo cerrado T resulta: 
L = P*C;
T = feedback(L,1);

pole(T)
%figure();rlocus(L);figure();bode(P,C,L,optionss);figure();step(T)
figure();margin(L);figure();step(T)

% Discretizo el controlador mediante metodo bilineal
Ts = 1/(10*w_gc);
C_digital = c2d(C,Ts,'tustin')

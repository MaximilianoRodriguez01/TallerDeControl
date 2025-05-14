% Ejercicio PID libro "Control systems, Godwin-Salgado"
% La resolucion es mediante el metodo Z-N

s=tf('s');
optionss=bodeoptions;
optionss.MagVisible='on';
optionss.PhaseMatching='on';
optionss.PhaseMatchingValue=-180;
optionss.PhaseMatchingFreq=1;
optionss.Grid='on';

% Planta ejemplo
Go = 1/(s+1)^3;

% Mediante el root locus observo para que ganancia los polos quedan sobre
% el eje jw, alli determino Kc y Wc. NOTA: Pudo haber sido obtenido
% mediante el caluclo Kc*Go(j*Wc) = -1 <--> Kc = -(j*Wc + 1)^3 

figure();rlocus(Go)

% Resutla
Kc = 7.74;
Wc = 1.71;

% Por lo tanto el periodo critico P = 2pi/Wc

Pc = 2*pi/Wc;

% Definimos el controlador como 
Kp = 0.6*Kc;
Tr = 0.5*Pc;
Td = 0.125*Pc;

C = Kp + (Kp/Tr)/s + Kp*Td*s

T = feedback(C*Go, 1);
figure();bode(T, optionss);figure();step(T)

% Debido al overshoot muy alto, atenuo 10 veces el valor de Ki

C_atenuado = Kp + (Kp/10*Tr)/s + Kp*Td*s
T_atenuado = feedback(C_atenuado*Go, 1);
figure();bode(T_atenuado, optionss);
title('Menor Ki')
figure();step(T_atenuado)
title('Menor Ki')



%% CONSIDERACIONES TEORICAS


Parámetros del PID y su efecto en la respuesta temporal del sistema en lazo cerrado:

1. Acción Proporcional (Kₚ)
   • Disminuye el tiempo de subida (rise time): cuanto mayor Kₚ, más rápida la respuesta.
   • Aumenta el sobreimpulso (overshoot): reduce el amortiguamiento relativo.
   • Reduce el error en estado estacionario, pero no lo elimina por completo.

2. Acción Integral (Kᵢ)
   • Elimina el error en régimen permanente frente a una entrada escalón.
   • Aumenta el tiempo de establecimiento (settling time): pueden aparecer oscilaciones de baja frecuencia.
   • Incrementa el sobreimpulso si se lleva a valores muy altos.

3. Acción Derivativa (Kₙ)
   • Mejora el amortiguamiento y reduce el sobreimpulso: “tira” de los polos hacia el semiplano izquierdo.
   • Disminuye el tiempo de establecimiento y atenúa oscilaciones rápidas.
   • Aumenta la sensibilidad al ruido de alta frecuencia (por eso suele filtrarse).

Resumen de tendencias:

• Kₚ ↑ → Rise time ↓, Overshoot ↑, Settling time ↔/↑, Error estacionario ↓ (no cero)
• Kᵢ ↑ → Rise time ↓/↔, Overshoot ↑, Settling time ↑, Error estacionario → 0
• Kₙ ↑ → Rise time ↔/↑, Overshoot ↓, Settling time ↓, Error estacionario ↔

Consejo de sintonización:

1. Ajustar primero Kₚ para conseguir velocidad de respuesta y un overshoot aproximado.
2. Añadir Kₙ para recortar el overshoot y acelerar el asentamiento.
3. Incorporar Kᵢ al final para eliminar el error en régimen permanente, controlando que no vuelva inestable.

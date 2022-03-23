clear; clc; close all;
%Declaração de constantes do projeto
%massa do corpo
mb = 0.470; %kg
%inercia do corpo
Ib = 5.09461174*10^-3;
%massa das rodas
mw = 0.020;
%inercia das rodas
Iw = 0.01139175;
%distancia centro de massa eixo
L = 0.0849; %mm
%amortecimento das rodas
Bw = 0.001; 
%raio da roda
r = 0.035;


g = 9.81;
alfa = mb + 2*mw + 2*Iw/(r * r);
betah = 2*Bw/(r*r);
gamah = mb*L;
delta = mb*L*L + Ib;


A = [0 1; -gamah*g/delta 0]
B = [0; -2/delta]
C = [1 0]
D = 0


csi = 0.5912;
wn = 6.766;
wb = wn*sqrt(1-2*(csi)^2 + sqrt(4*(csi)^4-4*(csi)^2+2))
ws = 20*wb
Ta = 2*pi/ws

[phi, gama, Cd, Dd] = c2dm(A, B, C, D, Ta)

s1 = -csi*wn + wn*sqrt(csi^2-1)
z1 = exp(s1*Ta)
z2 = conj(z1)

polos = [z1 z2]
K = place(phi, gama, polos)

phicl = phi - gama*K
syscl = ss(phicl,gama,C,D,Ta);
% step(syscl);

z_ep = exp(real(s1*5) * Ta)
polos_ep = [z_ep z_ep] % [0.17844+0.2548i 0.17844-0.2548i]%[z_ep z_ep]
Kep = acker(phi',Cd',polos_ep)'
Kec = inv(phi)*Kep

n = 2;
calc = inv([phi-eye(n) gama; C 0])*[zeros(n,1);1]
Nx = calc(1:n , 1)
Nu = calc(n+1 , 1)

phi_i = [phi gama; zeros(1,n) 1]
gama_i = [gama; 0];
C_i = [C 0];
O = obsv(phi, C)
detO = det(O)

novo_polo = 0.1802
polos_ep_i = [polos_ep novo_polo];
Kep_i = acker(phi_i', C_i', polos_ep_i)'
Kec_i = inv(phi_i)*Kep_i

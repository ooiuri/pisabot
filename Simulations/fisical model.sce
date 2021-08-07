//massa do corpo
mb = 0.470;
//inercia do corpo
Ib = 5.09461174;
//massa das rodas
mw = 0.020;
//inercia das rodas
Iw = 0.01139175;
//distancia centro de massa eixo
L = 0.0849;
//amortecimento das rodas
Bw = 0.001; 
//raio da roda
r = 0.035;

g = 9.81
alfa = mb + 2*mw + 2*Iw/(r * r)
betah = 2*Bw/(r*r)
gama = mb*L
delta = mb*L*L + Ib


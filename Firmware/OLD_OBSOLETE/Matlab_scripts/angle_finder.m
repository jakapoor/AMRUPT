function [theta] = angle_finder(IQ_Data_1, IQ_Data_2, d)
ang1 = angle(IQ_Data_1.I + 1i*IQ_Data_1.Q);
ang2 = angle(IQ_Data_2.I + 1i*IQ_Data_2.Q);

ang1(ang1 < 0) = ang1(ang1 < 0) + pi;
ang2(ang2 < 0) = ang2(ang2 < 0) + pi;

phi = ang1 - ang2;
phi = mean(phi);
theta = acos((3e8*phi)/d);

end
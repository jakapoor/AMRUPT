Fs = 1/434e6;
t = [0:1/(4*2*pi*434e6):2e-3];
w_c = 2*pi*434e6;
d = 4*(3e8/434e6);
c = 3e8;
bearing_source = pi/4;
p = (d/c)*cos(bearing_source);

dataModulation = square(2*pi*25*t);
dataModulation(dataModulation < 0) = 0;

Ant1 = sin(w_c*t);
Ant2 = sin(w_c*t + p);

Wav1 = dataModulation.*Ant1;
Wav2 = dataModulation.*Ant2;

Data1 = Antenna_Waveform_Generator(t, Wav1, 0, 2*pi*434e6);
Data2 = Antenna_Waveform_Generator(t, Wav2, 0, 2*pi*434e6);


bearing = angle_finder(Data1, Data2, d);







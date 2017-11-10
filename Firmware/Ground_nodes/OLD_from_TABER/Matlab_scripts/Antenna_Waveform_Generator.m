function [IQdata] = Antenna_Waveform_Generator(t, signal_in, WGN, w_o)
% Processes the incoming signal into I (in-phase) and Q (quadrature)
% components as seen by the antenna.  
% Also has the option to incorporate white gaussian noise (WGN)
% Returns a struct containing the IQ data of the input

if WGN
    noise = randn(1, numel(t));
else
    noise = zeros(1, numel(t));
end

osc_I = cos(w_o*t);
osc_Q = sin(w_o*t);

I = osc_I.*signal_in + noise;
Q = osc_Q.*signal_in + noise;

Fs = 434e6;
wn = ((2*pi*434e6) - w_o)/(2*pi*Fs);

[b,a] = cheby1(1,5,[.05,.1]);


IQdata.I = filter(b,a,I);
IQdata.Q = filter(b,a,Q);


end


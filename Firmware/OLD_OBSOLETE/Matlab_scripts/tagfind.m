function [out, txStart, txEnd] = tagfind(rx)
% Tag Transmission Cropper


% Train threshold
th = 2e-3;

wLen = 5;
out = [];

txStart = [];
prevStart = 0;
counter = 0;
txEnd = [];
for m = 1:length(rx)
    if(abs(rx(m)) > th && (m > prevStart + 500))
        txStart = [txStart m - 10];
        prevStart = m;
    end
end
txEnd = txStart + 200;

for n = 1:length(txStart)
    out = [out; zeros(100,1); rx(txStart(n):txEnd(n))];
end

end

        
    
        
    
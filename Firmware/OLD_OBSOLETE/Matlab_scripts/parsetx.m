function out = parsetx(rx, txStart, txEnd)
out = [];
for n = 1:length(txStart)
    out = [out; zeros(100,1); rx(txStart(n):txEnd(n))];
end

end
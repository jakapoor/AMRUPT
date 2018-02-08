[i1, Fs] = audioread('obI.wav');
[i2, Fs] = audioread('nbI.wav');
[q1, Fs] = audioread('obQ.wav');
[q2, Fs] = audioread('nbQ.wav');
%%
[i1, txStart, txEnd] = tagfind(i1);
q1 = parsetx(q1, txStart, txEnd);
q2 = parsetx(q2, txStart, txEnd);
i2 = parsetx(i2, txStart, txEnd);
%%
i1 = [(1:length(i1))' i1];
i2 = [(1:length(i2))' i2];
q1 = [(1:length(q1))' q1];
q2 = [(1:length(q2))' q2];

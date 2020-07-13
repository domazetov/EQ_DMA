clc;
clear all;

fs = 44100;
f = fopen('outputdouble.txt');
datay = textscan(f,'%s');
fclose(f);
y = str2double(datay{1}(1:1:end));
audiowrite('izlaz.wav',y,fs);

% player = audioplayer(y, fs, 24);
% play(player);

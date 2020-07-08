clc;
clear all;
[data, fs] = audioread('govor.wav','double');
data(:,2) = [];

x=data(1:1024*130);

fileID = fopen('audiodouble.txt','w');
fprintf(fileID,'%.7g\n',x);
fclose(fileID);

f = fopen('outputdouble.txt');
datay = textscan(f,'%s');
fclose(f);
y = str2double(datay{1}(1:1:end));

%yHex = num2hex(x);
%yWav = hex2num(yHex);

player = audioplayer(x, fs);
play(player);

audiowrite('izlaz.wav',y,fs);
audiowrite('original.wav',x,fs);
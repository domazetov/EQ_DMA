clc;
clear all;
[data, fs] = audioread('govor.wav','double');
data(:,2) = [];

x=data(1:1024*130);

%AMPLITUDE U DB PRIMERI{

    %MAX = 10, MIN = -10;

    %PROBA [10 8 6 4 2 0 -2 -4 -6 -8]
    %VISOKE FREKVENCIJE [-8 -6 -4 -2 0 2 4 6 8 10]
    %PROBA 1 [0 0 0 2 4 10 0 0 0 0]
    %PROBA 2 [-3 -2 -1 2 4 10 10 10 10 10]
    %PROBA 3 [-10 -10 -10 -5 0 5 10 10 10 10]
    %CIST SIGNAL [0 0 0 0 0 0 0 0 0 0]} 

%ODREDJIVANJE AMPLITUDE ZA SVAKU OD FREKVENCIJA
amps  = [-8 -6 -4 -2 0 2 4 6 8 10];
%amps = amps * 10;
amps = 10 .^ ((amps) / 40);

fileID = fopen('amps.txt','w');
fprintf(fileID,'%.7g\n',amps);
fclose(fileID);

freq = [5 10 19 35 70 117 163 232 348];

%freq = freq * 100;

fileID = fopen('freq.txt','w');
fprintf(fileID,'%d\n',freq);
fclose(fileID);

fileID = fopen('audiodouble.txt','w');
fprintf(fileID,'%.7g\n',x);
fclose(fileID);

f = fopen('outputdouble.txt');
datay = textscan(f,'%s');
fclose(f);
y = str2double(datay{1}(1:1:end));

%yHex = num2hex(x);
%yWav = hex2num(yHex);

player = audioplayer(y, fs, 24);
play(player);

audiowrite('izlaz.wav',y,fs);
audiowrite('original.wav',x,fs);
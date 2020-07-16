clc;
clear all;
[data, fs] = audioread('B.mp3','double');
data(:,2) = [];

x=data(1:1024*1292*2);
N_fft = 1024;

% for ii = 0:129
% ypr = x(((N_fft*ii)+1):(N_fft*(ii+1)));
%     Ypr1 = fft(ypr);
%     ry = real(Ypr1);
%     iy = imag(Ypr1);
%     ry = abs(ry);
%     iy = abs(iy);
%     dlmwrite('real.txt',ry,'delimiter','\n','-append')
%     dlmwrite('imag.txt',iy,'delimiter','\n','-append')
% end

%AMPLITUDE U DB PRIMERI{

    %MAX = 10, MIN = -10;

    %PROBA [10 8 6 4 2 0 -2 -4 -6 -8]
    %VISOKE FREKVENCIJE [-8 -6 -4 -2 0 2 4 6 8 10]
    %PROBA 1 [0 0 0 2 4 10 0 0 0 0]
    %PROBA 2 [-3 -2 -1 2 4 10 10 10 10 10]
    %PROBA 3 [-10 -10 -10 -5 0 5 10 10 10 10]
    %CIST SIGNAL [0 0 0 0 0 0 0 0 0 0]} 
    %BEZ SIGNALA[-60 -60 -60 -60 -60 -60 -60 -60 -60 -60]

%ODREDJIVANJE AMPLITUDE ZA SVAKU OD FREKVENCIJA
freq = [5 10 19 35 70 117 163 232 348 676 792 861 907 954 989 1005 1014 1019];
amps  = [-8 -6 -4 -2 0 2 4 6 8 10];
%amps = amps * 10;
amps = 10 .^ ((amps) / 40);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fileID = fopen('amps.txt','w');
fprintf(fileID,'%.7g\n',amps);
fclose(fileID);

%freq = freq * 100;

fileID = fopen('freq.txt','w');
fprintf(fileID,'%d\n',freq);
fclose(fileID);
% 
fileID = fopen('audiodouble.txt','w');
fprintf(fileID,'%.7g\n',x);
fclose(fileID);

% audiowrite('original.wav',x,fs);
% player = audioplayer(x, fs, 24);
% play(player);


clc;
clear all;
[data, fs] = audioread('file_example_WAV_10MG.wav','double');
data(:,2) = [];

%data_fft = fft(data);
%data = data / 100;

%data_ifft = ifft(data_fft);

%F = sfi(data);
%A=F.hex;

x=data(1:102400);



%fileID = fopen('audiodouble.txt','w');
%fprintf(fileID,'%.7g\n',x);
%fclose(fileID);

f = fopen('outputdouble.txt');
datay = textscan(f,'%s');
fclose(f);
y = str2double(datay{1}(1:1:end));

%yHex = num2hex(x);
%yWav = hex2num(yHex);

%fileID = fopen('song.txt','w');

%for r=1:size(yHex,1)
%    for k=1:size(yHex,2)
%        fprintf(fileID,'%c',yHex(r,k));
%    end
%        fprintf(fileID,'\n');
%end
%fclose(fileID);

%player = audioplayer(x, fs);
%play(player);

audiowrite('original.wav',x,fs);
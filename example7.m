%% Function descriptions:
% * This function is to create a sine wave asscoate with the white noise
% * If we want to add the flicker noise we need to add the a low pass filter
% on the white noise
%   -- 1. Generate a sinewave that will be the input of the ADC.
%   -- 2. Processing with the first order ADC.
%   -- 3. Calculate the SNDR.



%% General Information
%      1. fs*fbin/N = ftone;

%% Add necessary path and data files
addpath("util/sherierToolbox");
addpath("util/jianxiongToolbox");
close all;
clear;
clc;

%% nonuniform sampling ADC

% FFT parameters:
N = 2^16; % number of points for doing the FFT.
fbin = 307;

% ADC parameters:
fs = 1e8; % sampling frequency of the ADC.
nlev = 8; % the ADC output levels.
vfsc = 1; % the ADC full scale voltages.
OSR = 400; % the oversampling ratio of the ADC.
% -------- Process ----------
vlsb = vfsc/nlev;
BW = fs/(2*OSR);

% Input parameters:
freq = fs*fbin/N; % frequency of the input signals.
amp = 1/2; % Amplitude of the input signals. Vpp is 2 x Amplitude
vcm = vfsc/2; % common mode voltage of the input signal that should match to the ADC vcm.

% Sinewave generation: (Will be very useful for ADC analysis)

tPeriod = 1/freq;
tSample = (0:N-1)/fs; % The sampling interval of the ADC
vinSine = genSine_V1p0(amp,freq,vcm, tSample);
vinSineN = awgn(vinSine,60,'measured');
figure
plot(tSample, vinSine)
title('Generated Sine Wave')
xlabel('t(s)');
ylabel('Amplitude (V)');  
set(gca,'fontsize',35)
set(gca,'linewidth',6)
data(1:N) = 0;
j=1;
for i = 2 : N
    data(i) = floor(vinSine(i)/vlsb);
    if data(i) ~= data(i-1)
       X(j) = data(i);
       Qt(j) = tSample(i);
       j=j+1;
    end    
end

figure
plot(tSample, data)

hold on
plot(Qt, X)

title('ADC output')
xlabel('t(s)');
ylabel('ADC output');  
set(gca,'fontsize',35)
set(gca,'linewidth',6)

fprintf(' FLASH ADC \n \n')

% Processing FFT.
SNDR = calSNDR(data,N,fs,OSR,fbin,nlev);
fprintf(' Fin  = %.4f Hz\n', freq)
fprintf(' SNDR  = %.4f dB\n BW    = %.4f kHz\n', SNDR, BW/1000)

Out = movavgFilt (data, 4);
SNDR = calSNDR(Out,N,fs,OSR,fbin,nlev);
fprintf(' Fin  = %.4f Hz\n', freq)
fprintf(' SNDR  = %.4f dB\n BW    = %.4f kHz\n', SNDR, BW/1000)
% 
% N = length (X);
Len = length (X);
% Out1(1:N) = 0;
% ts = 1/1e6;
%%
freq = 1000000;
omega = 2 * pi * freq;
t= 0:0.0000001:pi;
func =@(t)  omega/pi*sinc(omega/pi*(t));
% figure
% plot(t,func)

%%

figure
plot(Qt, X)

% Out1 = resample(X,Qt,fs);
% timeSample = tSample(1:length(Out1));
% yout = movavgFilt (Out1, 4);
% figure
% plot(timeSample, yout)
% hold on
% % plot(Qt(1:100), X(1:100))
% plot(tSample, data)
% SNDR = calSNDR(Out1,length(Out1),fs,OSR,fbin,nlev);
% fprintf(' Fin  = %.4f Hz\n', freq)
% fprintf(' SNDR  = %.4f dB\n BW    = %.4f kHz\n', SNDR, BW/1000)


Out1 = 0 ;
 % for j = 0:0.000001:0.000654660000000000
 
for j = 1:400
    time = j/Len*Qt(end);
  for i = 1:1000
   Out1 = Out1 + X(i)*integral(@(p) func(p-time),Qt(i),Qt(i+1));
  end
  yout(j) = Out1;
  timeSample(j) = time;
  Out1 = 0;
  fprintf('  Done = %.4f  \n', j/400)
 end
% % 
% 
figure
plot(timeSample, yout)
hold on
plot(Qt(1:100), X(1:100))
plot(tSample, data)
SNDR = calSNDR(yout,length(yout),fs,OSR,fbin,nlev);
% % fprintf(' Fin  = %.4f Hz\n', freq)
% % fprintf(' SNDR  = %.4f dB\n BW    = %.4f kHz\n', SNDR, BW/1000)
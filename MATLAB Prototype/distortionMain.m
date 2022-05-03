%% The DSP design for a JUCE distortion
%Changable variables:
%   d1 -> distortion amount for D1.1
%   d2 -> distortion amount for D1.2
%   thresh -> cutoff threshold for D2
%   g -> Gain control for y[n]

clear;clc; 
%% Creating a sine wave
% Allows for prototyping with clear results
Fs = 48000; 
Ts = 1/Fs; 
f = 2; % f = 2 (Waveform and Char Curve), f = 2500 (THD)
t = [0:Ts:1].'; 
in = sin(2*pi*f*t); % Used as input signal for each distortion

%% Distortion Block Diagram
%%% Cubic soft–clipping -> a(n)
d1 = 1; %Amount: 0 (no distortion) - 1 (full)
an = cubicDistortion(in,d1);

%%% Cubic soft–clipping ->b(n)
d2 = 1; %Amount: 0 (no distortion) - 1 (full)
bn = cubicDistortion(an,d2);

%%% Hard–clipping -> c(n)
thresh = 0.5; 
cn = hardClip(in,thresh); 

%%% Addition of signals -> d(n)
dn = bn+cn;

%%% Gain-Control -> y(n)
g = 0.9;
yn = mgain(dn,g);

%% Plots of distorted figures
%%% Plotting 
figure(1); % Use f = 2 (above) 
subplot(1,2,1); % Waveform 
plot(t,in,t,yn); axis([0 1 -1.1 1.1]);
xlabel('Time (sec.)');ylabel('Amplitude'); 
title('Waveform'); 

subplot(1,2,2); % Characteristic curve 
plot(in,in,in,yn); axis([-1 1 -1.1 1.1]);
xlabel('Input Amplitude'); ylabel('Output Amplitude');
legend('x[n]','y[n]'); title('Characteristic Curve');

% figure(2); % Total harmonic distortion plot (f = 2500)
% thd(out,Fs,5); 
% axis([0 24 -50 0]);
% Cubic Distortion
% This function implements soft clipping
% distortion. An input parameter "a" is used
% to control the amount of distortion applied
% to the input signal

% Input variables
%   in : input signal
%   a : drive amount (0-1). amplitude of 3rd harmonic

function [ out ] = cubicDistortion(in,a)
N = length(in);
out = zeros(N,1);
for n = 1:N
out(n,1) = in(n,1) - a*(1/3)*in(n,1)^3;
end


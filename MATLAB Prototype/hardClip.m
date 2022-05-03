% Hard Clip
% This function implements hard-clipping
% distortion. Amplitude values of the input signal
% that are greater than a threshold are clipped.

% Input variables
%   in : signal to be processed
%   thresh : maximum amplitude where clipping occurs

function [out] = hardClip(in,thresh)
%HARDCLIP Summary of this function goes here
%   Detailed explanation goes here
N = length(in);
out = zeros(N,1);
for n = 1:N

    if in(n,1) >= thresh
        % if true, assign output = thresh
        out(n,1) = thresh;
    elseif in(n,1)<= -thresh
        % if true, set output = -thresh
        out(n,1) = -thresh;
    else
        out(n,1) = in(n,1);
    end

end


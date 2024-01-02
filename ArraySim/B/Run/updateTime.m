function [newTime] = updateTime(newDistance,currentSpeed,currentTime)
%This function returns new UTC time as a datenum 
%Adopted from https://www.mathworks.com/matlabcentral/answers/366202-how-can-i-add-add-sec-min-and-hours-to-time

%Convert to [m/s] 
currentSpeed = currentSpeed*(1000/3600);

%Convert distance to m 
newDistance = newDistance*1000;

%Calculate time to add
secondsAdded = round(newDistance/currentSpeed); 

%Update time
newTime = addtodate(currentTime,secondsAdded,'second');

end
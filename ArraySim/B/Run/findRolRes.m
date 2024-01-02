function [rollingResistance] = findRolRes(speed,rollingRes) 

%Change speed to km/h and round to neares 5th km/h 
speed = round((speed*(3600/1000)));

%Define rolling resistance vector in [W]
rollingResistance = rollingRes(speed);

end
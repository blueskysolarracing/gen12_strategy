function [bear] = findBear(lat_1,long_1,lat_2,long_2) 

%Find difference of long
dLong = long_2 - long_1;

%Find X
X = cosd(lat_2)*sind(dLong);

%Find Y
Y = (cosd(lat_1)*sind(lat_2))-(sind(lat_1) * cosd(lat_2)* cosd(dLong));

if dLong < 0
    
    bear = 360 + rad2deg(atan2(X,Y));
    
else
    
    bear = rad2deg(atan2(X,Y));
    
end

end
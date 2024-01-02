function [sunVector] = create_sun_vector_simple(azimuth,elevation,irr)

%Convert irr from W/m2 to W/mm2 
irr = irr*(0.000001);

%Create vector
x = irr*cosd(elevation)*cosd(azimuth);
z = -irr*sind(elevation);
y = -irr*cosd(elevation)*sind(azimuth); 

sunVector = [x;y;z]; %Magnitude in [W/mm^2] 

end 

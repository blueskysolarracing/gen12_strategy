function [sunPlane] = create_sun_plane(azimuth,elevation,dir,bearing,minPoint)
% Calculate a plane with a certain unit normal vector and that
% Parameters:
% azimuth: azimuth angle in degrees
% elevation: elevation angle in degrees
% dir: string indicating the axis that the nose of the car points
% bearing: bearing of the nose of the car from true north
% minPoint: The minimum distance that any point of the plane must be from the origin

% Return:
% sunPlane: A 2 member struct. One is the a 3x1 vector indicating the unit normal (normal)
%           and the other is a point on the plane (point)

% Correct the azimuth angle such that it points clockwise from the nose of the car
if bearing <= azimuth
    corrected_azimuth = azimuth - bearing;
else
    corrected_azimuth = 360 - (bearing - azimuth);
end

x = cosd(elevation)*cosd(corrected_azimuth);
y = cosd(elevation)*sind(corrected_azimuth); 
z = sind(elevation);

magnitude = 0;
if minPoint < 1
    magnitude = 1;
else
    magnitude = minPoint + 1;
end

if dir == "+x"
    y = -1 * y;
elseif dir == "-x"
    x = -1 * x;
elseif dir == "+y"
    temp_x = x;
    x = y;
    y = temp_x;
elseif dir == "-y"
    temp_x = x;
    x = -1 * y;
    y = -1 * temp_x;
else
    disp("Please indicate the direction of the nose: +x, -x, +y, -y");
end

sunPlane = struct();
sunPlane.normal = [x,y,z];
sunPlane.point = [x*magnitude, y*magnitude, z*magnitude];

end 

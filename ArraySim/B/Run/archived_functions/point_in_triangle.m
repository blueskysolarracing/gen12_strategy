function [binary] = point_in_triangle(point,triangle)

%Create vectors
AB = triangle(2,:)-triangle(1,:);
AC = triangle(3,:)-triangle(1,:);

PA = triangle(1,:)-point;
PB = triangle(2,:)-point;
PC = triangle(3,:)-point;

%Compute area of triangle
Area = 0.5*norm(cross(AB,AC));

%Compute ratios
a =  (0.5*norm(cross(PB,PC)))/Area;
b =  (0.5*norm(cross(PC,PA)))/Area;
g = (0.5*norm(cross(PA,PB)))/Area;

%Return value
tot = a+ b + g;

if tot == 1 && (0<=a) && (a<=1) && (0<=b) && (b<=1) && (0<=g) && (g<=1) 
    binary = 1;
else
    binary = 0;
end

end
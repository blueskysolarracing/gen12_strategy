function [triArea] = area_of_triangle(pointsMatrix)

%Define points
A = pointsMatrix(1,:);
B = pointsMatrix(2,:);
C = pointsMatrix(3,:);

%Create vectors
AB = B - A;
AC = C - A;

%Compute area 
triArea = 0.5*norm(cross(AB,AC)); 

end 






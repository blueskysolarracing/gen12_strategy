function [points] = remove_inner_points(varargin)

%Create matrix of points
t = cell2mat(transpose(varargin));

%Project onto x-y plane 
y = t;  
y(:,3) = []; 

%Find indices
k = convhull(y);

%Recreate matrix
a = t(k,:);
a(1,:) = [];

%Return points
points = a;

end
function [areas] = return_areas(vertices)
%Must use "order_vertices" function first

%Define size  
N = size(vertices,1);
areas = 0; 

for i = 1:3:N 
    
    %Define points
    A = vertices(i,:);
    B = vertices(i+1,:);
    C = vertices(i+2,:);
    
    %Define vectors
    AB = B - A;
    AC = C - A;
    
    %Compute area 
    areas = areas +(0.5*norm(cross(AB,AC))); 
    
end

end
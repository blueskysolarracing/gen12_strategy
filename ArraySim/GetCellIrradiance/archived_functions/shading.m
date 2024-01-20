function [points_reduced] = shading(points,vertices)
%Must use "order_vertices" function first
%"points" is assumed to be an Nx3 matrix

%Define size  
N = size(vertices,1);
V = size(points,1); 

%Define counter
O = 0;

%Define points in 
pointsIn = [];

for i = 1:3:N 
    
    %Define points
    A = vertices(i,:);
    B = vertices(i+1,:);
    C = vertices(i+2,:); 
    
    %Define vectors
    AB = B - A;
    AC = C - A;
    
    %Compute area of triangle
    Area = 0.5*norm(cross(AB,AC));
    
    for j = 1:V
        
        point = points(j,:);
       
        PA = A-point;
        PB = B-point;
        PC = C-point;
        
        %Compute ratios
        alpha =  (0.5*norm(cross(PB,PC)))/Area;
        beta =  (0.5*norm(cross(PC,PA)))/Area;  
        gamma = (0.5*norm(cross(PA,PB)))/Area;
        
        %Return value
        tot = alpha + beta + gamma;
        
        %Save index of point if in triangle 
        if tot == 1 && 0 <= alpha,beta,gamma <= 1; 
            
            if isempty(pointsIn) == 1 
           
                O = O+1;
                pointsIn(O,:) = point;
           
            elseif sum(ismember(pointsIn, point,'rows')) < 1
           
                O = O+1;
                pointsIn(O,:) = point;
                    
            end
        end
    end
end

points_reduced = pointsIn;
        
end
function [polyArea] = area_of_nsided_convex_polygon(varargin)

%Define size of input 
n = size(varargin,2);

%Generate vectors
for i=2:n
    
    vecs(i-1,:)= cell2mat(varargin(i))-cell2mat(varargin(1));
    
end

%Find angles 
ang(1) = 0; 

for i = 2:n-1
    
    if cross(vecs(1,:),vecs(i,:)) < 1
        
        ang(i) = -atan2d(norm(cross(vecs(1,:),vecs(i,:))),dot(vecs(1,:),(vecs(i,:))));
    
    else 
        
        ang(i) = atan2d(norm(cross(vecs(1,:),vecs(i,:))),dot(vecs(1,:),(vecs(i,:))));
       
    end
    
end

%Sort
A = transpose(ang); 
B = vecs;
C = transpose((varargin(2:n))); 

[A, index] = sort(A, 'ascend'); 
B = B(index,:);
C = C(index); 

%Calculate area
area = 0; 
for i = 1:n-2
    
    area = area + (0.5*norm(cross(B(i,:),B(i+1,:))));
    
end

%Return area
polyArea = area;

end






    
    
    


    
    
    
    

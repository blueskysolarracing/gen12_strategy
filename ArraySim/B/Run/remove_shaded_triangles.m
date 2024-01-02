function [newArray] = remove_shaded_triangles(projPoints,projVertices,origVertices)

%Define size  
N = size(projVertices,1);
V = size(projPoints,1); 

%For loop
for i = 1:3:N 
    
    %Define points
    A = projVertices(i,:);
    B = projVertices(i+1,:);
    C = projVertices(i+2,:); 
    
    %Define vectors
    AB = B - A;
    AC = C - A;
    
    %Compute area of triangle
    Area = 0.5*norm(cross(AB,AC));
    
    %Binary 
    binary = 1;
    
    for j = 1:V
                     
        point = projPoints(j,:);
       
        PA = A-point;
        PB = B-point;
        PC = C-point;
        
        %Compute ratios
        a = (0.5*norm(cross(PB,PC)))/Area;
        b = (0.5*norm(cross(PC,PA)))/Area;  
        g = (0.5*norm(cross(PA,PB)))/Area;
        
        %Return value
        tot = a + b + g;
        
        if tot == 1 && (0<=a) && (a<=1) && (0<=b) && (b<=1) && (0<=g) && (g<=1)
                
            binary = binary+1;
            origVertices(i:i+2,:) = NaN;
            
        end
        
        if binary > 1
            
            break
            
        end
        
    end
    
end

origVertices(any(isnan(origVertices),2),:) = [];
newArray = origVertices;

end
      



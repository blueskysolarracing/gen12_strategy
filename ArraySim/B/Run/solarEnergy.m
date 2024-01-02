function [solarOutput] = solarEnergy(sunVector,vertices)
%Define sizes
N = size(vertices,1); 

%Define solar output
solarOutput = 0;

%For loop 
for i = 1:3:N
    
    %Define points
    A = vertices(i,:);
    B = vertices(i+1,:);
    C = vertices(i+2,:); 
    
    %Define vectors
    AB = B - A;
    AC = C - A;
    
    %Compute area of triangle
    area = 0.5*norm(cross(AB,AC));  
    
    %Generate unit normal vector
    normal = cross(AB,AC)/norm(cross(AB,AC));
    
    %Calculate solar energy for triangle
    solar = abs(dot(sunVector,normal))*area*0.25*600; 
    
    %Update solarOutput
    solarOutput = solarOutput + solar;
    
end

end
function [solarOutput] = solarPower(sunVector,vertices,Irr)
% Calculate the solar power in W/m^2

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
    
    %Generate unit normal vector of the triangle
    normal = cross(AB,AC)/norm(cross(AB,AC));
    
    %Ensure normal is facing the +ve Z direction 
    if normal(3) < 0 
        normal = (-1)*normal;
    end
    
    %Calculate solar energy for triangle
    solar = dot(sunVector,normal)*area*Irr; 
    
    %Ensure sun is shining from above 
    if solar < 0 
        solar = 0;
    end

    %Update solarOutput
    solarOutput = solarOutput + solar;
    
end

end
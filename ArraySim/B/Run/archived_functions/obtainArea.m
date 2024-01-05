function [effArea] = obtainArea(solarTable,Az,El) 
%This function return effective area in [mm] for given solar Az and El 

%{
%If conditions 
if rem(El, 1) == 0 && rem(Az,1) == 0
    
    effArea = solarTable(El+1,Az+1); 
    
elseif rem(El, 1) == 0 && rem(Az,1) ~= 0
    
    %Define tempAz 
    highAz = ceil(Az); 
    lowAz = floor(Az); 
    
    %Define values
    x_1_y_2 = solarTable(El+1,highAz+1);  
    x_1_y_1 = solarTable(El+1,lowAz+1);
    
    %Obtain area
    effArea = ((((x_1_y_2)-(x_1_y_1))/(highAz-lowAz))*(Az-lowAz))+(x_1_y_1);
    
    
elseif rem(El, 1) ~= 0 && rem(Az,1) == 0
    
    %Define tempEl 
    highEl = ceil(El); 
    lowEl = floor(El); 
    
    %Define values
    x_1_y_1 = solarTable(lowEl+1,Az+1);  
    x_2_y_1 = solarTable(highEl+1,Az+1);
    
    %Obtain area
    effArea = ((((x_2_y_1)-(x_1_y_1))/(highEl-lowEl))*(El-lowEl))+(x_1_y_1);
    
elseif rem(El, 1) ~= 0 && rem(Az,1) ~= 0
    
    %Define tempEl 
    highEl = ceil(El); 
    lowEl = floor(El); 
    
    %Define tempAz 
    highAz = ceil(Az); 
    lowAz = floor(Az);
    
    %Solve first row 
    
    %Define values
    x_1_y_2 = solarTable(lowEl+1,highAz+1);  
    x_1_y_1 = solarTable(lowEl+1,lowAz+1);
    
    %Obtain area first row 
    effArea_1 = ((((x_1_y_2)-(x_1_y_1))/(highAz-lowAz))*(Az-lowAz))+(x_1_y_1);
   
    %Solve second row 
    
    %Define values
    x_2_y_2 = solarTable(highEl+1,highAz+1);  
    x_2_y_1 = solarTable(highEl+1,lowAz+1);
    
    %Obtain area first row 
    effArea_2 = ((((x_2_y_2)-(x_2_y_1))/(highAz-lowAz))*(Az-lowAz))+(x_2_y_1);
    
    %Solve for true El 
    effArea = ((((effArea_2)-(effArea_1))/(highEl-lowEl))*(El-lowEl))+(effArea_1);
    
end
      %} 

%Az if condition
if Az > 180
    Az = 180- (Az - 180);
end

%Define rounded El and Az 
newEl = round(El) + 1; 
newAz = round(Az) + 1;

%Get effective area 
effArea= solarTable(newEl,newAz);

end
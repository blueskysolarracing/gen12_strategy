function [cellIrr] = cellIrrTable(canopy)
%This function takes in array and canopy stls and returns a solar table 
%which contains the power output of the array for all possible solar Az  
%and El angles (0 - 180). This 'powTab' is then used as a lookup table. 
%res defines the resolution of the generated table (ex: 1 deg resolution).

%% Import files 
%Improt canopy
[v_C] = stlread2(canopy);

%Create cell structure
oCells = cMake();

%Store 'cells' field names for easy access
cN = fieldnames(oCells);

%Initiate sCells structure 
sCells.tC1 = [];

%% Loop over all Az and El to generate cellIrrTab
%Parallel for loop 
parfor El = 1:181 %El in row
    
    for Az = 1:181 %Az in col 
        
        %Create sun vector 
        sunVector = create_sun_vector_simple((Az-1),(El-1));
        
        %Loop over every cell and remove shaded tris
        sCells = remShadCellStruc(sunVector,v_C,oCells);
        
        %Find power output of each cell in 'sgtC' structure
        cellIrr{El,Az} = cellData(sunVector,sCells,oCells)
            
    end
    
end  

end


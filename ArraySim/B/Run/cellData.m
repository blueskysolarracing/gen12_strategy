function [cellIrr] = cellData(C,sunVector,sgtC,gtC)
%Define number of cells
N = C;

%Store 'sgtC' field names for easy access
stcN = fieldnames(sgtC);

%Find power output of each cell in 'sgtC' structure
parfor i = 1:N        
            
    %Obtain power output of 'stC' i 
    cellOut(i) = solarPower(sunVector,sgtC.(stcN{i})); 
      
    %Obtain area of cell i in 'cells' structure
    cellArea(i) = return_areas(gtC.(stcN{i}));
            
    %Obtain irradiance for cell i 
    if cellArea(i) > 0               
        cellIrr(i) = cellOut(i)/cellArea(i);                
    else
        cellIrr(i) = 0;
    end
    
end

end
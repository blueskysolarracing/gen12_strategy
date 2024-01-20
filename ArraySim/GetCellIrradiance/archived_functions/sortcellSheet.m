function [orCell] = sortcellSheet(cellSheet)
%Convert all NaN to 0
cellSheet(isnan(cellSheet)) = 0;

%Delete all empty rows
cellSheet = cellSheet(~all(cellSheet == 0, 2),:);

%Find indices of integer numbered rows
ind = find(cellSheet(:,3)==floor(cellSheet(:,3)));

%Find values at these indices
val = cellSheet(ind,3);

%Use only values less than 258 (# of cells)
val = val(abs(val)<259);

%Store ind and val in a new array then sort
orArr(:,1) = val;
orArr(:,2) = ind; 
orArr = sortrows(orArr);

%Define size of orArr 
N = size(orArr, 1); 

%Initiate cells structure
orCell.C1 = [1]; 

%Add in all rows of each cell consecutively into a structure 
for i = 1:N-1
    
    %Find finish index for cell i 
    fin = min(orArr(find(orArr(:,2) > orArr(i,2)),2));
    
    %Add in cell i vertices 
    orCell = setfield(orCell, ['C' num2str(i)], cellSheet((orArr(i,2)+1):fin-1,:)); 
    
end

%Store 'cells' field names for easy access
cN = fieldnames(orCell);

%Find convhull for each cell
for i = 1:N-1
    
   %Store current cell points in temp var
   temp = orCell.(cN{i}); 
   
   %Extract x-y coordinates only 
   xytemp = temp(:,[1,2]);
   
   %Find convhull of temp 
   ctemp = convhull(xytemp); 
   
   %Update cell i with convhull points
   orCell.(cN{i})= temp(ctemp,:);
    
end


end
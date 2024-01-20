function [cellStructure] = cStructure(cMat, aTris) 
%This function takes in a structure containg vertices of numbered sqaure array cells (cMat)
%and a matrix of array triangle vertices (aTris) and returns a cell
%structure with all triangles ordered in the cells they belong to within
%the structure 'cellStructure'

%Define number of cells
N = 257; 

%Create 2D arrayTri matrix (x,y)
aT2D = aTris(:,[1,2]); 

%Store 'cells' field names for easy access
cN = fieldnames(cMat);

%Initiate cellStructure 
cellStructure.tC1 = [1];

%Identify tris belonging to cell 'Ci' in structure 'cells'
for i = 1:N 
    
    %Obtain coordinates of current cell i 
    cC = cMat.(cN{i});
            
    %Identify tri points that are in cell i
    [in] = inpolygon(aT2D(:,1),aT2D(:,2),cC(:,1),cC(:,2));  
    
    %Find indices where in=1 thrice consectively and rem((index-1),3)=0
    in = transpose(in); 
    
    f = [1,1,1]; %Pattern to find 
    
    ind = strfind(in,f);
    
    ind = ind(mod((ind-1),3) == 0); %Extract all ind div by 3 
    
    %Include other 2 indices for each index 
    ind =  sort([ind ind+1 ind+2]);
    
    %Add triangles belonging to cell i to tCi field 
    cellStructure = setfield(cellStructure, ['tC' num2str(i)], aTris(ind,:)); 
    
end

end
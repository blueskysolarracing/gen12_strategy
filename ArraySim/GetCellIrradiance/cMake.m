function [orderedCellPoints, origCellPoints, origCellFaces] = cMake(numberOfCells, cellFilePrefix) 
% This function reads all cell stls and returns data that describes the points 
% of all array cells
% Parameters:
% numberOfCells: The total number of array cells
% cellFilePrefix: The prefix to each stl file including the path
%
% Returns:
% orderedCellPoints: A cell array of n x 3 matrices describing points on each cell of the array with ordering
% origCellPoints, origCellFaces: Cell arrays of n x 3 matrices that are the direct outputs of stlread2 before ordering

% Initialize file names 
fNames = cell(1,numberOfCells);
for i = 1:numberOfCells
    fNames{i} = strcat(cellFilePrefix, '(', int2str(i), ').stl');
end

% Initialize return values
orderedCellPoints = cell(1, numberOfCells);
origCellPoints = cell(1, numberOfCells);
origCellFaces = cell(1, numberOfCells);

%Import triangles for cell i 
for i = 1:numberOfCells
    
    %Import tris into cell i
    [v_A,f_A] = stlread2(fNames{i}); 
    origCellPoints{i} = v_A;
    origCellFaces{i} = f_A;
    
    %Order array vertices 
    arrayTri = order_vertices(v_A,f_A);
    orderedCellPoints{i} = arrayTri;

end

end 
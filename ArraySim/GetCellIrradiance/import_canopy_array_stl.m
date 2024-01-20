function [canopyMesh, cellMeshes, canopyPoints, orderedCellPoints] = import_canopy_array_stl(canopyFileName, arrayCellPrefix, numberOfCells)
% Loads all array cell stls, canopy stl and centers the models.
% Parameters:
% canopyFileName: the name of the canopy stl file
% arrayCellPrefixes: the prefix of all array cell stl files including the path
% numberOfCells: number of array cells

% Output:
% canopyMesh: An opcode mesh of the canopy
% cellMeshes: An array of opcode meshes for each array cell
% canopyPoints: An n x 3 matrix of points describing the canopy
% orderedCellPoints: A cell array of n x 3 matrices that describe each cell of the array ordered by triangles

% Load the canopy stl
[canopyPoints, canopyFaces] = stlread2(canopyFileName);

% Load all array stls
[orderedCellPoints, origCellPoints, origCellFaces] = cMake(numberOfCells, arrayCellPrefix); 

% Center the array and canopy
[orderedCellPoints, origCellPoints, canopyPoints] = centerArrayAndCanopy(orderedCellPoints, canopyPoints, origCellPoints);

% Create canopy opcode mesh
canopyMesh = opcodemesh(transpose(canopyPoints), transpose(canopyFaces));

% Create opcode meshes for array cells
cellMeshes = cell(1, numberOfCells);
for i = 1:numberOfCells
    cellMeshes{i} = opcodemesh(transpose(origCellPoints{i}), transpose(origCellFaces{i}));
end

end

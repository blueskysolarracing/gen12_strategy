function [oCells, v_C] = centerArrayAndCanopy(arrayCells, canopy, totalPoints)
% Given the struct arrayCells and the point cloud of the canopy, this
% function centers all elements around the origin
% @param arrayCells: 1x1 struct of array cells -> output of cMake()
% @param totalPoints: number of points that describe the array
% @param canopy: nx3 point cloud of canopy points -> output of stlread2()

allPoints = zeros(totalPoints+size(canopy, 1), 3);
index = 1;
allPoints(index:size(canopy, 1), :) = canopy;
index = size(canopy, 1);

cellNames = fieldnames(arrayCells);
numCells = numel(cellNames);

for i=1:numCells
    cellPoints = arrayCells.(cellNames{i});
    allPoints(index+1:index+size(cellPoints,1), :) = cellPoints;
    index = index + size(cellPoints, 1);
end

centroid = mean(allPoints);

canopy = bsxfun(@minus, canopy, centroid);
for i=1:numCells
    arrayCells.(cellNames{i}) = bsxfun(@minus, arrayCells.(cellNames{i}), centroid);
end

v_C = canopy;
oCells = arrayCells;

end


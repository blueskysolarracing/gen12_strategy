function [oCells, ogCells, v_C] = centerArrayAndCanopy(orderedCellV, canopy, origCells)
% Centers all array cell points and the canopy point cloud around the origin
% Parameters:
% orderedCellV: A cell array of n x 3 matrices that describe points of each cell ordered by triangles
% canopy: An n x 3 matrix that describe point cloud of the canopy
% origCells: A cell array of n x 3 matrices that describe points of each cell without ordering

% Output:
% oCells: Same as orderedCellV with shifting
% ogCells: Same as origCells with shifting
% v_C: Same as canopy with shifting

% Collect all points from the array and canopy
allArrayAndCanopyPoints = cat(1, canopy, orderedCellV{:});

% Calculate the centroid by taking the mean along each axis
centroid = mean(allArrayAndCanopyPoints);

% Center the canopy
canopy = bsxfun(@minus, canopy, centroid);

% Center the array
numCells = size(orderedCellV, 2);
for i=1:numCells
    orderedCellV{i} = bsxfun(@minus, orderedCellV{i}, centroid);
    origCells{i} = bsxfun(@minus, origCells{i}, centroid);
end

v_C = canopy;
oCells = orderedCellV;
ogCells = origCells;

end


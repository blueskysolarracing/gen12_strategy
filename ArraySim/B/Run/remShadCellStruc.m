function [sCells] = remShadCellStruc(sunPlane,canopyMesh, cellMeshes, cellPoints,idx)
% Remove shaded triangles of each array cell
% Parameters:
% sunPlane: 2 member struct describing plane of the sun
% canopyMesh: opcode mesh of the canopy
% cellMeshes: cell array of opcode meshes of each array cell
% cellPoints: A cell array of n x 3 matrices describing points each array cell with ordering

% Return:
% sCells: Same as cellPoints with shaded triangles/points removed

numberOfCells = size(cellPoints, 2);
rays = cell(1, numberOfCells);
sources = cell(1, numberOfCells);
sCells = cell(1, numberOfCells);

% Calculate rays extending from each point of the array towards the plane of the sun
for i = 1:numberOfCells
    [sources{i}, rays{i}] = calculate_rays(cellPoints{i}, sunPlane);

    % Graph rays and sources
    %scatter3(sources{i}(:,1), sources{i}(:,2), sources{i}(:,3), 'filled');
    %quiver3(sources{i}(:,1), sources{i}(:,2), sources{i}(:,3), rays{i}(:,1), rays{i}(:,2), rays{i}(:,3),'Color', 'b');
end

% Remove shaded triangles due to canopy and array shading
for i = 1:numberOfCells
    newCell = remove_shaded_triangles(canopyMesh, cellMeshes, cellPoints{i}, rays{i}, sources{i}, i);
    sCells{i} = newCell;
end

end
function [newCell] = remove_shaded_triangles(canopyMesh, cellMeshes, cellVertices, rays, sources, idx)
% Remove shaded triangles from an array cell
% Parameters:
% canopyMesh: opcode mesh of the canopy
% cellMeshes: cell array of opcode meshes of each array cell
% cellVertices: vertices of an array cell as an n x 3 matrix
% rays: rays of each point extending from the point to the sun plane as an n x 3 matrix
% sources: source vertices for each triangle of the cell being examined

% Return:
% newCell: same as cellVertices with removed rows for shaded triangles

numTriangles = size(rays, 1);
numCells = size(cellMeshes, 2);

% Query ray intersection with the canopy
[hit,~,~,~,~] = canopyMesh.intersect(transpose(sources), transpose(rays));

% Remove shaded triangles
for i=1:numTriangles
    if hit(i) == 1
        cellVertices((i-1)*3 + 1: i*3,:) = NaN;
    end
end

% Query ray intersection with all other cell array meshes
for i=1:numCells
    if i == idx
        continue
    end
    [hit,~,~,~,~] = cellMeshes{i}.intersect(transpose(sources), transpose(rays));

    for j=1:numTriangles
        if hit(j) == 1
            cellVertices((j-1)*3 + 1: j*3,:) = NaN;
        end
    end
    
end

cellVertices(any(isnan(cellVertices),2),:) = [];
newCell = cellVertices;

end
      



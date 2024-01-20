function [sources, dirs] = calculate_rays(cellVertices, sunPlane)
% Calculates the direction vectors of a set of vertices orthogonally projected
% onto a plane with a specified normal vector
% Parameters:
% cellVertices: set of vertices to project as an n x 3 array ordered into triangles
% sunPlane: plane to project on

% Return:
% source: n x 3 matrix of source vertices
% dirs: n x 3 matrix of directional vectors

% Loop over each triangle and find the ray for each
numPoints = size(cellVertices, 1);
sourceVertices = zeros(numPoints/3,3);
idx = 1;

for i = 1:3:numPoints
    pt1 = cellVertices(i,:);
    pt2 = cellVertices(i+1,:);
    pt3 = cellVertices(i+2,:);

    centroid = (pt1 + pt2 + pt3) / 3;
    sourceVertices(idx,:) = centroid;
    idx = idx + 1;
end

projection = project_onto_sun_plane(sunPlane, sourceVertices);
dirs = projection - sourceVertices;
sources = sourceVertices;

end
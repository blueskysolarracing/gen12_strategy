function [projectedPoints] = project_onto_sun_plane(sunPlane,pProject)
% Project a matrix of points onto a plane
% Parameters: 
% sunPlane: a 2 member struct describing the plane of the sun
% pProject: an n x 3 matrix with points to project onto the plane

% Return:
% projectedPoints: an n x 3 matrix with the projected points

numPoints = size(pProject,1); 
projectedPoints = zeros(numPoints, 3);
normalVector = sunPlane.normal;
pointOnPlane = sunPlane.point;

for i = 1:numPoints
    pointToPlane = pProject(i,:) - pointOnPlane;
    distance = dot(pointToPlane, normalVector);
    projectedPoints(i,:) = pProject(i,:) - distance * normalVector;

end

end
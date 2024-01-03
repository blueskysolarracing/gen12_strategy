function [newCell] = rem_shaded_triangles_plot(projCanopy,projCell,origVertices)

%Define size  
N = size(projCell,1);
V = size(projCanopy,1); 

% Iterate through each triangle of the cell
for i = 1:3:N 
    
    % Points of the triangle
    A = projCell(i,:);
    B = projCell(i+1,:);
    C = projCell(i+2,:); 
    triangle = [A;B;C];

    allPoints = vertcat(projCanopy, triangle);
    
    xyzc = mean(allPoints,1);
    [~,~,V] = svd(allPoints-xyzc, 0);
    allPoints2d = allPoints * V(:,1:2);
    
    triangle2d = allPoints2d(end-2:end,:);
    canopy2d = allPoints2d(1:end-3,:); 
    
    figure;
    hold on;
    plot(triangle2d(:,1), triangle2d(:,2), 'o','Color', 'r');
    plot(canopy2d(:,1), canopy2d(:,2), 'o','Color', 'b');
    % See if any of the triangle points lie inside the 2d convex hull of
    % the canopy
    if (inpolygon(triangle2d(:,1), triangle2d(:,2), canopy2d(:,1), canopy2d(:,2)))
        origVertices(i:i+2,:) = NaN;
    end
    
end

origVertices(any(isnan(origVertices),2),:) = [];
newCell = origVertices;

end
      



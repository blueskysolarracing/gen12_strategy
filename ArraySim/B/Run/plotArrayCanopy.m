function plotArrayCanopy(canopy,arrayCells, numberOfCells)
% Plots the array and canopy given the triangles of the array cells and the mesh of the canopy
% @param canopy: canopy points -> output of stlread()
% @param arrayCells: triangles that define the array -> output of cMake()
% @param numberOfCells: Number of array cells

fields = fieldnames(arrayCells);
scatter3(canopy(:, 1), canopy(:, 2), canopy(:, 3), 50, 'filled', 'MarkerFaceColor', 'b');
hold on;
colour = 'r';

for i=1:numberOfCells
    cellName = fields{i};
    triangle = arrayCells.(cellName);

    if (size(triangle,1) == 0)
        continue
    end
    x = triangle(:,1);
    y = triangle(:,2);
    z = triangle(:,3);

    chull = convhull(x,y);
    chull = chull(1:end-1);
    chull_points = triangle(chull,:);
    
    x = chull_points(:,1);
    y = chull_points(:,2);
    z = chull_points(:,3);
    scatter3(x, y, z, 50, 'filled', 'MarkerFaceColor', 'b');
    fill3(x, y, z, colour, 'FaceAlpha', 0.3);
end

end


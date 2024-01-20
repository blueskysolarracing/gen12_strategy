function plotArrayCanopy(canopy,arrayCells, highlightCells, uiAxes)
    % Plots the array and canopy given the triangles of the array cells and the mesh of the canopy
    % @param canopy: canopy points as an n x 3 matrix
    % @param arrayCells: Cell array of n x 3 matrices that describe points of each cell with ordering
    % @param highlightCells: array of cell numbers to highlight
    % @param uiAxes: The set of figure axes to plot on

    % Plot the canopy
    if (uiAxes ~= 0)
        scatter3(uiAxes, canopy(:, 1), canopy(:, 2), canopy(:, 3), 50, 'filled', 'MarkerFaceColor', 'b');
        hold(uiAxes,"on");
    else
        scatter3(canopy(:, 1), canopy(:, 2), canopy(:, 3), 50, 'filled', 'MarkerFaceColor', 'b');
        hold on;
    end

    xlabel('X-axis');
    ylabel('Y-axis');
    zlabel('Z-axis');

    numberOfCells = size(arrayCells, 2);
    % Iterate through all triangles of each cell and colour them in
    for i=1:numberOfCells
        colour = 'r';
        if ismember(i, highlightCells)
            colour = 'g';
        end
    
        cellTriangles = arrayCells{i};
        numPoints = size(cellTriangles, 1);
        if (numPoints == 0)
            continue;
        end
        
        for j=1:3:numPoints
            x = cellTriangles(j:j+2,1);
            y = cellTriangles(j:j+2,2);
            z = cellTriangles(j:j+2,3);
            
            if (uiAxes ~= 0) 
                fill3(uiAxes, x, y, z, colour);
            else
                fill3(x, y, z, colour);
            end
        end
    end
    
end
    
    
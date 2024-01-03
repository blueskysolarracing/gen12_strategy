function [sCells] = remShadCellStruc(C,sunVector,v_C,ogCells,idx)

% C = number of cells 

%Store 'cells' field names for easy access
cN = fieldnames(ogCells);

%Project canopy vertices onto sun vector
projCanopy = project_onto_sun_vector(sunVector,v_C);

% Calculate the convex hull of the canopy
[U,S] = svd(   bsxfun(@minus,projCanopy,mean(projCanopy)),   0);  
hullIndices = convhull(U*S(:,1:2));
hullIndices = hullIndices(1:end-1);
projCanopyConv = projCanopy(hullIndices,:);

%Loop over every cell and remove shaded tris
for j = 1:C
           
    %Proj cell j's tris onto sun vec
    projArray = project_onto_sun_vector(sunVector,ogCells.(cN{j}));
            
    %Remove cell j's shaded tris 
    newArray = remove_shaded_triangles(projCanopyConv,projArray,ogCells.(cN{j}));
            
    %Add in new set of tris into sCells.tCj
    sCells.(cN{j}) = newArray;

    if (j == 178) && idx == 6
        scatter3(projCanopy(:,1), projCanopy(:,2), projCanopy(:,3), 'filled', 'MarkerFaceColor', 'b');
        scatter3(projArray(:,1), projArray(:,2), projArray(:,3), 'filled', 'MarkerFaceColor','r');
        newArray = rem_shaded_triangles_plot(projCanopyConv,projArray,ogCells.(cN{j}));
    end
            
end

end
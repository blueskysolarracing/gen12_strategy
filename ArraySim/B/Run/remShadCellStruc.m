function [sCells] = remShadCellStruc(C,sunVector,v_C,ogCells)

% C = number of cells 

%Store 'cells' field names for easy access
cN = fieldnames(ogCells);

%Project canopy vertices onto sun vector
projCanopy = project_onto_sun_vector(sunVector,v_C);

%Loop over every cell and remove shaded tris
for j = 1:C
           
    %Proj cell j's tris onto sun vec
    projArray = project_onto_sun_vector(sunVector,ogCells.(cN{j}));
            
    %Remove cell j's shaded tris 
    newArray = remove_shaded_triangles(projCanopy,projArray,ogCells.(cN{j}));
            
    %Add in new set of tris into sCells.tCj
    sCells.(cN{j}) = newArray;
            
end
b = 5;

end
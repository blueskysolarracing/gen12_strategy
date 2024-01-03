function [cellStructure, totalPoints] = cMake(numberofCells) 
%This function takes in a list of cell stps with tris in them and returns a
%cell structure that is numbered and ordered along with the total number of
%points that describe the array

C = numberofCells;

%Initiate file names 
for i = 1:C
fNames{i} = append('./ArraySTLs/','NA(',int2str(i),').stl');
end

%Initiate cellStructure 
cellStructure.tC1 = [1];

totalPoints = 0;

%Import triangles for cell i 
for i = 1:size(fNames,2) 
    
    %Import tris into cell i
    [v_A,f_A] = stlread2(fNames{i}); 
    
    %Order array vertices 
    arrayTri = order_vertices(v_A,f_A);
    
    totalPoints = totalPoints + size(arrayTri, 1);

    %Add triangles belonging to cell i to tCi field 
    cellStructure = setfield(cellStructure, ['tC' num2str(i)], arrayTri); 
    
end

end 
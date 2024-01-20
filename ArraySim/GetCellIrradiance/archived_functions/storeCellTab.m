function [Mat] = storeCellTab(cellIrr) 
%This function creates a 3D matrix with two axis being AZ and El and the
%third axis contains the irradiance of cell i (total 257) for each combination of Az
%and El angles (180x180) 

%Initiate Mat with zeros
Mat = zeros(181,181,257);

%Begin for loop
parfor i = 1:181 %El
    for j = 1:181 %Az
        for k = 1:257 %Cell Irr
            Mat(i,j,k) = cellIrr{i,j}(k);
        end
    end
end
            

end
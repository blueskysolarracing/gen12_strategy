function [powTab] = fastPowTab(cellIrr)
%This function takes in a struc conating cell irr data for each cell
%for every combination of Az and El angles (181x181)

%Input values of cellIrr structure to a 3D matrix 
Mat = storeCellTab(cellIrr);

%Initiate powTab with zeros
powTab = zeros(181,181);

%Initiate varibale to store temp cell irr data
x = zeros(1,257);

%Begin for loop
for i = 96:102
    for j = 1:181
        %Store cell irr values in a new variable
        x(1,:) = Mat(i,j,:);
        %Obtain power
        powTab(i,j) = totalPower(x);
    end
end

end
function [dayPow,stringData] = dayPower() 
%%% This function returns the power output of the array for a given
%%% cellIrr cell structure 

%Import cellIrr data
cellIrr = (1000*1000)*table2array(readtable('cellIrr.csv'));

%Define parameters
N = size(cellIrr,2); %181

%Initiate string data structure 
stringData = struct;

%Loop over all entries and all cells to find daily power output
for i = 1:5
    [dayPow(i,1),strDat] = totalPower(cellIrr(i,:));
    stringData = setfield(stringData, ['R' num2str(i)], strDat);        
end

end
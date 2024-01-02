function [] = dayPower(file)

tic
%Extract index range for cellIrr for current folder 
currFol = pwd; 
ind = str2num(regexprep(currFol,{'\D*([\d\.]+\d)[^\d]*','[^\d\.]*'},{'$1 ', ' '}));
ind = ind-1;

%Import cellIrr data
cellIrr = (load(file));

%Define parameters
N = size(cellIrr,2);

%Loop over all entries and all cells to find daily power output
for i = (4169:4689)  %change if running one day sim
    disp(' ');
    disp(append('Running row ', int2str(i), ' of ', int2str(size(cellIrr,1)), ' of wscIrr', ' [', int2str(toc), ' seconds elapsed]'));
    disp(' ');
    dayPow(i,1) = totPower(cellIrr(i,:));
    disp(append('Finished row ', int2str(i), ' of ', int2str(size(cellIrr,1)), ' of wscIrr', ' [', int2str(toc), ' seconds elapsed]'));
    storeuData(dayPow); 
end

end

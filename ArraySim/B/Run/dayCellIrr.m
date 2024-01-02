% Step 1: Adjust lines 6, 10, and 13 accordingly
% Step 2: run the script

%Define parameters
delete(gcp('nocreate'));
parpool(6); % <- change this to number of cores on workstation
wscAngles = readmatrix('./wscAngles.csv');
N = size(wscAngles,1); 
wscIrrCell = {};
numberOfCells = 257; % <- change this to number of cells in array

%Import canopy
[v_C] = stlread2('./ArraySTLs/canopy.stl'); % <- change this to correct name of canopy stl file

%Create cell structure
oCells = cMake(numberOfCells); 
warning('off','all');

% Plot the array and canopy - testing purposes
plotArrayCanopy(v_C, oCells, numberOfCells);

%Loop over all rows of Az, El, Irr and Time table and find cell irr
for i = 1:N 
    
    %Read wscAngles csv
    wscAngles = readmatrix('./wscAngles.csv');
    
    %Extract data from wscAngles
    Az = wscAngles(i,1);
    El = wscAngles(i,2);
    Irr = wscAngles(i,3);

    %Create sun vector 
    sunVector = create_sun_vector_simple(Az,El,Irr);

    %Loop over every cell and remove shaded tris
    sCells = remShadCellStruc(numberOfCells, sunVector,v_C,oCells);
        
    %Find power output of each cell in 'sgtC' structure
    wscIrrCell{i} = cellData(numberOfCells, sunVector,sCells,oCells);
    
    % if i == 31
    %     clf;
    %     plotArrayCanopy(v_C, sCells, numberOfCells);
    %     quiver3(0, 0, 0, sunVector(1)*10e5, sunVector(2)*10e5, sunVector(3)*10e5, 'LineWidth', 2, 'Color', 'r');
    % end
end


%Create wscIrr array 
wscIrr = zeros(180,numberOfCells);
for i = 1:size(wscIrrCell,2)
    wscIrr(i,:) = wscIrrCell{i};
end 

%Export wscIrr.csv
writematrix(wscIrr,'wscIrr.csv');

%Copy wscIrr.csv to step C
currDir = pwd;
currDir = erase(currDir,'\B\Run');
copyfile('wscIrr.csv',append(currDir,'/C/Run/wscIrr.csv'));
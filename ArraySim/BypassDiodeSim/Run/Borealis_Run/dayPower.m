function [] = dayPower(file, starting_row, ending_row)
% Run the simulink model over a range of rows of the simulation
% @param file: n x num_cells csv generated from step 2
% @param starting_row: starting index to simulate from (inclusive)
% @param ending_row: ending index to simulate to (inclusive)
    tic
    %Extract index range for cellIrr for current folder 
    currFol = pwd; 
    ind = str2num(regexprep(currFol,{'\D*([\d\.]+\d)[^\d]*','[^\d\.]*'},{'$1 ', ' '}));
    ind = ind-1;
    disp(strcat("Starting at row: ", num2str(starting_row)));
    disp(strcat("Ending at row: ", num2str(ending_row)));
    %Import cellIrr data
    cellIrr = (load(file));
    N = size(cellIrr,2);
    
    %Loop over all entries and all cells to find daily power output
    for i = (starting_row:ending_row)
        disp(' ');
        disp(append('Running row ', int2str(i), ' of ', int2str(size(cellIrr,1)), ' of wscIrr', ' [', int2str(toc), ' seconds elapsed]'));
        disp(' ');
        dayPow(i,1) = totPower(cellIrr(i,:), "string_layout.json");
        disp(append('Finished row ', int2str(i), ' of ', int2str(size(cellIrr,1)), ' of wscIrr', ' [', int2str(toc), ' seconds elapsed]'));
        storeuData(dayPow, "diode.csv"); 
    end
    
    end
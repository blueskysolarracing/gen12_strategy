function [] = dayPower(irr_csv, array_layout, starting_row, ending_row)
    % Run the simulink model over a range of rows of the simulation and output the csv in "diode.csv"
    % @param irr_csv: n x num_cells csv generated from step 2
    % @param array_layout: .json file with layout of strings and substrings and cells on the array
    % @param starting_row: starting index to simulate from (inclusive)
    % @param ending_row: ending index to simulate to (inclusive)
    
    tic

    % Read array layout file
    fid = fopen(array_layout);
    raw = fread(fid,inf);
    str = char(raw);
    fclose(fid);
    string_layout = jsondecode(transpose(str));
    fn = fieldnames(string_layout);

    % Mapping of cells to each substring. cells2Substrings{i} gives the array range of cell numbers that 
    % are part of substring i
    cells2Substrings = {};

    % Mapping of substrings to each string. substrings2Strings(i) gives the array of substring numbers 
    % that are part of string i
    substrings2Strings = {};

    % Number of cells in each string. numCellsInString(i) gives the number of cells in string i
    numCellsInString = [];

    % Get all cell to substring mappings and substring to string mappings and number of cells in each string
    for string_num=1:numel(fn)
        string_name = strcat("String", num2str(string_num));

        if (ismember(string_name, fn))
            substrings = string_layout.(string_name);
            substrings2Strings{string_num} = substrings;

            num_substrings = numel(substrings);
            numCellsInString(end+1) = 0;
            
            % Insert cell to substring mapping
            for j=1:num_substrings
                substring_num = substrings(j);
                substring_name = strcat("ss", num2str(substring_num));
                cell_range = string_layout.(substring_name);
                cell_list = cell_range(1):cell_range(2);
                num_cells = numel(cell_list);

                cells2Substrings{substring_num} = cell_list;

                numCellsInString(string_num) = numCellsInString(string_num) + num_cells;
            end
        end
    end

    disp(strcat("Starting at row: ", num2str(starting_row)));
    disp(strcat("Ending at row: ", num2str(ending_row)));

    % Import cellIrr data
    cell_irr = (load(irr_csv));

    % Container for output power
    num_rows = size(cell_irr, 1);
    dayPow = zeros(num_rows,1);

    % Loop over each row and find power output in W
    for row = (starting_row:ending_row)
        disp(' ');
        disp(append('Running row ', int2str(row), ' of ', int2str(num_rows), ' of wscIrr', ' [', int2str(toc), ' seconds elapsed]'));
        disp(' ');

        dayPow(row) = totPower(cell_irr(row,:), cells2Substrings, substrings2Strings, numCellsInString);

        disp(append('Finished row ', int2str(row), ' of ', int2str(num_rows), ' of wscIrr', ' [', int2str(toc), ' seconds elapsed]'));

        storeuData(dayPow, "diode.csv"); 
    end
        
end
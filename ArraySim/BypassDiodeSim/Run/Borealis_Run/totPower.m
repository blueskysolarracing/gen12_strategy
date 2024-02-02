function [totPow] = totPower(cellIrr, string_config)
% Run the simulink model over a vector of irradiances
% @param cellIrr: 1 x num_cells array of effective irradiances in W/m^2
% @param string_config: json files describing the substring and cell layout of the array
    fid = fopen(string_config);
    raw = fread(fid,inf);
    str = char(raw);
    fclose(fid);
    string_layout = jsondecode(transpose(str));
    fn = fieldnames(string_layout);
    
    num_strings = 0;
    num_substrings = 0;
    cells2Substrings = [];
    substrings2Strings = {};

    for i=1:numel(fn)
        key = fn{i};
        if contains(key, "ss")
            cells2Substrings = [cells2Substrings, sum(string_layout.(key))];
            num_substrings = num_substrings + 1;
        end

        if contains(key, "String")
            substrings2Strings = [substrings2Strings; {transpose((string_layout.(key)))}];
            num_strings = num_strings + 1;
        end
    end
    
    % initiate variables for simulink
    irr_diode_string_String1 = [];
    irr_diode_string_String2 = [];
    irr_diode_string_String3 = [];
    
    Slope_Vdc_string1 = 75 * 0.77;
    Slope_Vdc_string2 = 88 * 0.77;
    Slope_Vdc_string3 = 94 * 0.77;
    
    % Loop through all strings and get the irradiances for each cell
    for j=1:length(substrings2Strings{1})
     
        substring = substrings2Strings{1}(j);
        
        key = strcat("ss", int2str(substring));
        cells = string_layout.(key);
        
        cell_ids = cells(1):cells(2);
        for cell = 1:length(cell_ids)
            irr_diode_string_String1 = [irr_diode_string_String1, cellIrr(1, cell_ids(1,cell))];
        end

    end

    irr_diode_string_String1 = [0, irr_diode_string_String1];
    
    for j=1:length(substrings2Strings{2})
        substring = substrings2Strings{2}(j);
        
        key = strcat("ss", int2str(substring));
        cells = string_layout.(key);
        
        cell_ids = cells(1):cells(2);
        for cell = 1:length(cell_ids)
            irr_diode_string_String2 = [irr_diode_string_String2, cellIrr(1, cell_ids(1,cell))];
        end

    end
    irr_diode_string_String2 = [0, irr_diode_string_String2];

    for j=1:length(substrings2Strings{3})
        substring = substrings2Strings{3}(j);
        
        key = strcat("ss", int2str(substring));
        cells = string_layout.(key);
        
        cell_ids = cells(1):cells(2);
        for cell = 1:length(cell_ids)
            irr_diode_string_String3 = [irr_diode_string_String3, cellIrr(1, cell_ids(1,cell))];
        end

    end
    irr_diode_string_String3 = [0, irr_diode_string_String3];

    %Define working directory 
    options = simset('SrcWorkspace','current');
    warning('off','all');
    
    % Run sim for all strings
    strPow = zeros(1, 3);
    
    sim('WSC_2023_Final_PVArray_Diode_String_1.slx',[],options);
    sim("WSC_2023_Final_PVArray_Diode_String_2.slx",[],options);
    sim("WSC_2023_Final_PVArray_Diode_String_3.slx",[],options);
    strPow(1) = max(diode_string1_power_out);
    strPow(2) = max(diode_string2_power_out);
    strPow(3) = max(diode_string3_power_out); 
    
    
    %Report total power
    totPow = sum(strPow);
    
    end
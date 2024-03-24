function [totPow] = totPower(cellIrr, cells2Substrings, substrings2Strings, numCellsInString)
% Run the simulink model over an array of irradiances
% @param cellIrr: 1 x num_cells array of effective irradiances in W/m^2
% @param cells2Substrings: Mapping of cells to substring numbers
% @param substrings2Strings: Mapping of substring numbers to string numbers
% @param numCellsInString: array of double with number of cells in each string

    num_strings = numel(substrings2Strings);
    % WARNING: YOU MUST CHANGE THE NAMES OF THE SIMULINK FILES TO RUN. THEY MUST BE IN THE ORDER OF STRINGS
    SIMULINK_FILE_NAMES = ["ThreeWheel_2_String1_Diode.slx",...
                           "ThreeWheel_2_String2_Diode.slx",...
                           "ThreeWheel_2_String3_Diode.slx"];

    % WARNING: YOU MUST ALSO MANUALLY DEFINE THE INPUT VARIABLES TO THE SIMULINK FILES SINCE SIMULINK IS LIKE THAT.
    % DO THAT HERE. !!!!!!
    damping_factor = 0.77;
    irr_diode_string = cell(num_strings,1);
    Slope_Vdc_string = numCellsInString * damping_factor;

    for string=1:num_strings

        substrings = substrings2Strings{string};
        num_substrings = numel(substrings);

        for substring = 1:num_substrings
            cells = cells2Substrings{substrings(substring)};
            irr_diode_string{string} = [irr_diode_string{string}, cellIrr(cells)];
        end

        irr_diode_string{string} = [0, irr_diode_string{string}];
    end

    % WARNING: MANUALLY DEFINE INPUT IRRADIANCE ARRAY VARIABLE FOR EACH STRING
    irr_diode_string_String1 = irr_diode_string{1};
    irr_diode_string_String2 = irr_diode_string{2};
    irr_diode_string_String3 = irr_diode_string{3};

    % WARNING: MANUALLY DEFINE SWEEPING VOLTAGE FOR EACH STRING
    Slope_Vdc_string1 = Slope_Vdc_string(1);
    Slope_Vdc_string2 = Slope_Vdc_string(2);
    Slope_Vdc_string3 = Slope_Vdc_string(3);

    % WARNING: MANUALLY DEFINE THE POWER OUT ARRAYS FOR EACH STRING
    diode_string1_power_out = [];
    diode_string2_power_out = [];
    diode_string3_power_out = [];

    %Define working directory 
    options = simset('SrcWorkspace','current');
    warning('off','all');

    % Run sim for all strings
    strPow = zeros(1, num_strings);

    % Run the simulink file for each string
    for i=1:num_strings
        sim(SIMULINK_FILE_NAMES(i),[],options);
    end
    
    % WARNING: MANUALLY ADD THE MAX OF EACH POWER OUT ARRAY TO STRPOW
    strPow(1) = max(diode_string1_power_out);
    strPow(2) = max(diode_string2_power_out);
    strPow(3) = max(diode_string3_power_out);

    %Report total power
    totPow = sum(strPow);
    
 end
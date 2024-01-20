function [totPow] = totPower(cellIrr)
% Run the simulink model over a vector of irradiances
% @param cellIrr: 1 x num_cells array of effective irradiances in W/m^2
    
    % Map substrings to string
    string.one = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 50, 51, 52, 53, 54, 55];
    string.two = [16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 31, 32, 33, 34, 35];
    string.three = [26, 27, 28, 29, 30, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49];
    
    %Define number of cells in each substring
    ncsS = [3,4,3,2,4,4,4,6,6,6,3,3,3,3,3,3,6,6,6,2,2,7,7,7,7,2,2,1,2,2,7,7,7,7,7,7,7,7,7,7,7,7,7,7,2,6,6,6,2,3,3,3,3,3,3];
    
    %Define cell indices in substrings
    %          1        2     3        4       5         6        7       8        9         10       11      12       13       14       15       16        17      18       19        20       21       22      23         24         25       26          27        28         29          30        31          32        33         34          35        36          37         38         39        40        41           42         43         44         45         46         47         48         49         50         51         52         53         54        55
    cCount = {[1:3], [4:7], [8:10], [11:12], [13:16], [17:20], [21:24], [25:30], [31:36], [37:42], [43:45], [46:48], [49:51], [52:54], [55:57], [58:60], [61:66], [67:72], [73:78], [79:80], [81:82], [83:89], [90:96], [97:103], [104:110], [111:112], [113:114], [115:115], [116:117], [118:119], [120:126], [127:133], [134:140], [141:147], [148:154], [155:161], [162:168], [169:175], [176:182], [183:189], [190:196], [197:203], [204:210], [211:217], [218:219], [220:225], [226:231], [232:237], [238:239], [240:242], [243:245], [246:248], [249:251], [252:254], [255:257]};
    
    %Define number of substrings and strings
    numOfSubstrings = size(ncsS,2);
    numOfStrings = 3;
    
    % initiate variables for simulink
    irr_diode_string1 = [];
    irr_diode_string2 = [];
    irr_diode_string3 = [];
    
    Slope_Vdc_string1 = 75 * 0.77;
    Slope_Vdc_string2 = 88 * 0.77;
    Slope_Vdc_string3 = 94 * 0.77;
    
    % Loop through all strings and get the irradiances for the string
    for i=1:length(string.one)
        substring = cCount{1, string.one(1,i)};
    
        for j=1:length(substring)
            irr_diode_string1 = [irr_diode_string1, cellIrr(1, substring(1,j))];
        end
    end
    irr_diode_string1 = [irr_diode_string1, 0];
    irr_diode_string1 = flip(irr_diode_string1);
    
    for i=1:length(string.two)
        substring = cCount{1, string.two(1,i)};
    
        for j=1:length(substring)
            irr_diode_string2 = [irr_diode_string2, cellIrr(1, substring(1,j))];
        end
    end
    irr_diode_string2 = [irr_diode_string2, 0];
    irr_diode_string2 = flip(irr_diode_string2);
    
    for i=1:length(string.three)
        substring = cCount{1, string.three(1,i)};
    
        for j=1:length(substring)
            irr_diode_string3 = [irr_diode_string3, cellIrr(1, substring(1,j))];
        end
    end
    irr_diode_string3 = [irr_diode_string3, 0];
    irr_diode_string3 = flip(irr_diode_string3);
    
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
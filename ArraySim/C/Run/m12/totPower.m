% Step 1: Update lines 6 and 9

function [totPow] = totPower(cellIrr) 

%Define number of cells in each substring
ncsS = [4,3,2,3,4,4,4,6,6,6,3,3,3,2,2,3,2,2,3,2,3,6,3,2,2,4,2,3,2,2,8,7,2,8,8,7,7,8,8,7,7,2,4,4,2,2,2,2,2,2,2,8,8,7,7,8,8,7,7];

%Define cell indices in substrings
cCount = {[1:4]; [5:7]; [8:9]; [10:12]; [13:16]; [17:20]; [21:24]; [25:30]; [31:36]; [37:42]; [43:45]; [46:48]; [49:51]; [52:53]; [54:55]; [56:58]; [59:60]; [61:62]; [63:65]; [66:67]; [68:70]; [71:76]; [77:79]; [80:81]; [82:83]; [84:87]; [88:89]; [90:92]; [93:94]; [95:96]; [97:104]; [105:111]; [112:113]; [114:121]; [122:129]; [130:136]; [137:143]; [144:151]; [152:159]; [160:166]; [167:173]; [174:175]; [176:179]; [180:183]; [184:185]; [186:187]; [188:189]; [190:191]; [192:193]; [194:195]; [196:197]; [198:205]; [206:213]; [214:220]; [221:227]; [228:235]; [236:243]; [244:250]; [251:257]};

%Define number of substrings
numOfSubstrings = size(ncsS,2);

%initiate variables
strPow = zeros(1,numOfSubstrings); %[W] 
slVdc = num2cell(zeros(1,numOfSubstrings));
gat = num2cell(zeros(1,numOfSubstrings));
ir = num2cell(zeros(1,numOfSubstrings));

%Create input cell arrays for sim 
for j = 1:numOfSubstrings
    
    %Find number of cells in current substring
    num_cell_subtring = ncsS(j);
    
    %Update slope Vdc (V/s)
    slVdc{j} = 0.74*num_cell_subtring; 
    
    %Update gate input param
    gat{j} = [zeros(1, (29-num_cell_subtring)) 1 zeros(1, (num_cell_subtring - 1))];

    %Update irr input param
    ir{j} = [ones(1, (29-num_cell_subtring)) flip(cellIrr(cCount{j}))];
    ir{j}(1) = 0;
    
end

%Define working directory 
options = simset('SrcWorkspace','current');
warning('off','all');

%Loop over every string and find its max power output
for i = 1:size(ncsS, 2)    
    %Run sim for str i    
    Slope_Vdc = slVdc{i};
    gate = gat{i};
    irr = ir{i};
    sim('PVArray_MPPT.slx',[],options);
    strPow(i) = max(Substring_MPPT_Out_Power);
    disp(append('   Finished string ',int2str(i),' of ',int2str(size(ncsS,2)),' [',int2str(toc),' seconds elapsed]'));       
end

%Report total power
totPow = sum(strPow);

end
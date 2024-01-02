function [totPow,strData] = totalPower(cellIrr) 
%Begin timing 
tic

%Define number of cells in each substring
ncsS = [4,3,2,3,4,4,4,6,6,6,3,3,3,2,2,3,2,2,3,2,3,6,3,2,2,4,2,3,2,2,8,7,2,8,8,7,7,8,8,7,7,2,4,4,2,2,2,2,2,2,2,8,8,7,7,8,8,7,7];

%Define cell indices in substrings
cCount = {[1:4]; [5:7]; [8:9]; [10:12]; [13:16]; [17:20]; [21:24]; [25:30]; [31:36]; [37:42]; [43:45]; [46:48]; [49:51]; [52:53]; [54:55]; [56:58]; [59:60]; [61:62]; [63:65]; [66:67]; [68:70]; [71:76]; [77:79]; [80:81]; [82:83]; [84:87]; [88:89]; [90:92]; [93:94]; [95:96]; [97:104]; [105:111]; [112:113]; [114:121]; [122:129]; [130:136]; [137:143]; [144:151]; [152:159]; [160:166]; [167:173]; [174:175]; [176:179]; [180:183]; [184:185]; [186:187]; [188:189]; [190:191]; [192:193]; [194:195]; [196:197]; [198:205]; [206:213]; [214:220]; [221:227]; [228:235]; [236:243]; [244:250]; [251:257]};

%initiate variables
strPow = zeros(1,59); %[W] 
slVdc = num2cell(zeros(1,59));
gat = num2cell(zeros(1,59));
ir = num2cell(zeros(1,59));

%Create input cell arrays for sim 
for j = 1:size(ncsS, 2)
    
    %Find number of cells in current substring
    num_cell_subtring = ncsS(j);
    
    %Update slope Vdc (V/s)
    slVdc{j} = 0.74*num_cell_subtring; 
    
    %Update gate input param
    gat{j} = [zeros(1, (29-num_cell_subtring)) 1 zeros(1, (num_cell_subtring - 1))];

    %Update irr input param
    ir{j} = [600*ones(1, (29-num_cell_subtring)) flip(cellIrr(cCount{j}))];
    ir{j}(1) = 0;
    
end
toc

%Define working directory 
options = simset('SrcWorkspace','current');

%Run sim for str 1
Slope_Vdc = slVdc{1};
gate = gat{1};
irr = ir{1}; 
sim('PVArray_MPPT.slx',[],options);
strPow(1) = max(Substring_MPPT_Out_Power);
strData.S1.C = Substring_MPPT_Out_Current; 
strData.S1.P = Substring_MPPT_Out_Power; 
strData.S1.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 2
Slope_Vdc = slVdc{2};
gate = gat{2};
irr = ir{2}; 
sim('PVArray_MPPT.slx',[],options);
strPow(2) = max(Substring_MPPT_Out_Power);
strData.S2.C = Substring_MPPT_Out_Current; 
strData.S2.P = Substring_MPPT_Out_Power; 
strData.S2.V = Substring_MPPT_Out_Voltage; 
toc 

%Run sim for str 3
Slope_Vdc = slVdc{3};
gate = gat{3};
irr = ir{3}; 
sim('PVArray_MPPT.slx',[],options);
strPow(3) = max(Substring_MPPT_Out_Power);
strData.S3.C = Substring_MPPT_Out_Current; 
strData.S3.P = Substring_MPPT_Out_Power; 
strData.S3.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 4
Slope_Vdc = slVdc{4};
gate = gat{4};
irr = ir{4}; 
sim('PVArray_MPPT.slx',[],options);
strPow(4) = max(Substring_MPPT_Out_Power);
strData.S4.C = Substring_MPPT_Out_Current; 
strData.S4.P = Substring_MPPT_Out_Power; 
strData.S4.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 5
Slope_Vdc = slVdc{5};
gate = gat{5};
irr = ir{5}; 
sim('PVArray_MPPT.slx',[],options);
strPow(5) = max(Substring_MPPT_Out_Power);
strData.S5.C = Substring_MPPT_Out_Current; 
strData.S5.P = Substring_MPPT_Out_Power; 
strData.S5.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 6
Slope_Vdc = slVdc{6};
gate = gat{6};
irr = ir{6}; 
sim('PVArray_MPPT.slx',[],options);
strPow(6) = max(Substring_MPPT_Out_Power);
strData.S6.C = Substring_MPPT_Out_Current; 
strData.S6.P = Substring_MPPT_Out_Power; 
strData.S6.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 7
Slope_Vdc = slVdc{7};
gate = gat{7};
irr = ir{7}; 
sim('PVArray_MPPT.slx',[],options);
strPow(7) = max(Substring_MPPT_Out_Power);
strData.S7.C = Substring_MPPT_Out_Current; 
strData.S7.P = Substring_MPPT_Out_Power; 
strData.S7.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 8
Slope_Vdc = slVdc{8};
gate = gat{8};
irr = ir{8}; 
sim('PVArray_MPPT.slx',[],options);
strPow(8) = max(Substring_MPPT_Out_Power);
strData.S8.C = Substring_MPPT_Out_Current; 
strData.S8.P = Substring_MPPT_Out_Power; 
strData.S8.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 9
Slope_Vdc = slVdc{9};
gate = gat{9};
irr = ir{9}; 
sim('PVArray_MPPT.slx',[],options);
strPow(9) = max(Substring_MPPT_Out_Power);
strData.S9.C = Substring_MPPT_Out_Current; 
strData.S9.P = Substring_MPPT_Out_Power; 
strData.S9.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 10
Slope_Vdc = slVdc{10};
gate = gat{10};
irr = ir{10}; 
sim('PVArray_MPPT.slx',[],options);
strPow(10) = max(Substring_MPPT_Out_Power);
strData.S10.C = Substring_MPPT_Out_Current; 
strData.S10.P = Substring_MPPT_Out_Power; 
strData.S10.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 11
Slope_Vdc = slVdc{11};
gate = gat{11};
irr = ir{11}; 
sim('PVArray_MPPT.slx',[],options);
strPow(11) = max(Substring_MPPT_Out_Power);
strData.S11.C = Substring_MPPT_Out_Current; 
strData.S11.P = Substring_MPPT_Out_Power; 
strData.S11.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 12
Slope_Vdc = slVdc{12};
gate = gat{12};
irr = ir{12}; 
sim('PVArray_MPPT.slx',[],options);
strPow(12) = max(Substring_MPPT_Out_Power);
strData.S12.C = Substring_MPPT_Out_Current; 
strData.S12.P = Substring_MPPT_Out_Power; 
strData.S12.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 13
Slope_Vdc = slVdc{13};
gate = gat{13};
irr = ir{13}; 
sim('PVArray_MPPT.slx',[],options);
strPow(13) = max(Substring_MPPT_Out_Power);
strData.S13.C = Substring_MPPT_Out_Current; 
strData.S13.P = Substring_MPPT_Out_Power; 
strData.S13.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 14
Slope_Vdc = slVdc{14};
gate = gat{14};
irr = ir{14}; 
sim('PVArray_MPPT.slx',[],options);
strPow(14) = max(Substring_MPPT_Out_Power);
strData.S14.C = Substring_MPPT_Out_Current; 
strData.S14.P = Substring_MPPT_Out_Power; 
strData.S14.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 15
Slope_Vdc = slVdc{15};
gate = gat{15};
irr = ir{15}; 
sim('PVArray_MPPT.slx',[],options);
strPow(15) = max(Substring_MPPT_Out_Power);
strData.S15.C = Substring_MPPT_Out_Current; 
strData.S15.P = Substring_MPPT_Out_Power; 
strData.S15.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 16
Slope_Vdc = slVdc{16};
gate = gat{16};
irr = ir{16}; 
sim('PVArray_MPPT.slx',[],options);
strPow(16) = max(Substring_MPPT_Out_Power);
strData.S16.C = Substring_MPPT_Out_Current; 
strData.S16.P = Substring_MPPT_Out_Power; 
strData.S16.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 17
Slope_Vdc = slVdc{17};
gate = gat{17};
irr = ir{17}; 
sim('PVArray_MPPT.slx',[],options);
strPow(17) = max(Substring_MPPT_Out_Power);
strData.S17.C = Substring_MPPT_Out_Current; 
strData.S17.P = Substring_MPPT_Out_Power; 
strData.S17.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 18
Slope_Vdc = slVdc{18};
gate = gat{18};
irr = ir{18}; 
sim('PVArray_MPPT.slx',[],options);
strPow(18) = max(Substring_MPPT_Out_Power);
strData.S18.C = Substring_MPPT_Out_Current; 
strData.S18.P = Substring_MPPT_Out_Power; 
strData.S18.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 19
Slope_Vdc = slVdc{19};
gate = gat{19};
irr = ir{19}; 
sim('PVArray_MPPT.slx',[],options);
strPow(19) = max(Substring_MPPT_Out_Power);
strData.S19.C = Substring_MPPT_Out_Current; 
strData.S19.P = Substring_MPPT_Out_Power; 
strData.S19.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 20
Slope_Vdc = slVdc{20};
gate = gat{20};
irr = ir{20}; 
sim('PVArray_MPPT.slx',[],options);
strPow(20) = max(Substring_MPPT_Out_Power);
strData.S20.C = Substring_MPPT_Out_Current; 
strData.S20.P = Substring_MPPT_Out_Power; 
strData.S20.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 21
Slope_Vdc = slVdc{21};
gate = gat{21};
irr = ir{21}; 
sim('PVArray_MPPT.slx',[],options);
strPow(21) = max(Substring_MPPT_Out_Power);
strData.S21.C = Substring_MPPT_Out_Current; 
strData.S21.P = Substring_MPPT_Out_Power; 
strData.S21.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 22
Slope_Vdc = slVdc{22};
gate = gat{22};
irr = ir{22}; 
sim('PVArray_MPPT.slx',[],options);
strPow(22) = max(Substring_MPPT_Out_Power);
strData.S22.C = Substring_MPPT_Out_Current; 
strData.S22.P = Substring_MPPT_Out_Power; 
strData.S22.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 23
Slope_Vdc = slVdc{23};
gate = gat{23};
irr = ir{23}; 
sim('PVArray_MPPT.slx',[],options);
strPow(23) = max(Substring_MPPT_Out_Power);
strData.S23.C = Substring_MPPT_Out_Current; 
strData.S23.P = Substring_MPPT_Out_Power; 
strData.S23.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 24
Slope_Vdc = slVdc{24};
gate = gat{24};
irr = ir{24}; 
sim('PVArray_MPPT.slx',[],options);
strPow(24) = max(Substring_MPPT_Out_Power);
strData.S24.C = Substring_MPPT_Out_Current; 
strData.S24.P = Substring_MPPT_Out_Power; 
strData.S24.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 25
Slope_Vdc = slVdc{25};
gate = gat{25};
irr = ir{25}; 
sim('PVArray_MPPT.slx',[],options);
strPow(25) = max(Substring_MPPT_Out_Power);
strData.S25.C = Substring_MPPT_Out_Current; 
strData.S25.P = Substring_MPPT_Out_Power; 
strData.S25.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 26
Slope_Vdc = slVdc{26};
gate = gat{26};
irr = ir{26}; 
sim('PVArray_MPPT.slx',[],options);
strPow(26) = max(Substring_MPPT_Out_Power);
strData.S26.C = Substring_MPPT_Out_Current; 
strData.S26.P = Substring_MPPT_Out_Power; 
strData.S26.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 27
Slope_Vdc = slVdc{27};
gate = gat{27};
irr = ir{27}; 
sim('PVArray_MPPT.slx',[],options);
strPow(27) = max(Substring_MPPT_Out_Power);
strData.S27.C = Substring_MPPT_Out_Current; 
strData.S27.P = Substring_MPPT_Out_Power; 
strData.S27.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 28
Slope_Vdc = slVdc{28};
gate = gat{28};
irr = ir{28}; 
sim('PVArray_MPPT.slx',[],options);
strPow(28) = max(Substring_MPPT_Out_Power);
strData.S28.C = Substring_MPPT_Out_Current; 
strData.S28.P = Substring_MPPT_Out_Power; 
strData.S28.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 29
Slope_Vdc = slVdc{29};
gate = gat{29};
irr = ir{29}; 
sim('PVArray_MPPT.slx',[],options);
strPow(29) = max(Substring_MPPT_Out_Power);
strData.S29.C = Substring_MPPT_Out_Current; 
strData.S29.P = Substring_MPPT_Out_Power; 
strData.S29.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 30
Slope_Vdc = slVdc{30};
gate = gat{30};
irr = ir{30}; 
sim('PVArray_MPPT.slx',[],options);
strPow(30) = max(Substring_MPPT_Out_Power);
strData.S30.C = Substring_MPPT_Out_Current; 
strData.S30.P = Substring_MPPT_Out_Power; 
strData.S30.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 31
Slope_Vdc = slVdc{31};
gate = gat{31};
irr = ir{31}; 
sim('PVArray_MPPT.slx',[],options);
strPow(31) = max(Substring_MPPT_Out_Power);
strData.S31.C = Substring_MPPT_Out_Current; 
strData.S31.P = Substring_MPPT_Out_Power; 
strData.S31.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 32
Slope_Vdc = slVdc{32};
gate = gat{32};
irr = ir{32}; 
sim('PVArray_MPPT.slx',[],options);
strPow(32) = max(Substring_MPPT_Out_Power);
strData.S32.C = Substring_MPPT_Out_Current; 
strData.S32.P = Substring_MPPT_Out_Power; 
strData.S32.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 33
Slope_Vdc = slVdc{33};
gate = gat{33};
irr = ir{33}; 
sim('PVArray_MPPT.slx',[],options);
strPow(33) = max(Substring_MPPT_Out_Power);
strData.S33.C = Substring_MPPT_Out_Current; 
strData.S33.P = Substring_MPPT_Out_Power; 
strData.S33.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 34
Slope_Vdc = slVdc{34};
gate = gat{34};
irr = ir{34}; 
sim('PVArray_MPPT.slx',[],options);
strPow(34) = max(Substring_MPPT_Out_Power);
strData.S34.C = Substring_MPPT_Out_Current; 
strData.S34.P = Substring_MPPT_Out_Power; 
strData.S34.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 35
Slope_Vdc = slVdc{35};
gate = gat{35};
irr = ir{35}; 
sim('PVArray_MPPT.slx',[],options);
strPow(35) = max(Substring_MPPT_Out_Power);
strData.S35.C = Substring_MPPT_Out_Current; 
strData.S35.P = Substring_MPPT_Out_Power; 
strData.S35.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 36
Slope_Vdc = slVdc{36};
gate = gat{36};
irr = ir{36}; 
sim('PVArray_MPPT.slx',[],options);
strPow(36) = max(Substring_MPPT_Out_Power);
strData.S36.C = Substring_MPPT_Out_Current; 
strData.S36.P = Substring_MPPT_Out_Power; 
strData.S36.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 37
Slope_Vdc = slVdc{37};
gate = gat{37};
irr = ir{37}; 
sim('PVArray_MPPT.slx',[],options);
strPow(37) = max(Substring_MPPT_Out_Power);
strData.S37.C = Substring_MPPT_Out_Current; 
strData.S37.P = Substring_MPPT_Out_Power; 
strData.S37.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 38
Slope_Vdc = slVdc{38};
gate = gat{38};
irr = ir{38}; 
sim('PVArray_MPPT.slx',[],options);
strPow(38) = max(Substring_MPPT_Out_Power);
strData.S38.C = Substring_MPPT_Out_Current; 
strData.S38.P = Substring_MPPT_Out_Power; 
strData.S38.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 39
Slope_Vdc = slVdc{39};
gate = gat{39};
irr = ir{39}; 
sim('PVArray_MPPT.slx',[],options);
strPow(39) = max(Substring_MPPT_Out_Power);
strData.S39.C = Substring_MPPT_Out_Current; 
strData.S39.P = Substring_MPPT_Out_Power; 
strData.S39.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 40
Slope_Vdc = slVdc{40};
gate = gat{40};
irr = ir{40}; 
sim('PVArray_MPPT.slx',[],options);
strPow(40) = max(Substring_MPPT_Out_Power);
strData.S40.C = Substring_MPPT_Out_Current; 
strData.S40.P = Substring_MPPT_Out_Power; 
strData.S40.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 41
Slope_Vdc = slVdc{41};
gate = gat{41};
irr = ir{41}; 
sim('PVArray_MPPT.slx',[],options);
strPow(41) = max(Substring_MPPT_Out_Power);
strData.S41.C = Substring_MPPT_Out_Current; 
strData.S41.P = Substring_MPPT_Out_Power; 
strData.S41.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 42
Slope_Vdc = slVdc{42};
gate = gat{42};
irr = ir{42}; 
sim('PVArray_MPPT.slx',[],options);
strPow(42) = max(Substring_MPPT_Out_Power);
strData.S42.C = Substring_MPPT_Out_Current; 
strData.S42.P = Substring_MPPT_Out_Power; 
strData.S42.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 43
Slope_Vdc = slVdc{43};
gate = gat{43};
irr = ir{43}; 
sim('PVArray_MPPT.slx',[],options);
strPow(43) = max(Substring_MPPT_Out_Power);
strData.S43.C = Substring_MPPT_Out_Current; 
strData.S43.P = Substring_MPPT_Out_Power; 
strData.S43.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 44
Slope_Vdc = slVdc{44};
gate = gat{44};
irr = ir{44}; 
sim('PVArray_MPPT.slx',[],options);
strPow(44) = max(Substring_MPPT_Out_Power);
strData.S44.C = Substring_MPPT_Out_Current; 
strData.S44.P = Substring_MPPT_Out_Power; 
strData.S44.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 45
Slope_Vdc = slVdc{45};
gate = gat{45};
irr = ir{45}; 
sim('PVArray_MPPT.slx',[],options);
strPow(45) = max(Substring_MPPT_Out_Power);
strData.S45.C = Substring_MPPT_Out_Current; 
strData.S45.P = Substring_MPPT_Out_Power; 
strData.S45.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 46
Slope_Vdc = slVdc{46};
gate = gat{46};
irr = ir{46}; 
sim('PVArray_MPPT.slx',[],options);
strPow(46) = max(Substring_MPPT_Out_Power);
strData.S46.C = Substring_MPPT_Out_Current; 
strData.S46.P = Substring_MPPT_Out_Power; 
strData.S46.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 47
Slope_Vdc = slVdc{47};
gate = gat{47};
irr = ir{47}; 
sim('PVArray_MPPT.slx',[],options);
strPow(47) = max(Substring_MPPT_Out_Power);
strData.S47.C = Substring_MPPT_Out_Current; 
strData.S47.P = Substring_MPPT_Out_Power; 
strData.S47.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 48
Slope_Vdc = slVdc{48};
gate = gat{48};
irr = ir{48}; 
sim('PVArray_MPPT.slx',[],options);
strPow(48) = max(Substring_MPPT_Out_Power);
strData.S48.C = Substring_MPPT_Out_Current; 
strData.S48.P = Substring_MPPT_Out_Power; 
strData.S48.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 49
Slope_Vdc = slVdc{49};
gate = gat{49};
irr = ir{49}; 
sim('PVArray_MPPT.slx',[],options);
strPow(49) = max(Substring_MPPT_Out_Power);
strData.S49.C = Substring_MPPT_Out_Current; 
strData.S49.P = Substring_MPPT_Out_Power; 
strData.S49.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 50
Slope_Vdc = slVdc{50};
gate = gat{50};
irr = ir{50}; 
sim('PVArray_MPPT.slx',[],options);
strPow(50) = max(Substring_MPPT_Out_Power);
strData.S50.C = Substring_MPPT_Out_Current; 
strData.S50.P = Substring_MPPT_Out_Power; 
strData.S50.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 51
Slope_Vdc = slVdc{51};
gate = gat{51};
irr = ir{51}; 
sim('PVArray_MPPT.slx',[],options);
strPow(51) = max(Substring_MPPT_Out_Power);
strData.S51.C = Substring_MPPT_Out_Current; 
strData.S51.P = Substring_MPPT_Out_Power; 
strData.S51.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 52
Slope_Vdc = slVdc{52};
gate = gat{52};
irr = ir{52}; 
sim('PVArray_MPPT.slx',[],options);
strPow(52) = max(Substring_MPPT_Out_Power);
strData.S52.C = Substring_MPPT_Out_Current; 
strData.S52.P = Substring_MPPT_Out_Power; 
strData.S52.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 53
Slope_Vdc = slVdc{53};
gate = gat{53};
irr = ir{53}; 
sim('PVArray_MPPT.slx',[],options);
strPow(53) = max(Substring_MPPT_Out_Power);
strData.S53.C = Substring_MPPT_Out_Current; 
strData.S53.P = Substring_MPPT_Out_Power; 
strData.S53.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 54
Slope_Vdc = slVdc{54};
gate = gat{54};
irr = ir{54}; 
sim('PVArray_MPPT.slx',[],options);
strPow(54) = max(Substring_MPPT_Out_Power);
strData.S54.C = Substring_MPPT_Out_Current; 
strData.S54.P = Substring_MPPT_Out_Power; 
strData.S54.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 55
Slope_Vdc = slVdc{55};
gate = gat{55};
irr = ir{55}; 
sim('PVArray_MPPT.slx',[],options);
strPow(55) = max(Substring_MPPT_Out_Power);
strData.S55.C = Substring_MPPT_Out_Current; 
strData.S55.P = Substring_MPPT_Out_Power; 
strData.S55.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 56
Slope_Vdc = slVdc{56};
gate = gat{56};
irr = ir{56}; 
sim('PVArray_MPPT.slx',[],options);
strPow(56) = max(Substring_MPPT_Out_Power);
strData.S56.C = Substring_MPPT_Out_Current; 
strData.S56.P = Substring_MPPT_Out_Power; 
strData.S56.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 57
Slope_Vdc = slVdc{57};
gate = gat{57};
irr = ir{57}; 
sim('PVArray_MPPT.slx',[],options);
strPow(57) = max(Substring_MPPT_Out_Power);
strData.S57.C = Substring_MPPT_Out_Current; 
strData.S57.P = Substring_MPPT_Out_Power; 
strData.S57.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 58
Slope_Vdc = slVdc{58};
gate = gat{58};
irr = ir{58}; 
sim('PVArray_MPPT.slx',[],options);
strPow(58) = max(Substring_MPPT_Out_Power);
strData.S58.C = Substring_MPPT_Out_Current; 
strData.S58.P = Substring_MPPT_Out_Power; 
strData.S58.V = Substring_MPPT_Out_Voltage; 
toc

%Run sim for str 59
Slope_Vdc = slVdc{59};
gate = gat{59};
irr = ir{59}; 
sim('PVArray_MPPT.slx',[],options);
strPow(59) = max(Substring_MPPT_Out_Power);
strData.S59.C = Substring_MPPT_Out_Current; 
strData.S59.P = Substring_MPPT_Out_Power; 
strData.S59.V = Substring_MPPT_Out_Voltage; 
toc

%Report total power
totPow = sum(strPow);

end
function [powTab] = discpowTab(array, canopy, res, cellSheet,maxEl)
%This function takes in array and canopy stls and returns a solar table 
%which contains the power output of the array for all possible solar Az  
%and El angles (0 - 180). This 'powTab' is then used as a lookup table. 
%res defines the resolution of the generated table (ex: 1 deg resolution).

%% Import files 
[v_A,f_A] = stlread2(array);
[v_C,f_C] = stlread2(canopy);
cellSheet = table2array(cellSheet);

%% Order array vertices 
arrayTri = order_vertices(v_A,f_A);

%% Create cell vertices structure
%Define number of cells
N = 257; 

%Initiate cells
cells = sortcellSheet(cellSheet);

%% Group triangles into cells they belong to 
gtC = cStructure(cells,arrayTri);

%% Group cells (gtC) into subsubstrings they belong to 
%Create matrix containing cells belonging to each substring (row # = str #)
indcellStrings = {[1:3]; [4:6]; [7:9]; [10:12]; [13:15]; [16:19]; [21:28]; [29:35]; [36:40]; [41:43]; [44:47]; [48:52]; [53:56]; [57:60]; [61:66]; [67:72]; [74:80]; [81:86]; [87:93]; [94]; [95:101]; [102:108]; [109:116]; [117:122]; [126:130]; [133:137]; [140:147]; [149:157]; [158]; [159:160]; [161:164]; [165:170]; [171:176]; [177:179]; [180:182]; [183:188]; [189:194]; [195:197]; [198:201]; [202:204]; [205:206]; [207:209]; [210:215]; [216:221]; [222:227]; [228:233]; [234:237]; [238:241]; [242:245]; [246:247]; [248:250]; [251:254]; [255:257]};

%Call function 'toStrings' 
ssStrings = toStrings(gtC,indcellStrings); 


%% Group subsubstrings into substrings they blong to 
%Create matrix containing subsubstrings belonging to each substring (row # = str #)
%indsubStrings = {[53:-1:49]; [48:-1:44]; [43:-1:39]; [38:-1:34]; [33:-1:29]; [28:-1:25]; [24:-1:21]; [20:-1:16]; [15:-1:11]; [10:-1:6]; [5:-1:1]};

%Define number of substrings 
%sS = 11; 

%Define number of cells in each substring
ncsS = [3,3,3,3,3,5,8,7,5,3,4,5,4,4,6,7,7,6,7,1,7,7,8,9,7,7,9,9,1,2,4,6,6,3,3,6,6,3,4,3,2,3,6,6,6,6,4,4,4,2,3,4,3];

%Call function 'toStrings'
%sStrings = toStrings(ssStrings, indsubStrings);

%% Loop over all Az and El to generate powTab

for El = maxEl:maxEl %El in row
    
    for Az = 1:res:181 %Az in col 
        
        %Create sun vector 
        sunVector = create_sun_vector_simple(Az,El);

        %Project canopy vertices onto sun vector
        projCanopy = project_onto_sun_vector(sunVector,v_C);

        %Project array triangles onto sun vector 
        projArray = project_onto_sun_vector(sunVector,arrayTri);

        %Eliminate array triangles containing canopy points
        newArray = remove_shaded_triangles(projCanopy,projArray,arrayTri);
        
        %Group new array triangles into a shaded cells structure 
        sgtC = cStructure(cells,newArray);
        
        %Store 'sgtC' field names for easy access
        stcN = fieldnames(sgtC);
        
        %Define cell indices in strings 
        %cCount = {[257:-1:242];[241:-1:216];[215:-1:198];[197:-1:177];[176:-1:158];[157:-1:126];[125:-1:95];[94:-1:67];[66:-1:44];[43:-1:16];[15:-1:1]};

        %Define cell indices in substrings
        cCount = {[1:3]; [4:6]; [7:9]; [10:12]; [13:15]; [16:19]; [21:28]; [29:35]; [36:40]; [41:43]; [44:47]; [48:52]; [53:56]; [57:60]; [61:66]; [67:72]; [74:80]; [81:86]; [87:93]; [94]; [95:101]; [102:108]; [109:116]; [117:122]; [126:130]; [133:137]; [140:147]; [149:157]; [158]; [159:160]; [161:164]; [165:170]; [171:176]; [177:179]; [180:182]; [183:188]; [189:194]; [195:197]; [198:201]; [202:204]; [205:206]; [207:209]; [210:215]; [216:221]; [222:227]; [228:233]; [234:237]; [238:241]; [242:245]; [246:247]; [248:250]; [251:254]; [255:257]};
        
        %Find power output of each cell in 'sgtC' structure
        for i = 1:N        
            
            %Obtain power output of 'stC' i 
            cellOutput(i) = solarPower(sunVector,sgtC.(stcN{i})); 
      
            %Obtain area of cell i in 'cells' structure
            cellAreas(i) = return_areas(gtC.(stcN{i}));
            
            %Obtain irradiance for cell i 
            if cellAreas(i) > 0                
                cellIrr(i) = cellOutput(i)/cellAreas(i);                 
            else
                cellIrr(i) = 0;
            end
            
        end
            
        %initiate total power output 
        totPow = 0; %[W] 
            
        %Obtain power output for substring j (1-53)
        for j = 1:size(ncsS, 2)
            totPow = totPow + mpptSim(ncsS(j),cellIrr(cCount{j}));
        end
            
        %Input power value into powerTable at current El,Az
        powTab(El,Az) = totPow; 
            
    end
    
end  

end


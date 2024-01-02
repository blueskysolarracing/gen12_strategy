% Step 1: run script

%Define total number of folders desired
totFolders = 31;

%Copy contents of m1 to other folders
for i = 2:totFolders
    copyfile('m1',append('m',int2str(i)));
end

clear
clc
% Step 1: change lines 5 and 6
% Step 2: run multiRun.m script

clear;
clc;
pause(2);

delete(gcp('nocreate'));
parpool(4);
BasePath = 'C:\gen12_blueskystrategy\ArraySims\C\Run'; % <- change this to current path containing multiRun.m
DirList  = dir(BasePath);
DirList  = DirList([DirList.isdir]); 
for iDir = 1:numel(DirList)
  aDir = fullfile(BasePath, DirList(iDir).name);
  if aDir(length(aDir)) ~= '.'       
      cd(aDir);
      !matlab -r dayPower('../wscIrr.csv') &   
  end  
  pause(12);
end
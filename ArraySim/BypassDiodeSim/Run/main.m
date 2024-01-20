function [] = main(nCores, irrCsv)
% Run a distributed simulink simulation
% @param nCores: Number of available cores to run on
% @param irrCsv: Name of the n x num_cells csv generated from step 2

  pause(2);

  delete(gcp('nocreate'));
  parpool(nCores);
  BasePath = pwd; % <- change this to current path containing multiRun.m
  DirList  = dir(BasePath);
  DirList  = DirList([DirList.isdir]); 
  for iDir = 1:numel(DirList)
    aDir = fullfile(BasePath, DirList(iDir).name);
    if aDir(length(aDir)) ~= '.'       
        cd(aDir);
        !matlab -r dayPower(strcat("../", irrCsv)) &   
    end  
    pause(12);
  end
end
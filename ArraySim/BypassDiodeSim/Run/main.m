function [] = main(nCores, irrCsv, numFolders)
% Run a distributed simulink simulation
% @param nCores: Number of available cores to run on
% @param irrCsv: Name of the n x num_cells csv generated from step 2
% @param numFolders: Number of folders that can run the dayPower script in parallel

  pause(2);

  delete(gcp('nocreate'));
  parpool(nCores);
  BasePath = pwd;

  counter = 1;
  cellIrr = (load(irrCsv));
  numRows = size(cellIrr,2);
  stepSize = round(numRows / numFolders);

  for iDir = 1:numFolders
    aDir = fullfile(BasePath, "/", strcat("m", int2str(iDir)));
    disp(aDir);
    cd(aDir);
    endingStep = counter+stepSize;

    if iDir == numFolders
      endingStep = numRows;
    end
    
    csvPath = strcat('''', '../', irrCsv, '''');

    command = strcat('matlab -r dayPower(', csvPath, ', ', num2str(counter), ', ', num2str(endingStep), ') &');
    system(command);
  
    counter = endingStep + 1; 
    pause(12);
  end
end
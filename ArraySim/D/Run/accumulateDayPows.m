% This function assembles all daypow files, and interpolates to create
% the powerfactor csv for the race simulation

numFolders = 23; % Change based on number of m__ folders
interval = 362; % Change based on the number of rows covered by each m__ folder
originalPower = 1000; % The sun irradiance used in wscAngles
power = [];

% Assemble one array of all dayPow values
for i=1:numFolders
    nameFolder = ['m', num2str(i)];
    cd(nameFolder);
    
    nameFile = [num2str(i*interval), '_dayPow.csv'];
    tempPower = readmatrix(nameFile);
    power = [power; tempPower((i-1)*interval+1 : interval * i)];
    cd("..");
end

% Get the power factors
power = power / 1000;

% Split the columns
power = reshape(power, [46, 181]); % The size depends on the interval of azimuth, elevation increments

%writematrix(power, "accumulatedPowerFactors.csv");

% Interpolate along each column
interpolatedColumnPowerFactors = [];
for i=1:181
    x = 1:46;
    y = power(:,i);
    xi = 1:0.5:46;
    
    yi = interp1(x, y, xi, 'linear');
    yi = transpose(yi);
    interpolatedColumnPowerFactors = [interpolatedColumnPowerFactors, yi];
end

% Interpolate along each row
interpolatedRowPowerFactors = [];
for i=1:91
    x = 1:181;
    y = interpolatedColumnPowerFactors(i,:);
    xi = 1:0.5:181;

    yi = interp1(x, y, xi, 'linear');
    interpolatedRowPowerFactors = [interpolatedRowPowerFactors; yi];
end

writematrix(interpolatedRowPowerFactors, "powerfactor.csv");

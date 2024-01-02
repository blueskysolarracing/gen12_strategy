% Step 1: adjust lines 10, 11, 13, 14, 15, 16, 17, 18, 22, and 58 accordingly
% Step 2: run this script

% Define parameters
wscAngles = zeros(180,4);

%Define UTC,Lat,Lon and Alt vectors
In = datenum({'22-Oct-2023 20:29:02';'23-Oct-2023 09:09:06'}); % <- change this to desired sunrise;sunset times (in UTC)
Out = datevec(In(1):1/340:In(2)); % <- change middle fraction such that size(Out,1) = 180
UTC = datenum(Out); 
lat = -23.6980; % <- change to correct latitude of location [deg]
lon = 133.8807; % <- change to correct longitude of location [deg]
alt = 0.545; % <- change to correct altitude of location [km]
chargeStandAngle = 75; % <- change to correct max charge stand angle [deg]
raceStartTime = 8; % <- change to correct race start time [24hr format]
raceEndTime = 16; % <- change to correct race end time [24hr format]

%Define quanitites 
N = size(UTC,1);
bear = 160.92; % <- change to correct bearing of car [deg]

%Initiate stAz and stEl
stAz = [];
stEl = [];
stIrr = [];
stTime = [];

%Integrate over all time
for i = 1:N 
    
    %Update UTC 
    newUTC = UTC(i); 
    
    %Get AZ and EL angles
    [Az,El] =  SolarAzEl(newUTC,lat,lon,alt);
    
    %Get new Az
    Az = (Az - (bear-180));
    
    if Az > 180        
        Az = 180- (Az - 180);        
    end
    
    if Az < 0         
        Az = abs(Az);        
    end
    
    if Az > 180        
        Az = 180- (Az - 180);        
    end
    
    %Calculate Irr for given Az and El 
    Irr = 1*(1307.075509)*(exp(-0.207/sind(El)))*(0.136+sind(El));
    
    %Change time back to local time
    newT = ((Out(i,4)+9.5)+(Out(i,5)/60)); % <- change UTC->Local Time correction in Column 23 [deg]
    if newT > 24
        newT = newT-24;
    elseif newT == 24
        newT = 12;
    end

    % Adjust Az and El for charging periods
    if newT < raceStartTime ||  newT > raceEndTime
        Az = 90;
        if El < (90-chargeStandAngle)
            El = El + chargeStandAngle;
        else
            El = 90;
        end
    end
    
    %Store Az and El angles
    wscAngles(i,1) = Az;
    wscAngles(i,2) = El;
    wscAngles(i,3) = Irr;
    wscAngles(i,4) = newT;
    
end

%Export wscAngles.csv
writematrix(wscAngles,'wscAngles.csv');

%Copy wscAngles.csv to step B
currDir = pwd;
currDir = erase(currDir,'\A\Run');
copyfile('wscAngles.csv',append(currDir,'/B/Run/wscAngles.csv'));
function [stAngles] = dayAzElIrr() 
%%% This function takes in a lat, lon and alt of a location and returns an
%%% array contaiing the pairs of Az and El angles of the sun from 9am to 6
%%% pm with an increment of 10 minutes

%Define UTC,Lat,Lon and Alt vectors
In = datenum({'12-Oct-2019 20:38:00';'13-Oct-2019 9:04:00'});
Out = datevec(In(1):1/349:In(2)); %Increments of 4.12 minutes 
UTC = datenum(Out);
lat = -23.6980;
lon = 133.8807;
alt = 0.545;

%Define quanitites 
N = size(UTC,1);
bear = -16.0811; %163.9189-180 becuase car is facing south 

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
    Az = (Az - bear);
    
    if Az < 0 
        
        Az = 360 + Az;
        
    end

    %Calculate Irr for given Az and El 
    Irr = 1*(1307.075509)*(exp(-0.207/sind(El)))*(0.136+sind(El));
    
    %Store Az and El angles 
    stAz = [stAz; Az];
    stEl = [stEl; El];
    stIrr = [stIrr; Irr];
    stTime = [stTime; ((Out(i,4)+9.5)+(Out(i,5)/60))];
    
end

stAngles(:,1) = stAz;
stAngles(:,2) = stEl;
stAngles(:,3) = stIrr;
stAngles(:,4) = stTime;

end
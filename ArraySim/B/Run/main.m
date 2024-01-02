function [newVelocity,totalTime,SOCav,SOCfin] = main(initialVelocity,routeData,overnightData,solarLookup,rollingRes,regenEff,arrayAngle,stopTime)
%This  function takes in an initial velocity in [km/h] and returns an optimal maximum velocity in [km/h] 

%Define tables to be used 
routeTable = table2array(routeData); 
overnightTable = table2array(overnightData);
solarTable = table2array(solarLookup);
rollingTable = table2array(rollingRes);

%Define tables to be used 
%routeTable = routeData; 
%overnightTable = overnightData;
%solarTable = solarLookup;
%rollingTable = rollingRes;

%Define current speed 
currentSpeed = initialVelocity; 

%Define binary value 
binary = 1;

%While loop for velocity  
while binary == 1
    
    %Obtain battery charge at current speed
    [battCharge, finalTime] = batteryCharge(routeTable,overnightTable,solarTable,rollingTable,currentSpeed,regenEff,arrayAngle,stopTime);
    
    %Conditionals 
    if any(battCharge(:)<0) %(adopted from http://bit.ly/3chnATp)
        
        binary = 0;
        
    else
        
        currentSpeed = currentSpeed + 0.5; %[km/h] 
        
    end
    
end 

%Return new veloctiy
newVelocity = currentSpeed-0.5; 

%Update battry charge and endtime at optimal speed
[battCharge, finalTime] = batteryCharge(routeTable,overnightTable,solarTable,rollingTable,newVelocity,regenEff,arrayAngle,stopTime);

%Plot charge vs. distance 
% f = figure('visible','off');
% plot(routeTable(:,4),battCharge);
% title('Battery Charge [kWh] vs. Distance [km]');
% ylabel('Battery Charge [kWh]');
% xlabel('Distance [km]');
% saveas(f,sprintf('regenEff = %f.png', regenEff));
% hold off 

%Calculate time to completing of race in hours (adopted from http://bit.ly/32A65cA) 
T_1 = datevec(datenum('12-Oct-2019 23:00:00')); 
T_2 = finalTime; 
%totaTime = seconds(etime(T_2,T_1));
%totaTime.Format = 'hh:mm:ss.SSS'; %(adopted from http://bit.ly/2Pxt9Do) 

%totalTime = totaTime;

totalTime = (etime(T_2,T_1))/3600; 

%Report average battery SOC 
SOCav = (mean(battCharge)/5)*100; 

%Report final battery SOC 
SOCfin = (battCharge(length(battCharge))/5)*100; 

end
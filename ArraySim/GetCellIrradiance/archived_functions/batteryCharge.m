function [batteryEnergy,finalUTC,totEn] = batteryCharge(routeTable,overnightTable,solarTable,rollingRes,newSpeed,regenEff,arrayAngle,stopTime,powTab)
%This function calculates final battery charge at end of race

%Define tables to be used 
routeTable = table2array(routeTable); 
overnightTable = table2array(overnightTable);
solarTable = table2array(solarTable);
rollingTable = table2array(rollingRes);
powTable = table2array(powTable);

%Define total energy from sun
totEn = 0;

%Define size 
N = size(routeTable,1);

%Define initial battery charge in [kwH] 
tempCharge = 5;

%Begin time
UTC = datenum('4-Aug-2022 14:00:00');  

%Define all control stop positions
controlStops = [2250,4500,6750,9000]; 

%Begin for loop in route.csv
for i = 1:(N-1)
    
    %Check if in new time zone 
    if i == 7377
        
        %Adjust time by 1 hour
        UTC(i) = addtodate(UTC(i),-3600,'second'); 
        
    end
    
    %Define lat, lon, and alt
    Lon = routeTable(i,2);
    Lat = routeTable(i,1);
    Alt = 0.001*routeTable(i,3);
        
    %Check for control stop 
    if any(controlStops(:)==i)
               
        %Add in charge from control stop in kWh        
        controlSolarIn = powTab(91,91)*0.96*1800*(1/(3.6e6)); 
        
        %Add to totEn
        totEn = totEn + controlSolarIn;
        
        %Add this charge to current battery charge 
        tempCharge(i) = tempCharge(i) + controlSolarIn;
        
        %Bring down to 5 kWh if exceeds
        if tempCharge(i) > 5 
            
            tempCharge(i) = 5; 
            
        end
        
        %Add 30 minutes to current time 
        UTC(i) = addtodate(UTC(i),1800,'second'); 
        
    end
    
    %Overnight charge
    
    %Create datevec from UTC
    tempUTC = datevec(UTC(i)); 
    
    %Find current day 
    day = tempUTC(3);
    
    %Define overnight charge [kWh]
    oCharge = 0; 
    
    %Check if its approximately stop time  
    if tempUTC(4) == ((stopTime+12) + 5)  && tempUTC(5) >= 0
               
        %Create overnight UTC datenum vector
        onUTC(1) = UTC(i);
        
        %Find current Az and El
        [Az,El] = SolarAzEl(UTC(i),Lat,Lon,Alt);
        
        %Define upadting index 
        j = 1; 
        
        %Find total overnight charge 
        
        %Sunset
        while El > 0 
            
            if (90-El) <= arrayAngle
                   
                %Evaluate solar energy in [kWh]
                solarIn = powTab(91,91)*0.96*60*(1/(3.6e6)); %Increments of 1 mins
                
                %Add to totEn
                totEn = totEn + solarIn;
        
                %Update overnight charge [kWh] 
                oCharge = oCharge + solarIn; 
            
                %Update time by 5 minutes 
                onUTC(j+1) = addtodate(onUTC(j),60,'second');
            
                %Update Az and El 
                [Az,El] = SolarAzEl(onUTC(j+1),Lat,Lon,Alt);
            
                %Update j 
                j = j+1; 
                
            else
                   
                %Evaluate solar energy in [kWh]
                solarIn = powTab((El+1+arrayAngle),91)*0.96*60*(1/(3.6e6)); %Increments of 1 mins
                
                %Add to totEn
                totEn = totEn + solarIn;
            
                %Update overnight charge [kWh] 
                oCharge = oCharge + solarIn; 
            
                %Update time by 5 minutes 
                onUTC(j+1) = addtodate(onUTC(j),60,'second');
            
                %Update Az and El 
                [Az,El] = SolarAzEl(onUTC(j+1),Lat,Lon,Alt);
            
                %Update j 
                j = j+1;                 
       
            end
            
        end
        
        %Create onUTC daetvec
        dvonUTC = datevec(onUTC(j)); 
        
        %Nighttime
        while El < 0   
            
            %Update time by 5 minutes 
            onUTC(j+1) = addtodate(onUTC(j),60,'second');
            
            %Update onUTC daetvec
            dvonUTC = datevec(onUTC(j+1)); 
            
            %Update Az and El 
            [Az,El] = SolarAzEl(onUTC(j+1),Lat,Lon,Alt);
            
            %Update j 
            j = j+1; 
                       
        end
        
        %Sunrise        
        while El > 0 && dvonUTC(4) < 14 
            
            if (90-El) <= arrayAngle
                   
                %Evaluate solar energy in [kWh]
                solarIn = powTab(91,91)*0.96*60*(1/(3.6e6)); %Increments of 1 mins
                
                %Add to totEn
                totEn = totEn + solarIn;
            
                %Update overnight charge [kWh] 
                oCharge = oCharge + solarIn; 
            
                %Update time by 5 minutes 
                onUTC(j+1) = addtodate(onUTC(j),60,'second');
            
                %Update Az and El 
                [Az,El] = SolarAzEl(onUTC(j+1),Lat,Lon,Alt);
            
                %Update j 
                j = j+1; 
                             
            else
               
                %Evaluate solar energy in [kWh]
                solarIn = powTab((El+1+arrayAngle),91)*0.96*60*(1/(3.6e6)); %Increments of 1 mins
                
                %Add to totEn
                totEn = totEn + solarIn;
            
                %Update overnight charge [kWh] 
                oCharge = oCharge + solarIn; 
            
                %Update time by 1 minute 
                onUTC(j+1) = addtodate(onUTC(j),60,'second');
            
                %Update Az and El 
                [Az,El] = SolarAzEl(onUTC(j+1),Lat,Lon,Alt);
            
                %Create onUTC daetvec
                dvonUTC = datevec(onUTC(j)); 
            
                %Update j 
                j = j+1; 
                
            end
            
        end        
                
        %Update day by one day (adopted from http://bit.ly/38XFjNo)
        UTC(i) = datenum(sprintf('%f-Aug-2022 14:00:00', fix(day+1)));
              
        %Add overnight charge  
        tempCharge(i) = tempCharge(i) + (oCharge);   
        
        %Bring battery charge down to 5kWh if its over  
        if tempCharge(i) > 5 
            
            tempCharge(i) = 5; 
            
        end
        
    end
            
    %Define distance in [km]
    dist = (routeTable(i+1,4) - routeTable(i,4)); 
    
    %Define change in elevation
    deltaElv = (routeTable(i+1,3)-routeTable(i,3));
    
    %Define delta time in [s] 
    deltaTime = (dist*(1000))/(newSpeed*(1000/3600));
    
    %Update UTC
    UTC(i+1) = updateTime(dist,newSpeed,UTC(i));
    
    %Get AZ and EL angles
    [Az,El] =  SolarAzEl(UTC(i),Lat,Lon,Alt);
    
    %Az if condition
    if Az > 180
        
        Az = 180- (Az - 180);
        
    end
    
    %Shift Az depending on bearing angle 
    
    %Find bearing angle 
    bear = findBear(routeTable(i,1),routeTable(i,2),routeTable(i+1,1),routeTable(i+1,2)); 
    
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
       
    %Evaluate solar energy in [kWh]
    solarIn = powTab((El+1),(Az+1))*0.96*deltaTime*(1/(3.6e6)); 
    
    %Add to totEn
    totEn = totEn + solarIn;
    
    %Evaluate energy losses in [kWh]
    totalLoss = energyLoss(newSpeed,dist,rollingTable,deltaElv,regenEff);
    
    %Conditionals for battery charge 
    if totalLoss > 0.97*solarIn 
        
        %Define energy needed from battery in [kWh]
        energyNeeded = totalLoss-(0.97*(solarIn)); 
        
        %Update battery charge 
        tempCharge(i+1) = tempCharge(i) - (energyNeeded/0.98); 
        
    elseif totalLoss == 0.97*solarIn
       
        %Update battery charge 
        tempCharge(i+1) = tempCharge(i);
        
    elseif totalLoss < 0.97*solarIn && totalLoss > 0
        
        %Define fraction of solarIn energy that goes to motor 
        frac = totalLoss/(0.97*(solarIn));
        
        %Define energy to go to battery 
        energytoBatt = solarIn*(1-frac); 
        
        %Update battery charge 
        tempCharge(i+1) = tempCharge(i) + (energytoBatt*0.98); 
        
    elseif totalLoss < 0 
        
        tempCharge(i+1) = tempCharge(i) - (0.98*totalLoss); 
        
        if tempCharge(i+1) > 5 
            
            tempCharge(i+1) = 5;
            
        end
        
    end
    
end
    
batteryEnergy = tempCharge;
finalUTC = datevec(UTC(11162));

end



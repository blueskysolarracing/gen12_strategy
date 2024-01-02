function [battCh,totTime,socAv,socFin,totEn] = varSweep(route1,overnightdata,finsolarlookupErowAzcol,rollingresistance,inVel,regEff,arrayAngle,N,powTab) 

%N is max stop time

for i = 4:N
    
    [battCh(:,(i-3)),finTime,totEn(i-3)] = batteryCharge(route1,overnightdata,finsolarlookupErowAzcol,rollingresistance,inVel,regEff,arrayAngle,i,powTab);
    
    %Calculate time to completion of race in hours (adopted from http://bit.ly/32A65cA) 
    T_1 = datevec(datenum('4-Aug-2022 14:00:00')); 
    T_2 = finTime; 

    %Find total time 
    totTime(i-3) = (etime(T_2,T_1))/3600; 

    %Report average battery SOC 
    socAv(i-3) = (mean(battCh(:,i-3))/5)*100; 

    %Report final battery SOC 
    socFin(i-3) = ((battCh(length(battCh(:,1)),(i-3)))/5)*100; 
    
end


end
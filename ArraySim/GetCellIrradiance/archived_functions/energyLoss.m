function [totalLoss] = energyLoss(speed,dist,rollingRes,deltaElv,regenEff)

%Convert speed to [m/s]
speed = speed*(1000/3600);

%Define delta time in [s]
deltaTime = (dist*(1000))/(speed);

%Compute component losses in [kwH]
aeroLoss = (0.5)*(1.177)*((speed)^3)*(0.12)*deltaTime*(1/(3.6e6));
rollingLoss = (findRolRes(speed,rollingRes))*deltaTime*(1/(3.6e6));
electricalLoss = 20*deltaTime*(1/(3.6e6));
elevLoss = (300*9.81*deltaElv)*(1/(3.6e6));

%Sum up all losses in [kWh]
if elevLoss > 0
    totalLoss = aeroLoss + rollingLoss + electricalLoss + elevLoss;
else
    totalLoss = aeroLoss + rollingLoss + electricalLoss + ((regenEff/100)*elevLoss);
end

end
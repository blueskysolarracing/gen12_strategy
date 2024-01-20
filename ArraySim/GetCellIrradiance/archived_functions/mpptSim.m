function [maxPower] = mpptSim(slVDC,Gate,Irr)
%Initiate inputs
Slope_Vdc = slVDC;
gate = Gate;
irr = Irr;

%Run the sim
sim("PVArray_MPPT.slx");

%Get the maximum power
maxPower = max(Substring_MPPT_Out_Power);

end
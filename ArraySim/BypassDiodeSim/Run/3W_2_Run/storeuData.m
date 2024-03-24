function [] = storeuData(dayPow, out_name)
% Save result of simulink simulation
% @param dayPow: Data to save
% @param out_name: output csv name

    N = size(dayPow,1); 
    dayPow = array2table(dayPow);
    writetable(dayPow,out_name);
    
end
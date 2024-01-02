function [] = storeuData(dayPow)

%Save dayPow file 
N = size(dayPow,1); 
dayPow = array2table(dayPow);
writetable(dayPow,'dayPow.csv');

end
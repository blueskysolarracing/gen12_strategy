function [] = storeuData(dayPow)

%Save dayPow file 
N = size(dayPow,1); 
dayPow = array2table(dayPow);
writetable(dayPow,append(num2str(N),'_dayPow.csv'));

end
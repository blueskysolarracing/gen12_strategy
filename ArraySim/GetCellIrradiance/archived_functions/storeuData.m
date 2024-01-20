function [] = storeuData(dayPow,stringData)

%Store 'stringData' field names for easy access
rN = fieldnames(stringData);

%Save dayPow file 
dayPow = array2table(dayPow);
writetable(dayPow,append(rN{1},'_dayPow.csv'));

%Initiate string data
stData = [];

%Save string data
for i = 1:size(rN,1)
    sN = fieldnames(stringData.(rN{i}));
    for j = 1:53
        cE = transpose(stringData.(rN{i}).(sN{j}).C);
        pE = transpose(stringData.(rN{i}).(sN{j}).P);
        vE = transpose(stringData.(rN{i}).(sN{j}).V);
        tempData = [[str2double(append(num2str((sscanf(rN{i},'R%d'))),'0',num2str((sscanf(sN{j},'S%d'))),'0','99')), cE];[str2double(append(num2str((sscanf(rN{i},'R%d'))),'0',num2str((sscanf(sN{j},'S%d'))),'0','112')), pE];[str2double(append(num2str((sscanf(rN{i},'R%d'))),'0',num2str((sscanf(sN{j},'S%d'))),'0','118')), vE]];
        stData = [stData;tempData];  
    end
end

%Save stData file 
stData = array2table(stData);
writetable(stData,append(rN{1},'_stData.csv'));

end
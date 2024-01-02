function [larStrings] = toStrings(subStruct, indStruct)
%This function takes in a substructure 'subStruct' and a matrix containing indices
%of substructures that belong together in the larger structure 'indStruct'
%and returns the new large structure 'larStruct'

%Define number of subsubstrings
S = size(indStruct, 1);

%Initiate larStrings structure 
larStrings.S1 = [1];

%Store 'gtC' field names for easy access
tN = fieldnames(subStruct);

%Loop over every string and create it 
for i = 1:S 
    
    %Find size of cStrings(i,:) 
    sizecS = size(indStruct{i});
    
    sizecS = sizecS(2);
    
    %Loop over every entry of cStrings(i,:)
    for j = 1:sizecS
        
        %Obtain index for tN
        curIndx = indStruct{i}(j);
        
        %Obtain gtCs to add to string i 
        atC = subStruct.(tN{curIndx});
        
        %Update field values
        if j == 1 
            %Initiate strings struct with first entry 
            larStrings = setfield(larStrings, ['S' num2str(i)], atC); 
        else
            %Append other gtCs to string i
            larStrings.(strcat('S',num2str(i))) = [larStrings.(strcat('S',num2str(i))); atC];
        end
            
    end
    
end

end 
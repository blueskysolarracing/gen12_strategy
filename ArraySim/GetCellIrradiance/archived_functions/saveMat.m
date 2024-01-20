function [] = saveMat(Mat)
%This function takes in a 3D matrix and saves it to file 

%Begin for loop 
for b=1:size(Mat, 3)
    A = Mat(:,:,b); 
    save(sprintf('Q_%f.txt', b), 'A', '-ASCII', '-double');
end

end
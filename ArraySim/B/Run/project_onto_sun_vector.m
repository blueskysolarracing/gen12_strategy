function [points] = project_onto_sun_vector(sun_vector,pProject)
%Partially copied from
%https://www.mathworks.com/matlabcentral/answers/395296-projection-of-a-3d-point-on-a-2d-plane 

%Define plane variables
a = sun_vector(1);
b = sun_vector(2); 
c = sun_vector(3); 
d = 0;

%For loop
N = size(pProject,1); 
A=[1 0 0 -a; 0 1 0 -b; 0 0 1 -c; a b c 0];

for i = 1:N
    
    x = pProject(i,1);
    y = pProject(i,2);
    z = pProject(i,3);
    
    B=[x; y; z; d];
    
    X=A\B;
    x_p=X(1);
    y_p=X(2);
    z_p=X(3);
    
    points{i} = [x_p,y_p,z_p];

end

points = cell2mat(transpose(points));

end
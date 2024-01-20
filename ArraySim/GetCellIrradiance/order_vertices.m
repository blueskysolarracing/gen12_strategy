function vertices_fixed=order_vertices(vertices,faces)
    
    sizes=size(faces);
   
    vertices_fixed=zeros(sizes(1)*3,3);
    for i=1:(sizes(1))
        a=faces(i,1);
        b=faces(i,2);
        c=faces(i,3);
        vertices_fixed(i*3-2,:)=vertices(a,:);
        vertices_fixed(i*3-1,:)=vertices(b,:);
        vertices_fixed(i*3,:)=vertices(c,:);
        
        
    end

end

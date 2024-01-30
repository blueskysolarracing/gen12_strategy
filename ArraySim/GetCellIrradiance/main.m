function [] = main(nCells, direction, bearing, outputName, canopyPath, arrayCellsPath, positionsPath)
    % Main function that computes the n x d csv containing the irradiances on
    % each cell of the array at all azimuth/elevation combos described in the
    % csv from step 1

    % Parameters:
    % nCells: Number of cells of the array
    % direction: A string indicating the direction of the nose of the array. Can be one of "+x", "-x", "+y", "-y"
    % Bearing: the angle of the nose of the car clockwise from true north.
    % outputName: output name of the csv
    % canopyPath: A string indicating the path to the canopy stl file
    % arrayCellPath: A string indicating the path to the array stl files. Note
    % that each stl file must be numbered in order and have the same prefix
    % positionsPath: A string indicating the path to the 4 column csv generated in step 1

    % Example: main(257, "-x", 180.0, "wscIrr.csv", "./ArraySTLs/canopy.stl", "./ArraySTLs/NA", "./wscAngles.csv")

    % Output: 
    % An n x d csv named outputName

    wscAngles = readmatrix(positionsPath);
    N = size(wscAngles,1); 
    wscIrrCell = cell(N, 1);

    % Import canopy and array cells
    numberOfCells = nCells;
    [canopyMesh, arrayCellMeshes, canopyPoints, arrayCellPoints] = import_canopy_array_stl(canopyPath, arrayCellsPath, numberOfCells);

    % Plot the array and canopy - testing purposes
    highlightCells = [103, 104];
    plotArrayCanopy(canopyPoints, arrayCellPoints, highlightCells, 0);

    % Find the largest coordinate value along any dimension
    largestCoordinate = max(cat(1, canopyPoints, arrayCellPoints{:}), [], "all");

    % Loop over all sun positions described in wscAngles and find irradiance on each cell
    for i = 1:N
    
        %Extract sun data 
        Az = wscAngles(i,1);
        El = wscAngles(i,2);
        Irr = wscAngles(i,3);

        % Create sun plane
        sunPlane = create_sun_plane(Az, El, direction, bearing, largestCoordinate);

        %Loop over every cell and remove shaded triangles
        sCells = remShadCellStruc(sunPlane, canopyMesh, arrayCellMeshes, arrayCellPoints, i);

        % Find power output of each cell in W/m^2
        wscIrrCell{i} = cellData(sunPlane,sCells,arrayCellPoints,Irr);
        
        disp(append("Done Row ", int2str(i)));

    end

    % Export output csv
    writecell(wscIrrCell,outputName);

end

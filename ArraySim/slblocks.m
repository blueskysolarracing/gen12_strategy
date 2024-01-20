function blkStruct = slblocks
% This function specifies that the library 'SolarCellLibrary'
% should appear in the Library Browser with the 
% name 'Solar'

    Browser.Library = 'SolarCellLibrary';
    % 'mylib' is the name of the library

    Browser.Name = 'Solar';
    % 'My Library' is the library name that appears
    % in the Library Browser

    blkStruct.Browser = Browser;
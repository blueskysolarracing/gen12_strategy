% Generate a bypass diode simulink model
function generate_bypass_diode_model(model_name, string_config)
    % @param model_name: name of the simulink model
    % @param string_config: A json file indicating the number of cells in a
    % substring and number of substrings in a cell
    
    % Load the json file describing string layout
    fid = fopen(string_config);
    raw = fread(fid,inf);
    str = char(raw);
    fclose(fid);
    string_layout = jsondecode(transpose(str));
    
    % Initialize simulink model
    open_system(new_system(model_name));

    % Create each string
    fn = fieldnames(string_layout);
    for i=1:size(fn,1)
        substrings = string_layout.(fn{i});

        % Irradiance import block
        import_irradiance = strcat(model_name, "/irr_string", int2str(i));
        ss_block = add_block("SolarCellLibrary/2x1_Substring", import_irradiance);
        import_irradiances_block(i) = add_block("simulink/Sources/From Workspace", import_irradiance);
        set_param(import_irradiance, 'VariableName', strcat("irr_diode_string", int2str(i)));
        
        % Demux block
        demux_block = strcat(model_name, "/demux_string", int2str(i));
        demuxes(i) = add_block("simulink/Commonly Used Blocks/Demux", demux_block);
        set_param(demux_block, "Outputs", int2str(sum(substrings, 1)));
        
        % Connect demux to irradiance import block
        import_irradiance_out = get_param(import_irradiance, 'PortHandles').Outport;
        demux_in = get_param(demux_block, 'PortHandles').Inport;
        add_line(model_name, import_irradiance_out, demux_in);

        % Create each substring
        for j=1:size(substrings,1)
            num_cells = substrings(j);

            %add_block("")

        end

    end
end
% Generate a bypass diode simulink model
function generate_bypass_diode_model(model_name, string_config)
    % @param model_name: name of the simulink model
    % @param string_config: A json file indicating the layout of the
    % strings on the array

    %   {
    %       "String1": Array of substring sizes    
    %       .
    %       .
    %       .
    %       "StringN": Array of substring sizes
    %   }
    
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
    num_strings = size(fn,1);
    
    demuxes = zeros(1,num_strings);
    import_irradiances = zeros(1,num_strings);
    current_sensors = zeros(1,num_strings);
    products = zeros(1,num_strings);
    voltage_sensors = zeros(1,num_strings);
    voltage_sweeps = zeros(1,num_strings);
    to_workspaces = zeros(1,num_strings);

    for i=1:num_strings
        substrings = string_layout.(fn{i});

        % Irradiance import block
        import_irradiance_name = strcat(model_name, "/irr_string", int2str(i));
        import_irradiances(i) = add_block("simulink/Sources/From Workspace", import_irradiance_name);
        set_param(import_irradiances(i), 'VariableName', strcat("irr_diode_string", int2str(i)));
        
        % Demux block
        demux_block_name = strcat(model_name, "/demux_string", int2str(i));
        demuxes(i) = add_block("simulink/Commonly Used Blocks/Demux", demux_block_name);
        set_param(demuxes(i), "Outputs", int2str(sum(substrings, 1)));
        
        % Connect demux to irradiance import block
        add_line(model_name, get_param(import_irradiances(i), 'PortHandles').Outport, ...
            get_param(demuxes(i), 'PortHandles').Inport);

        % Create each substring
        demux_counter = 1;
        num_substrings = size(substrings, 1);
        substring_blocks = zeros(1, num_substrings);
        for j=1:num_substrings
            num_cells = substrings(j);
            
            % Get substring from custom library
            substring_source = strcat("SolarCellLibrary/", strcat(int2str(num_cells), "x1_Substring_Bypass"));
            subsystem_name = strcat(model_name, "/", int2str(i), "_", int2str(j));
            substring_blocks(j) = add_block(substring_source, subsystem_name);
            
            for k=1:num_cells
                % Connect demux to counter
                add_line(model_name, get_param(demuxes(i), "PortHandles").Outport(demux_counter), ...
                    get_param(substring_blocks(j), "PortHandles").Inport(k));

                demux_counter = demux_counter + 1;
            end

            % Connect current substring last substring (+) -> (-)
            if j > 1
               add_line(model_name, get_param(substring_blocks(j), "PortHandles").LConn, get_param(substring_blocks(j-1), "PortHandles").RConn);
            end
        end
        
        voltage_sweep_name = strcat(model_name, "/Voltage_Sweep", int2str(i));
        voltage_sweeps(i) = add_block("SolarCellLibrary/V_Sweep", voltage_sweep_name);
        
        product_name = strcat(model_name, "/Product", int2str(i));
        products(i) = add_block("simulink/Commonly Used Blocks/Product", product_name);

        current_sensor_name = strcat(model_name, "/Current_Sensor", int2str(i));
        current_sensors(i) = add_block("sps_lib/Sensors and Measurements/Current Measurement", current_sensor_name);

        voltage_sensor_name = strcat(model_name,"/Voltage_Sensor", int2str(i));
        voltage_sensors(i) = add_block("sps_lib/Sensors and Measurements/Voltage Measurement", voltage_sensor_name);

        out_name = strcat(model_name, "/To_Workspace", int2str(i));
        to_workspaces(i) = add_block("simulink/Sinks/To Workspace", out_name);
        set_param(to_workspaces(i), "VariableName", strcat("diode_string", int2str(i), "_power_out"));
        
        % + substring to current sensor
        add_line(model_name, get_param(substring_blocks(1), "PortHandles").LConn, get_param(current_sensors(i), "PortHandles").LConn);
        
        % current sensor to product
        add_line(model_name, get_param(current_sensors(i), "PortHandles").Outport, get_param(products(i), "PortHandles").Inport(1));
        
        % current sensor to voltage sweep
        add_line(model_name, get_param(current_sensors(i), "PortHandles").RConn, get_param(voltage_sweeps(i), "PortHandles").LConn);
        
        % current sensor to voltage sensor
        add_line(model_name, get_param(current_sensors(i), "PortHandles").RConn, get_param(voltage_sensors(i), "PortHandles").LConn(1));
        
        % - substring to voltage sweep
        add_line(model_name, get_param(substring_blocks(num_substrings), "PortHandles").RConn, get_param(voltage_sweeps(i), "PortHandles").RConn);

        % - substring to voltage sensor
        add_line(model_name, get_param(substring_blocks(num_substrings), "PortHandles").RConn, get_param(voltage_sensors(i), "PortHandles").LConn(2));

        % voltage sensor to product
        add_line(model_name, get_param(voltage_sensors(i), "PortHandles").Outport, get_param(products(i), "PortHandles").Inport(2));

        % product to workspace
        add_line(model_name, get_param(products(i), "PortHandles").Outport, get_param(to_workspaces(i), "PortHandles").Inport);
    end
end
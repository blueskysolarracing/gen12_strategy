% Generate a bypass diode simulink model for one string
function generate_bypass_diode_model(model_name, string_config, string_name)
    % @param model_name: name of the simulink model
    % @param string_config: A json file indicating the layout of the
    % strings on the array
    % @param string_name: key name inside string_config of the string to build

    %   {
    %       "<substring number>": array of cell numbers,
    %       .
    %       .
    %       .
    %       "String1": Array of substring numbers
    %       .
    %       .
    %       .
    %       "StringN": Array of substring numbers
    %   }
    
    % Load the json file describing string layout
    fid = fopen(string_config);
    raw = fread(fid,inf);
    str = char(raw);
    fclose(fid);
    string_layout = jsondecode(transpose(str));
    
    % Initialize simulink model
    open_system(new_system(model_name));
    fn = fieldnames(string_layout);

    substrings = string_layout.(string_name);
    num_substrings = numel(substrings);

    cells2Substrings = zeros(1,num_substrings);
    for i=1:num_substrings
        cell_range = string_layout.(strcat("ss", int2str(substrings(i))));
        num_cells = cell_range(2) - cell_range(1) + 1;
        cells2Substrings(i) = num_cells;
    end

    % Irradiance import block
    import_irradiance_name = strcat(model_name, "/irr_string");
    import_irradiance = add_block("simulink/Sources/From Workspace", import_irradiance_name);
    set_param(import_irradiance, 'VariableName', strcat("irr_diode_string_", string_name));
    
    % Demux block
    demux_block_name = strcat(model_name, "/demux_string");
    demux = add_block("simulink/Commonly Used Blocks/Demux", demux_block_name);
    set_param(demux, "Outputs", int2str(sum(cells2Substrings)));
    
    % Connect demux to irradiance import block
    add_line(model_name, get_param(import_irradiance, 'PortHandles').Outport, ...
        get_param(demux, 'PortHandles').Inport);

    % Create each substring
    substring_blocks = zeros(1, num_substrings);
    demux_counter = 1;
    for j=1:num_substrings
        num_cells = cells2Substrings(j);
        
        % Get substring from custom library
        substring_source = strcat("SolarCellLibrary/", strcat(int2str(num_cells), "x1_Substring_Bypass"));
        subsystem_name = strcat(model_name, "/substring", int2str(j));
        substring_blocks(j) = add_block(substring_source, subsystem_name);
        
        for k=1:num_cells
            % Connect demux to counter
            add_line(model_name, get_param(demux, "PortHandles").Outport(demux_counter), ...
                get_param(substring_blocks(j), "PortHandles").Inport(k));

            demux_counter = demux_counter + 1;
        end

        % Connect current substring last substring (+) -> (-)
        if j > 1
            add_line(model_name, get_param(substring_blocks(j), "PortHandles").LConn, get_param(substring_blocks(j-1), "PortHandles").RConn);
        end
    end
        
    voltage_sweep_name = strcat(model_name, "/Voltage_Sweep");
    voltage_sweep = add_block("SolarCellLibrary/V_Sweep", voltage_sweep_name);
    
    product_name = strcat(model_name, "/Product");
    product = add_block("simulink/Commonly Used Blocks/Product", product_name);

    current_sensor_name = strcat(model_name, "/Current_Sensor");
    current_sensor = add_block("sps_lib/Sensors and Measurements/Current Measurement", current_sensor_name);

    voltage_sensor_name = strcat(model_name,"/Voltage_Sensor");
    voltage_sensor = add_block("sps_lib/Sensors and Measurements/Voltage Measurement", voltage_sensor_name);

    out_name = strcat(model_name, "/To_Workspace");
    to_workspace = add_block("simulink/Sinks/To Workspace", out_name);
    set_param(to_workspace, "VariableName", "diode_string_power_out");
    
    % + substring to current sensor
    add_line(model_name, get_param(substring_blocks(1), "PortHandles").LConn, get_param(current_sensor, "PortHandles").LConn);
    
    % current sensor to product
    add_line(model_name, get_param(current_sensor, "PortHandles").Outport, get_param(product, "PortHandles").Inport(1));
    
    % current sensor to voltage sweep
    add_line(model_name, get_param(current_sensor, "PortHandles").RConn, get_param(voltage_sweep, "PortHandles").LConn);
    
    % current sensor to voltage sensor
    add_line(model_name, get_param(current_sensor, "PortHandles").RConn, get_param(voltage_sensor, "PortHandles").LConn(1));
    
    % - substring to voltage sweep
    add_line(model_name, get_param(substring_blocks(num_substrings), "PortHandles").RConn, get_param(voltage_sweep, "PortHandles").RConn);

    % - substring to voltage sensor
    add_line(model_name, get_param(substring_blocks(num_substrings), "PortHandles").RConn, get_param(voltage_sensor, "PortHandles").LConn(2));

    % voltage sensor to product
    add_line(model_name, get_param(voltage_sensor, "PortHandles").Outport, get_param(product, "PortHandles").Inport(2));

    % product to workspace
    add_line(model_name, get_param(product, "PortHandles").Outport, get_param(to_workspace, "PortHandles").Inport);

end
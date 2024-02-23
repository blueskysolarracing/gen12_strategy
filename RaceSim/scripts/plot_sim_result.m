function [] = plot_sim_result(csv_path)
    data = readtable(csv_path, 'Delimiter', ',');
    
    x = datetime(data{:, 2}, 'InputFormat', 'yyyy-MM-dd HH:mm:ssZ', 'TimeZone', 'Australia/Adelaide');
    y = data{:, 1};
    
    plot(x, y, '-');
    xlabel('Time');
    ylabel('Array Power(W)');
    title('Array Power vs. Time (59kph)');
    grid on;
end


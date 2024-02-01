function [average_power] = process_results(power, sun_position)
% Output the average power of the array
% @param power: output of simulink simulation 
% @param sun_position: output of step 1

  sun_position_csv = readcell(sun_position);
  power_csv = readmatrix(power);
  sun_times = sun_position_csv(:,4);
  sun_times_dt = [sun_times{:}];
  num_rows = size(sun_position_csv,1);

  plot(power_csv, sun_times_dt);

  time_difference = sun_times_dt - sun_times_dt(1);

   % Convert the time difference to hours
  hours_vector = hours(time_difference);

  area_under_curve = trapz(hours_vector, power_csv);
  difference_hours = hours(sun_times_dt(num_rows) - sun_times_dt(1));

  average_power = area_under_curve / difference_hours;
  disp(strcat("Average Power: ", num2str(average_power)));
end
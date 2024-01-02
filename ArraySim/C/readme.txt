Go into the Run folder then do the following: 

1. Make sure the wscIrr.csv table that you created in step B is present in ./wscLUT/C/Run folder
2. Open up totPower.m found under ./wscLUT/C/Run/m1 and update lines 6 and 9
3. Save totPower.m and return to ./wscLUT/C/Run
4. Open up createFolders.m and run the script
5. Return to ./wscLUT/C/Run
6. Open up multiRun.m script and run it
7. After 2 hours, check the ./wscLUT/C/Run/results folder to see all created csvs
8. Compile all created csvs into a single csv
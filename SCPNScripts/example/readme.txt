This is a simple example to demonstrate all steps necessary for running a JavaScript file on data provided by an Excel table.
It also should demonstrate some of the possibilities of the JavaScript API. The files necessary to run this example are:

	SimpleTable.xls   A simple Excel table with some random data
	SimpleDemo.xml    A simple Petrinet model which should be modified by JavaScript and simulated
	SimpleDemo.js     An example JavaScript file which can be used to modify the SimpleDemo.xml model and start the simulation

The steps necessary to run this example are:
============================================

1. Open SimpleTable.xls in Microsoft Excel.

2. Click on "File" and then on "Save as..", set the file format to "Text (Tab-delimited)" and click on "Save".
   Please ignore all warnings! Now in the same directory where SimpleTable.xls is located there should be a
   new text file named SimpleTable.txt.

3. Open a command line and enter the directory where the example files are stored ("TimeNET\SCPNScripts\example").

4. In your command line type "ConvertExcel.bat SimpleTable.txt SimpleWarehouse.xml" to convert tab-delimited text file to XML.
   Now there should be a new file named "SimpleWarehouse.xml" in the current directory.

5. Type "cd .." in your command line to go back to the parent directory ("TimeNET\SCPNScripts").
   Here type "run.bat example\SimpleDemo.js ModifiedModel.xml" to run the JavaScript and simulation.
   The modified original model ("SimpleDemo.xml") will be saved as "ModifiedNet.xml".
   You might want to compare the original file with the modified one to see the modifications.


For further information on the functions of the JavaScript file ("SimpleDemo.js"), open this file in a text editor and read the comments. Basically, there
are four steps that each JavaScript should do:
			1. Read data from an XML data warehouse.
			2. Open a base model, which should be modified.
			3. Modify attributes of model objects by using the data of the XML date warehouse and save the modified model.
			4. Run a simulation on the modified model.
For further information on the functions to modify Petrinet models take a look into the JavaScript API specifications. This documentation can be found in "Docs\JavaScript-API docs\index.html".


Prerequisites for Developers:
It is assumed that all libraries necessary to run JavaScript are located in "..\..\lib" and the TableConverter program in "..\..\Tools\bin", where 
"..\.." is the TimeNET base directory. The batch file to run Javascript ("run.bat") should be located in "..".


// STEP 1: Read data from XML warehouse
// Warehouse access: load data from SimpleWarehouse.xml and use it as warehouse for this script.
// Although SimpleWarehouse.xml is in the same directory as this JavaScript file, we have to add
// the entire path relative to the TimeNET base directory.
if(!session.initializeWarehouse("SCPNScripts/example/SimpleWarehouse.xml")) {
	session.throwException("initializeWarehouse failed!");
}

// Read command line arguments
if (arguments.length < 1) {
	println("File name where to save the resulting net is missing.");
	java.lang.System.exit(0);
}

// A simple input to get the ID of the loop to be processed.
currentID = readln("Please enter the ID of the data to process!");
println("Processing loop containing part number " + currentID + "...");

saveFile = arguments[0];

// Collect all data with the tag ID from the warehouse and save it in the array ids.
// Each array element is of the type ResultNode, so the actual value is accessed via getValue().
// The string "//id" means that all data with the tag "id" should be found.
ids = (warehouse.evaluate("//id")).getResultNodes();

// Search the warehouse for the previously entered ID.
loopIndex = 0;
while (loopIndex < ids.length && !(ids[loopIndex].getValue()).equals(currentID)) {
	loopIndex = loopIndex + 1;
}

// Check if the data entered has been found, if not then exit.
if (loopIndex == ids.length) {
	println("Specified id could not be found in data!");
	java.lang.System.exit(0);
}

// Collect all remaining data.
// After the data has been collected it is assured, that two arrays contain data of the same loop
// at the same index.
println("Collecting data...");
t1Delays = (warehouse.evaluate("//t1Delay")).getResultNodes();
t2Delays = (warehouse.evaluate("//t2Delay")).getResultNodes();
p1Capacities = (warehouse.evaluate("//p1CapacityModifier")).getResultNodes();
p2Capacities = (warehouse.evaluate("//p2CapacityModifier")).getResultNodes();
p1Names = (warehouse.evaluate("//p1Name")).getResultNodes();
p2Names = (warehouse.evaluate("//p2Name")).getResultNodes();
variableNames = (warehouse.evaluate("//variableName")).getResultNodes();

// STEP 2: Load the base net
println("Loading base net...");
rootNet = Net.load("SCPNScripts/example/SimpleDemo.xml");


// STEP 3: Make all modifications and save the modified net
println("Modifying net...");
// simply set the transitions' time functions without any calculations.
t1 = TimedTransition.withName(rootNet, "T1");
t1.setTimeFunction("EXP(" + t1Delays[loopIndex].getValue() + ")");

t2 = TimedTransition.withName(rootNet, "T2");
t2.setTimeFunction("EXP(" + t2Delays[loopIndex].getValue() + ")");

// do some calculations before setting the places' capacities, but still simply set the names.
p1 = Place.withName(rootNet, "P1");
p1.setName(p1Names[loopIndex].getValue());
// parseInt converts a string into an integer value.
p1CapacityModifier = parseInt(p1Capacities[loopIndex].getValue());
// get the place's current capacity value.
p1OldCapacity = parseInt(p1.getCapacity());
// calulate the new capacity by adding the modifier to the old value.
p1NewCapacity = p1OldCapacity + p1CapacityModifier;
// a places capacity should not be negative, so make some checks to prevent this.
if (p1NewCapacity < 0)
	p1.setCapacity(0);
else
	p1.setCapacity(p1NewCapacity);

// the other place is modified in the same way.
p2 = Place.withName(rootNet, "P2");
p2.setName(p2Names[loopIndex].getValue());
p2CapacityModifier = parseInt(p2Capacities[loopIndex].getValue());
p2OldCapacity = parseInt(p2.getCapacity());
p2NewCapacity = p2OldCapacity + p2CapacityModifier;
if (p2NewCapacity < 0)
	p2.setCapacity(0);
else
	p2.setCapacity(p2NewCapacity);

// here the measure expression is set properly, so that the new place names are used in the expression instead of the
// old ones, which do not exist anymore.
r1 = Measure.withName(rootNet, "R1");
r1.setExpression("#" + p1Names[loopIndex].getValue() + "+#" + p2Names[loopIndex].getValue());

// all arcs need to be modified, so first get an array containing all arcs existing within the net.
// then set all inscriptions to the defined variable name.
arcs = rootNet.getArcs();
variable = variableNames[loopIndex].getValue();
for (i = 0; i < arcs.length; i++)
	arcs[i].setInscription(variable);

// Save the modified net.
println("Saving modified net...");
rootNet.toFile(saveFile);

// STEP 4: Run a short simulation on the modified net. (temporarily turned off)
println("Starting simulation...");
seed = 12345;
System.copyFile(saveFile, "../Models/GMPN/" + saveFile);
resultMonitor = new Packages.statisticclient.ResultMonitor(4455);
resultMonitor.start();
System.runNetSeeded(saveFile, "07:00:00@01/01/2004", "07:00:00@02/01/2004", "localhost", 4445, "localhost", 4455, false, seed);

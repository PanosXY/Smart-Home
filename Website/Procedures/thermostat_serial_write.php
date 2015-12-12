<?php
	require_once("../includes/dbconnect.php");
	
	$command="T";
	
	if(isset($_POST['myonoffswitch']) == "on"){
		$query="UPDATE Thermostat SET OnOffSet='True' WHERE ID='AE'";
		$db->MakeQuery($query);	
		$command=$command."1";
	}else{
		$query="UPDATE Thermostat SET OnOffSet='False' WHERE ID='AE'";
		$db->MakeQuery($query);
		$command=$command."0";
	}
	if(isset($_POST['threshold']) ){ 
		$number=(float)$_POST['threshold'];//20.5
		$query="UPDATE Thermostat SET TempUserSet=$number WHERE ID='AE'";
		$db->MakeQuery($query);
		$temp=(int)$number;//20
		$temp2=$number-$temp; //0.5
		$temp2*=10;
		
		$command=$command.$temp.$temp2."NN";
		
	}
	$db->Close();

	require("../includes/php_serial.class.php");

	$serial = new phpSerial();
	$serial->deviceSet("/dev/ttyUSB0");
	$serial->confBaudRate(9600); //Baud rate: 9600
	$serial->confParity("none");  //Parity (this is the "N" in "8-N-1")
	$serial->confCharacterLength(8); //Character length     (this is the "8" in "8-N-1")
	$serial->confStopBits(1);  //Stop bits (this is the "1" in "8-N-1")
	$serial->confFlowControl("none");
	$serial->deviceOpen();

	$serial->sendMessage($command);

	$serial->deviceClose();
	
	
	if(error_get_last()){
		header("Location: /Devices/thermostat.php?Command=Error");
	}else{
		header("Location: /Devices/thermostat.php?Command=OK");
	}

?>

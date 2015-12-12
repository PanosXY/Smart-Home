<?php
	
	require_once("../includes/dbconnect.php");
	
	$command="R";
	
	if(isset($_POST['roundedOne']) == "on"){
		$query="UPDATE Radar SET AlarmSet='True' WHERE ID='7E'";
		$db->MakeQuery($query);	
		$command=$command."1";
	}else{
		$query="UPDATE Radar SET AlarmSet='False' WHERE ID='7E'";
		$db->MakeQuery($query);
		$command=$command."0";
	}
	
	if(isset($_POST['hidden']) == "on"){	
		$command=$command."N"; //Dummy Byte
	}
	if(isset($_POST['clear']) == "on"){	
		$command=$command."1";
	}else{
		$command=$command."0";
	}
	
	if(isset($_POST['hidden2']) == "on"){	
		$command=$command."N"; //Dummy Byte
	}
	if(isset($_POST['clear2']) == "on"){	
		$command=$command."1";
	}else{
		if(isset($_POST['hidden2']) == "on"){	
			$command=$command."N"; //Dummy Byte
		}else{
			$command=$command."0";
		}
	}
	
	$command=$command."NNN";
	
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
		header("Location: /Devices/alarm.php?Command=Error");
	}else{
		header("Location: /Devices/alarm.php?Command=OK");
	}
	
	

?>
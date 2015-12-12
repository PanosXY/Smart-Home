<?php
	require_once("../includes/dbconnect.php");
	
	$command="S";

	if(isset($_POST['myonoffswitch']) == "on"){
		$query="UPDATE Switch SET OnOffSet='True' WHERE ID='BF'";
		$db->MakeQuery($query);	
		$command=$command."1";
	}else{
		$query="UPDATE Switch SET OnOffSet='False' WHERE ID='BF'";
		$db->MakeQuery($query);
		$command=$command."2";
	}
	
	if(isset($_POST['dimmer']) ){
		$dimmer=$_POST['dimmer'];
		switch ($dimmer) {
		    case 0:
		        $dimmer=8;
		        break;
		    case 20:
		        $dimmer=7;
		        break;
		    case 40:
		        $dimmer=6;
		        break;
			case 60:
		        $dimmer=5;
		        break;
		    case 80:
		        $dimmer=4;
		        break;
		    case 100:
		        $dimmer=0;
		        break;	
		}
		$query="UPDATE Switch SET DimmerSet=$dimmer WHERE ID='BF'";
		$db->MakeQuery($query);	
		$command=$command.$dimmer;	
	} 
	
	if(isset($_POST['roundedOne']) == "on"){
		$query="UPDATE Switch SET SetMov='True' WHERE ID='BF'";
		$db->MakeQuery($query);	
		$command=$command."1";
	}else{
		$query="UPDATE Switch SET SetMov='False' WHERE ID='BF'";
		$db->MakeQuery($query);
		$command=$command."0";
	}
	
	if(isset($_POST['roundedOne2']) == "on"){
		$query="UPDATE Switch SET SetAutoLumensOnOff='True' WHERE ID='BF'";
		$db->MakeQuery($query);	
		$command=$command."1";
	}else{
		$query="UPDATE Switch SET SetAutoLumensOnOff='False' WHERE ID='BF'";
		$db->MakeQuery($query);
		$command=$command."0";
	}
	
	if(isset($_POST['LumensRange'])){
		$lumensrthershold=($_POST['LumensRange']*30)/100;
		$query="UPDATE Switch SET SetAutoLumens=$lumensrthershold WHERE ID='BF'";
		$db->MakeQuery($query);	
		$command=$command.$lumensrthershold;		
	}	
	
	$command=$command."N";
	
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
		header("Location: /Devices/switch.php?Command=Error");
	}else{
		header("Location: /Devices/switch.php?Command=OK");
	}
	
?>
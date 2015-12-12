<?php
	
	require_once("includes/dbconnect.php");
	$query="UPDATE LoginFlag SET Flag='False' WHERE Flag='True'";
	$db->MakeQuery($query);	
	$db->Close();
	
	session_start();
	session_unset();
	session_destroy();
	setcookie(session_name(),'',time()-42000,'/');
	header("Location: /index.php");

?>

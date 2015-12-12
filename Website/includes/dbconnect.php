<?php

	include_once("mydbclass.php");
	$db = new myDB('127.0.0.1', 'root', 'passwd');
	$db->SelectDB('Home');

?>

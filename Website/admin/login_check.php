<?php
	
	include_once("../includes/validate_user.php");
	
	$AuthUser=getUser($_POST['LoginUserName'],$_POST['LoginPassword']);
			
	if(isset($AuthUser)){
		@session_start();
		$_SESSION['Authenticated']=true;
		$_SESSION['UserName']=$AuthUser['UserName'];
		$_SESSION['Name']=$AuthUser['Name'];
		$_SESSION['Surname']=$AuthUser['Surname'];
		$_SESSION['ID']=$AuthUser['ID'];
		$_SESSION['Password']=$AuthUser['Password'];
						
		header("Location: /main.php");
		die();
	}
		
	header("Location: /index.php?LoginError=Failed");

?>


<?php

	function getUser($username,$password){
		require_once("../includes/dbconnect.php");
		
		if($username==NULL || $password==NULL){
			header("Location: /index.php?LoginError=NULLFailed");
			die();
		}
				
		$query="SELECT * FROM LoginFlag WHERE Flag='True'";
		$result=$db->MakeQuery($query);
		$rowcount=$db->GetRecordCount($result);
		if($rowcount==1){
			header("Location: /index.php?LoginError=FlagFailed");
			die();
		}
		
		$query="SELECT Password FROM Users WHERE Password=md5('$password') AND UserName='$username'";
		$result=$db->MakeQuery($query);
		$rowcount=$db->GetRecordCount($result);
			
		if($rowcount==1){
			$query="UPDATE LoginFlag SET Flag='False' WHERE Flag='True'";
			$db->MakeQuery($query);
			
			$query="SELECT Name,Surname,UserName,ID,Password FROM Users WHERE UserName='$username' AND Password=md5('$password')";
			$result=$db->MakeQuery($query);
			return $db->GetRecord($result);
			
		}
		else{
			return null;
		}
			
		$db->Close();
		
	}

?>

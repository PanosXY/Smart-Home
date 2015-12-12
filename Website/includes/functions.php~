<?php
	
	require_once("dbconnect.php");
	
	function updateTableRelationRT(){
		
		$query="SELECT ID FROM Tags WHERE `Name`='$_POST[RTagsList]'";
		$result=mysql_query($query);
		$tagid=mysql_result($result,0);
		$query="UPDATE Relation_RT,Restaurants SET Relation_RT.TID=$tagid WHERE Relation_RT.RID=Restaurants.ID AND Restaurants.`Name`='$_POST[RestaurantsProcessList]'";
		mysql_query($query);
		
	}
		
	function isValidEmail($email){
		return eregi("^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$", $email);
	}
	
	@session_start();
	
	function isAuthenticatedUser(){
		if(isset($_SESSION['Authenticated']) && ($_SESSION['Authenticated']==true)){
			return true;
		}
		else{
			return false;
		}
	}
	
?>
<?php
	
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

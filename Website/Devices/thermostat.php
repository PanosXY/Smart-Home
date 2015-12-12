<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
	"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">

<head>
	<title>Thermostat</title>
	<link href="../stylesheets/devices.css" rel="stylesheet" type="text/css" />
	<link href="../stylesheets/thermostat.css" rel="stylesheet" type="text/css" />
	<meta http-equiv="Refresh" content="15">
	<script src="../jscripts/thermostat.js" type="text/javascript"></script>
</head>

<body>
	<?php
		include_once("../includes/functions.php");
		if (!isAuthenticatedUser()){
			echo '<p align="center"><h1>Δεν έχετε δικαίωμα πρόσβασης σε αυτή τη σελίδα</h1></p><meta http-equiv="Refresh" content="4; url=../index.php" /><p align="center"><img src="../images/animated-loading-bar.gif"></p><p style="font-size:12px" align="center">We redirect you automatically</p>';
		die();
	}
	?>
	
	<div class="Web_OnlineTools_Logo" style="padding:20px;">
		<h1 id="header1_logo">Smart Home</h1>
		<h2 id="header2_logo">Home Automation</h2>
	</div>
	
	<div>		
		<table id="Return" onclick="document.location = '../main.php';" style="float:left">
			<tr>
				<td>
					Return 
				</td>
				<td>
					<img style="width:60%" src="../images/back1.gif" >
				</td>	
			</tr>
		</table>
		<table id="Logout" onclick="document.location = '../logout.php';" style="float:right">
			<tr>
				<td>
					Logout 
				</td>
				<td>
					<img style="width:50%" src="../images/system_log_out.png" >
				</td>	
			</tr>
		</table>
	</div>
	
	
	<div align="center" id="MenuLayer">
		<form name="ThermostatChanges" id="ThermostatChanges" method="post" action="../Procedures/thermostat_serial_write.php" enctype="multipart/form-data">
		<table id="ChangesTable">
			<tr>
				<td id="Common">
					<p id="pcommon">Temperature:</p>
				</td>
				
				<td align="center" id="Temp">
					<font id="DataFont" color="blue">
						<?php 
							sleep(1);
							require_once("../includes/dbconnect.php");
							
							$query="SELECT Temp From Thermostat WHERE ID='AE'";
							$result=$db->MakeQuery($query);								
							$result=mysql_result($result,0);
							echo $result;
							
						?>
					</font><font id="DataFont">&deg;C</font>
				</td>
			</tr>
	
			<tr >
				<td id="Common">
					<p id="pcommon">Switch to On/Off:</p>
				</td>
				
				<td align="center" id="OnOff">
					<div class="onoffswitch">
						<input type="checkbox" name="myonoffswitch" class="onoffswitch-checkbox" id="myonoffswitch" 
						
						<?php
						
							$query="SELECT OnOff From Thermostat WHERE ID='AE'";
							$result=$db->MakeQuery($query);								
							$result=mysql_result($result,0);
							if(strcmp($result, "True")==0){
									echo 'checked>'; 								
							}else{
								echo '>';								
							}
						
						?>
						
						<label class="onoffswitch-label" for="myonoffswitch">
							<div class="onoffswitch-inner"></div>
							<div class="onoffswitch-switch"></div>
						</label>
					</div>
				</td>
			</tr>
			
			<tr>
				<td id="Common">
					<p id="pcommon">Change Temperature Threshold:</p>
				</td>
				
				<td id="TempRange">
					<input type="range" min="10" max="30" value=
					<?php
						
						$query="SELECT TempUser From Thermostat WHERE ID='AE'";
						$result=$db->MakeQuery($query);								
						$result=mysql_result($result,0);
						echo '"';
						echo $result;
						echo '"';
						
					?>					
					 step="0.5" onchange="updateInput(this.value)" style="width:200px;"></br>
					<input type="text" name="threshold" value=
					<?php
						
						$query="SELECT TempUser From Thermostat WHERE ID='AE'";
						$result=$db->MakeQuery($query);								
						$result=mysql_result($result,0);
						echo '"';
						echo $result;
						echo '"';
						
						$db->Close();
						
					?>
					 id="threshold" style="width:30px;margin-left:38%" readonly>
					 <font id="CTTFont">&deg;C</font>
				</td>
			</tr>
			
			<tr>
				<td colspan="3" align="center" id="Button">
					<input type="submit" name="ChangesSubmit" id="ChangesSubmit" value="Apply Changes">				
				</td>
			</tr>
		</table>
		
		<table>
			<tr>
				<td>
					<?php
						if(isset($_GET["Command"])) {
							switch($_GET["Command"]) {
								case "OK":
									echo '<p style="color:#0066FF">Changes has been applied.</p>';
									break;
								case "Error":
									echo '<p style="color:red">An error has been occurred.</p>';
									break;
							}
						}
					?>
				</td>
			</tr>
		</table>
		</form>
	</div>

		
</body>

</html>

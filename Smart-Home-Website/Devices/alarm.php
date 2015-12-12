<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
	"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">

<head>
	<title>Alarm</title>
	<link href="../stylesheets/devices.css" rel="stylesheet" type="text/css" />
	<link href="../stylesheets/alarm.css" rel="stylesheet" type="text/css" />
	<meta http-equiv="Refresh" content="15">
	<script src="../jscripts/alarm.js" type="text/javascript"></script>
</head>

<body onload="informsetting()">
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
		<form name="RadarChanges" id="RadarChanges" method="post" action="../Procedures/radar_serial_write.php" enctype="multipart/form-data">
			<table id="ChangesTable">
				
				<tr>
					<td id="Common">
						<p id="pcommon">Set Alarm:</p>
					</td>
				
					<td align="center" id="AlarmOnOff">
					
						<table>
							<tr>
								<td>
									<div class="roundedOne">
							    		<input type="checkbox" name="roundedOne" id="roundedOne"  onclick="informsetting()" 
							    		<?php 
											
											sleep(1);
											require_once("../includes/dbconnect.php");
											
											$query="SELECT AlarmSet From Radar WHERE ID='7E'";
											$result=$db->MakeQuery($query);								
											$result=mysql_result($result,0);
											if(strcmp($result, "True")==0){
												echo 'checked>'; 								
											}else{
												echo '>';								
											}
											
										?>
							    		<label for="roundedOne"></label>
	  								</div>
	  							</td>
								<td>
									<font style="margin-left: 5px">Alarm is </font>
									<font id="AlarmSetJS"></font>
								</td>
							</tr>
						</table>
						
					</td>
				</tr>
	
				<tr >
					<td id="Common">
						<p id="pcommon">Alarm By Movement:</p>
					</td>
					
					<td align="center" id="AlarmByMovement">
						<?php 
											
							$query="SELECT AlarmMove From Radar WHERE ID='7E'";
							$result=$db->MakeQuery($query);								
							$result=mysql_result($result,0);
							if(strcmp($result, "True")==0){
								echo '	<table cellspacing="0">
											<tr>
												<td align="right" style="width: 20px; padding-right:20px;">
													<img id="LED" src="../images/Flashin_Red"/>
												</td>
												<td style="padding-right:10px;">
													<font style="margin-left: -8px">Security Breach</font>
												</td>
											</tr>
										</table>
										<table align="center">
											<tr>
												<td>
													<input type="checkbox" name="clear2" id="clear2">
													<font>Clear</font>
												</td>
											</tr>
										</table>'; 								
							}else{
								echo '	<table cellspacing="0">
											<tr>
												<td align="right" style="width: 20px">
													<img id="LED" src="../images/led_green_black.png"/>
												</td>
												<td>
													<font style="margin-left: 10px">Home is secure</font>
												</td>
												<td>
													<input type="checkbox" name="hidden2" id="hidden2" style="visibility: hidden; width: 0px; height:0px;" checked>
												</td>
											</tr>
										</table>';								
							}
							
						?>
					</td>
					
				</tr>
				
				<tr>
					<td id="Common">
						<p id="pcommon">Alarm By Radar Failure:</p>
					</td>
					
					<td align="left" id="AlarmByRadarFailure">
						<?php 
											
							$query="SELECT AlarmReport From Radar WHERE ID='7E'";
							$result=$db->MakeQuery($query);								
							$result=mysql_result($result,0);
							if(strcmp($result, "True")==0){
								echo '	<table cellspacing="0">
											<tr>
												<td align="right" style="width: 20px">
													<img id="LED" src="../images/Flashin_Red"/>
												</td>
												<td>
													<font style="margin-left: 10px">Security Breach</font>
												</td>
											</tr>
										</table>
										<table align="center">
											<tr>
												<td>
													<input type="checkbox" name="clear" id="clear">
													<font>Clear</font>
												</td>
											</tr>
										</table>';
							}else{
								echo '	<table cellspacing="0">
											<tr>
												<td align="right" style="width: 20px">
													<img id="LED" src="../images/led_green_black.png"/>
												</td>
												<td>
													<font style="margin-left: 10px">Home is secure</font>
												</td>
												<td>
													<input type="checkbox" name="hidden" id="hidden" style="visibility: hidden; width: 0px; height:0px;" checked>
												</td>
											</tr>
										</table>';								
							}
							
							$db->Close();
							
						?>
					</td>
				</tr>
				
				<tr>
					<td colspan="3" align="center" id="Button">
						<input type="submit" name="ChangesSubmit" id="ChangesSubmit" value="Apply Changes">				
					</td>
				</tr>
				
			</table>
		</form>	
		
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
	</div>
	
</body>

</html>

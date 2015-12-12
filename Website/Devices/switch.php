<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
	"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">

<head>
	<title>Switch</title>
	<link href="../stylesheets/devices.css" rel="stylesheet" type="text/css" />
	<link href="../stylesheets/switch.css" rel="stylesheet" type="text/css" />
	<meta http-equiv="Refresh" content="15">
	<script src="../jscripts/switch.js" type="text/javascript"></script>
</head>

<body onload="informsetting();informsetting2()">
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
		<form name="SwitchChanges" id="SwitchChanges" method="post" action="../Procedures/switch_serial_write.php" enctype="multipart/form-data">
		<table id="ChangesTable">
			<tr>
				<td id="Common">
					<p id="pcommon">Room Temperature:</p>
				</td>
				
				<td align="center" id="Temp">
					<font id="DataFont" color="blue">
						<?php 
							sleep(1);
							require_once("../includes/dbconnect.php");
							
							$query="SELECT Temp From Switch WHERE ID='BF'";
							$result=$db->MakeQuery($query);								
							$result=mysql_result($result,0);
							echo $result;
							
						?>
					</font><font id="DataFont">&deg;C</font>
				</td>
			</tr>
			
			<tr>
				<td id="Common">
					<p id="pcommon">Luminosity:</p>
				</td>
				
				<td align="center" id="Lumens">
					<table style="margin-left: 10%">
						<tr>
							<td>
								<font id="DataFont" color="blue">
									<?php 
										
										$query="SELECT Lumens From Switch WHERE ID='BF'";
										$result=$db->MakeQuery($query);								
										$result=mysql_result($result,0);
										$percentage=($result*100)/30;
										echo intval($percentage);
										echo '&#37;';
									?>									
								</font>
							</td>
							<td style="width: 50px;">
								<img style="width:100%" src="../images/sunnyicons/
									<?php
										if($result>=0 && $result<5){
											echo 'sunny-icon-0.gif ">';
										}
										if($result>=5 && $result<10){
											echo 'sunny-icon-30.gif ">';
										}	
										if($result>=10 && $result<15){
											echo 'sunny-icon-60.gif ">';
										}
										if($result>=15 && $result<20){
											echo 'sunny-icon-90.gif ">';
										}
										if($result>=20 && $result<25){
											echo 'sunny-icon-120.gif ">';
										}
										if($result>=25 && $result<30){
											echo 'sunny-icon-150.gif ">';
										}
										if($result>=30){
											echo 'sunny-icon-180.gif ">';
										}
									?>
							</td>
						</tr>
					</table>
					
					
				</td>
			</tr>
			
			<tr>
				<td id="Common">
					<p id="pcommon">Switch to On/Off:</p>
				</td>
				
				<td align="center" id="OnOff">
					<div class="onoffswitch">
						<input type="checkbox" name="myonoffswitch" class="onoffswitch-checkbox" id="myonoffswitch" 
						
						<?php
						
							$query="SELECT OnOff From Switch WHERE ID='BF'";
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
					<p id="pcommon">Set Dimmer:</p>
				</td>
				
				<td id="DimmerRange">
					<input type="range" min="0" max="100" value=
					<?php
						
						$query="SELECT Dimmer From Switch WHERE ID='BF'";
						$result=$db->MakeQuery($query);								
						$result=mysql_result($result,0);
						switch ($result) {
						    case 0:
						        $result=100;
						        break;
						    case 4:
						        $result=80;
						        break;
						    case 5:
						        $result=60;
						        break;
							case 6:
						        $result=40;
						        break;
						    case 7:
						        $result=20;
						        break;
						    case 8:
						        $result=0;
						        break;	
						}
						echo '"';
						echo $result;
						echo '"';
						
					?>					
					 step="20" onchange="updateInput(this.value)" style="width:200px;"></br>
					
					<table style="padding-left: 30%">
						<tr>
							<td>
								<input type="text" name="dimmer" value=
								<?php
									
									$query="SELECT Dimmer From Switch WHERE ID='BF'";
									$result=$db->MakeQuery($query);								
									$result=mysql_result($result,0);
									switch ($result) {
									    case 0:
									        $result=100;
									        break;
									    case 4:
									        $result=80;
									        break;
									    case 5:
									        $result=60;
									        break;
										case 6:
									        $result=40;
									        break;
									    case 7:
									        $result=20;
									        break;
									    case 8:
									        $result=0;
									        break;	
									}
									echo '"';
									echo $result;
									echo '"';
									
								?>
								 id="dimmer" style="width:22px;margin-left:44%" readonly>
							</td>
							<td style="padding-left: 9px;"><font id="ButtonFont">&#37;</font></td>
							<td style="width: 20px; padding-left: 7px;">
								<img id="lightImg" style="width:100%" src="../images/lighticons/
								<?php
										if($result>=0 && $result<20){
											echo 'light-icon-30.gif ">';
										}
										if($result>=20 && $result<40){
											echo 'light-icon-60.gif ">';
										}	
										if($result>=40 && $result<60){
											echo 'light-icon-90.gif ">';
										}
										if($result>=60 && $result<80){
											echo 'light-icon-120.gif ">';
										}
										if($result>=80 && $result<100){
											echo 'light-icon-150.gif ">';
										}
										if($result>=100){
											echo 'light-icon-180.gif ">';
										}
									?>
							</td>
						</tr>
					</table>
					
				</td>				
			</tr>
			
			<tr>
				<td id="Common">
					<p id="pcommon">Set Motion Sensor Lighting:</p>
				</td>
			
				<td align="center" id="AutoMove">
				
					<table>
						<tr>
							<td>
								<div class="roundedOne">
						    		<input type="checkbox" name="roundedOne" id="roundedOne"  onclick="informsetting()"
						    		<?php 
				
										$query="SELECT SetMov From Switch WHERE ID='BF'";
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
								<font id="ButtonFont" style="margin-left: 5px;">Function is </font>
								<font id="AutoLight1SetJS"></font>
							</td>
						</tr>
					</table>
					
				</td>
			</tr>
			
			<tr>
				<td id="Common">
					<p id="pcommon">Set Luminosity Sensor Lighting:</p>
				</td>
			
				<td align="center" id="AutoLumens">
				
					<table>
						<tr>
							<td>
								<div class="roundedOne2">
						    		<input type="checkbox" name="roundedOne2" id="roundedOne2"  onclick="informsetting2()"
						    		<?php 
				
										$query="SELECT SetAutoLumensOnOff From Switch WHERE ID='BF'";
										$result=$db->MakeQuery($query);								
										$result=mysql_result($result,0);
										if(strcmp($result, "True")==0){
											echo 'checked>'; 								
										}else{
											echo '>';								
										}
										
									?> 
						    		
						    		<label for="roundedOne2"></label>
  								</div>
  							</td>
							<td>
								<font id="ButtonFont" style="margin-left: 5px;">Function is </font>
								<font id="AutoLight2SetJS"></font>
							</td>
						</tr>
					</table>
					
				</td>
			</tr>
			
			<tr>
				<td id="Common">
					<p id="pcommon">Change Luminosity Threshold:</p>
				</td>
				
				<td id="ChangeLumensRange">
					<input type="range" min="0" max="100" value=
					<?php
						
						$query="SELECT SetAutoLumens From Switch WHERE ID='BF'";
						$result=$db->MakeQuery($query);								
						$result=mysql_result($result,0);
						$percentage=($result*100)/30;
						echo '"';
						echo $percentage;
						echo '"';
						
					?>					
					 step="10" onchange="updateInput2(this.value)" style="width:200px;"></br>
					
					<table style="padding-left: 35%">
						<tr>
							<td>
								<input type="text" name="LumensRange" value=
								<?php
									
									$query="SELECT SetAutoLumens From Switch WHERE ID='BF'";
									$result=$db->MakeQuery($query);								
									$result=mysql_result($result,0);
									$percentage=($result*100)/30;
									echo '"';
									echo $percentage;
									echo '"';
									
									$db->Close();
																		
								?>
								 id="LumensRange" style="width:22px;margin-left:44%" readonly>
							</td>
							<td style="padding-left: 9px;"><font id="ButtonFont">&#37;</font></td>
						</tr>						
					</table>
					
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

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
	"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">

<head>
	<title>Smart Home Main Page</title>
	<link href="stylesheets/main.css" rel="stylesheet" type="text/css" />
	<meta http-equiv="content-type" content="text/html;charset=utf-8" />
	<meta name="generator" content="Geany 0.21" />
	<script src="../jscripts/main.js" type="text/javascript"></script>
</head>

<body 
<?php 
											
	require_once("includes/dbconnect.php");
	
	$query1="SELECT AlarmReport From Radar WHERE ID='7E'";
	$result1=$db->MakeQuery($query1);								
	$result1=mysql_result($result1,0);
	
	$query2="SELECT AlarmMove From Radar WHERE ID='7E'";
	$result2=$db->MakeQuery($query2);								
	$result2=mysql_result($result2,0);
	if((strcmp($result1, "True")==0) || (strcmp($result2, "True")==0)){
		echo ' onload="AlarmBlink()">'; 								
	}else{
		echo '>';								
	}

	$db->Close();
	
?>

	<?php
		include_once("includes/functions.php");
		if (!isAuthenticatedUser()){
			echo '<p align="center"><h1>Δεν έχετε δικαίωμα πρόσβασης σε αυτή τη σελίδα</h1></p><meta http-equiv="Refresh" content="4; url=index.php" /><p align="center"><img src="images/animated-loading-bar.gif"></p><p style="font-size:12px" align="center">We redirect you automatically</p>';
		die();
	}
	?>
	
	<div class="Web_OnlineTools_Logo" style="padding:20px;">
		<h1 id="header1_logo">Smart Home</h1>
		<h2 id="header2_logo">Home Automation</h2>
	</div>
	
	
	<div>
		<table id="Logout" onclick="document.location = 'logout.php';" style="float:right">
			<tr>
				<td>	
					Logout 
				</td>
				<td>
					<img style="width:50%" src="images/system_log_out.png" >
				</td>	
			</tr>
		</table>
		
		<table id="clock">
			<tr>
				<td>
					<iframe src="http://free.timeanddate.com/clock/i3kch6qw/n26/fn7/fs20/tct/pct/ftb/tt0/th1" frameborder="0" width="421" height="26" allowTransparency="true"></iframe>
				</td>
			</tr>
		</table>
		
		
	</div>
	
	<div align="center" style="padding-top:50px">
		<h1 id="Wlcm">Welcome Home</h1> <h1 id="Name"><?php echo $_SESSION['Name']; ?></h1>
	</div>
	
	<div align="center" id="MenuLayer">
		<table id="switch" onclick="document.location = 'Devices/switch.php';">
			<tr>
				<td>
					<p id="pcommon">Switch</p>
				</td>
			</tr>
		</table>
		</br>	
		<table id="Thermostat" onclick="document.location = 'Devices/thermostat.php';">
			<tr>
				<td>
					<p id="pcommon">Thermostat</p>
				</td>
			</tr>
		</table>
		</br>	
		<table id="Radar" onclick="document.location = 'Devices/alarm.php';">
			<tr>
				<td>
					<p id="pcommon">Alarm</p>
				</td>
			</tr>
		</table>
		</br>
		<table id="Camera" onclick="document.location = 'Devices/ipcamera.php';">
			<tr>
				<td>
					<p id="pcommon">IP Camera</p>
				</td>
			</tr>
		</table>
	</div>
	
</body>

</html>

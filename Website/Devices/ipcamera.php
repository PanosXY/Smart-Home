<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
	"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">

<head>
	<title>IP Camera</title>
	<link href="../stylesheets/devices.css" rel="stylesheet" type="text/css" />
	<meta http-equiv="content-type" content="text/html;charset=utf-8" />
	<meta name="generator" content="Geany 0.21" />
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
	
	<div align="right" style="z-index:2">		
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
	
	<div align="center" style="margin-top:100px;">
	
	<script language="JavaScript" type="text/javascript">
		function reload()
		{
			setTimeout('reloadImg("refresh")',300)
		}; 

		function reloadImg(id)
		{
			
			var serverip="<?php echo $_SERVER['SERVER_NAME']; ?>";
			var clientip="<?php echo $_SERVER['REMOTE_ADDR']; ?>";
			var clientip2 = clientip.substring(0,7);
			
			var obj = document.getElementById(id);
			var rand = Math.random();
			
			if(clientip2="192.168"){
				obj.src = "http://192.168.2.20:8080/snapshot.cgi?user=PanosXYadmin&pwd=168941teipir&t="+rand;
			}else{
				obj.src = "http://"+serverip+":8080/snapshot.cgi?user=PanosXYadmin&pwd=168941teipir&t="+rand;
			}
		}
	</script>
		<?php
			$str1=substr($_SERVER['REMOTE_ADDR'],0,-4);
			$str2="192.168";
			$res=strcmp($str1, $str2);
			if($res==0){
				echo '<img src="http://192.168.2.20:8080/snapshot.cgi?user=PanosXYadmin&pwd=168941teipir&t=" name="refresh" id="refresh" onload="reload()">';
			}else{
				echo '<img src="http://';
				echo $_SERVER['SERVER_NAME']; 
				echo ':8080/snapshot.cgi?user=PanosXYadmin&pwd=168941teipir&t=" name="refresh" id="refresh" onload="reload()">';
			}
		?>
	</div>
	
</body>

</html>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
	"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">

<head>
	<title>Smart Home</title>
	<link href="stylesheets/index.css" rel="stylesheet" type="text/css" />
	<meta http-equiv="content-type" content="text/html;charset=utf-8" />
	<meta name="generator" content="Geany 0.21" />
</head>

<body>
	
	<div class="Web_OnlineTools_Logo" style="padding:20px;">
		<h1 id="header1_logo">Smart Home</h1>
		<h2 id="header2_logo">Home Automation</h2>
	</div>
	<div id="LoginLayer" align="center">
		<table>
			<tr>
				<td colspan="2" align="middle"><img style="width:50%" id="LoginImg" src="images/login.gif"></td>
			</tr>
			<form name="LoginBox" id="LoginBox" method="post" action="admin/login_check.php" enctype="multipart/form-data">
				<tr>
					<td align="left" style="font-family:Trebuchet MS;">		
						Username:
					</td>
					<td align="right">
						<input type="text" name="LoginUserName" id="LoginUserName">
					</td>	
				</tr>
				<tr>
					<td align="left" style="font-family:Trebuchet MS;">	
						Password: 
					</td>
					<td align="right">
						<input type="password" name="LoginPassword" id="LoginPassword">
					</td>
				</tr>	
				<tr>
					<td colspan="2" align="right">
						<input type="submit" name="LoginSubmit" id="LoginSubmit" value="Login">
					</td>
				</tr>				
			</form> 
			<tr>
				<td colspan="2">
					<?php
						if (isset($_GET["LoginError"])) {
												
							switch($_GET["LoginError"]) {
								case "Failed":
									echo '<p style="color:#F00; font-family: Trebuchet MS;">You give wrong data.</p>';
									break;
								case "NULLFailed":
									echo '<p style="color:#F00; font-family: Trebuchet MS;">Fill the fields.</p>';
									break;
								case "FlagFailed":
									echo '<p style="color:#F00; font-family: Trebuchet MS;">The page is used by another user.</p>';
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

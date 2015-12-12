function informsetting(){
	if (document.getElementById('roundedOne').checked){
		document.getElementById("AlarmSetJS").innerHTML="&nbsp;ON";
		document.getElementById("AlarmSetJS").style.color="green";
	}else{
		document.getElementById("AlarmSetJS").innerHTML="OFF";
		document.getElementById("AlarmSetJS").style.color="red";
	}
}
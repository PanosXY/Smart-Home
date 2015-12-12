function updateInput(ish){
	document.getElementById("dimmer").value = ish;
	
	if(ish>=0 && ish <20){	
		document.getElementById("lightImg").src="../images/lighticons/light-icon-30.gif";
	}
	if(ish>=20 && ish <40){	
		document.getElementById("lightImg").src="../images/lighticons/light-icon-60.gif";
	}
	if(ish>=40 && ish <60){	
		document.getElementById("lightImg").src="../images/lighticons/light-icon-90.gif";
	}
	if(ish>=60 && ish <80){	
		document.getElementById("lightImg").src="../images/lighticons/light-icon-120.gif";
	}
	if(ish>=80 && ish <100){	
		document.getElementById("lightImg").src="../images/lighticons/light-icon-150.gif";
	}
	if(ish>=100){	
		document.getElementById("lightImg").src="../images/lighticons/light-icon-180.gif";
	}
}	

function updateInput2(ish){
	document.getElementById("LumensRange").value = ish;
}	

function informsetting(){
	if (document.getElementById('roundedOne').checked){
		document.getElementById("AutoLight1SetJS").innerHTML="&nbsp;ON";
		document.getElementById("AutoLight1SetJS").style.color="green";
	}else{
		document.getElementById("AutoLight1SetJS").innerHTML="OFF";
		document.getElementById("AutoLight1SetJS").style.color="red";
	}
}

function informsetting2(){
	if (document.getElementById('roundedOne2').checked){
		document.getElementById("AutoLight2SetJS").innerHTML="&nbsp;ON";
		document.getElementById("AutoLight2SetJS").style.color="green";
	}else{
		document.getElementById("AutoLight2SetJS").innerHTML="OFF";
		document.getElementById("AutoLight2SetJS").style.color="red";
	}
}
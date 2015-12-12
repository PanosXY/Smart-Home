var x;

function AlarmBlink() {
	x = 1;
    setInterval(change, 500);
}

function change() {
    if(x == 1) {
        color = "red";
    	x = 2;
	}else{
    	color = "white";
        x = 1;
	}

	document.getElementById("Radar").style.backgroundColor = color;
}
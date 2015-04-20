/***************************************************************************************************
* Copyright(C), Sonix Technology Co., Ltd.
* File name: motion.js
* Author: jinxian_he
* Version: 1.0
* Date: 01/16/2013
* Description: This file provides javascript functions of motion detection.
* History:    
  (1) V1.0(01/16/2013): Initial version.
***************************************************************************************************/
function motion_areas_select()
{	
	var URL;	
	var date=new Date().getMilliseconds();
	var ipaddr = window.location.host;

	URL="http://"+ipaddr+"/cgi-bin/get_background.cgi?"+date;	
	window.open(URL, "mainframe");
}

var mousedown_left_flag = false;
var mousedown_right_flag = false;


$(document).ready(function () {
$(document).mousedown(function(e)
{
	switch(e.which)//Determine the button of mouse which is pressed by jQuery . 1 left , 2 middle , 3 right . Modify by xyd
	{
		case 1:
			mousedown_left_flag = true;
			break;
		//case 2:
		case 3:
			mousedown_right_flag = true;
			break;
		default:
			break;
	}
});

$(document).mouseup(function(e)
{
	switch(e.which)
	{
		case 1:
			mousedown_left_flag = false;
			break;
		//case 2:
		case 3:
			mousedown_right_flag = false;
			break;
		default:
			break;
	}
});
$(document).contextmenu(function () {
	mousedown_right_flag = false;
	return false;
        })
});



var block_flag_array = new Array();
for(var i=1; i<=192; i++)
{
	block_flag_array[i]=0;
}

function visible(block_id)
{	
	if(mousedown_left_flag)
	{
		if(!block_flag_array[block_id])
		{
			//document.getElementById(block_id).style.backgroundColor="#ff0000";
			//document.getElementById(block_id).style.filter="alpha(opacity=20)";
			
			$("#"+block_id).css("background-color", "#ff0000").css("opacity", "0.2"); //modify by xyd , for support firefox , chrome and IE . Filter is spacific attributes of IE
			block_flag_array[block_id]=!block_flag_array[block_id];	
		}
	}
	else if(mousedown_right_flag)
	{
		if(block_flag_array[block_id])
		{
			//document.getElementById(block_id).style.backgroundColor="#ff0000";
			//document.getElementById(block_id).style.filter="alpha(opacity=0)";
			$("#"+block_id).css("background-color", "#ff0000").css("opacity", "0"); //modify by xyd 
			block_flag_array[block_id]=!block_flag_array[block_id];
		}
	}
		
}

function select_all()
{	
	for(var i=1; i<=192; i++)
	{
		block_flag_array[i] = 1;
		//document.getElementById(i).style.backgroundColor="#ff0000";
		//document.getElementById(i).style.filter="alpha(opacity=20)";
		$("#"+i).css("background-color", "#ff0000").css("opacity", "0.2"); //modify by xyd
	}
}

function clean_all()
{	
	for(var i=1; i<=192; i++)
	{
		block_flag_array[i] = 0;
		//document.getElementById(i).style.backgroundColor="#ff0000";
		//document.getElementById(i).style.filter="alpha(opacity=0)";
		$("#"+i).css("background-color", "#ff0000").css("opacity", "0"); //modify by xyd
	}
}

function save_areas()
{
	var count = "";
	var ipaddr = window.location.host;
	var URL;
	var date=new Date().getMilliseconds();

	for(var i = 1; i <=192; i++)
	{
		block_flag_array_backup[i] = block_flag_array[i];
		if(block_flag_array[i])
		{
			if(count == "")
			{
				count = count+i;
			}
                     else
                     {
				count = count+"&"+i;
                     }
		}
	}
	
	URL="http://"+ipaddr+"/cgi-bin/motion_area.cgi?"+date+"@"+count;
	document.getElementById("areas_hiddenframe").src = URL;	
}

var block_flag_array_backup = new Array();
for(var i=1; i<=192; i++)
{
	block_flag_array_backup[i]=0;
}

function show_before()
{
	for(var i=1; i<=192; i++)
	{
		block_flag_array[i] = block_flag_array_backup[i];
		if(!block_flag_array_backup[i])
		{
			//document.getElementById(i).style.backgroundColor="#ff0000";
			//document.getElementById(i).style.filter="alpha(opacity=0)";
			$("#"+i).css("background-color", "#ff0000").css("opacity", "0"); //modify by xyd
		}
		else
		{
			//document.getElementById(i).style.backgroundColor="#ff0000";
			//document.getElementById(i).style.filter="alpha(opacity=20)";
			$("#"+i).css("background-color", "#ff0000").css("opacity", "0.2"); //modify by xyd
		}
	}
}

function show_areas(motion_areas)
{
	var array;

	if(motion_areas.replace(/(^\s*)|(\s*$)/g, "").length == 0)
	{
		return false;
	}
	
	array = motion_areas.split("&");
	
	for (var i=0 ; i < array.length ; i++)
	{    
		if(!block_flag_array[array[i]])
		{
			//document.getElementById(array[i]).style.backgroundColor="#ff0000";
			//document.getElementById(array[i]).style.filter="alpha(opacity=20)";
			$("#"+array[i]).css("background-color", "#ff0000").css("opacity", "0.2"); //modify by xyd
			block_flag_array[array[i]] = !block_flag_array[array[i]];	
			block_flag_array_backup[array[i]] = block_flag_array[array[i]];
		}
	}
	
}

var sensitivity_value;
var current_value;
var min_value;
var max_value;
var step_value;

$(function() {
$( "#slider" ).slider({
value:current_value,
min: min_value,
max: max_value,
step: step_value,
slide: function( event, ui ) {
$( "#amount" ).val( ui.value );
sensitivity_value = ui.value;
}
});
$( "#amount" ).val( $( "#slider" ).slider( "value" ) );
});

function motion_save()
{ 
	var ipaddr = window.location.host;
	var URL;
	var date=new Date().getMilliseconds();
	var sensitivity = sensitivity_value;
	
	save_areas();		
	
	URL="http://"+ipaddr+"/cgi-bin/motion_detection.cgi?action=md_save"+
		"&date="+date+"&sensitivity="+sensitivity;
	
	document.getElementById("motion_hiddenframe").src = URL;	
}

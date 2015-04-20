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
var block_flag_array_backup = new Array();
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
			//document.getElementById(block_id).style.backgroundColor=document.getElementById('pm_color').style.backgroundColor;
			//document.getElementById(block_id).style.filter="alpha(opacity=90)";
			$("#"+block_id).css("background-color", $("#pm_color").css("background-color")).css("opacity", "0.9"); 		
			block_flag_array[block_id]=!block_flag_array[block_id];	
		}
	}
	else if(mousedown_right_flag)
	{
		if(block_flag_array[block_id])
		{
			//document.getElementById(block_id).style.backgroundColor=document.getElementById('pm_color').style.backgroundColor;
			//document.getElementById(block_id).style.filter="alpha(opacity=0)";
			$("#"+block_id).css("background-color", $("#pm_color").css("background-color")).css("opacity", "0"); 		
			block_flag_array[block_id]=!block_flag_array[block_id];
		}
	}
		
}

function submit_pm()
{
	var count = "";
	var date_t=new Date().getMilliseconds();
	var i;
	var URL;
	var first=true;
	//var color=document.getElementById('pm_color').style.backgroundColor;
	//alert($("#pm_color").css("background-color"));
	var color=$("#pm_color").css("background-color").colorHex();
	//alert("color="+color);
	for(i = 1; i <=192; i++)
	{
		block_flag_array_backup[i] = block_flag_array[i];
		if(block_flag_array[i])
		{
			if(first)
				{
					count = count+i;
					first=false;
				}
			else
				count = count+"~"+i;
		}
	}
       URL = "/cgi-bin/private_mask.cgi?date="+date_t+"&op=action"+"&enable="+document.getElementById("pm_enable").checked+"&color="+"0x"+color.substring(1,color.length);
	if(!count)
	{
		URL += "&areas=";
	}
	else
	{
		URL += "&areas="+count;	
	}	
//	alert(URL);
	document.getElementById("hiddenframe_image").src = URL;
//	pm_enable_g = document.getElementById("pm_enable").checked;
//	pm_color_g = color.substring(1,color.length);
	pm_area_g =	count;

}

function show_areas_pm(pm_areas)
{
	var array = pm_areas.split("~");
	//alert(color+"=" + array.length);
	for (var i=0 ; i < array.length ; i++)
	{   
		if(!array[i])
			break;
		if(!block_flag_array[array[i]])
		{
			//document.getElementById(array[i]).style.backgroundColor=pm_color_g;
			//document.getElementById(array[i]).style.filter="alpha(opacity=90)";
			$("#"+array[i]).css("background-color", pm_color_g).css("opacity", "0.9"); 	
			block_flag_array[array[i]]=!block_flag_array[array[i]];	
			block_flag_array_backup[array[i]] = block_flag_array[array[i]];
		}
	}
	
}
function change_color(color)
{
	for (var i=0 ; i < block_flag_array.length ; i++)
	{   
		if(block_flag_array[i])
		{
			//document.getElementById(i).style.backgroundColor=color;
			//document.getElementById(i).style.filter="alpha(opacity=90)";
			$("#"+i).css("background-color", color).css("opacity", "0.9"); 	
		}
	}
}
function selectAll()
{
		for(var i=1; i<=192; i++)
		{
			block_flag_array[i]=1;
			//document.getElementById(i.toString()).style.backgroundColor=document.getElementById('pm_color').style.backgroundColor;
			//document.getElementById(i.toString()).style.filter="alpha(opacity=90)";
			$("#"+i).css("background-color", $("#pm_color").css("background-color")).css("opacity", "0.9"); 		
		}
}
function selectNone()
{
		for(var i=1; i<=192; i++)
		{
			block_flag_array[i]=0;
			//document.getElementById(i.toString()).style.backgroundColor=document.getElementById('pm_color').style.backgroundColor;
			//document.getElementById(i.toString()).style.filter="alpha(opacity=0)";
			$("#"+i).css("background-color", $("#pm_color").css("background-color")).css("opacity", "0"); 
		}
}
function reset()
{
	for(var i=1; i<=192; i++)
	{
		block_flag_array[i] = block_flag_array_backup[i];
		if(!block_flag_array_backup[i])
		{
			//document.getElementById(i.toString()).style.backgroundColor=document.getElementById('pm_color').style.backgroundColor;
			//document.getElementById(i.toString()).style.filter="alpha(opacity=0)";
			$("#"+i).css("background-color", $("#pm_color").css("background-color")).css("opacity", "0"); 
		}
		else
		{
			//document.getElementById(i.toString()).style.backgroundColor=document.getElementById('pm_color').style.backgroundColor;
			//document.getElementById(i.toString()).style.filter="alpha(opacity=90)";
			$("#"+i).css("background-color", $("#pm_color").css("background-color")).css("opacity", "0.9");
		}
	}

//	document.getElementById('pm_enable').checked=pm_enable_g;
//	document.getElementById('pm_color').style.backgroundColor=pm_color_g;
//	change_color(pm_color_g);
	
}
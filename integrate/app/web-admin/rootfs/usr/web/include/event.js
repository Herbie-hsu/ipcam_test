/***************************************************************************************************
* Copyright(C), Sonix Technology Co., Ltd.
* File name: event.js
* Author: jinxian_he
* Version: 1.0
* Date: 04/24/2013
* Description: This file provides javascript functions of event settins.
* History:    
  (1) V1.0(04/24/2013): Initial version.
***************************************************************************************************/
var  u_type = document.getElementById("UploadType").value;
change_upload_type(u_type);
/*************************************************************************************************
* Function : change_upload_type()
* Description: show upload type settings button according to the upload type.
* Input: upload_type
* Output: N/A
* Return: N/A
**************************************************************************************************/
function change_upload_type(upload_type)
{
	var  ut = document.getElementById("ut_settings");
	
	if(upload_type == "FTP")
	{
		if(ut.style.display == "none")
		{
			ut.style.display = "block";
		}
		//ut.value = "FTP server settings...";
		ut.value = ES_FTP_server_settings;
	}
	else if(upload_type == "Email")
	{
		if(ut.style.display == "none")
		{
			ut.style.display = "block";
		}
		//ut.value = "SMTP server settings...";
		ut.value = ES_SMTP_server_settings;
	}
	else if(upload_type == "Samba")
	{
		if(ut.style.display == "none")
		{
			ut.style.display = "block";
		}
		//ut.value = "Samba server settings...";
		ut.value = ES_Samba_server_settings;
	}
	else if(upload_type == "SDCard")
	{
		if(ut.style.display == "block")
		{
			ut.style.display = "none";
		}
		ut.value = "";
	}
}
/**************************************************************
* Function : upload_type_settings()
* Description:setting the ftp/smtp/samba server.
* Input: upload_server
* Output: N/A
* Return: N/A
***************************************************************/
function upload_server_settings(upload_server)
{
	var ipaddr = window.location.host;
	var URL;
	var date=new Date().getMilliseconds();
	
	if(upload_server == ES_FTP_server_settings)
	{
		URL="http://"+ipaddr+"/cgi-bin/ftp_settings.cgi?date="+date;
		window.open(URL, "newwindow", "width=600, height=300, left=180, top=100, status=yes");
	}
	else if(upload_server == ES_SMTP_server_settings)
	{
		URL="http://"+ipaddr+"/cgi-bin/smtp_settings.cgi?date="+date;
		window.open(URL, "newwindow", "width=600, height=450, left=180, top=100, status=yes");
	}
	else if(upload_server == ES_Samba_server_settings)
	{
		URL="http://"+ipaddr+"/cgi-bin/samba_settings.cgi?date="+date;
		window.open(URL, "newwindow", "width=600, height=350, left=180, top=100, status=yes");
	}	
}
/***********************************************************************************
* Function : show_or_hide_upload_video_image()
* Description: show/hide "upload video/image".
* Input: upload_video_image, upload_format
* Output: N/A
* Return: N/A
***********************************************************************************/
var if_upload = document.getElementById("UploadVideoImage").checked;
show_or_hide_upload_video_image(if_upload);
function show_or_hide_upload_video_image(upload_video_image)
{
	if(upload_video_image)
	{
		document.getElementById("upload_div_1").style.display = "block";
		document.getElementById("upload_div_2").style.display = "block";
		document.getElementById("upload_div_3").style.display = "block";
		document.getElementById("upload_div_4").style.display = "block";
		document.getElementById("upload_div_5").style.display = "block";
		document.getElementById("upload_div_6").style.display = "block";
	}
	else
	{
		document.getElementById("upload_div_1").style.display = "none";
		document.getElementById("upload_div_2").style.display = "none";
		document.getElementById("upload_div_3").style.display = "none";
		document.getElementById("upload_div_4").style.display = "none";
		document.getElementById("upload_div_5").style.display = "none";
		document.getElementById("upload_div_6").style.display = "none";
	}
	show_or_hide_snapshot(document.getElementById("UploadFormat").value);
}
/***********************************************************************************
* Function : show_or_hide_snapshot()
* Description: show/hide basefilename and pre-trigger seconds.
* Input: upload_format
* Output: N/A
* Return: N/A
***********************************************************************************/
function show_or_hide_snapshot(upload_format)
{
	if(upload_format == "snapshot")
	{
		document.getElementById("upload_div_4").style.display = "none";
		document.getElementById("upload_div_5").style.display = "none";
		document.getElementById("upload_div_6").style.display = "none";
	}
}
/****************************************************************************************************
* Function : change_upload_format()
* Description: show/hide "Include pre-trigger buffer" and "Base file name".
* Input: upload_format
* Output: N/A
* Return: N/A
******************************************************************************************************/
var  u_format = document.getElementById("UploadFormat").value;
change_upload_format(u_format);
function change_upload_format(upload_format)
{
	var if_upload = document.getElementById("UploadVideoImage").checked;
	if(upload_format == "stream1video"  
		|| upload_format == "stream2video"  
		|| upload_format == "stream3video")
	{
		if(if_upload)
		{
			document.getElementById("upload_div_4").style.display = "block";
			document.getElementById("upload_div_5").style.display = "block";
			document.getElementById("upload_div_6").style.display = "block";
		}
		else
		{
			document.getElementById("upload_div_4").style.display = "none";
			document.getElementById("upload_div_5").style.display = "none";
			document.getElementById("upload_div_6").style.display = "none";
		}
	}
	else if(upload_format == "snapshot")
	{
		document.getElementById("upload_div_4").style.display = "none";
		document.getElementById("upload_div_5").style.display = "none";
		document.getElementById("upload_div_6").style.display = "none";
	}
}
/**************************************************************************
* Function : event_settings_save()
* Description: save the settings of event settings web page.
* Input: if_sd
* Output: N/A
* Return: N/A
***************************************************************************/
function event_settings_save(if_sd)
{
	var ipaddr = window.location.host;
	var URL;
	var str, arr;
	var date=new Date().getMilliseconds();

	var event_enabled; 
	if(document.getElementById("EventEnabled").checked)
	{
		//if(!document.getElementById("MotionDetection").checked)
		//{
		//	alert("You must select a trigger in the \"Trigger by...\" \nif you want to enable the event.");
		//	return false;
		//}
		event_enabled = 1;
	}
	else
	{
		event_enabled = 0;
	}

	var motion_detection; 
	if(document.getElementById("MotionDetection").checked)
	{
		motion_detection = 1;
	}
	else
	{
		motion_detection = 0;
	}


	var upload_video_image;
	if(document.getElementById("UploadVideoImage").checked)
	{
		upload_video_image = 1;
	}
	else
	{
		upload_video_image = 0;
	}

	var upload_type = document.getElementById("UploadType").value; 
	var upload_format = document.getElementById("UploadFormat").value; 
	var pre_trigger_enabled;
	if(document.getElementById("PreTriggerEnabled").checked)
	{
		pre_trigger_enabled = 1;
	}
	else
	{
		pre_trigger_enabled = 0;
	}

	var pre_trigger_seconds = document.getElementById("PreTriggerSeconds").value; 
	var base_file_name = document.getElementById("BaseFilename").value;
	
	var motion_osd_enabled;
	if(document.getElementById("MotionOsdEnabled").checked)
	{
		motion_osd_enabled = 1;
	}
	else
	{
		motion_osd_enabled = 0;
	}
	str = document.getElementById("MotionOsdPicture").src;	
	arr = str.split('/');
	var motion_osd_picture = arr[arr.length-1];
	var motion_osd_position = document.getElementById("MotionOsdPosition").value;
	
	if(if_sd == 1)
	{
		var record_osd_enabled;
		if(document.getElementById("RecordOsdEnabled").checked)
		{
			record_osd_enabled = 1;
		}
		else
		{
			record_osd_enabled = 0;
		}
		str = document.getElementById("RecordOsdPicture").src;
		arr = str.split('/');
		var record_osd_picture = arr[arr.length-1];
		var record_osd_position = document.getElementById("RecordOsdPosition").value;

		URL="http://"+ipaddr+"/cgi-bin/event_settings.cgi?action=event_save"+"&date="+date+
			 "&EventEnabled="+event_enabled+"&MotionDetection="+motion_detection+
			 "&UploadVideoImage="+upload_video_image+"&UploadType="+upload_type+
			 "&UploadFormat="+upload_format+"&PreTriggerEnabled="+pre_trigger_enabled+
			 "&PreTriggerSeconds="+pre_trigger_seconds+"&BaseFilename="+base_file_name+
			 "&MotionOsdEnabled="+motion_osd_enabled+"&MotionOsdPicture="+motion_osd_picture+
			 "&MotionOsdPosition="+motion_osd_position+"&RecordOsdEnabled="+record_osd_enabled+
			 "&RecordOsdPicture="+record_osd_picture+"&RecordOsdPosition="+record_osd_position;
	}
	else
	{
			URL="http://"+ipaddr+"/cgi-bin/event_settings.cgi?action=event_save"+"&date="+date+
			 "&EventEnabled="+event_enabled+"&MotionDetection="+motion_detection+
			 "&UploadVideoImage="+upload_video_image+"&UploadType="+upload_type+
			 "&UploadFormat="+upload_format+"&PreTriggerEnabled="+pre_trigger_enabled+
			 "&PreTriggerSeconds="+pre_trigger_seconds+"&BaseFilename="+base_file_name+
			 "&MotionOsdEnabled="+motion_osd_enabled+"&MotionOsdPicture="+motion_osd_picture+
			 "&MotionOsdPosition="+motion_osd_position;
	}
	if(document.getElementById("MotionOsdEnabled").checked&&document.getElementById("RecordOsdEnabled").checked)
	{
		if(!confirm(ES_OSD_show_warning))
		{
			return;
		}
	}
	document.getElementById("event_hiddenframe").src = URL;	
}


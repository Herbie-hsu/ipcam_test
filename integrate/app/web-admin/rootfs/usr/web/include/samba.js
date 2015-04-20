/***************************************************************************************************
* Copyright(C), Sonix Technology Co., Ltd.
* File name: samba.js
* Author: jinxian_he
* Version: 1.0
* Date: 05/02/2013
* Description: This file provides javascript functions of samba settings.
* History:    
  (1) V1.0(05/02/2013): Initial version.
***************************************************************************************************/
/*********************************************************************
* Function : test_samba()
* Description: test if the samba server is connected or not.
* Input: N/A
* Output: N/A
* Return: N/A
**********************************************************************/
function test_samba(samba_server_message, samba_shared_folder_message, samba_user_message, samba_password_message)
{
	var ip_addr = window.location.host;
	var date=new Date().getMilliseconds();
	var URL;
	
	var form = document.samba_form;
	var samba_server = form.SambaServer.value;
	var samba_shared_folder = form.SambaSharedFolder.value; 
	var samba_user = form.SambaUser.value;
	var samba_password = form.SambaPassword.value;
	
	if(samba_server == "")
	{
		window.alert(samba_server_message);
		form.SambaServer.focus();
		return false;
	}

	if(samba_shared_folder == "")
	{
		window.alert(samba_shared_folder_message);
		form.SambaServer.focus();
		return false;
	}
	
	if(samba_user == "")
	{
		window.alert(samba_user_message);
		form.MailSender.focus();
		return false;
	}

	if(samba_password == "")
	{
		window.alert(samba_password_message);
		form.test_mail_addr.focus();
		return false;
	}

	URL="http://"+ip_addr+"/cgi-bin/samba_settings.cgi?action=samba_test"+"&date="+date+
		"&SambaServer="+samba_server+"&SambaSharedFolder="+samba_shared_folder+
		"&SambaUser="+samba_user+"&SambaPassword="+samba_password;
	
	document.getElementById("test_hiddenframe").src = URL;	
}

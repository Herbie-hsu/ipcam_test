/***************************************************************************************************
* Copyright(C), Sonix Technology Co., Ltd.
* File name: ftp.js
* Author: jinxian_he
* Version: 1.0
* Date: 04/24/2013
* Description: This file provides javascript functions of ftp settins.
* History:    
  (1) V1.0(04/24/2013): Initial version.
***************************************************************************************************/
/*********************************************************************
* Function : test_ftp()
* Description: test if the ftp server is connected or not.
* Input: N/A
* Output: N/A
* Return: N/A
**********************************************************************/
function test_ftp()
{
	var ip_addr = window.location.host;
	var date=new Date().getMilliseconds();
	var URL;

	var form = document.ftp_form;
	var ftp_server = form.FtpServer.value;
	var ftp_server_port = form.FtpServerPort.value;
	var ftp_user = form.FtpUser.value; 
	var ftp_password = form.FtpPassword.value; 
	
	if(ftp_server == "")
	{
		window.alert(FTP_Please_input_ftp_server);
		form.FtpServer.focus();
		return false;
	}
	
	if(ftp_server_port == "")
	{
		window.alert(FTP_Please_input_ftp_server_port);
		form.FtpServerPort.focus();
		return false;
	}
		
	if(ftp_user == "")
	{
		window.alert(FTP_Please_input_ftp_user);
		form.FtpUser.focus();
		return false;
	}
	
	
	if(ftp_password == "")
	{
		window.alert(FTP_Please_input_ftp_password);
		form.FtpPassword.focus();
		return false;
	}

	URL="http://"+ip_addr+"/cgi-bin/ftp_settings.cgi?action=ftp_test"+"&date="+date+
		  "&FtpServer="+ftp_server+"&FtpServerPort="+ftp_server_port+"&FtpUser="+
		  ftp_user+"&FtpPassword="+ftp_password;
	
	document.getElementById("test_hiddenframe").src = URL;	
}

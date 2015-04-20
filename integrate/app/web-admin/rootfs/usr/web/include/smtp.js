/***************************************************************************************************
* Copyright(C), Sonix Technology Co., Ltd.
* File name: smtp.js
* Author: jinxian_he
* Version: 1.0
* Date: 04/24/2013
* Description: This file provides javascript functions of smtp settins.
* History:    
  (1) V1.0(04/24/2013): Initial version.
***************************************************************************************************/
/*********************************************************************
* Function : test_mail()
* Description: test if the mail server is connected or not.
* Input: N/A
* Output: N/A
* Return: N/A
**********************************************************************/
function test_mail()
{
	var ip_addr = window.location.host;
	var date=new Date().getMilliseconds();
	var URL;
	
	var form = document.smtp_form;
	var mail_server = form.MailServer.value;
	var mail_server_port = form.MailServerPort.value;
	var mail_user = form.MailUser.value;
	var mail_password = form.MailPassword.value;
	var mail_sender = form.MailSender.value;
	var test_mail_addr = form.test_mail_addr.value; 
	
	if(mail_server == "")
	{
		window.alert(SMTP_Please_input_mail_server);
		form.MailServer.focus();
		return false;
	}

	if(mail_server_port == "")
	{
		window.alert(SMTP_Please_input_mail_server_port);
		form.MailServer.focus();
		return false;
	}

	if(mail_sender == "")
	{
		window.alert(SMTP_Please_input_mail_sender);
		form.MailSender.focus();
		return false;
	}

	if(test_mail_addr == "")
	{
		window.alert(SMTP_Please_input_test_mail_address);
		form.test_mail_addr.focus();
		return false;
	}

	URL="http://"+ip_addr+"/cgi-bin/smtp_settings.cgi?action=smtp_test"+"&date="+date+
		"&ip_addr="+ip_addr+"&MailServer="+mail_server+"&MailServerPort="+mail_server_port+
		"&MailUser="+mail_user+"&MailPassword="+mail_password+
		"&MailSender="+mail_sender+"&test_mail_addr="+test_mail_addr;
	
	document.getElementById("test_hiddenframe").src = URL;	
}
/*********************************************************************
* Function : test_mail()
* Description: test if the mail server is connected or not.
* Input: N/A
* Output: N/A
* Return: N/A
**********************************************************************/
function check_mail_format(which, error_message)
{
    var str=document.getElementById(which).value;
    var mailArray;   
    var patterns = /^[a-zA-Z0-9_-]+@[a-zA-Z0-9_-]+(\.[a-zA-Z0-9_-]+)+$/; 
    mailArray=str.split(",");
    for(i=0;i<mailArray.length;i++)
    { 
        if(patterns.test(mailArray[i]))
        {
            return true;
        }
        else
        {    
        	alert(error_message);
		document.getElementById(which).value="";
            return false;
        }
    }
}

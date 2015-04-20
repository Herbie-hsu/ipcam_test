/***************************************************************************************************
* Copyright(C), Sonix Technology Co., Ltd.
* File name: qos.js
* Author: jinxian_he
* Version: 1.0
* Date: 01/03/2014
* Description: This file provides javascript functions of qos.
* History:    
  (1) V1.0(01/03/2014): Initial version.
***************************************************************************************************/

/*******************************************************
* Function : save_data()
* Description: check and save dscp value
* Input: N/A
* Output: N/A
* Return: N/A
********************************************************/
function save_data()
{
	var form = document.qos_form;
	var liveview_dscp = form.liveview_dscp.value;
	var http_dscp = form.http_dscp.value;
	var ftp_dscp = form.ftp_dscp.value;
	
	if (liveview_dscp == "" || liveview_dscp < 0 || liveview_dscp > 63) 
	{
		alert(QS_The_DSCP_value_must_be_between_0_and_63)
		form.liveview_dscp.focus();
		form.liveview_dscp.select();
		return 0;
	}
	if (http_dscp == "" || http_dscp < 0 || http_dscp > 63)
	{
		alert(QS_The_DSCP_value_must_be_between_0_and_63)
		form.http_dscp.focus();
		form.http_dscp.select();
		return 0;
	}
	if (ftp_dscp == "" || ftp_dscp < 0 || ftp_dscp > 63)
	{
		alert(QS_The_DSCP_value_must_be_between_0_and_63)
		form.ftp_dscp.focus();
		form.ftp_dscp.select();
		return 0;
	}

	form.submit();
}
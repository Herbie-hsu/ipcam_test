/***************************************************************************************************
* Copyright(C), Sonix Technology Co., Ltd.
* File name: port_settings.js
* Author: jinxian_he
* Version: 1.0
* Date: 03/13/2014
* Description: This file provides javascript functions of port settings.
* History:    
  (1) V1.0(03/13/2014): Initial version.
***************************************************************************************************/
function save_data()
{
	var form = document.port_settings_form;
	var http_port = form.http_port.value;
	var https_port = form.SSLPort.value;
	var rtsp_port = form.rtsp_port.value;
	var two_way_audio_port = form.listen_port.value;

	/* Port can not be null */
	if (http_port == "" || http_port < 0 || http_port > 65535) 
	{
		alert(PS_The_http_port_must_be_between_0_and_65535)
		form.http_port.focus();
		form.http_port.select();
		return 0;
	}
	
	if (https_port == "" || https_port < 0 || https_port > 65535)
	{
		alert(PS_The_https_port_must_be_between_0_and_65535)
		form.SSLPort.focus();
		form.SSLPort.select();
		return 0;
	}
	
	if (rtsp_port == "" || rtsp_port < 0 || rtsp_port > 65535)
	{
		alert(PS_The_rtsp_port_must_be_between_0_and_65535)
		form.rtsp_port.focus();
		form.rtsp_port.select();
		return 0;
	}
	
	if (two_way_audio_port == "" || two_way_audio_port < 0 || two_way_audio_port > 65535)
	{
		alert(PS_The_two_way_audio_port_must_be_between_0_and_65535)
		form.listen_port.focus();
		form.listen_port.select();
		return 0;
	}

	/* Port can not be equal */
	if ((http_port == https_port) || (http_port == rtsp_port) || (http_port == two_way_audio_port)
		||(https_port == rtsp_port) ||(https_port == two_way_audio_port) || (rtsp_port == two_way_audio_port)) 
	{
		alert(PS_The_port_can_not_be_equal)
		return 0;
	}
	
	form.submit();
}

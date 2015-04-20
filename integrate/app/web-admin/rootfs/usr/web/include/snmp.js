/***************************************************************************************************
* Copyright(C), Sonix Technology Co., Ltd.
* File name: ftp.js
* Author: jinxian_he
* Version: 1.0
* Date: 09/26/2013
* Description: This file provides javascript functions of snmp settins.
* History:    
  (1) V1.0(09/26/2013): Initial version.
***************************************************************************************************/
/*********************************************************************
* Function : check_community()
* Description: check community names.
* Input: N/A
* Output: N/A
* Return: N/A
**********************************************************************/
function check_community()
{
	var form = document.snmp_form;
	var rocommunity = form.read_community.value;
	var rwcommunity = form.write_community.value; 
	
	if(rocommunity == "")
	{
		window.alert(SP_Read_community_can_not_be_null);
		form.read_community.focus();
		return false;
	}
	
	
	if(rwcommunity == "")
	{
		window.alert(SP_Write_community_can_not_be_null);
		form.write_community.focus();
		return false;
	}
	
	
	if(rocommunity == rwcommunity)
	{
		window.alert(SP_Read_community_and_write_community_can_not_be_same);
		form.read_community.focus();
		return false;
	}

	form.submit();
	
}

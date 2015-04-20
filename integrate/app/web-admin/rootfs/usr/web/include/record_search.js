/***************************************************************************************************
* Copyright(C), Sonix Technology Co., Ltd.
* File name: record_search.js
* Author: jinxian_he
* Version: 1.0
* Date: 04/28/2013
* Description: This file provides javascript functions of record search.
* History:    
  (1) V1.0(04/28/2013): Initial version.
***************************************************************************************************/
function sd_format(message)
{
	var form = document.sd_format_form;
	var check = confirm(message);
	
	if(check)
	{
		form.submit();
	}
}

function sd_umount(message)
{
	var form = document.sd_umount_form;
	var check = confirm(message);
	
	if(check)
	{
		form.submit();
	}
}

function sd_mount(message)
{
	var form = document.sd_mount_form;
	var check = confirm(message);
	
	if(check)
	{
		form.submit();
	}
}

function sd_download(filepath) {
    document.getElementById("filepath").value = filepath.toString();
    document.getElementById("download_file_from_SD").submit();
}

function hide_or_show_search_min(search_type)
{
	var  search_min = document.getElementById("search_min");
	
	if(search_type == "record")
	{
		if(search_min.style.display == "block")
		{
			search_min.style.display = "none";
		}
	}
	else if(search_type == "snapshot")
	{
		if(search_min.style.display == "none")
		{
			search_min.style.display = "block";
		}
	}

}

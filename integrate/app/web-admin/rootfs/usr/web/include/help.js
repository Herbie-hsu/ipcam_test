// all Javascripts used in WebAdmin should be added here
// lizhijie, 2006.07.22
// $Id: help.js 775 2010-11-22 05:30:17Z jinxian_he $

/*added by Yangbo*/
function checkDdnsText(formName)
{
	try{
		if(formName.username.value=="" ||formName.username.value.length==0)
		{
			alert(DN_Username_can_not_be_null);
			return false;
		}
		if(formName.password.value=="" ||formName.password.value.length==0)
		{
			alert(DN_Password_can_not_be_null);
			return false;
		}
		if(formName.domain.value=="" ||formName.domain.value.length==0)
		{
			alert(DN_Domain_name_can_not_be_null);
			return false;
		}
	}catch(exception)
	{
		alert(exception.message);
	}
}

/*added by YangBo 1-16*/
function deleteUser()
{
	var deleteConfirm = window.confirm(User_del_warning);
	if(!deleteConfirm)
		return false;
	var action = "delete";
  	var submitForm = deleteForm;
	var userForm = addUserForm;
  	var userName = userForm.userList.value;
  // Build submitstring
  	if(userName == "root")
  	{
  		alert(User_del_root_err);
		return false;
  	}
  	submitForm.action.value = action;
	submitForm.op.value = "user";
  	submitForm.user.value = userName;
  	submitForm.submit();
//	window.reload();
  	return true;  
}
function clearDeleteUser()
{
	var action = "";
  	var submitForm = deleteForm;
	
  // Build submitstring
  	submitForm.action.value = action;
	submitForm.op.value = "";
  	submitForm.user.value = "";
  	return true;  
}
/*added by YangBo 1-19*/
function openAddUserWindow()
{
	var action = "";
  	var submitForm = deleteForm;
 	
  // Build submitstring
  	submitForm.action.value = action;
	submitForm.op.value = "";
  	submitForm.user.value = "";
	window.open('/cgi-bin/sys_user.cgi?action=get_add_page','Adduser','width=350,height=300');
}

function openModifyUserWindow()
{
    var index = document.getElementById("userList").selectedIndex;
    var value = document.getElementById("userList")[index].value;
	window.open('/cgi-bin/sys_user.cgi?action=get_modify_page&user='+value,'Modifyuser','width=350,height=300');
}

/*display the user in order*/
function displayUser(document)
{
	var userArrayValue =new Array();
	var userArrayText = new Array();
	var userList = document.forms[0].elements[0];

	for(var i = 0; i < userList.length; i++)
	if(userList.options[i].value.toLowerCase() == "root")
 	{
		userArrayText = userArrayText.concat(userList.options[i].text);
		userArrayValue = userArrayValue.concat(userList.options[i].value);
		break;
 	}
	for(var i = 0; i < userList.length; i++)
	if(userList.options[i].text.match(User_admin)&& userList.options[i].value.toLowerCase()!= "root")
 	{
		userArrayText = userArrayText.concat(userList.options[i].text);
		userArrayValue = userArrayValue.concat(userList.options[i].value);
 	}
	for(var i = 0; i < userList.length; i++)
	if(userList.options[i].text.match(User_oper))
 	{
		userArrayText = userArrayText.concat(userList.options[i].text);
		userArrayValue = userArrayValue.concat(userList.options[i].value);
 	}

  	for(var i = 0; i < userList.length; i++)
	if(userList.options[i].text.match(User_viewer))
 	{
		userArrayText = userArrayText.concat(userList.options[i].text);
		userArrayValue = userArrayValue.concat(userList.options[i].value);
 	}
	for(var i = 0; i < userList.length; i++)
	{
		userList.options[i].value = userArrayValue[i];
		userList.options[i].text= userArrayText[i];
	}
}
function open_help_window(help_target )
{
	window.open (help_target, "newwindow", "height=250, width=550, top=80, left=25, toolbar=no, menubar=no, scrollbars=yes, resizable=no,location=no, status=no") 
}

function open_scan_preview_window(help_target )
{
	window.open (help_target, "newwindow", "height=650, width=750, top=5, left=5, toolbar=no, menubar=no, scrollbars=yes, resizable=no,location=no, status=no") 
}

function open_page( page_url)
{
	window.open(page_url, "mainframe" );

}

function open_page_view( page_url)
{
	window.open(page_url, "newwindow", "height=650, width=950, toolbar=no, menubar=no, scrollbars=yes, resizable=no,location=no, status=no");
}
function analyzeIP(num,str)
{
	if(num==3 && str<100)
		return false;
	if(num==2 && str<10)
		return false;
	
	return true;
}

function inspectIP(sIP)
{
	var sIPAddress=sIP;
	var IPsplit;
	var chkflag=true;
	var ErrMsg="���������һ�����淶��IP��ַ�Σ�\nIP��Ϊ:xxx.xxx.xxx.xxx��xxxΪ0-255)!";
	var len=0;
	var substr_one;
	len=sIPAddress.length;
	var i1;
	var i2;
	var i3;
	var i=0;
	var sub_i;
	
	substr_one =sIPAddress.substring(i,i+1);
	i1=i;i++;
	
	if ( len <7 || len >15)
		chkflag =false;
	else
	{
		while ( i <len && substr_one !='.' && chkflag != false)
		{
			substr_one = sIPAddress.substring(i,i+1);
			i++;
		}
		sub_i =sIPAddress.substring(i1, i-1);
		if ( substr_one != '.' || sub_i >255 || i-i1 <2 || sub_i=="")
			chkflag=false;
		if(chkflag==false)
			return chkflag;
			
		sub_i = sIPAddress.substring(i1,i1+1);
		if ( sub_i == '0')
			chkflag = false;
		
		i1=i;
		substr_one = sIPAddress.substring(i,i+1);
		i++;
		
		while (i<len && substr_one !='.' && chkflag !=false)
		{
			substr_one = sIPAddress.substring(i,i+1);
			i++;
		}
		
		if (chkflag == true)
		{
			sub_i =sIPAddress.substring(i1, i-1);
			if ( substr_one != '.' || sub_i >255 || i-i1 <1||sub_i=="")
				chkflag=false;
			if(chkflag!=false)
				chkflag=analyzeIP(i-i1-1,sub_i);
			i1=i;
			
			substr_one = sIPAddress.substring(i,i+1);
			i++;
		}
		
		while (i<len && substr_one !='.' && chkflag !=false)
		{
			substr_one = sIPAddress.substring(i,i+1);
			i++;
		}
		
		if (chkflag == true)
		{
			sub_i =sIPAddress.substring(i1, i-1);
			if ( substr_one != '.' || sub_i >255 || i-i1 <1||sub_i=="")
				chkflag=false;
			if(chkflag!=false)
				chkflag=analyzeIP(i-i1-1,sub_i);
			i1=i;
			substr_one = sIPAddress.substring(i,i+1);
			i++;
		}
		while ( i <len && substr_one !='.'&&  chkflag != false)
		{
			substr_one = sIPAddress.substring(i,i+1);i++;
		}
		
		if (chkflag == true)
		{
			sub_i =sIPAddress.substring(i1, i);
			if ( substr_one == '.' || sub_i >254 || i-i1 <1||sub_i=="")
				chkflag=false;
				
			if(chkflag!=false && i-i1<4)
				chkflag=analyzeIP(i-i1,sub_i);
			else
				chkflag=false;
		}
	}
	return chkflag;
}

function checkIP(sIPAddress, me)
{
	var ret=true;
	ret=inspectIP(sIPAddress);
	if (ret==false)
	{
		me.value="";
		document.me.focus();
		return false;
	}
}
function CheckIP(ipaddr) 
{
   var ip_reg=/[^0-9\.]/g;
   var t_var= $('#'+ipaddr).val();
   $('#'+ipaddr).val(t_var.replace(ip_reg,""));
}
function CheckIPAddress(ip_str)
{
    var myreg_ip = /^\b(([01]?\d?\d|2[0-4]\d|25[0-5])\.){3}([01]?\d?\d|2[0-4]\d|25[0-5])\b$/;
    var ret = ip_str.match(myreg_ip);
    if(ret==null)
        return false;
    else
        return true;
}
function CheckEthernet()
{
    var netRadio = $('input[name=netselect]:checked').val();
    var testArray=null;
    if(netRadio=="dhcp")
    {
    }
    else if(netRadio=="static")
    {
        testArray = new Array('ipblank','netmaskblank','gatewayblank');
    }
    else if(netRadio=="pppoe")
    {
        testArray = new Array('pdnsblank','pdnsblank_2');
    }
    if(testArray!=null)
        testArray.push('primary_dns','secondary_dns');
    else
        testArray= new Array('primary_dns','secondary_dns');
    for(var i=0; i<testArray.length;i++)
        if((testArray[i]!='secondary_dns'&&!CheckIPAddress($('#'+testArray[i]).val()))||
        (testArray[i]=='secondary_dns'&& $('#'+testArray[i]).val()!=""&&!CheckIPAddress($('#'+testArray[i]).val())))
        {
            alert(ETH_ERR_IP); 
            $('#'+testArray[i]).focus().select();
            return false;
        }
    return true;
}
function CheckNumber()
{
	var sNumber= document.getElementById(arguments[0]).value;
	var regu =/^\+?[1-9][0-9]*$/;
	var reg2=regu.test(sNumber);

    var ErrMsg="wrong number input!";   
 
    if(reg2==false)
    {
        alert(ErrMsg);
    }
    else
    {
 
    }
}
function isIP(strIP) {
if (isNull(strIP)) 
	return false;
var re=/^(\d+)\.(\d+)\.(\d+)\.(\d+)$/g 	
	if(re.test(strIP))
		{
			if( RegExp.$1 <256 && RegExp.$2<256 && RegExp.$3<256 && RegExp.$4<256) return true;
		}
	return false;
}


function checkIntegerNumber(obj)
{
	var objInput = obj;
	cod = window.event.keyCode;
	if ( !(  (cod >= 48) && (cod <= 57) ) )
	{
		alert("Only digital number can be input here!");
		window.event.keyCode = 0;
	}
}

// digit and '.' of IP address
function checkNumber(obj)
{
	var objInput = obj;
	cod = window.event.keyCode;
	if ( !(((cod >= 48) && (cod <= 57)) || (cod == 46) ))
	{
		window.event.keyCode = 0;
	}
}


function winconfirm()
{
	var question;
	question = confirm("����ϵͳ!��ȷ����?")
	if (question == true)
	{
		window.open("/cgi-bin/sysconf.cgi?vt=1&ct=reboot","mainframe");
	}
}

function checkIsNotNULL(checkField, msg )
{
	if( checkField.value=="")
	{
		alert("" + msg );
		return false;
	}
	return true;
}

/* browser may be block this new window */
function alertConfirm(target, alertMsg )
{
	var question;
	question = confirm( alertMsg +"?")
	if (question == true)
	{
		window.open( target, "mainframe" );
	}
}

// submit button with alert msg 
function submitConfirm(alertMsg, formName )
{
	var question;
	question = confirm( alertMsg +"?")
	if (question == true)
	{
		formName.submit(); 
	}
}

function inc(obj)
{
	var i = 0;
//	alert("increase number!");
	var curValue = parseInt(obj.data.value);
//	alert("increase number!" + curValue);
	if (curValue < 9)
		curValue ++;
	else 
		return;


	obj.data.value = curValue;

//	obj.submit();
}

function dec(obj)
{
	var i = 0;
	var curValue = parseInt(obj.data.value);
	if (curValue > 1)
		curValue --; 
	else
		return;
	obj.data.value = curValue;

//	obj.submit();
}

function mid(obj)
{
	var i = 0;
	
	obj.data.value = 5;

//	obj.submit();
}

function viewlog(index)
{
	if(index==1)
	{
		//window.open("/cgi-bin/sys_log.cgi?action=systemlog", "newwindow", "width=640, height=480, left=150, top=40, menubar=no, toolbar=no, scrollbars=yes, resizable=yes, location=no, status=no");
	    window.location.href="/cgi-bin/sys_log.cgi?action=systemlog";
	}
	else if(index==2)
	{
		//window.open("/cgi-bin/sys_log.cgi?action=sonixserverlog", "newwindow", "width=640, height=480, left=150, top=40, menubar=no, toolbar=no, scrollbars=yes, resizable=yes, location=no, status=no");
	    window.location.href="/cgi-bin/sys_log.cgi?action=sonixserverlog";
	}
	else if(index==3)
	{
		//window.open("/cgi-bin/sys_log.cgi?action=boalog", "newwindow", "width=640, height=480, left=150, top=40, menubar=no, toolbar=no, scrollbars=yes, resizable=yes, location=no, status=no");
	    window.location.href="/cgi-bin/sys_log.cgi?action=boalog";
	}
}
function viewMDSCH()
{
open_page_view("/cgi-bin/motionSCH");
}

function w_reload(){
	
var t=arguments[0];
var URL="/cgi-bin/motionSCH?"+t;
location.reload(URL);
}

function w_config_reload(){
	var	t1=start_hour.value;
	var	t2=start_min.value;
	var	t3=dayselect1.value;
	var	t4=end_hour.value;
	var	t5=end_min.value;
	var	t6=dayselect2.value;
	var str="&"+t1+"&"+t2+"&"+t3+"&"+t4+"&"+t5+"&"+t6;
	var URL="/cgi-bin/motionSCH?active"+str;
	document.getElementById("hiddenframe").src = URL;
	setTimeout("",2000);
	var URL="/cgi-bin/motionSCH";
	location.reload(URL);
}

function areasSel(){	
	var URL;	
	var date=new Date().getMilliseconds();
	URL="/cgi-bin/getbackground?"+date;	
	//alert(URL);
	document.getElementById("hiddenframe").src = URL;		
	//setTimeout('window.open("/areaselect.html", "mainframe");',5000);	 		
}

function hide_element()
{
var tmp=document.getElementById("from");
tmp.disabled=1;
var tmp=document.getElementById("to");
tmp.disabled=1;
}

function display_element()
{
var tmp=document.getElementById("from");
tmp.disabled=0;	
var tmp=document.getElementById("to");
tmp.disabled=0;
}

function change_operation(){
var tmp=document.getElementById("operation");
if(tmp.value==0)
hide_element();
if(tmp.value==1)
display_element();
}


function check_port(port)
{
	var tmp=document.getElementById(port);
	if(tmp.value>65535||tmp.value<1025)
	alert("the port can be set 1025~65535");
}

function check_user(port)
{

	var tmp=document.getElementById(port);
	var sNumber= tmp.value;
	var regu =/^\+?[1-9][0-9]*$/;
	var reg2=regu.test(sNumber);

    var ErrMsg="wrong number input!"    
 
    if(reg2==false)
    {
        alert(ErrMsg);
        return ;
    }
    else
    {
 
    }
	if(tmp.value>20||tmp.value<0)
	alert("the client can be set not more than 20!");
}

function check_maxBW(port)
{
	var tmp=document.getElementById(port);
	var sNumber= tmp.value;
	var regu =/^\+?[1-9][0-9]*$/;
	var reg2=regu.test(sNumber);

    var ErrMsg="wrong number input!"    
 
    if(reg2==false)
    {
        alert(ErrMsg);
        return ;
    }
    else
    {
 
    }
	if(tmp.value>20480||tmp.value<0)
	alert("the maxbandwidth can be set less than 20480K");
}

function restart_server()
{
	var bConfirm=window.confirm("Are you sure to restart now?");
	var t=window.location.host;
	var URL="http://"+t+"/cgi-bin/ServerReboot";
	if(bConfirm)
	{
		document.getElementById("rserver").src = URL;
		//setTimeout('alert("ffserver restart complete")',2000);
	//	window.open ("/waitRestart.html", "newwindow", "height=200, width=550, top=400, left=400, toolbar=no, menubar=no, scrollbars=yes, resizable=no,location=no, status=no") ;
		setTimeout('window.location.reload()',9000);
	//	URL="http://"+t+"/cgi-bin/content_cgi";
	//	document.getElementById("rserver").src = URL;
	//	window.location.reload();
	}
}
function change_day_select()
{
	var tmp=document.getElementById("Dayselect");
	var mon=document.getElementById("Monselect");
	var yearyear=document.getElementById("Yearselect");
	var	year=yearyear.value;
	var t=mon.value;
	var max;
	for(var m=0;m<5;m++){
		for(var i=1;i<=tmp.length;i++){
			tmp.remove(i);
		}
	}

	//for(var i=1;tmp.length>0;){
	//		tmp.remove(i);
	//}

	if(t==2){
		if((year%400==0)||((year%4==0)&&(year%100!=0)))
			max=29;
		else
		max=28;
	}
	else if(t==1||t==3||t==5||t==7||t==8||t==10||t==12)
	max=31;
	else
	max=30;
	
	for (var j=0;j<max;j++){
		tmp.add(new Option(j+1,j+1));
	}
}

function hide_dhcp()
{
	var tmp=document.getElementById("ipblank");
	tmp.disabled=1;
	
	tmp=document.getElementById("netmaskblank");
	tmp.disabled=1;
	
	tmp=document.getElementById("gatewayblank");
	tmp.disabled=1;
	
	tmp=document.getElementById("Userblank");
	tmp.disabled=1;
	
	tmp=document.getElementById("Passwordblank");
	tmp.disabled=1;
	
	tmp=document.getElementById("pdnsblank");
	tmp.disabled=1;

	tmp=document.getElementById("pdnsblank_2");
	tmp.disabled=1;
}  

function display_static()
{
	var tmp=document.getElementById("ipblank");
	tmp.disabled=0;

	tmp=document.getElementById("netmaskblank");
	tmp.disabled=0;
	
	tmp=document.getElementById("gatewayblank");
	tmp.disabled=0;
	
	tmp=document.getElementById("Userblank");
	tmp.disabled=1;
	
	tmp=document.getElementById("Passwordblank");
	tmp.disabled=1;
	
	tmp=document.getElementById("pdnsblank");
	tmp.disabled=1;

	tmp=document.getElementById("pdnsblank_2");
	tmp.disabled=1;
} 
	
function display_pppoe()
{
	var tmp=document.getElementById("ipblank");
	tmp.disabled=1;

	tmp=document.getElementById("netmaskblank");
	tmp.disabled=1;
	
	tmp=document.getElementById("gatewayblank");
	tmp.disabled=1;
	
	tmp=document.getElementById("Userblank");
	tmp.disabled=0;
	
	tmp=document.getElementById("Passwordblank");
	tmp.disabled=0;
	
	tmp=document.getElementById("pdnsblank");
	tmp.disabled=0;

	tmp=document.getElementById("pdnsblank_2");
	tmp.disabled=0;
}    

  
 function checkIPADD(IPADD){
 	 var tmp=document.getElementById("IPshow");
	 for(var i=0;i<tmp.length;i++){
		if(tmp.options[i].text==IPADD){
			return false;
		}
	 }
	 return true;
 	}
function add(){
 	 var tmp=document.getElementById("IPshow");
	 var ttt=document.getElementById("IPadd");
	 var to=document.getElementById("IPaddto");
	 var tt;
	 if(tmp.length>9){
		alert("10 items at most!");
		ttt.value="";
		to.value="";	
		return ;
	 }
	 if(!ttt.value&&!to.value){
	 	alert("fill IP in at least one blank!");
		return;
	 	}
	 if(ttt.value&&to.value){
		if(!CheckIP("IPadd")){
		ttt.value="";
		return;
		 }
		if(!CheckIP("IPaddto")){
		to.value="";
		return;
		 }
		if(!checkIPADD(ttt.value+" "+to.value)||!checkIPADD(to.value+" "+ttt.value)){
			alert("same IP address or range! ");
			return;
		}
		var tosplit=ttt.value;
		var result=tosplit.split(".");
		var from=parseInt(result[3]);
		tosplit=to.value;
		result=tosplit.split(".");
		var tto=parseInt(result[3]);
		if(tto<from){
			tt=to.value+" "+ttt.value;
			var tts=to.value+"&"+ttt.value;
		}
		else{
		 	tt=ttt.value+" "+to.value;
			var tts=ttt.value+"&"+to.value;
		}
		 tmp.add(new Option(tt,tt)); 	
	 	var t=window.location.host;
 		var URL="http://"+t+"/cgi-bin/effectiveACL?add&"+tts; 
		document.getElementById("hiddenframe1").src = URL;
		ttt.value="";
		to.value="";	
		return;
	 }
	if(ttt.value){
		if(!CheckIP("IPadd")){
			ttt.value="";
			return;
		 }
		if(!checkIPADD(ttt.value)){
			alert("same IP address or range! ");
			return;
		}
		 tt=ttt.value;
		 var tts=ttt.value;
		 tmp.add(new Option(tt,tt)); 	
	 	var t=window.location.host;
 		var URL="http://"+t+"/cgi-bin/effectiveACL?add&"+tts; 
		document.getElementById("hiddenframe1").src = URL;
		ttt.value="";
		return;
		
	}
	else{
		if(!CheckIP("IPaddto")){
			to.value="";
			return;
		 }
		if(!checkIPADD(to.value)){
			alert("same IP address or range! ");
			return;
		}
	 	tt=to.value;
		var tts=to.value;
 	 	tmp.add(new Option(tt,tt)); 	
	 	var t=window.location.host;
 		var URL="http://"+t+"/cgi-bin/effectiveACL?add&"+tts; 
		document.getElementById("hiddenframe1").src = URL;
		to.value="";
		}
 }

 function	remove(){
var tmp=document.getElementById("IPshow");
 		var i=tmp.selectedIndex;
		if((i==-1)){
 			alert("select first") ;
		}
		else{
		var todel=tmp.options[i].text;   
		var result=todel.split(" ");
		if(result[1]){
		var todels=result[0]+"&"+result[1];
			}
		else
		var	 todels=result[0];
		var t=window.location.host;
 		var URL="http://"+t+"/cgi-bin/effectiveACL?del&"+todels; 	
		document.getElementById("hiddenframe1").src = URL;
		 tmp.remove(i);
 		}   
}

function press_update(){	
	var value=document.getElementById("uploadfiles").value;	
	if(!value){		
		alert("no firmware file!");		
		return;	
		}	
	document.upfiles.submit();	
	//open_help_window("/update.html");	
}


function SetSensorDefault(channelname)
{
	var URL;
	var date=new Date().getMilliseconds();
	var s = document.getElementById(channelname);
	
	//URL="/cgi-bin/activeCtrl?modeindex&0"+date;	
	//alert(URL+":"+date);
	URL="/cgi-bin/activeCtrl?modeindex&"+s.value+"&"+date;	
	//alert(URL);
	
	document.getElementById("hiddenframedefault").src = URL;		
}

function checkMail(which){
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
        	alert("wrong format for E-mail");
		document.getElementById(which).value="";
            return false;
        }
    }
}

function bitrate_control()
{
	var c = document.getElementById("SelectChannel");
	var br_control, video_br;
	
	if(c.value == "1")
	{
		br_control = document.getElementById("Bitrate Control_1");
		video_br =document.getElementById("Video Bitrate_1");
	}
	else if(c.value == "2")
	{
		br_control = document.getElementById("Bitrate Control_2");
		video_br =document.getElementById("Video Bitrate_2");
	}
	else if(c.value == "3")
	{
		br_control = document.getElementById("Bitrate Control_3");
		video_br =document.getElementById("Video Bitrate_3");
	}
	else if(c.value == "4")
	{
		br_control = document.getElementById("Bitrate Control_4");
		video_br =document.getElementById("Video Bitrate_4");
	}
	
	if(br_control.value=="Constant Bit Rate")
	{
		video_br.disabled=1;
	}
	else if(br_control.value == "Variable Bit Rate")
	{
		video_br.disabled=0;
	}
	
}

function CheckMulticastAdd(h)
{
	var t=document.getElementById(h);
	var t_string=t.value.split(".");
	var mesg="Invalid MulticastAddress,it can be set 224.2.0.1~239.255.255.255! ";
	if(t_string[0]<224 ||t_string[0]>239)
	{
		alert(mesg);
		t.value="";
		return false;
	}
	if(t_string[1]<2 ||t_string[1]>255)
	{
		alert(mesg);
		t.value="";
		return false;
	}
	if(t_string[2]<0 ||t_string[2]>255)
	{
		alert(mesg);
		t.value="";
		return false;
	}
	if(t_string[3]<1 ||t_string[3]>255)
	{
		alert(mesg);
		t.value="";
		return false;
	}
	 return true;
}
/*******************************************************************************************
* function: show the stream configration of the selected channel
* input para: channelname
* output para: N/A
* return: N/A
* author: jinxian_he
********************************************************************************************/
function ShowSelectedChannelStreamInfo(channelname)
{
	var c = document.getElementById(channelname);
	var i = 0;
	var newstr1="", newstr2="", newstr3="", newstr4="", newstr5="", newstr6="";
	var newstr7="", newstr8="", newstr9="", newstr10="", newstr11="", newstr12="";
	var newstr13="", newstr14="", newstr15="", newstr16="", newstr17="", newstr18="";

	for(i = 1; i <= 4; i++)
	{
		newstr1 = "Audio on/off"+i;
		newstr2 = "Audio Samplerate"+i;
		newstr3 = "Audio Channel"+i;
		
		newstr4 = "Video Size"+i;
		newstr5 = "Bitrate Control"+i;
		newstr6 = "Video Bitrate"+i;
		newstr7 = "Video Framerate"+i;
		
		newstr8 = "EnableMulticast"+i;
		newstr9 = "MulticastAddress"+i;
		newstr10 = "MulticastPort"+i;

		newstr11 = "Flicker"+i;
		newstr12 = "Rotateimage"+i;
		newstr13 = "Lowlight"+i;
		newstr14 = "Whitebalance"+i;
		newstr15 = "Brightness"+i;
		newstr16 = "Contrast"+i;
		newstr17 = "Sharpness"+i;
		newstr18 = "Saturation"+i;

		if(i == c.value)
		{
			//Audio on/off
			document.getElementById(newstr1).style.display = "inline"; 
			//Audio Samplerate
			document.getElementById(newstr2).style.display = "inline";
			//Audio Channel
			document.getElementById(newstr3).style.display = "inline";

			//Video Size
			document.getElementById(newstr4).style.display = "inline";
			//Bitrate Control
			document.getElementById(newstr5).style.display = "inline";
			//Video Bitrate
			document.getElementById(newstr6).style.display = "inline";
			//Video Framerate
			document.getElementById(newstr7).style.display = "inline";

			//Enable Multicast
			document.getElementById(newstr8).style.display = "inline";
			//MulticastAddress
			document.getElementById(newstr9).style.display = "inline";
			//MulticastPort
			document.getElementById(newstr10).style.display = "inline";

			//Flicker
			document.getElementById(newstr11).style.display = "inline";
			//Rotateimage
			document.getElementById(newstr12).style.display = "inline";
			//Lowlight
			document.getElementById(newstr13).style.display = "inline";
			//Whitebalance
			document.getElementById(newstr14).style.display = "inline";
			//Brightness
			document.getElementById(newstr15).style.display = "inline";
			//Contrast
			document.getElementById(newstr16).style.display = "inline";
			//Sharpness
			document.getElementById(newstr17).style.display = "inline";
			//Saturation
			document.getElementById(newstr18).style.display = "inline";
		}
		else
		{
			//Audio on/off
			document.getElementById(newstr1).style.display = "none";
			//Audio Samplerate
			document.getElementById(newstr2).style.display = "none";
			//Audio Channel
			document.getElementById(newstr3).style.display = "none";

			//Video Size
			document.getElementById(newstr4).style.display = "none";
			//Bitrate Control
			document.getElementById(newstr5).style.display = "none";
			//Video Bitrate
			document.getElementById(newstr6).style.display = "none";
			//Video Framerate
			document.getElementById(newstr7).style.display = "none";

			//Enable Multicast
			document.getElementById(newstr8).style.display = "none";
			//MulticastAddress
			document.getElementById(newstr9).style.display = "none";
			//MulticastPort
			document.getElementById(newstr10).style.display = "none";

			//Flicker
			document.getElementById(newstr11).style.display = "none";
			//Rotateimage
			document.getElementById(newstr12).style.display = "none";
			//Lowlight
			document.getElementById(newstr13).style.display = "none";
			//Whitebalance
			document.getElementById(newstr14).style.display = "none";
			//Brightness
			document.getElementById(newstr15).style.display = "none";
			//Contrast
			document.getElementById(newstr16).style.display = "none";
			//Sharpness
			document.getElementById(newstr17).style.display = "none";
			//Saturation
			document.getElementById(newstr18).style.display = "none";
		}
		
	}
}

/****************************************************************************************
* function: show the record configration of the selected channel
* input para: channelname, weekname
* output para: N/A
* return: N/A
* author: jinxian_he
*****************************************************************************************/
function ShowSelectedChannelRecordConfig(channelname, weekname)
{
	var c = document.getElementById(channelname);
	var w = document.getElementById(weekname);
	var i=0, j=0,k=0;
	var newstr="";
	
	for(i = 1; i <=4; i++)
	{
		for(j = 1; j <= 7; j++)
		{
			for(k = 1; k <= 6; k++)
			{
				newstr = "channel"+i+"_"+"week"+j+"_"+"time"+k;
				//alert(newstr);
				if(i == c.value && j == w.value)
				{
					document.getElementById(newstr).style.display = "inline";
				}
				else
				{
					document.getElementById(newstr).style.display = "none";
				}
			}
		}
	}
}
/**************************************************************************************************
* function: show the hour_min_sec control for the time selection
* includes: SetTime( ), SelectTime( ),  noPermitInput( ), isIE( )
* author: jinxian_he
***************************************************************************************************/
var str = "";
document.writeln("<div id=\"contents\" style=\"padding:6px; background-color:#E3E3E3; font-size: 12px; border: 1px solid #777777;  position:absolute; left:?px; top:?px; width:?px; height:?px; z-index:1; visibility:hidden\">");
str += "Hour<select name=\"hour\">";
for (h = 0; h <= 9; h++)
{    
	str += "<option value=\"0" + h + "\">0" + h + "</option>";
}
for (h = 10; h <= 23; h++) 
{ 
	str += "<option value=\"" + h + "\">" + h + "</option>";
}
str += "</select>";
str += "Min<select name=\"minute\">";
for (m = 0; m <= 9; m++)
{ 
	str += "<option value=\"0" + m + "\">0" + m + "</option>";
}
for (m = 10; m <= 59; m++) 
{ 
	str += "<option value=\"" + m + "\">" + m + "</option>";
}
str += "</select>";
str += "Sec<select name=\"second\">";
for (s = 0; s <= 9; s++)
{ 
	str += "<option value=\"0" + s + "\">0" + s + "</option>";
}
for (s = 10; s <= 59; s++)
{ 
	str += "<option value=\"" + s + "\">" + s + "</option>";
}
str += "</select>";
str += "<input type=\"button\" name=\"queding\" onclick=\"SelectTime()\" value=\"ok\"  style=\"font-size:12px\" />";
str += "</div>";
document.writeln(str);
var fieldname;
function SetTime(tt)
{ 
	fieldname = tt; 
	var ttop = tt.offsetTop;    //TT�ؼ��Ķ�λ���?
	var thei = tt.clientHeight;    //TT�ؼ�����ĸ�?
	var tleft = tt.offsetLeft;    //TT�ؼ��Ķ�λ���? 
	while (tt = tt.offsetParent)  
	{        
		ttop += tt.offsetTop;   
		tleft += tt.offsetLeft;  
	}    
	document.all.contents.style.top = ttop + thei + 4;   
	document.all.contents.style.left = tleft;  
	document.all.contents.style.visibility = "visible";
}
function SelectTime() 
{   
	fieldname.value = document.all.hour.value + ":" + document.all.minute.value + ":" + document.all.second.value;    
	document.all.contents.style.visibility = "hidden";
}			
function noPermitInput(E)			
{					  
	var evt = window.event || e ;					
	if(isIE())				
	{			
		evt.returnValue=false; //ie ��ֹ��������					
	}
	else
	{
		evt.preventDefault(); //fire fox ��ֹ��������
	}		
}		
function isIE()
{	
	if (window.navigator.userAgent.toLowerCase().indexOf("msie") >= 1)
	{
		return true;
	}
	else
	{
		return false;
	}
} 
/*******************************************************************************************
* function: download record file to local PC
* input para: N/A
* output para: N/A
* return: N/A
* author: jinxian_he
********************************************************************************************/
  function   download(filename)  
{     
 	return confirm("Do you want to download the file of \n\n\"" + filename + "\"?\n");   
}

  function   submit_for_search( form )  
{     
	var temp,type;
	//var s_type=document.getElementsByName("searchtype_t"); //also OK
	var s_type=document.all.searchtype_t;
	for(var i=0;i<s_type.length;i++)
		{
				if(s_type[i].checked == true)
				type=s_type[i].value;
		}
	var start_calendar=document.getElementById("start_calendar").value;
	if(start_calendar=="")
	{
		alert("Please check your configuration !");
		return;
	}
	

	var starttime=document.getElementById("starttime").value;
	if(starttime=="")
	{
		alert("Please check your configuration !");
		return;
	}
	
	var end_calendar=document.getElementById("end_calendar").value;
	if(end_calendar=="")
	{
		alert("Please check your configuration !");
		return;
	}
	
	var endtime=document.getElementById("endtime").value;
	if(endtime=="")
	{
		alert("Please check your configuration !");
		return;
	}
	if((start_calendar+starttime)>(end_calendar+endtime))
		{
		alert("Please check your configuration !");
		return;
		}
	var channel=document.getElementById("channel").value;
	var cmdurl=type+"="+start_calendar+"="+starttime+"="+end_calendar+"="+endtime+"="+channel
	window.open("/cgi-bin/search_display?"+cmdurl, "mainframe");
}
function submit_for_return_search()
{
	window.open("/cgi-bin/video/video.cgi?op=VideoSearch", "mainframe");


}
function submit_for_backup()
{
	var searchform=document.searchrecordform;
	var flag=0;

	for(var i=0;i<searchform.elements.length;i++)
	{
		if(searchform.elements[i].checked)
		{
			flag=1;
			break;
		}		
	}
	if(flag==0)
	{
		alert("At least select one file!");
	}
	else
	{
		//alert(filelist);
		//window.open("/cgi-bin/backup?"+filelist, "mainframe");
		window.open("/cgi-bin/backup");

	}
	
}
function execbackup()
{
	var submitform=submitForm;
	var filelist="";
	var tmp="";
	var comfirm;


	for(var i=0;i<window.opener.document.forms[0].elements.length;i++)
	{

		if(window.opener.document.forms[0].elements[i].checked)
		{
			tmp=window.opener.document.forms[0].elements[i].name+"/"+window.opener.document.forms[0].elements[i].value;

			filelist=tmp+" "+filelist;
		}
		
	}
	if(document.Backupform.Devicelist.value=="")
	{
		alert("Choose target!");
	}
	else
	{	
		comfirm=window.confirm("It will take some times,are you sure?");
		if(!comfirm)
		{
			return false;
		}

		submitform.filelist.value=filelist;

		submitform.device.value=document.Backupform.Devicelist.value;
		submitform.submit();//-----------
		return true;
	}
	
}

function connecting()
{
	var check=window.document.forms[0].elements["check"];
	var ap="";

	for(var i=0;i<check.length;i++)
	{

		if(check[i].checked==true)
		{
			ap=check[i].value;
			break;
		}
	}
	if(ap=="")
	{
		alert(WL_2warning);
		return false;
	}
	window.open("/cgi-bin/wireless?op=wireless&action=connect&accesspoint="+ap);
	return true;
}

document.onkeydown = function()
{
	if(event.keyCode == 8) //backspace ,except focus on textbox , passwordbox or textarea
	{
		if(((document.activeElement.tagName=="INPUT"||document.activeElement.tagName=="input")&&
			(document.activeElement.type=="text"||document.activeElement.type=="password"))||
			document.activeElement.tagName=="textarea"||document.activeElement.tagName=="TEXTAREA")
			{
				event.returnValue = true;
			}
			
		else
		{
			event.keyCode = 0;
			event.returnValue = false;
		}
	}
	if(event.keyCode == 13) //enter
	{
		event.keyCode = 0;
		event.returnValue = false;
	}
	if(event.keyCode == 116) //F5
	{
		event.keyCode = 0;
		event.returnValue = false;
	}
	if((event.ctrlKey) && (event.keyCode == 78)) //ctrl+n
	{
		event.keyCode = 0;
		event.returnValue = false;
	}
	if((event.altKey) && ((window.event.keyCode == 37) || (window.event.keyCode == 39))) //Alt+<- or  Alt+->
	{
		event.keyCode = 0;
		event.returnValue = false;
	}
	if((event.shiftKey) && (event.keyCode == 121)) //shift+F10
	{
		event.keyCode = 0;
		event.returnValue = false;
	}
}

document.oncontextmenu = function() //right menu
{
	event.returnValue = false;
}

function get_date()
{
	var t=new Date();
	var ret;
	var month=1+t.getMonth();
	ret=t.getFullYear()+"-"+month+"-"+t.getDate();
	return ret;
}

function get_time()
{
	var t=new Date();
	var ret;
	ret = t.getHours()+":"+t.getMinutes()+":"+t.getSeconds();
	return ret;
}

function get_datetime()
{
	return get_date()+" " + get_time();
}

function trigger_1s()
{
	
	var time=get_datetime();
	document.getElementById("computer_time").value=time;
	document.getElementById("computer_time_show").value=time;
	document.getElementById("current_time").innerText = get_datetimeByseconds(time_seconds);
	time_seconds ++;
}
function get_dateByseconds(seconds)
{
	var t=new Date();
	var ret;
	t.setTime(seconds*1000);
	var month=1+t.getMonth();
	ret = t.getFullYear()+"-"+month+"-"+t.getDate();
	return ret;
	
}
function get_timeByseconds(seconds)
{
	var t=new Date();
	var ret;
	t.setTime(seconds*1000);
	ret = t.getHours()+":"+t.getMinutes()+":"+t.getSeconds();
	return ret;
}
function get_datetimeByseconds(seconds)
{
	return get_dateByseconds(seconds)+" " + get_timeByseconds(seconds);
}
function date_time_init()
{
	var time=get_datetime();
	document.getElementById("computer_time").value=time;
	document.getElementById("computer_time_show").value=time;
	document.getElementById("manually_date").value = get_dateByseconds(time_seconds);
	document.getElementById("manually_time").value = get_timeByseconds(time_seconds);
	document.getElementById("TimeZone").selectedIndex=re_time_mode_select;
	document.getElementById("ntp_server").value = document.getElementById("re_ntp").value;
	$(document).unbind("ready");
	$(document).ready(function () {
	    $("#TimeZone").unbind("change");
	    $("#TimeZone").change(function () {
	        var param = {
	            r: new Date().getTime(),
	            action: "gettime",
	            time_zone: $(this).val().replace(/\_.*/, "")
	        };
	        $.get("/cgi-bin/sys_time.cgi", param, function (data, status) {
	            if (status == "success") {
	                time_seconds = new Date(data).getTime() / 1000;
	                $("#time_zone_current").text($("#TimeZone option:selected").text());
	                trigger_1s();
	            }
	        });
	    });
	});
	$("#TimeZone").change();
	
}

function date_time_ready()
{
    $("#savetz").val(Date_set_time_zone_btn).click(function () {
            $("[name=action]").val("savetz");
        });
}
function remove_ip_addr()
{
	var    add_form;
	var    selected_ip_addr;
	var    remove_confirm;
	var    remove_form;

	add_form = window.document.liveview_acl_add_form;
	selected_ip_addr = add_form.ip_list.value;
	if(!selected_ip_addr)
	{
	    alert(LVA_Remove_select_warning);
		return false;
	}

    remove_confirm = window.confirm(LVA_Remove_warning);
	if(!remove_confirm)
	{
		return false;
	}
       
	remove_form = window.document.liveview_acl_remove_form;
	remove_form.ip_addr.value = selected_ip_addr.substr(0, selected_ip_addr.length-1);
	
  	remove_form.submit();
	
  	return true;  
}

function remove_ipaddr()
{
	var    add_form;
	var    selected_ip_addr;
	var    remove_confirm;
	var    remove_form;

	add_form = window.document.ip_addr_filter_add_form;
	selected_ip_addr = add_form.ip_list.value;
	if(!selected_ip_addr)
	{
		alert(IAF_Remove_select_warning);
		return false;
	}

	remove_confirm = window.confirm(IAF_Remove_warning);
	if(!remove_confirm)
	{
		return false;
	}
       
	remove_form = window.document.ip_addr_filter_remove_form;
	remove_form.ip_addr.value = selected_ip_addr.substr(0, selected_ip_addr.length-1);
	
  	remove_form.submit();
	
  	return true;  
}
function tvFrameRates(width)
{
	var fps_id = document.getElementById("frameRate");
	fps_id.options.length=0;
	if(width == 1280 || width == 640)
	{
		fps_id.add(new Option("1fps","1"));
		fps_id.add(new Option("2fps","2"));
		fps_id.add(new Option("3fps","3"));
		fps_id.add(new Option("4fps","4"));
		fps_id.add(new Option("5fps","5"));		
		fps_id.add(new Option("6fps","6"));
		fps_id.add(new Option("7fps","7"));
		fps_id.add(new Option("10fps","10"));
	}
	else
	{
		fps_id.add(new Option("2fps","2"));
		fps_id.add(new Option("4fps","4"));
		fps_id.add(new Option("6fps","6"));
		fps_id.add(new Option("8fps","8"));
		fps_id.add(new Option("10fps","10"));		
		fps_id.add(new Option("12fps","12"));
		fps_id.add(new Option("14fps","14"));
		fps_id.add(new Option("20fps","20"));
		fps_id.add(new Option("30fps","30"));
	}
}
function image_set(type, value)
{
	var cmdurl;
	cmdurl="/cgi-bin/imagesetting.cgi?op=action"+"&type="+type+"&value="+value;
	document.getElementById("hiddenframe_image").src = cmdurl;
	//alert(cmdurl);
}

function frequency(i)
{
	var frq =document.getElementById("frequency");
	frq[i].selected=true;
}
function setBlackWhite()
{
	var str;
	var bw=document.getElementById("blackwhite");
	//alert(bw.checked);
	if(bw.checked)
		image_set("str",0);
	else
	{
		var saturation=document.getElementById("sliderstrdetail").innerHTML;
		str=saturation.substring(11,saturation.length);
		image_set("str",str);
	}
}
function setWhiteBalanceAuto()
{
	var wba=document.getElementById("whitebalanceauto");
	if(wba.checked)
		image_set("wb_auto",1);
	else
	{
		image_set("wb_auto",0);
	}
}
function setFrq()
{
	var frq=document.getElementById("frequency");
	//alert(frq.value);
	image_set("frq",frq.value);
}

function setCookie(c_name,value,expiredays){
	var exdate=new Date();
	exdate.setDate(exdate.getDate()+expiredays);
	document.cookie=c_name+ '=' +escape(value)+((expiredays==null) ? '' : ';expires='+exdate.toGMTString());
}
function getCookie(c_name){
	if (document.cookie.length>0){
		c_start=document.cookie.indexOf(c_name + '=');
		if (c_start!=-1){ 
			c_start=c_start + c_name.length+1;
				        c_end=document.cookie.indexOf(';',c_start);
				        if (c_end==-1) 
						c_end=document.cookie.length;
			 return unescape(document.cookie.substring(c_start,c_end));
		} 
	}
	return '';
}

function change_lang(language)
{
	
	var ipaddr = window.location.host;
	var URL;
	var date=new Date().getMilliseconds();
	
	URL="http://"+ipaddr+"/cgi-bin/save_language.cgi?"+"date="+date+"&lang="+language;
	document.getElementById("main_hiddenframe").src = URL;	
	
	setTimeout("window.parent.frames['topframe'].location.reload()", 1000);
	setTimeout("window.parent.frames['bottomframe'].location.reload()", 1000);
}

function select_time_mode_change(elem){
	var select_t = document.getElementById("select_time_mode");
	
	var manually_div = document.getElementById("manually_div");
	var computer_div = document.getElementById("computer_div");
	var ntp_div = document.getElementById("ntp_div");
	
	var manually_text = document.getElementById("manually_text");
	var manually_val = document.getElementById("manually_val");
	var computer_text = document.getElementById("computer_text");
	var computer_val = document.getElementById("computer_val");
	var ntp_text = document.getElementById("ntp_text");
	var ntp_val = document.getElementById("ntp_val");
	
	
	document.getElementById("action").value=elem.value;
	
	manually_text.style.display = manually_val.style.display = "none";
	computer_text.style.display = computer_val.style.display = "none";
	ntp_text.style.display = ntp_val.style.display = "none";
	manually_div.style.display = computer_div.style.display = ntp_div.style.display="none";
	if(select_t.value=="computer"){
		computer_div.style.display="";
		computer_text.style.display = computer_val.style.display ="";
	}
	else if(select_t.value=="ntp"){
		ntp_div.style.display="";
		ntp_text.style.display = ntp_val.style.display = "";
	}
	else{
		manually_div.style.display="";
		manually_text.style.display = manually_val.style.display ="";
	}
	
}

function time_radio_click(elem)
{
	document.getElementById("action").value=this.value;
}

function dw(s)
{
	document.write(s);
}

function cconnecting()
{
	var check=document.getElementsByName("check");
	var ap="";

	for(var i=0;i<check.length;i++)
	{

		if(check[i].checked==true)
		{
			ap=check[i].value;
			break;
		}
	}
	if(ap=="")
	{
	    alert(WL_2warning);
		return false;
	}
	location.href="/cgi-bin/wireless.cgi?op=wireless&action=connect&accesspoint="+ap;
	return true;
}

function DI(id, innertext)
{
	document.getElementById(id).innerText=innertext;
}
function DV(id, value)
{
	document.getElementById(id).value=value;	
}

String.prototype.trim= function(){  
    return this.replace(/(^\s*)|(\s*$)/g, "");  
}

String.prototype.format = function()
{
    var args = arguments;
    return this.replace(/\{(\d+)\}/g,                
        function(m,i){
            return args[i];
        });
}

function isDate(dateString) {
    var date_err_info_str = Date_err_info.format(Date_man_data, get_date(), Date_man_time, get_time());
    var dtstring = dateString.trim();
    if (dtstring == "") {
        alert(date_err_info_str);
        return false;
    }
    var r = dtstring.match(/^(\d{1,4})\-(\d{1,2})\-(\d{1,2}) (\d{1,2}):(\d{1,2}):(\d{1,2})$/);
    if (r == null) {
        alert(date_err_info_str);
        return false;
    }
    var d = new Date(r[1], r[2] - 1, r[3], r[4], r[5], r[6]);
    var num = (d.getFullYear() == r[1] && (d.getMonth() + 1) == r[2] && d.getDate() == r[3] && d.getHours() == r[4] && d.getMinutes() == r[5] && d.getSeconds() == r[6]);
    if (num == 0) {
        alert(date_err_info_str);
    }
    if (d.getTime() < Date.parse("1/1/2000 0:0:0")) {
        alert(Date_range_err);
        num = 0;
    }
    if (d.getTime() > Date.parse("1/1/2037 0:0:0")) {
        alert(Date_range_err);
        num = 0;
    }
    return (num != 0);
}

function isDateTime(){
	var date_test = document.getElementById("manually_date").value;
	var time_test = document.getElementById("manually_time").value;
	var dtstring = date_test.trim() + " " + time_test.trim();
	return isDate(dtstring);
}
  function setRadioBox(radioName, radiovalue)  
  {  
    var obj = document.getElementsByName(radioName);
    for(i = 0; i < obj.length; i++)
    {  

      if(obj[i].value == radiovalue)
      {  
        obj[i].checked = true;
      }  
    }
    
    return true;
  }
   function getRadioBoxValue(radioName)  
  {  
    var obj = document.getElementsByName(radioName);
    for(i = 0; i < obj.length; i++)
    {  

      if(obj[i].checked == true)
      {  
        return obj[i].value;
      }  
    }
    
    return false;
  }
  
  function check_file_empty(name)
  {
    if(document.getElementById(name).value&&document.getElementById(name).value!="")
    return false;
    else
    {
    alert(Maintenance_no_inputfile);
    return true;
    }
  }

function change_font_family_to_songti(id)
{
    var elem= document.getElementById(id);
    if(elem)
    {
        elem.style.fontFamily="宋体";
    }
}

function clear_etc_checkbox_onclick(x)
{
    if(x.checked)
        document.getElementById("clear_etc_value").value="true";
    else
        document.getElementById("clear_etc_value").value="false";
}

String.prototype.colorHex = function(){
	var reg = /^#([0-9a-fA-f]{3}|[0-9a-fA-f]{6})$/;
	var that = this;
	if(/^(rgb|RGB)/.test(that)){
		var aColor = that.replace(/(?:\(|\)|rgb|RGB)*/g,"").split(",");
		var strHex = "#";
		for(var i=0; i<aColor.length; i++){
			var hex = Number(aColor[i]).toString(16);
			/*if(hex === "0"){
				hex += hex;	
			}
			*/
			if(hex.length == 1){
				hex = "0" + hex;
			}
			strHex += hex;
		}
		if(strHex.length !== 7){
			strHex = that;	
		}
		return strHex;
	}else if(reg.test(that)){
		var aNum = that.replace(/#/,"").split("");
		if(aNum.length === 6){
			return that;	
		}else if(aNum.length === 3){
			var numHex = "#";
			for(var i=0; i<aNum.length; i+=1){
				numHex += (aNum[i]+aNum[i]);
			}
			return numHex;
		}
	}else{
		return that;	
	}
};
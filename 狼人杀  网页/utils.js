function getCookie(c_name){
	if (document.cookie.length>0){ 
		c_start=document.cookie.indexOf(c_name + "=");
		if (c_start!=-1){ 
			c_start=c_start + c_name.length+1;
			c_end=document.cookie.indexOf(";",c_start);
			if (c_end==-1)
				c_end=document.cookie.length;
			return unescape(document.cookie.substring(c_start,c_end));
		}
	}
	return "";
}

function setCookie(c_name,value){
	var exdate=new Date(),expiredays=1;
	exdate.setDate(exdate.getDate()+expiredays)
	document.cookie=c_name+ "=" +escape(value)+
	((expiredays==null) ? "" : "; expires="+exdate.toGMTString())
}
function writeKillList(num){//记录死亡的玩家，使该玩家对象的life属性为0
	if(num!=0)
		lrs_data.persons[num-1].life=0;//num-1就是人的号码减一
}
function judge(){
	var lrs_data = JSON.parse(getCookie('lrs_data'));
	var wolfNum=0/*狼人数量*/,personNum=0/*平民数量*/,godNum=0/*神的数量*/;
	for(var i in lrs_data.persons){//遍历库中每个人物对象的type属性，判断游戏有多少个神，民，狼
			if(lrs_data.persons[i].type=="defender"||lrs_data.persons[i].type=="prophet"||lrs_data.persons[i].type=="witch"||lrs_data.persons[i].type=="hunter"||lrs_data.persons[i].type=="fool")
				godNum++;//有一个神则神数量加一
			if(lrs_data.persons[i].type=="villager")
				personNum++;//有一个民则民数量加一
			if(lrs_data.persons[i].type=="wolf")
				wolfNum++;//有一个狼则狼数量加一
	}
	for(var i in lrs_data.persons){//再次遍历库中每个人物对象的life属性
			if((lrs_data.persons[i].type=="defender"||lrs_data.persons[i].type=="prophet"||lrs_data.persons[i].type=="witch"||lrs_data.persons[i].type=="hunter"||lrs_data.persons[i].type=="fool")&&lrs_data.persons[i].life==0)
				godNum--;//如果神的life属性为0，则神的数量减一
			if(lrs_data.persons[i].type=="villager"&&lrs_data.persons[i].life==0)
				personNum--;//如果民的life属性为0，则民的数量减一
			if(lrs_data.persons[i].type=="wolf"&&lrs_data.persons[i].life==0)
				wolfNum--;//如果狼的life属性为0，则狼的属性减一
	}
	if(wolfNum==0)//判定狼的数量，如果为0，则平民获胜
		location.href="./ice_ghost_personWin.html";
	else if(personNum==0||godNum==0)//判定神和民的数量，有一方为0，则狼获胜
		//改进方案：将屠边与屠城加入游戏的选择中，以此决定获胜的条件
		location.href="./ice_ghost_wolfWin.html";
}
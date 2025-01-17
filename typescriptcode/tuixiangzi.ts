var c=document.getElementById("myCanvas");
            var ctx=c.getContext("2d");
            var img_1 = new Image();
            img_1.src="墙.png";
            var img_9 = new Image();
            img_9.src="人.png";
            var img_3 = new Image();
            img_3.src="完成地点.png";
            var img_4 = new Image();
            img_4.src="完成的箱子.png";
            var img_2 = new Image();
            img_2.src="箱子.png";
            var img_0 = new Image();
            img_0.src="空.png";
            var map=[[1,1,1,1,1,0,0,0,0],
                     [1,0,0,9,1,0,0,0,0],
                     [1,0,2,0,1,0,1,1,1],
                     [1,0,2,0,1,0,1,4,1],
                     [1,1,1,0,1,1,1,3,1],
                     [0,1,1,0,0,0,0,3,1],
                     [0,1,0,0,0,1,0,0,1],
                     [0,1,0,0,0,1,1,1,1],
                     [0,1,1,1,1,1,0,0,0]];//0→空,1→墙,2→箱子,3→点,4→箱子在点上,9→人
            var peo={
                x:1,
                y:3
            };
            window.onload = function(){
                for(var i=0;i<9;i++){
                    for(var j=0;j<9;j++){
                        switch(map[i][j]){
                            case  1 :ctx.drawImage(img_1,j*50,i*50);break;
                            case  0 :ctx.drawImage(img_0,j*50,i*50);break;
                            }
                        }
                    }
                Flash();
            }
            function Flash(){
                for(var i=0;i<9;i++){
                    for(var j=0;j<9;j++){
                        switch(map[i][j]){
                            case 0:ctx.drawImage(img_0,j*50,i*50);break;
                            case 1:ctx.drawImage(img_1,j*50,i*50);break;
                            case 2:ctx.drawImage(img_2,j*50,i*50);break;
                            case 3:ctx.drawImage(img_3,j*50,i*50);break;
                            case 4:ctx.drawImage(img_4,j*50,i*50);break;
                            case 9:ctx.drawImage(img_9,j*50,i*50);break;
                        }
                    }
                }
            }
            function move(a){
                if(a==1){
                    if(map[peo.x-1][peo.y]==2||map[peo.x-1][peo.y]==4){
                        if(map[peo.x-2][peo.y]!=1&&map[peo.x-2][peo.y]!=2&&map[peo.x-2][peo.y]!=4){
                            map[peo.x-2][peo.y]=2;
                            map[peo.x-1][peo.y]=0;
                            map[peo.x][peo.y]=0;
                            peo.x=peo.x-1;
                        }
                    }
                    else{
                        if(map[peo.x-1][peo.y]!=1){
                            map[peo.x][peo.y]=0;
                            peo.x=peo.x-1;
                        }
                    }
                }
                else if(a==2){
                    if(map[peo.x][peo.y+1]==2||map[peo.x][peo.y+1]==4){
                        if(map[peo.x][peo.y+2]!=1&&map[peo.x][peo.y+2]!=2&&map[peo.x][peo.y+2]!=4){
                            map[peo.x][peo.y+2]=2;
                            map[peo.x][peo.y+1]=0;
                            map[peo.x][peo.y]=0;
                            peo.y=peo.y+1;
                        }
                    }
                    else{
                        if(map[peo.x][peo.y+1]!=1){
                            map[peo.x][peo.y]=0;
                            peo.y=peo.y+1;
                        }
                    }
                }
                else if(a==3){
                    if(map[peo.x+1][peo.y]==2||map[peo.x+1][peo.y]==4){
                        if(map[peo.x+2][peo.y]!=1&&map[peo.x+2][peo.y]!=2&&map[peo.x+2][peo.y]!=4){
                            map[peo.x+2][peo.y]=2;
                            map[peo.x+1][peo.y]=0;
                            map[peo.x][peo.y]=0;
                            peo.x=peo.x+1;
                        }
                    }
                    else{
                        if(map[peo.x+1][peo.y]!=1){
                            map[peo.x][peo.y]=0;
                            peo.x=peo.x+1;
                        }
                    } 
                }
                else if(a==0){
                    if(map[peo.x][peo.y-1]==2||map[peo.x][peo.y-1]==4){
                        if(map[peo.x][peo.y-2]!=1&&map[peo.x][peo.y-2]!=2&&map[peo.x][peo.y-2]!=4){
                            map[peo.x][peo.y-2]=2;
                            map[peo.x][peo.y-1]=0;
                            map[peo.x][peo.y]=0;
                            peo.y=peo.y-1;
                        }
                    }
                    else{
                        if(map[peo.x][peo.y-1]!=1){
                            map[peo.x][peo.y]=0;
                            peo.y=peo.y-1;
                        }
                    }
                }
            }
            function judge(){
                if(map[3][7]==0){
                    map[3][7]=3;
                }
                if(map[4][7]==0){
                    map[4][7]=3;
                }   
                if(map[5][7]==0){
                     map[5][7]=3;
                }
                if(map[3][7]==2){
                    map[3][7]=4;
                }
                if(map[4][7]==2){
                    map[4][7]=4;
                }   
                if(map[5][7]==2){
                     map[5][7]=4;
                }
                map[peo.x][peo.y]=9;
            }
            //Flash();
            document.onkeydown = function(e) { 
                var code = e.keyCode - 37;
                move(code);
                judge();
                Flash();
                if(map[3][7]==4&&map[4][7]==4&&map[5][7]==4){
                    alert("Lady Luck is smiling!");
                    alert("START ANEW");
                    location.reload();
                }
	// switch(code){ 
	// 	case 1 : ;break;//上 
	// 	case 2 : ;break;//右 
	// 	case 3 : ;break;//下 
	// 	case 0 : ;break;//左 
	// }
            }
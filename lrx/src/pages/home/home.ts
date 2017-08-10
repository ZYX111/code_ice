import { Component } from '@angular/core';
import { NavController } from 'ionic-angular';

@Component({
  selector: 'page-home',
  templateUrl: 'home.html'
})
export class HomePage {
  myCanvas:any;
  constructor(public navCtrl: NavController) {
    for(var i=0;i<9;i++){
      for(var j=0;j<9;j++){
        switch (this.map[i][j]) {
          case 1: this.ctx.drawImage(this.img_1, j * 50, i * 50); break;
          case 0: this.ctx.drawImage(this.img_0, j * 50, i * 50); break;
        }
      }
    }
    this.Flash();
  }
  c:any=document.getElementById(this.myCanvas);
  ctx:any=this.c.getContext("2d");
  img_1:string='assets/images/墙.png'
  img_9:string='assets/images/人.png'
  img_3:string='assets/images/完成地点.png'
  img_4:string='assets/images/完成的箱子.png'
  img_2:string='assets/images/箱子.png'
  img_0:string='assets/images/空.png'
  map:any=[[1,1,1,1,1,0,0,0,0],
            [1,0,0,9,1,0,0,0,0],
            [1,0,2,0,1,0,1,1,1],
            [1,0,2,0,1,0,1,4,1],
            [1,1,1,0,1,1,1,3,1],
            [0,1,1,0,0,0,0,3,1],
            [0,1,0,0,0,1,0,0,1],
            [0,1,0,0,0,1,1,1,1],
            [0,1,1,1,1,1,0,0,0]];//0→空,1→墙,2→箱子,3→点,4→箱子在点上,9→人
  peo:any={
      x:1,
      y:3
  };
  Flash(){
      for(var i=0;i<9;i++){
          for(var j=0;j<9;j++){
            switch (this.map[i][j]) {
              case 0: this.ctx.drawImage(this.img_0, j * 50, i * 50); break;
              case 1: this.ctx.drawImage(this.img_1, j * 50, i * 50); break;
              case 2: this.ctx.drawImage(this.img_2, j * 50, i * 50); break;
              case 3: this.ctx.drawImage(this.img_3, j * 50, i * 50); break;
              case 4: this.ctx.drawImage(this.img_4, j * 50, i * 50); break;
              case 9: this.ctx.drawImage(this.img_9, j * 50, i * 50); break;
              }
          }
      }
  }
  move(a:any){
      if(a==1){
        if (this.map[this.peo.x - 1][this.peo.y] == 2 || this.map[this.peo.x - 1][this.peo.y] == 4) {
          if (this.map[this.peo.x - 2][this.peo.y] != 1 && this.map[this.peo.x - 2][this.peo.y] != 2 && this.map[this.peo.x - 2][this.peo.y] != 4) {
            this.map[this.peo.x - 2][this.peo.y] = 2;
            this.map[this.peo.x - 1][this.peo.y] = 0;
            this.map[this.peo.x][this.peo.y] = 0;
            this.peo.x = this.peo.x - 1;
              }
          }
          else{
          if (this.map[this.peo.x - 1][this.peo.y] != 1) {
            this.map[this.peo.x][this.peo.y] = 0;
            this.peo.x = this.peo.x - 1;
              }
          }
      }
      else if(a==2){
        if (this.map[this.peo.x][this.peo.y + 1] == 2 || this.map[this.peo.x][this.peo.y + 1] == 4) {
          if (this.map[this.peo.x][this.peo.y + 2] != 1 && this.map[this.peo.x][this.peo.y + 2] != 2 && this.map[this.peo.x][this.peo.y + 2] != 4) {
            this.map[this.peo.x][this.peo.y + 2] = 2;
            this.map[this.peo.x][this.peo.y + 1] = 0;
            this.map[this.peo.x][this.peo.y] = 0;
            this.peo.y = this.peo.y + 1;
          }
        }
        else{
          if (this.map[this.peo.x][this.peo.y + 1] != 1) {
            this.map[this.peo.x][this.peo.y] = 0;
            this.peo.y = this.peo.y + 1;
          }
        }
      }
      else if(a==3){
        if (this.map[this.peo.x + 1][this.peo.y] == 2 || this.map[this.peo.x + 1][this.peo.y] == 4) {
          if (this.map[this.peo.x + 2][this.peo.y] != 1 && this.map[this.peo.x + 2][this.peo.y] != 2 && this.map[this.peo.x + 2][this.peo.y] != 4) {
            this.map[this.peo.x + 2][this.peo.y] = 2;
            this.map[this.peo.x + 1][this.peo.y] = 0;
            this.map[this.peo.x][this.peo.y] = 0;
            this.peo.x = this.peo.x + 1;
          }
        }
        else{
          if (this.map[this.peo.x + 1][this.peo.y] != 1) {
            this.map[this.peo.x][this.peo.y] = 0;
            this.peo.x = this.peo.x + 1;
          }
        } 
      }
      else if(a==0){
        if (this.map[this.peo.x][this.peo.y - 1] == 2 || this.map[this.peo.x][this.peo.y - 1] == 4) {
          if (this.map[this.peo.x][this.peo.y - 2] != 1 && this.map[this.peo.x][this.peo.y - 2] != 2 && this.map[this.peo.x][this.peo.y - 2] != 4) {
            this.map[this.peo.x][this.peo.y - 2] = 2;
            this.map[this.peo.x][this.peo.y - 1] = 0;
            this.map[this.peo.x][this.peo.y] = 0;
            this.peo.y = this.peo.y - 1;
          }
        }
        else{
          if (this.map[this.peo.x][this.peo.y - 1] != 1) {
            this.map[this.peo.x][this.peo.y] = 0;
            this.peo.y = this.peo.y - 1;
          }
        }
      }
  }
  judge(){
    if (this.map[3][7] == 0) {
        this.map[3][7] = 3;
      }
      if (this.map[4][7] == 0) {
        this.map[4][7] = 3;
      }   
      if (this.map[5][7] == 0) {
        this.map[5][7] = 3;
      }
      if (this.map[3][7] == 2) {
        this.map[3][7] = 4;
      }
      if (this.map[4][7] == 2) {
        this.map[4][7] = 4;
      }   
      if (this.map[5][7] == 2) {
        this.map[5][7] = 4;
      }
      this.map[this.peo.x][this.peo.y] = 9;
  }
  right(){
    this.move(0);
    this.judge();
    this.Flash();
    if (this.map[3][7] == 4 && this.map[4][7] == 4 && this.map[5][7] == 4) {
          alert("Lady Luck is smiling!");
          alert("START ANEW");
    }
  }
  up(){
    this.move(1);
    this.judge();
    this.Flash();
    if (this.map[3][7] == 4 && this.map[4][7] == 4 && this.map[5][7] == 4) {
          alert("Lady Luck is smiling!");
          alert("START ANEW");
    }
  }
  left(){
    this.move(2);
    this.judge();
    this.Flash();
    if (this.map[3][7] == 4 && this.map[4][7] == 4 && this.map[5][7] == 4) {
          alert("Lady Luck is smiling!");
          alert("START ANEW");
    }
  }
  down(){
    this.move(3);
    this.judge();
    this.Flash();
    if (this.map[3][7] == 4 && this.map[4][7] == 4 && this.map[5][7] == 4) {
          alert("Lady Luck is smiling!");
          alert("START ANEW");
    }
  }
}

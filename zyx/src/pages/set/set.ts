import { Component } from '@angular/core';

import { NavController, AlertController } from 'ionic-angular';
import { DataService } from '../../services/dataservice';
import { licensingPage } from '../licensing/licensing';

@Component({
  selector: 'page-set',
  templateUrl: 'set.html'
})
export class setPage {
  userNumber:number;
  witchNumber:boolean;
  huntsManNumber:boolean;
  prophetNumber:boolean;
  guardNumber:boolean;
  idiotNumber:boolean;
  wereWolfNumber:number;
  playerList= new Array(this.userNumber);
  constructor(private alertCtrl: AlertController, private dataService: DataService,private navCtrl:NavController) {
    this.userNumber=8;
    this.witchNumber=false;
    this.huntsManNumber=false;
    this.prophetNumber=false;
    this.guardNumber=false;
    this.idiotNumber=false;
    this.wereWolfNumber=3;
  }
  setUp(){
    for (var i = 0; i < this.userNumber; i++) {
			var idx = Math.floor(Math.random() * this.userNumber);
			this.playerList[i]={
        number:idx+1,
				type: "civilian",
				name:"平民",
				image:'assets/images/civilian.jpg',
				life:1
			};
		}
    for (var i = 0; i < this.wereWolfNumber; i++) {
      var idx = Math.floor(Math.random() * this.userNumber);
      if (this.playerList[idx].type == "wereWolf")
				i--;
      this.playerList[idx] = {
          number:idx+1,
				type: "wereWolf",
				name:"狼人",
				image:"assets/images/wereWolf.jpg",
				life:1
			};
		}
    if (this.witchNumber) {
			for(;;){
        var idx = Math.floor(Math.random() * this.userNumber);
        if (this.playerList[idx].type == "civilian") {
          this.playerList[idx] = {
            number:idx+1,
						type: "witch",
						name:"女巫",
						image:'assets/images/witch.jpg',
						life:1
					};
					break;
				}
			}
		}
    if (this.huntsManNumber) {
			for(;;){
        var idx = Math.floor(Math.random() * this.userNumber);
        if (this.playerList[idx].type == "civilian") {
          this.playerList[idx] = {
            number:idx+1,
						type: "huntsMan",
						name:"猎人",
						image:'assets/images/huntsMan.jpg',
						life:1
					};
					break;
				}
			}
		}
    if (this.idiotNumber) {
			for(;;){
        var idx = Math.floor(Math.random() * this.userNumber);
        if (this.playerList[idx].type == "civilian") {
          this.playerList[idx] = {
            number:idx+1,
						type: "idiot",
						name:"白痴",
						image:'assets/images/idiot.jpg',
						life:1
					};
					break;
				}
			}
		}
    if (this.guardNumber) {
			for(;;){
        var idx = Math.floor(Math.random() * this.userNumber);
        if (this.playerList[idx].type == "civilian") {
          this.playerList[idx] = {
            number:idx+1,
						type: "guard",
						name:"守卫",
						image:'assets/images/guard.jpg',
						life:1
					};
					break;
				}
			}
		}
    if (this.prophetNumber) {
			for(;;){
        var idx = Math.floor(Math.random() * this.userNumber);
        if (this.playerList[idx].type == "civilian") {
          this.playerList[idx] = {
            number:idx+1,
						type: "prophet",
						name:"预言家",
						image:'assets/images/prophet.jpg',
						life:1
					};
					break;
				}
			}
	  }	
  }

  next(){
    this.setUp();   
    this.dataService.setPlayerList({playerList:this.playerList});
    this.navCtrl.push(licensingPage);
	}
}

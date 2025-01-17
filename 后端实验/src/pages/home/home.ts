import { Component } from '@angular/core';

import { NavController, NavParams } from 'ionic-angular';
import { Geolocation } from 'ionic-native';
import { AlertController } from 'ionic-angular';

import { DataService } from '../../services/data.service'
import { WorkListPage } from './workList/workList';
import { CheckWorkPage } from './checkWork/checkWork';
import { TaskPage } from './task/list/list';
import { DynTaskPage } from './dyntask/list/list';

@Component({
	selector: 'page-home',
	templateUrl: 'home.html'
})
export class HomePage {
	
	mySlideOptions = {
	    initialSlide: 1,
	    loop: true,
	    paper: true,
	    autoplay: 3000
  	};

	slides = [
	    { image: "assets/img/bar1.jpg" },
	    { image: "assets/img/bar2.jpg" }
  	];

  	constructor(public navCtrl: NavController, public navParams: NavParams, private dataService: DataService, public alertCtrl: AlertController) {
  		
  	}

  	itemTapped(event,item){
  		switch(item){
  			case 1:
            if(this.dataService.getLocalData('taskdata', 'id')==null){
              if(this.dataService.isDebug()){
                this.navCtrl.push(WorkListPage, {
                  lon: 123.347626,
                  lat: 41.667719
                });
              }
              else{
                Geolocation.getCurrentPosition().then((resp) => {
                  this.navCtrl.push(WorkListPage, {
                    lon: resp.coords.longitude,
                    lat: resp.coords.latitude
                  });
                }).catch((error) => {
                  console.log('Error getting location', error);
                      let alert = this.alertCtrl.create({
                        title: '错误',
                        subTitle: error,
                        buttons: ['确认']
                      });
                      alert.present();
                });
              }              
            }else{
              this.navCtrl.push(CheckWorkPage);
            };

  			break;
  			case 2:
  				this.navCtrl.push(WorkListPage);
  			break;
  			case 3:
  				this.navCtrl.push(WorkListPage);
  			break;
  			case 4:
  				this.navCtrl.push(WorkListPage);
  			break;
  		}  
  	}
    showGbtc(){
    }
    showTask(){
      this.navCtrl.push(TaskPage);
    }
    showDynTask(){
      this.navCtrl.push(DynTaskPage);
    }
    showSupervise(){
    }
}

import { Component } from '@angular/core';

import { AlertController } from 'ionic-angular';

import { Geolocation } from 'ionic-native';

@Component({
  templateUrl: 'alert.html'
})
export class AlertPage {

  constructor(public alerCtrl: AlertController) { }

  doAlert() {
  	Geolocation.getCurrentPosition().then((resp) => {
		 // resp.coords.latitude
		 // resp.coords.longitude
		 	    let alert = this.alerCtrl.create({
			      title: 'New Friend!',
			      message: resp.coords.longitude+','+resp.coords.longitude,
			      buttons: ['Ok']
			    });
			    alert.present();
		}).catch((error) => {
		  console.log('Error getting location', error);
		});

  }

}
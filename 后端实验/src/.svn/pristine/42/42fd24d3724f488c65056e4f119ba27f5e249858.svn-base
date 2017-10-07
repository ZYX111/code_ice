import { Component } from '@angular/core';

import { NavController } from 'ionic-angular';

import { TabsPage } from '../tabs/tabs';

import { Http } from '@angular/http';
import 'rxjs/add/operator/toPromise';

@Component({
	templateUrl: 'login.html'
})

export class LoginPage {

    loginid: any;
    password: any;

  	constructor(public navCtrl: NavController,private http: Http) {

  	}

    login(){
        this.navCtrl.push(TabsPage);
    }

}

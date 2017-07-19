import { Component } from '@angular/core';

import { NavController, AlertController } from 'ionic-angular';
import { DataService } from '../../services/dataservice';
import { setPage } from '../set/set';
import { licensingPage } from '../licensing/licensing';

@Component({
  selector: 'page-home',
  templateUrl: 'start.html'
})
export class startPage {
  constructor(private alertCtrl: AlertController, private dataService: DataService,private navCtrl: NavController) {
  }
  next(){
    this.navCtrl.push(setPage);
  }
  next1(){
    this.navCtrl.push(licensingPage);
  }
}

import { Component } from '@angular/core';

import { NavController, AlertController } from 'ionic-angular';
import { DataService } from '../../services/dataservice';

@Component({
  selector: 'page-home',
  templateUrl: 'licensing.html'
})
export class licensingPage {
  playerList= new Array();
  constructor(private alertCtrl: AlertController, private dataService: DataService) {
    this.dataService.nightArithmetic();
  }

}

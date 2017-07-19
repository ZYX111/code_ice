import { Component } from '@angular/core';

import { NavController, AlertController } from 'ionic-angular';
import { DataService } from '../../services/dataservice';

@Component({
  selector: 'page-home',
  templateUrl: 'policeFace.html'
})
export class policeFacePage {
  constructor(private alertCtrl: AlertController, private dataService: DataService) {
  }
}

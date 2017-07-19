import { Component } from '@angular/core';

import { NavController, AlertController } from 'ionic-angular';
import { DataService } from '../../services/dataservice';

@Component({
  selector: 'page-home',
  templateUrl: 'non_playerNightFace.html'
})
export class non_playerNightFacePage {
  constructor(private alertCtrl: AlertController, private dataService: DataService) {
  }
}

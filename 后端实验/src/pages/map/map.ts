import { Component } from '@angular/core';

import { NavController } from 'ionic-angular';

@Component({
  selector: 'page-about',
  templateUrl: 'map.html'
})
export class MapPage {

	areaItems = [{
        title: '地点1'
    },{
        title: '地点2'
    },{
        title: '地点3'
    },{
        title: '地点4'
    }];

	constructor(public navCtrl: NavController) {

	}

}

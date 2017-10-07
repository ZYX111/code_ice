import { Component } from '@angular/core';

import { NavController, NavParams } from 'ionic-angular';

import { ItemDetailsPage } from '../item-details/item-details';


@Component({
  templateUrl: 'datacenter.html'
})
export class DataCenterPage {
  selectedItem: any;
  icons: string[];
  items: Array<{title: string, note: string, icon: string}>;

  constructor(public navCtrl: NavController, public navParams: NavParams) {

  }

  itemTapped() {
    this.navCtrl.push(ItemDetailsPage, {

    });
  }
}

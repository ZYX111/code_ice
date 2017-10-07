import { Component } from '@angular/core';

import { NavController, NavParams } from 'ionic-angular';

import { DataService } from '../../services/data.service'

@Component({
  selector: 'list-page',
  templateUrl: 'userList.html'
})
export class UserListPage {
  selectedItem: any;
  icons: string[];
  items: Array<any>;

  constructor(public navCtrl: NavController, public navParams: NavParams, private dataService: DataService) {
    // If we navigated to this page, we will have an item available as a nav param
    this.selectedItem = navParams.get('item');

    // this.items = this.dataService.getDatas('select%20*%20from%20fw_user');
  }

  itemTapped(event, item) {
  /*
    this.navCtrl.push(ItemDetailsPage, {
      item: item
    });
    */
  }
}

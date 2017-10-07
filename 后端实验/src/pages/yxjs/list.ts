import { Component } from '@angular/core';

import { NavController, NavParams, LoadingController } from 'ionic-angular';

import { ItemDetailsPage } from '../item-details/item-details';
import { DataService } from '../../services/data.service';
import { BarcodeScanner } from 'ionic-native';


@Component({
  selector: 'list-page',
  templateUrl: 'list.html'
})
export class YxjsListPage {
  items: Array<any>;

  constructor(public navCtrl: NavController, public navParams: NavParams, private dataService: DataService, public loadingCtrl: LoadingController) {
    this.items = this.dataService.getLocalDatas('runpost');
  }

  itemTapped(event, item) {
    this.navCtrl.push(ItemDetailsPage, {
      item: item
    });
  }
  loadData(){
    BarcodeScanner.scan().then((barcodeData) => {
        let loader = this.loadingCtrl.create({
          content: "正在处理..."
        });
        loader.present();
        let dataid = barcodeData.text;
        let sql = `select a.* from z_jcyy_sendrunpost a where a.dataid%3D'${dataid}' order by a.sn`;
        this.dataService.getDatas(sql)
        .then(items => {
          loader.dismiss();
          this.dataService.setLocalData('runpost', items);
          this.items = this.dataService.getLocalDatas('runpost');
        });   
      }, (err) => {
    });
  }
}

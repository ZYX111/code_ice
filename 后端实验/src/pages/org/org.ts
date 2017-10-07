import { Component } from '@angular/core';
import { NavController, NavParams, ViewController } from 'ionic-angular';
import { DataService } from '../../services/data.service';

@Component({
  templateUrl: 'org.html'
})
export class OrgPage {
  depot: string;
  workshop: string;
  depots: Array<Object>;
  workshops: Array<Object>;

  constructor(public navCtrl: NavController, public navParams: NavParams, public viewCtrl: ViewController, private dataService: DataService) {
    this.getDepots();
  }

  getDepots() {
      this.dataService.getDatas(`select * from fw_org where orglevel=3`)
      .then(items => this.depots = items);   
  }

  getWorkshop(){
      let sql = `select id||','||name id,name from fw_org where parentid='${this.depot}'`;
      this.dataService.getDatas(sql)
      .then(items => this.workshops = items);   
  }

  cancel() {
    this.viewCtrl.dismiss();
  }

  select(){
    var strs = this.workshop.split(',');
  	this.viewCtrl.dismiss({id: strs[0], name: strs[1]});
  }
}

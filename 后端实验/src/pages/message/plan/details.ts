import { Component } from '@angular/core';

import { NavController, NavParams } from 'ionic-angular';
import { DataService } from '../../../services/data.service';

@Component({
  selector: 'item-details-page',
  templateUrl: 'details.html'
})
export class PlanDetailsPage {
  content: any;
  title: any;
  taskid: any;
  constructor(public navCtrl: NavController, public navParams: NavParams, private dataService: DataService) {
    this.taskid = navParams.get('taskid');
  	let instanceid = navParams.get('instanceid');
    this.dataService.getData(`select * from sys_info where id in (select business_key_ from act_hi_procinst where id_='${instanceid}')`)
          .then(result => {
          this.title = result.data.title;
          this.content = result.data.content;
          });
  }
  sign(){
    this.dataService.signTask(this.taskid).then(
      result => {
        this.navCtrl.pop();
      }
    );
  }
}

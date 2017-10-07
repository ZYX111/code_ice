import { Component } from '@angular/core';
import { NavController, NavParams, ViewController } from 'ionic-angular';
import { DataService } from '../../services/data.service';

@Component({
  templateUrl: 'jobstandard.html'
})
export class JobstandardPage {
  joblink: string;
  jobprocedure: string;
  jobstandard: string;
  joblinks: Array<Object>;
  jobprocedures: Array<Object>;
  jobstandards: Array<Object>;

  constructor(public navCtrl: NavController, public navParams: NavParams, public viewCtrl: ViewController, private dataService: DataService) {
    this.getJoblink();
  }

  getJoblink() {
      this.dataService.getDatas(`select distinct joblink from aqgl_base_jobstandard`)
      .then(items => this.joblinks = items);   
  }

  getJobprocedure(){
      let sql = `select distinct jobprocedure from aqgl_base_jobstandard where joblink='${this.joblink}'`;
      this.dataService.getDatas(sql)
      .then(items => this.jobprocedures = items);   
  }

  getJobstandard(){
      let sql = `select * from aqgl_base_jobstandard where joblink='${this.joblink}' and jobprocedure='${this.jobprocedure}'`;
      this.dataService.getDatas(sql)
      .then(items => this.jobstandards = items);   
  }

  cancel() {
    this.viewCtrl.dismiss();
  }

  select(){
  	this.viewCtrl.dismiss({id: this.jobstandard});
  }
}

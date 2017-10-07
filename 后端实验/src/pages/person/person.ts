import { Component } from '@angular/core';
import { NavController, NavParams, ViewController } from 'ionic-angular';
import { DataService } from '../../services/data.service';

@Component({
  templateUrl: 'person.html'
})
export class PersonPage {
  orgid: String;
  items: Array<Object>;

  constructor(public navCtrl: NavController, public navParams: NavParams, public viewCtrl: ViewController, private dataService: DataService) {
    this.orgid = navParams.get('orgid');
  }

  getItems(ev) {
    var val = ev.target.value;
    console.log(val);
    if (val && val.trim() != '') {
        this.dataService.getDatas(`select a.id personid,a.hname personname from base_personnel a where orgid in (
select id from fw_org where seq like (select seq from fw_org where id='${this.orgid}')||'#')
and a.spell like '${val}#'`)
        .then(items => this.items = items);   
    }
  }

  cancel() {
    this.viewCtrl.dismiss();
  }

  select(item){
  	this.viewCtrl.dismiss(item);
  }
}

import { Component } from '@angular/core';
import { NavController, NavParams, ViewController } from 'ionic-angular';
import { DataService } from '../../services/data.service';

@Component({
  templateUrl: 'illegal.html'
})
export class IllegalPage {
  category: string;
  dept: string;
  illegal: string;
  categorys: Array<Object>;
  orgs: Array<Object>;
  contents: Array<Object>;

  constructor(public navCtrl: NavController, public navParams: NavParams, public viewCtrl: ViewController, private dataService: DataService) {
    this.getCategory();
  }

  getCategory() {
      this.dataService.getDatas(`select distinct category name from aqgl_base_illegal`)
      .then(items => this.categorys = items);   
  }

  selectCategory(){
      let sql = `select distinct category,dept from aqgl_base_illegal where category='${this.category}'`;
      this.dataService.getDatas(sql)
      .then(items => this.orgs = items);   
  }

  selectDept(){
      let sql = `select id,content from aqgl_base_illegal where category='${this.category}' and dept='${this.dept}'`;
      this.dataService.getDatas(sql)
      .then(items => this.contents = items);   
  }

  cancel() {
    this.viewCtrl.dismiss();
  }

  select(){
  	this.viewCtrl.dismiss({id: this.illegal});
  }
}

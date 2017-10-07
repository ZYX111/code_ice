import { Component } from '@angular/core';
import { NavController, NavParams } from 'ionic-angular';

import { DataService } from '../../../services/data.service'
@Component({
  templateUrl: 'riskDetail.html'
})
export class RiskDetailPage {
    selectedSegment: string = 'detail';
    riskbehavior: any;
    checkDatas: Array<Object>;
    problemDatas: Array<Object>;

    constructor(public navCtrl: NavController, public navParams: NavParams, private dataService: DataService) {
      this.riskbehavior = navParams.get('riskbehavior');
      this.loadCheckDatas();
      this.loadProblemDatas();
    }
    loadCheckDatas(){
        this.dataService.getDatas(`select u.name checkername,a.* from aqgl_data_checkitem a left join aqgl_data_task b on b.id=a.taskid left join fw_user u on u.id=b.checkerid where riskbehaviorid='${this.riskbehavior.id}' order by a.checktime desc`)
          .then(items => this.checkDatas = items);
    }
    loadProblemDatas(){
        this.dataService.getDatas(`select t.description,b.hname name,t.occuretime from aqgl_data_problems t 
          left join base_personnel b on b.id=t.personid
          where t.behaviorid='${this.riskbehavior.id}' order by t.occuretime`)
        .then(items => this.problemDatas = items);
    }

}

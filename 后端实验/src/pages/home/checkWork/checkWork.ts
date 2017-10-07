import { Component } from '@angular/core';
import { NavController, NavParams, AlertController } from 'ionic-angular';

import { DataService } from '../../../services/data.service'

import { CheckRoundRiskPage } from '../checkRoundRisk/checkRoundRisk';
import { AddProblemPage } from '../addProblem/addProblem';
import { AddRiskPage } from '../addRisk/addRisk';
import { RiskDetailPage } from '../riskDetail/riskDetail';

@Component({
    templateUrl: 'checkWork.html'
})

export class CheckWorkPage {
  isAndroid: boolean = false;
  selectedSegment: string = 'risk';
	risks: Array<Object>;
  problems: Array<Object>;
  foundRisks: Array<Object>;

    constructor(public navCtrl: NavController, public navParams: NavParams, public alertCtrl: AlertController, private dataService: DataService) {
    }
    ionViewWillEnter(){
        this.loadRisk();
        this.loadProblem();
        this.loadFoundRisk();
    }
    loadRisk(){
        let riskid = this.dataService.getLocalData('taskdata', 'riskid');
        /*
        let sql = `select b.name categoryname,nvl2(e1.name,e1.name||'->','')||e.name riskname,j.joblink||'->'||j.jobprocedure||'->'||j.jobstandard jobname,a.* from  aqgl_base_riskbehaviors a left join aqgl_dict_riskcategory b on b.id=a.category left join aqgl_base_risk e on e.id=a.riskid left join aqgl_base_risk e1 on e1.id=e.parentid left join aqgl_base_jobstandard j on j.id=a.jobcategory where a.riskid in (select id from aqgl_base_risk where seq like (select seq from aqgl_base_risk where id='${riskid}')||'#') order by b.name`;
        */
        let sql = `select e.id riskid,a.id, a.name,a.description,a.orgid,a.risknature,a.category,a.jobcategory,a.uploader,e.name riskname, d.name categoryname,o.name orgname,c.joblink || '->' || c.jobprocedure || '->' || c.jobstandard jobcategoryname from aqgl_base_baseriskbehaviors a left join fw_org o on o.id = a.orgid left join aqgl_dict_riskcategory d on d.id = a.category left join aqgl_base_jobstandard c on c.id = a.jobcategory join aqgl_base_risk e on e.placekind =a.place and e.orgid = a.orgid where e.id in (select id from aqgl_base_risk where seq like '#' || '${riskid}' || '#') union all
select a.riskid,a.id,a.name,a.description,a.orgid,a.risknature,a.category,a.jobcategory,a.uploader,
       nvl2(e1.name, e1.name || '->', '') || e.name riskname,
       d.name categoryname,
       o.name orgname,
       c.joblink || '->' || c.jobprocedure || '->' || c.jobstandard jobcategoryname
  from aqgl_base_riskbehaviors a
  left join fw_org o
    on o.id = a.orgid
  left join aqgl_dict_riskcategory d
    on d.id = a.category
  left join aqgl_base_jobstandard c
    on c.id = a.jobcategory
  left join aqgl_base_risk e
    on e.id = a.riskid
  left join aqgl_base_risk e1
    on e1.id = e.parentid
 where riskid in
       (select id
          from aqgl_base_risk
         where seq like
               (select seq from aqgl_base_risk where id = '${riskid}') || '#')
 order by riskname, category`;
        this.dataService.getDatas(sql)
          .then(items => this.risks = items);
    }

    loadProblem(){
        let taskid = this.dataService.getLocalData('taskdata', 'id');
        this.dataService.getDatas(`select a.*,p.hname personname,o.name orgname from aqgl_data_problems a left join base_personnel p on p.id=a.personid left join fw_org o on o.id=a.orgid where a.taskid='${taskid}'`)
        .then(items => this.problems = items);
    }

    loadFoundRisk(){
        let taskid = this.dataService.getLocalData('taskdata', 'id');
        this.dataService.getDatas(`select a.* from aqgl_data_judge a where a.taskid='${taskid}'`)
        .then(items => this.foundRisks = items);
    }

	addProblme(){
		this.navCtrl.push(AddProblemPage);
	}

	addRisk(){
		this.navCtrl.push(AddRiskPage);
	}
  check(riskbehavior){
      this.navCtrl.push(CheckRoundRiskPage,{
        riskbehavior: riskbehavior
      });
  }
  showRiskDetail(riskbehavior){
      this.navCtrl.push(RiskDetailPage, {
        riskbehavior: riskbehavior
      });
  }
  endTask(){
      let confirm = this.alertCtrl.create({
        title: '提示',
        message: '是否结束工作?',
        buttons: [
          {
            text: '取消',
            handler: () => {

            }
          },
          {
            text: '确定',
            handler: () => {
              let taskid = this.dataService.getLocalData('taskdata', 'id');
              let endtime = this.dataService.now();
              this.dataService.saveData('aqgl_data_task', 'id', {
                  id: taskid,
                  endtime: endtime
              }).then(result => {
                  this.dataService.setLocalData('taskdata', null);
                  this.navCtrl.pop();
              })
            }
          }
        ]
      });
      confirm.present()
  }
}

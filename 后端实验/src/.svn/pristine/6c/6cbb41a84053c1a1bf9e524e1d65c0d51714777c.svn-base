import { Component } from '@angular/core';
import { NavController, NavParams } from 'ionic-angular';
import { DataService } from '../../../../services/data.service'

@Component({
  templateUrl: 'detail.html'
})
export class TaskDetailPage {
  items: Array<Object>;
  constructor(public navCtrl: NavController, public navParams: NavParams, private dataService: DataService) {
    let item = navParams.get('item');
    this.loadData(item.id);
  }
  loadData(taskid){
      let curUserid = this.dataService.getLocalData('userdata', 'id');
      let sql = `select a.*,c.name taskname,nvl2(e1.name,e1.name||'->','')||e.name riskname from aqgl_data_task a
left join aqgl_base_tasks b on b.id=a.taskid
left join aqgl_dict_task c on c.id=b.taskid
left join aqgl_base_risk e on e.id=a.riskid 
left join aqgl_base_risk e1 on e1.id=e.parentid
where a.checkerid='${curUserid}' and a.taskid='${taskid}'`;
      console.log(sql);
      this.dataService.getDatas(sql)
      .then(items => this.items = items);
  }
}

import { Component } from '@angular/core';
import { NavController, NavParams } from 'ionic-angular';
import { DataService } from '../../../../services/data.service';
import { TaskDetailPage } from '../detail/detail'

@Component({
  templateUrl: 'list.html'
})
export class TaskPage {
  items: Array<Object>;
  constructor(public navCtrl: NavController, public navParams: NavParams, private dataService: DataService) {
    this.loadData();
  }
  loadData(){
      let curUserid = this.dataService.getLocalData('userdata', 'id');
      let sql = `select t1.taskid id, t3.name, t1.amount,
       (select count(*)  from aqgl_data_task where checkerid = t2.userid
           and taskid = t1.taskid and endtime is not null
           and to_char(checktime, 'yyyymm') = to_char(sysdate, 'yyyymm')) as finished
          from (select a.taskid,  a.postid postid, a.amount, b.name   taskname
          from aqgl_base_tasks a left join aqgl_dict_task b  on b.id=a.taskid
          union all
          select a.taskid, to_char(c.id) postid,  a.amount, b.name taskname
            from aqgl_base_basetask a, fw_post c, aqgl_dict_task b
            where b.id = a.taskid  and c.basepost = a.postid) t1,
            fw_post_user t2,
            aqgl_dict_task t3
          where t1.postid = t2.postid and t1.taskid = t3.id and t2.userid='${curUserid}'`;
      this.dataService.getDatas(sql)
      .then(items => this.items = items);
  }
  showDetail(item){
      this.navCtrl.push(TaskDetailPage,{
        item: item
      });
  }
}

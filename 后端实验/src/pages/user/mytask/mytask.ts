import { Component } from '@angular/core';
import { NavController, NavParams } from 'ionic-angular';
import { DataService } from '../../../services/data.service';
import { MyTaskDetailPage } from '../mytaskDetail/mytaskDetail'

@Component({
  templateUrl: 'mytask.html'
})
export class MyTaskPage {
  items: Array<Object>;
  constructor(public navCtrl: NavController, public navParams: NavParams, private dataService: DataService) {
    this.loadData();
  }
  loadData(){
      let curUserid = this.dataService.getLocalData('userdata', 'id');
      let sql = `select c.id,d.name,c.amount,k.finished from fw_user a
                   left join fw_post_user b on b.userid=a.id
                   left join aqgl_base_tasks c on c.postid=b.postid
                   left join aqgl_dict_task d on d.id=c.taskid
                   left join (
                  (select c.id,count(*) finished from fw_user a
                   left join fw_post_user b on b.userid=a.id
                   left join aqgl_base_tasks c on c.postid=b.postid
                   left join aqgl_dict_task d on d.id=c.taskid
                   join aqgl_data_task e on e.taskid=c.id
                   where a.id='${curUserid}'
                  group by c.id)) k on k.id=c.id
                   where a.id='${curUserid}'
                   order by c.id`;
      this.dataService.getDatas(sql)
      .then(items => this.items = items);
  }
  showDetail(item){
      this.navCtrl.push(MyTaskDetailPage,{
        item: item
      });
  }
}

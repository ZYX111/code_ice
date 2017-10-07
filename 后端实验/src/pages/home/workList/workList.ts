import { Component } from '@angular/core';
import { NavController, NavParams } from 'ionic-angular';

import { DataService } from '../../../services/data.service'

import { CheckWorkPage } from '../checkWork/checkWork';

@Component({
    selector: 'page-home',
    templateUrl: 'workList.html'
})

export class WorkListPage{
    checktime: string;
    task: string;
    place: string;
    lon: number;
    lat: number;
    places: Array<Object>;
    tasks: Array<Object>;
    constructor(public navCtrl: NavController, public navParams: NavParams, private dataService: DataService) {
        let curUserid = this.dataService.getLocalData('userdata', 'id');
        this.checktime = this.dataService.now();
    	this.lon = navParams.get('lon');
    	this.lat = navParams.get('lat');
        this.dataService.getDatas(`select id,name,GetDistance(lon, lat, ${this.lon}, ${this.lat}) dist from aqgl_base_risk where GetDistance(lon, lat, ${this.lon}, ${this.lat})<2 order by dist`)
          .then(items => this.places = items);
        this.dataService.getDatas(`select t1.taskid id, t3.name,excway
            from (select a.taskid,a.excway,a.postid postid,a.amount,b.name taskname
                from aqgl_base_tasks a left join aqgl_dict_task b on b.id=a.taskid
                union all
                select a.taskid,a.excway, to_char(c.id) postid,  a.amount, b.name taskname
                from aqgl_base_basetask a, fw_post c, aqgl_dict_task b
                where b.id = a.taskid  and c.basepost = a.postid) t1,
                fw_post_user t2,
                aqgl_dict_task t3
                where t1.postid = t2.postid
                and t1.taskid = t3.id
                and excway in ('现场检查')
                and t2.userid = '${curUserid}'`)
            .then(items => this.tasks = items);
  	}

    checkWork(){
        console.log(this.task);
        this.dataService.saveData('aqgl_data_task', 'id', {
            checkerid: this.dataService.getLocalData('userdata', 'id'),
            checktime: this.checktime,
            lon: this.lon,
            lat: this.lat,
            taskid: this.task,
            riskid: this.place
        }).then(result => {
            console.log(result);
            this.dataService.setLocalData('taskdata', result.data);
            this.navCtrl.push(CheckWorkPage);
        })
    };

}

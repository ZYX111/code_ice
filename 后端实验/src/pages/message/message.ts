import { Component } from '@angular/core';
import { NavController, ViewController } from 'ionic-angular';
import { DataService } from '../../services/data.service';
import { PlanDetailsPage } from './plan/details';

@Component({
	selector: 'page-about',
	templateUrl: 'message.html'
})
export class MessagePage {
	
	items: Array<Object>;

	constructor(public navCtrl: NavController, private dataService: DataService, public viewCtrl: ViewController) {
	}
	itemSelected(item){
		this.navCtrl.push(PlanDetailsPage, {
            instanceid: item.instanceid,
            taskid: item.id
        });
	}
	ionViewWillEnter(){
	    //let userid = this.dataService.getLocalData('userdata', 'id');
	    let userid = '13718766117';
	    this.dataService.getMyTasks(userid).then(datas => this.items = datas);
	}
}

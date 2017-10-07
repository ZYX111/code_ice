import { Component } from '@angular/core';

import { HomePage } from '../home/home';
import { MessagePage } from '../message/message';
import { DataCenterPage } from '../datacenter/datacenter';
import { UserPage } from '../user/user';
import { YxjsListPage } from '../yxjs/list';
import { DataService } from '../../services/data.service';

// 组件
@Component({
    templateUrl: 'tabs.html'
})
export class TabsPage {
    taskcount: number = 2;
    isCadre: boolean = true;
    
    tab1Root: any = HomePage;
    tab2Root: any = MessagePage;
    tab3Root: any = DataCenterPage;
    tab4Root: any = UserPage;
    yxjsRoot: any = YxjsListPage;

    constructor(private dataService: DataService){
        let me = this;
        this.isCadre = this.dataService.isCadre();
        let worker = new Worker("worker.js");
        worker.onmessage = function(event) {
            //let userid = me.dataService.getLocalData('userdata', 'id');
            let userid = '13718766117';
            me.dataService.getMyTaskCount(userid).then(data => me.taskcount = data);
        };
    }
    
}

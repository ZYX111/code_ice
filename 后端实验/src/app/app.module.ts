import { NgModule, ErrorHandler } from '@angular/core';
import { IonicApp, IonicModule, IonicErrorHandler } from 'ionic-angular';
import { DatePipe } from '@angular/common';

import { MyApp } from './app.component';
import { AboutPage } from '../pages/about/about';
import { ContactPage } from '../pages/contact/contact';
import { HomePage } from '../pages/home/home';
import { TabsPage } from '../pages/tabs/tabs';
import { DataService } from '../services/data.service';

import { UserPage } from '../pages/user/user';
import { DataCenterPage } from '../pages/datacenter/datacenter';
import { ItemDetailsPage } from '../pages/item-details/item-details';
import { MessagePage } from '../pages/message/message';
import { PlanDetailsPage } from '../pages/message/plan/details';

import { DynTaskPage } from '../pages/home/dyntask/list/list';
import { DynTaskDetailPage } from '../pages/home/dyntask/detail/detail';
import { TaskPage } from '../pages/home/task/list/list';
import { TaskDetailPage } from '../pages/home/task/detail/detail';

import { WorkListPage } from '../pages/home/workList/workList';
import { CheckWorkPage } from '../pages/home/checkWork/checkWork';
import { AddProblemPage } from '../pages/home/addProblem/addProblem';
import { PersonPage } from '../pages/person/person';
import { IllegalPage } from '../pages/illegal/illegal';
import { OrgPage } from '../pages/org/org';
import { JobstandardPage } from '../pages/jobstandard/jobstandard';
import { AddRiskPage } from '../pages/home/addRisk/addRisk';
import { RiskDetailPage } from '../pages/home/riskDetail/riskDetail';
import { CheckRoundRiskPage } from '../pages/home/checkRoundRisk/checkRoundRisk';
import { MyTaskPage } from '../pages/user/mytask/mytask';
import { MyTaskDetailPage } from '../pages/user/mytaskDetail/mytaskDetail';

import { YxjsListPage } from '../pages/yxjs/list';

@NgModule({
  declarations: [
    MyApp,
    AboutPage,
    ContactPage,
    HomePage,
    TabsPage,
    PersonPage,
    UserPage,
    DataCenterPage,
    ItemDetailsPage,
    MessagePage,
    DynTaskPage,
    DynTaskDetailPage,
    TaskPage,
    TaskDetailPage,
    WorkListPage,
    CheckWorkPage,
    AddProblemPage,
    IllegalPage,
    JobstandardPage,
    AddRiskPage,
    RiskDetailPage,
    CheckRoundRiskPage,
    OrgPage,
    YxjsListPage,
    MyTaskPage,
    MyTaskDetailPage,
    PlanDetailsPage
  ],
  imports: [
    IonicModule.forRoot(MyApp)
  ],
  bootstrap: [IonicApp],
  entryComponents: [
    MyApp,
    AboutPage,
    ContactPage,
    HomePage,
    TabsPage,
    PersonPage,
    UserPage,
    DataCenterPage,
    ItemDetailsPage,
    MessagePage,
    DynTaskPage,
    DynTaskDetailPage,
    TaskPage,
    TaskDetailPage,
    WorkListPage,
    CheckWorkPage,
    AddProblemPage,
    IllegalPage,
    JobstandardPage,
    AddRiskPage,
    RiskDetailPage,
    CheckRoundRiskPage,
    OrgPage,
    YxjsListPage,
    MyTaskPage,
    MyTaskDetailPage,
    PlanDetailsPage
  ],
  providers: [{provide: ErrorHandler, useClass: IonicErrorHandler}, DataService, DatePipe]
})
export class AppModule {}

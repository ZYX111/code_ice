import { NgModule, ErrorHandler } from '@angular/core';
import { IonicApp, IonicModule, IonicErrorHandler } from 'ionic-angular';
import { MyApp } from './app.component';
import { dieInterFacePage } from '../pages/dieInterFace/dieInterFace';
import { wereWolfWinPage } from '../pages/wereWolfWin/wereWolfWin';
import { humanWinPage } from '../pages/humanWin/humanWin';
import { licensingPage } from '../pages/licensing/licensing';
import { wereWolfFacePage } from '../pages/wereWolfFace/wereWolfFace';
import { witchFacePage } from '../pages/witchFace/witchFace';
import { exileFacePage } from '../pages/exileFace/exileFace';
import { idiotFacePage } from '../pages/idiotFace/idiotFace';
import { non_playerNightFacePage } from '../pages/non_playerNightFace/non_playerNightFace';
import { prophetFacePage } from '../pages/prophetFace/prophetFace';
import { voteFacePage } from '../pages/voteFace/voteFace';
import { guardFacePage } from '../pages/guardFace/guardFace';
import { policeFacePage } from '../pages/policeFace/policeFace';
import { hunterFacePage } from '../pages/hunterFace/hunterFace';
import { setPage } from '../pages/set/set';
import { startPage } from '../pages/start/start';
import { DataService } from '../services/dataservice';

@NgModule({
  declarations: [
    MyApp,
    startPage,
    setPage,
    hunterFacePage,
    policeFacePage,
    guardFacePage,
    voteFacePage,
    prophetFacePage,
    non_playerNightFacePage,
    idiotFacePage,
    exileFacePage,
    witchFacePage,
    wereWolfFacePage,
    licensingPage,
    humanWinPage,
    wereWolfWinPage,
    dieInterFacePage
  ],
  imports: [
    IonicModule.forRoot(MyApp)
  ],
  bootstrap: [IonicApp],
  entryComponents: [
    MyApp,
    startPage,
    setPage,
    hunterFacePage,
    policeFacePage,
    guardFacePage,
    voteFacePage,
    prophetFacePage,
    non_playerNightFacePage,
    idiotFacePage,
    exileFacePage,
    witchFacePage,
    wereWolfFacePage,
    licensingPage,
    humanWinPage,
    wereWolfWinPage,
    dieInterFacePage
  ],
  providers: [
    {provide: ErrorHandler, useClass: IonicErrorHandler}, 
    DataService
  ]
})
export class AppModule {}

import { Component } from '@angular/core';
import { Camera } from 'ionic-native';
import { NavController, NavParams } from 'ionic-angular';
import { DataService } from '../../../services/data.service'
import { AddProblemPage } from '../addProblem/addProblem';

@Component({
    templateUrl: 'checkRoundRisk.html'
})
export class CheckRoundRiskPage{
    checktime: string;
	  riskbehavior: any;
    slides: Array<any> = new Array();
    description: string = '正常';
    saved: boolean;
    options = {
        quality : 50,
        destinationType : Camera.DestinationType.FILE_URI,
        sourceType : Camera.PictureSourceType.CAMERA,
        targetWidth: 1000,
        targetHeight: 1000,
        correctOrientation: true,
        encodingType: 1,
        saveToPhotoAlbum: true                                  
    }
    constructor(public navCtrl: NavController, public navParams: NavParams, private dataService: DataService) {
        this.riskbehavior = navParams.get('riskbehavior');
        this.checktime = this.dataService.now();
        this.saved = false;
    }  
    cancel(){
    	this.navCtrl.pop();
    }
    save(){
      if(!this.saved){
        console.log('save: save');
        this.dataService.saveData('aqgl_data_checkitem', 'id', {
          riskbehaviorid: this.riskbehavior.id,
          checktime: this.checktime,
          taskid: this.dataService.getLocalData('taskdata', 'id'),
          description: this.description
        }).then(result => {
          this.navCtrl.pop();
        })
      }else{
          this.navCtrl.pop();
      }
    }
    takePicture(){
      Camera.getPicture(this.options).then((imageData) => {
              this.slides.push({ image: imageData });
      }, (err) => {
        // Handle error
      });
    }
    recordWz(){
        this.dataService.saveData('aqgl_data_checkitem', 'id', {
          riskbehaviorid: this.riskbehavior.id,
          checktime: this.checktime,
          taskid: this.dataService.getLocalData('taskdata', 'id'),
          description: this.description
        }).then(result => {
          console.log('save:'+result);
          this.saved = true;
          this.navCtrl.push(AddProblemPage, result.data);
        })
    }

}

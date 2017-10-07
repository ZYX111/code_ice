import { Component } from '@angular/core';
import { Camera } from 'ionic-native';
import { NavController, NavParams } from 'ionic-angular';
import { DataService } from '../../../services/data.service';

@Component({
    selector: 'page-home',
    templateUrl: 'addRisk.html'
})
export class AddRiskPage{
    foundtime: string;
    description: string;
    slides: Array<any> = new Array();
    icons: string[];
    items: Array<{title: string, note: string, icon: string}>;
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

    constructor(public navCtrl: NavController, public navParams: NavParams, private dataService: DataService){
        this.foundtime = this.dataService.now();
    }

    takePicture(){
		Camera.getPicture(this.options).then((imageData) => {
            // this.slides.push({ image: 'data:image/jpeg;base64,' + imageData});
            this.slides.push({ image: imageData });
		}, (err) => {
		 	// Handle error
		});
    }

    cancel(){
    	this.navCtrl.pop();
    }

    save(){
        this.dataService.startWorkflow('aqgl_fxyp','aqgl_data_judge', 'id', {
            taskid: this.dataService.getLocalData('taskdata', 'id'),
            starterid: this.dataService.getLocalData('userdata', 'id'),
            startname: this.dataService.getLocalData('userdata', 'name'),
            startdeptid: this.dataService.getLocalData('userdata', 'orgid'),
            description: this.description,
            _participant: 'r:gkzx'
        }).then(result => {
          this.navCtrl.pop();
        });
        /*
        this.dataService.saveData('aqgl_data_judge', 'id', {
            taskid: this.dataService.getLocalData('taskdata', 'id'),
            starterid: this.dataService.getLocalData('userdata', 'id'),
            startname: this.dataService.getLocalData('userdata', 'name'),
            startdeptid: this.dataService.getLocalData('userdata', 'orgid'),
            description: this.description
        }).then(result => {
          this.navCtrl.pop();
        })
        */
    }
}

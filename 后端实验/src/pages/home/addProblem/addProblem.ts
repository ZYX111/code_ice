import { Component } from '@angular/core';
import {  ModalController, NavController, NavParams } from 'ionic-angular';
import { DataService } from '../../../services/data.service';
import { PersonPage } from '../../person/person';
import { OrgPage } from '../../org/org';
import { IllegalPage } from '../../illegal/illegal';
import { JobstandardPage } from '../../jobstandard/jobstandard';
import { Camera } from 'ionic-native';

@Component({
    templateUrl: 'addProblem.html'
})
export class AddProblemPage{
    behaviorid: string;
    personid: string;
    personname: string;
    orgid: string;
    orgname: string;
    checktime: string;
    checker: string;
    source: string = '车间';
    method: string = '现场测查';
    riskkind: string;
    post: string;
    placename: string;
    occuretime: string;
    evaluatemode: string;
    evaluatemodename: string;
    evaluateresult: string = '0';
    jobstandard: string;
    jobstandardname: string;
    description: string;
    slides: Array<any> = new Array();

    constructor(public navCtrl: NavController, public navParams: NavParams, public modalCtrl: ModalController, private  dataService: DataService) {
        this.behaviorid = navParams.data.riskbehaviorid;
        this.checktime = this.dataService.now();
        this.checker = this.dataService.getLocalData('userdata', 'name');
        this.post = this.dataService.getLocalData('userdata', 'post');
        this.placename = this.dataService.getLocalData('taskdata', 'riskid');
        this.occuretime = this.dataService.now();
    }
   
    cancel(){
    	this.navCtrl.pop();
    }

    save(){
        this.dataService.startWorkflow('aqgl_jwwt','aqgl_data_problems', 'id', {
            personid: this.personid,
            orgid: this.orgid,
            taskid: this.dataService.getLocalData('taskdata', 'id'),
            checkerid: this.dataService.getLocalData('userdata', 'id'),
            post: this.dataService.getLocalData('userdata', 'post'),
            place: this.placename,
            occuretime: this.occuretime,
            checkmode: this.method,
            evaluatemode: this.evaluatemode,
            evaluateresult: this.evaluateresult,
            riskkind: this.riskkind,
            source: this.source,
            checktime: this.checktime,
            jobstandard: this.jobstandard,
            description: this.description,
            behaviorid: this.behaviorid,
            _participant: 'r:gkzx'
        }).then(result => {
            for(var i in this.slides){
                this.dataService.upload(this.slides[i].image);
            }
            this.navCtrl.pop();
        });
        /*
        this.dataService.saveData('aqgl_data_problems', 'id', {
            personid: this.personid,
            orgid: this.orgid,
            taskid: this.dataService.getLocalData('taskdata', 'id'),
            checkerid: this.dataService.getLocalData('userdata', 'id'),
            post: this.dataService.getLocalData('userdata', 'post'),
            place: this.placename,
            occuretime: this.occuretime,
            checkmode: this.method,
            evaluatemode: this.evaluatemode,
            evaluateresult: this.evaluateresult,
            riskkind: this.riskkind,
            source: this.source,
            checktime: this.checktime,
            jobstandard: this.jobstandard,
            description: this.description,
            behaviorid: this.behaviorid
        }).then(result => {
          this.navCtrl.pop();
        })
        */
    }

    openPerson() {
        let modal = this.modalCtrl.create(PersonPage, {orgid: this.orgid});
        modal.onDidDismiss(data => {
            if(data){
                this.personid = data.personid;
                this.personname = data.personname;
                //this.orgid = data.orgid;
                //this.orgname = data.orgname;
            }
        });
        modal.present();
    }

    openOrg() {
        let modal = this.modalCtrl.create(OrgPage, {});
        modal.onDidDismiss(data => {
            if(data){
                this.orgid = data.id;
                this.orgname = data.name;
            }
        });
        modal.present();
    }

    openIllegal() {
        let modal = this.modalCtrl.create(IllegalPage, {});
        modal.onDidDismiss(data => {
            if(data){
            console.log(data);
                this.evaluatemode = data.id;
                this.evaluatemodename = data.content;
            }
        });
        modal.present();
    }

    openJobstandard() {
        let modal = this.modalCtrl.create(JobstandardPage, {});
        modal.onDidDismiss(data => {
            if(data){
            console.log(data);
                this.jobstandard = data.id;
                this.jobstandardname = data.content;
            }
        });
        modal.present();
    }

    takePicture(){
        let options = {
            quality : 50,
            destinationType : Camera.DestinationType.FILE_URI,
            sourceType : Camera.PictureSourceType.CAMERA,
            targetWidth: 1000,
            targetHeight: 1000,
            correctOrientation: true,
            encodingType: 1,
            saveToPhotoAlbum: true                                  
        }
        Camera.getPicture(options).then((imageData) => {
            this.slides.push({ image: imageData });
        }, (err) => {
            // Handle error
        });
    }

}

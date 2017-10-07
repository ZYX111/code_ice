import { Component } from '@angular/core';
import { BarcodeScanner, Camera, TextToSpeech } from 'ionic-native';
import { NavController, ModalController, AlertController } from 'ionic-angular';

import { MyTaskPage } from './mytask/mytask';
import { DataService } from '../../services/data.service';
import { PersonPage } from '../person/person';

@Component({
  selector: 'page-about',
  templateUrl: 'user.html',
  providers: [DataService]
})
export class UserPage {
  name: string;
  postname: string;
  orgname: string;
  isDebug: boolean;

  constructor(public navCtrl: NavController, private dataService: DataService, public modalCtrl: ModalController, private alertCtrl: AlertController) {
      this.name = this.dataService.getLocalData('userdata', 'name');
      this.postname = this.dataService.getLocalData('userdata', 'postname');
      this.orgname = this.dataService.getLocalData('userdata', 'orgname');
      this.isDebug = this.dataService.isDebug();
  }

  logout(){
      window.close();
  }

  register(){
  	BarcodeScanner.scan().then((barcodeData) => {
        let text = barcodeData.text; //'regist:'+20;
        text=text.replace('regist:','');
  	  	this.dataService.getData(`select a.*,p.name postname,o.name orgname from fw_user a 
          left join fw_post_user pu on pu.userid=a.id 
          left join fw_post p on p.id=pu.postid
          left join fw_org o on o.id=a.orgid
          where a.id='${text}'`)
          .then(item => {
          	let userData = item.data;
            this.dataService.setLocalData('userdata', userData);
          	this.name = userData.name;
          	this.postname = userData.postname;
            this.orgname = userData.orgname;
          });
      }, (err) => {
  	});
  }
  showTasks(){
    this.navCtrl.push(MyTaskPage);
  }
  download(id){
      this.dataService.download(id);
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
        this.dataService.upload(imageData);
    }, (err) => {
      // Handle error
    });
  }
  test(){
    let modal = this.modalCtrl.create(PersonPage);
    modal.present();
  }

  testTextToSpeech(){
    let prompt = this.alertCtrl.create({
      title: '文字朗读',
      message: "请输入文字(中文)",
      inputs: [
        {
          name: 'title'
        },
      ],
      buttons: [
        {
          text: '取消',
          handler: data => {
            console.log('Cancel clicked');
          }
        },
        {
          text: '确定',
          handler: data => {
            TextToSpeech.speak({
              text: data.title,
              locale: 'zh-CN',
              rate: 1
            })
              .then(() => console.log('Success'))
              .catch((reason: any) => console.log(reason));    
          }
        }
      ]
    });
    prompt.present();
  }
  getSimInfo(){
    this.dataService.getSimInfo();
  }

  playVoice(){
    this.dataService.playVoice('audio.mp3');
  }

}

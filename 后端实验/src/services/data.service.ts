import { Injectable } from '@angular/core';
import { Headers, Http } from '@angular/http';
import { DatePipe } from '@angular/common';

import 'rxjs/add/operator/toPromise';
import { Transfer, FileOpener, Sim } from 'ionic-native';

import { Platform, AlertController, LoadingController } from 'ionic-angular';

declare var cordova: any;

@Injectable()
export class DataService {
	private url = 'http://61.161.203.73:8080';
	//private url = 'http://localhost:8080/dataproxy/route/getDatas.do';
	//private url = 'http://localhost:8080/dataserver/dataService/getDatas.do';
	private headers = new Headers({'Content-Type': 'application/x-www-form-urlencoded; charset=UTF-8'});
	private storageDirectory: string;
	private fileTransfer: any = null;
	private _audio: any;
	private _source: any;
	constructor(private http: Http, private datePipe: DatePipe, private platform: Platform, private alertCtrl: AlertController, public loadingCtrl: LoadingController) {
	}
	isDebug(){
		return false;
	}
	init(){
		this.platform.ready().then(() => {
            if(!this.platform.is('cordova')) {
                return false;
            }
            if (this.platform.is('ios')) {
                this.storageDirectory = cordova.file.documentsDirectory;
            }
            else if(this.platform.is('android')) {
                this.storageDirectory = cordova.file.externalDataDirectory;
            }
            else {
                return false;
            }
        });
        this.fileTransfer = new Transfer();
	}
	getData(sql: string){
		return this.http.post(`${this.url}/setUp`, JSON.stringify({
				type: 'getData',
				sql: sql
			}), {headers: this.headers})
               .toPromise()
               .then(response => response.json())
               .catch(this.handleError);
	}
	getDatas(sql: string){
		return this.http.post(`${this.url}`, JSON.stringify({
				type: 'getDatas',
				sql: sql
			}), {headers: this.headers})
               .toPromise()
               .then(response => {
               		return response.json()
               	})
               .catch(this.handleError);
 	}
	saveData(tablename: string, keyname: string, data: Object){
		return this.http.post(`${this.url}`, JSON.stringify({
				type: 'saveData',
				tablename: tablename,
				keyname: keyname,
				data: data
			}), {headers: this.headers})
               .toPromise()
               .then(response => response.json())
               .catch(this.handleError);
	}
	startWorkflow(procdefname:string, tablename: string, keyname: string, data: Object){
		return this.http.post(`${this.url}`, JSON.stringify({
				type: 'startWorkflow',
				procdefname: procdefname,
				userid: this.getLocalData('userdata', 'id'),
				tablename: tablename,
				keyname: keyname,
				data: data
			}), {headers: this.headers})
               .toPromise()
               .then(response => response.json())
               .catch(this.handleError);
	}
	signTask(taskid){
		return this.http.post(`${this.url}`, JSON.stringify({
				type: 'signTask',
				userid: this.getLocalData('userdata', 'id'),
				taskid: taskid
			}), {headers: this.headers})
               .toPromise()
               .then(response => response.json())
               .catch(this.handleError);
	}
	getMyTaskCount(userid){
        return this.http.post(`${this.url}`, JSON.stringify({
			type: 'getMyTaskCount',
			userid: userid
		}), {headers: this.headers})
        	.toPromise()
            .then(response => response.json().data)
            .catch(this.handleError);
	}
	getMyTasks(userid){
        return this.http.post(`${this.url}`, JSON.stringify({
			type: 'getMyTasks',
			userid: userid
		}), {headers: this.headers})
        	.toPromise()
            .then(response => response.json())
            .catch(this.handleError);
	}
	private handleError(error: any): Promise<any> {
	  	console.error('An error occurred', error); // for demo purposes only
  		return Promise.reject(error.message || error);
	}
	getLocalDatas(group:string){
	  	let str = localStorage.getItem(group);
	  	if(str){
	  		return JSON.parse(str);
	  	}
		return null;
	}
	getLocalData(group:string, name: string){
	  	let str = localStorage.getItem(group);
	  	if(str){
	  		let data = JSON.parse(str);
	  		if(data!=null)
	  			return data[name];
	  	}
		return null;
	}
	setLocalData(group:string, data: any){
		localStorage.setItem(group, data!=null?JSON.stringify(data):null);
	}
	now(){
		let time = new Date();
		return this.datePipe.transform(time, 'yyyy-MM-dd HH:mm:ss')
	}
	isCadre(){
		return true;
	}
	download(id) {
		if(this.fileTransfer==null)
			this.init();
        this.platform.ready().then(() => {
		    let loader = this.loadingCtrl.create({
		      content: "正在下载..."
		    });
		    loader.present();
            let fileLocation = encodeURI(`http://192.168.217.86:8080/dataproxy/route/download.do?id=${id}`);
            this.fileTransfer.download(fileLocation, this.storageDirectory + 'file.doc').then((entry) => {
                loader.dismiss();
                if('app_aqglApk'==id)
                	FileOpener.open(`${entry.toURL()}`,'application/vnd.android.package-archive');
                else
                	FileOpener.open(`${entry.toURL()}`,'application/msword');
            }, (error) => {
            	loader.dismiss();
                let alertFailure = this.alertCtrl.create({
                    title: `Download Failed!`,
                    subTitle: `file was not successfully downloaded. Error code: ${error.code}`,
                    buttons: ['确定']
                });
                alertFailure.present();
            });
        })
	}
	upload(imageData){
		/*
		if(this.fileTransfer==null)
			this.init();
		let fileUrl = imageData;
        let fileName = fileUrl.substring(fileUrl.lastIndexOf('/')+1);
        let options = {
            'fileName': fileName
        }
		this.fileTransfer.upload(fileUrl,'http://192.168.217.86:8080/dataproxy/route/upload.do',options).then((entry) => {
		}, (error) => {
			let alertFailure = this.alertCtrl.create({
				title: `上传失败`,
				subTitle: `失败原因: ${JSON.stringify(error)}`,
				buttons: ['确定']
			});
			alertFailure.present();
		});
		*/
	}
	getSimInfo(){
    	Sim.getSimInfo().then(
	      (info) => {
	          let alert = this.alertCtrl.create({
	            title: '信息!',
	            subTitle: JSON.stringify(info),
	            buttons: ['确定']
	          });
	          alert.present();      },
	      (err) => console.log('Unable to get sim info: ', err)
	    );
  	}
  	playVoice(filename){
		var fileLocation = `${cordova.file.applicationDirectory}www/assets/audio/${filename}`;
		if(this._audio == undefined){
			this._audio = document.createElement('audio');
			this._source = document.createElement('source');
			this._audio.setAttribute('controls','controls');
			this._audio.setAttribute('autoplay','autoplay');
			this._audio.setAttribute('style','display:none');
			this._source.setAttribute('src',fileLocation);
			this._source.setAttribute('type','audio/mpeg');
			this._audio.appendChild(this._source);
			document.body.appendChild(this._audio);
		}else{
			this._audio.play();
		}
	}
}
import { Injectable } from '@angular/core';
import { Headers, Http } from '@angular/http';
import 'rxjs/add/operator/toPromise';

import { LoadingController } from 'ionic-angular';

declare var cordova: any;

@Injectable()
export class DataService {
    private headers = new Headers({'Content-Type': 'application/x-www-form-urlencoded; charset=UTF-8'});
    private url = 'http://192.168.0.105:3000';
    constructor(private http: Http, public loadingCtrl: LoadingController) {
	}
	getPlayerList(){
		return this.http.post(`${this.url}/getPlayerList`, JSON.stringify({}), {headers: this.headers})
               .toPromise()
               .then(response => response.json())
               .catch(this.handleError);
	}
    setPlayerList(data:any){
		return this.http.post(`${this.url}/setUp`, JSON.stringify(data), {headers: this.headers})
            .toPromise()
            .then(response => response.json()) 
            .catch(this.handleError);
	}
    search(){

    }
    nightArithmetic(){
            setTimeout("this.getPlayerList()",3000);
    }
    private handleError(error: any): Promise<any> {
	  	console.error('An error occurred', error); // for demo purposes only
  		return Promise.reject(error.message || error);
	}
}

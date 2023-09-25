import {Component} from '@angular/core';
import {HttpClient} from "@angular/common/http";
import {Router} from "@angular/router";
import {Observable, tap} from "rxjs";

interface IUserProfile {
  account_verification_profile: {
    birthdate: Date,
    first_name: string,
    last_name: string,
    middle_name: string,
    sex: number,
  },
  bdate: string,
  bdate_visibility: number,
  city: {
    id: number,
    title: string,
  },
  country: {
    id: number,
    title: string,
  },
  home_town: string,
  id: number,
  is_sber_verified: boolean,
  is_service_account: boolean,
  is_tinkoff_linked: boolean,
  is_tinkoff_verified: boolean,
  last_name: string,
  oauth_linked: string[],
  phone: string,
  photo_200: string,
  relation: number,
  screen_name: string,
  sex: number,
  status: string,
}

@Component({
  selector: 'app-authorization',
  templateUrl: './authorization.component.html',
  styleUrls: ['./authorization.component.scss']
})
export class AuthorizationComponent {
  public userInfo$: Observable<{response: IUserProfile}> = this.http.get<{response: IUserProfile}>('http://localhost:8080/api/authorize', {
    params: {
      'access_token': (() => {
        const url: string = this.router.url;
        const accessTokenStart: number = url.indexOf('access_token') + 'access_token'.length + 1;
        const accessTokenEnd: number = url.indexOf('expires_in') - 1;

        return url.substring(accessTokenStart, accessTokenEnd);
      })()
    }
  });


  constructor(private http: HttpClient, private router: Router) {
  }
}

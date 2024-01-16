import { Component } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Router } from '@angular/router';
import { Observable } from 'rxjs';
import { IUserProfile } from 'src/app/services/core.service';

@Component({
  selector: 'app-authorization',
  templateUrl: './authorization.component.html',
  styleUrls: ['./authorization.component.scss']
})
export class AuthorizationComponent {
  public userInfo$: Observable<{ response: IUserProfile }> = this.http.get<{ response: IUserProfile }>('http://localhost:8080/authorization', {
    params: {
      'access_token': (() => {
        const url: string = this.router.url;
        const accessTokenStart: number = url.indexOf('access_token') + 'access_token'.length + 1;
        const accessTokenEnd: number = url.indexOf('expires_in') - 1;

        return url.substring(accessTokenStart, accessTokenEnd);
      })()
    }
  });

  constructor(private http: HttpClient, private router: Router) { }
}

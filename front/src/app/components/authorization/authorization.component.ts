import {Component, OnInit} from '@angular/core';
import {HttpClient} from "@angular/common/http";
import {Router} from "@angular/router";

@Component({
  selector: 'app-authorization',
  templateUrl: './authorization.component.html',
  styleUrls: ['./authorization.component.scss']
})
export class AuthorizationComponent implements OnInit {
  constructor(private http: HttpClient, private router: Router) {
  }

  ngOnInit(): void {
    const url: string = this.router.url;
    const accessTokenStart: number = url.indexOf('access_token') + 'access_token'.length + 1;
    const accessTokenEnd: number = url.indexOf('expires_in') - 1;

    const accessToken: string = url.substring(accessTokenStart, accessTokenEnd);
    this.http.get('http://localhost:8080/api/authorize', {
      params: { 'access_token': accessToken },
      responseType: 'text' as any,
    }).subscribe((res) => console.log(res));
  }
}

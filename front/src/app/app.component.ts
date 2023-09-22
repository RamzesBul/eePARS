import {Component} from '@angular/core';
import {HttpClient} from '@angular/common/http';
import {CoreService} from "./services/core.service";
import {Observable, tap} from "rxjs";

export interface IUser {
  name: string,
  role: string,
  details: string,
  image: string,
}

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent {
  public user: Observable<IUser> = this.http.get<IUser>(`${this.coreService.APPLICATION_INFO.HOST}/api/login`).pipe(
    tap((response: IUser) => console.log(response)),
  );

  constructor(public http: HttpClient, public coreService: CoreService) {
  }
}

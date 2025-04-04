import { inject, Injectable } from '@angular/core';
import { HttpService } from './http.service';
import { distinctUntilChanged, shareReplay } from 'rxjs';

export interface UserInfo {
  id: number;
  login: string;
  email: string;
  role: number;
  score: number;
}

@Injectable({
  providedIn: 'root'
})
export class UserService {
  private readonly httpService = inject(HttpService);
  
  private readonly controllerUrl = 'getUserInfo';
  
  private readonly userInfo$ = this.httpService.makeGet<UserInfo>(this.controllerUrl).pipe(shareReplay(1));
  
  getUserInfo() {
    return this.userInfo$;
  }
}

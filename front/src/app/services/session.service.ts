import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class SessionService {
  login(token: string | null) {
    if (token) {
      localStorage.setItem('ee_user_token', token);
    } else {
      localStorage.removeItem('ee_user_token');
    }
  }
  
  logout() {
    localStorage.removeItem('ee_user_token');
  }
}

import { HttpClient, HttpHeaders, HttpParams } from '@angular/common/http';
import { inject, Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class HttpService {
  private readonly httpClient = inject(HttpClient);
  
  // private readonly serverUrl = 'http://localhost:8080';
  private readonly serverUrl = 'http://eepars.ru/api';
  
  makeGet<T>(methodName: string, params?: HttpParams) {
    const headers = this.getHttpHeaders();
    return this.httpClient.get<T>(`${this.serverUrl}/${methodName}`, { params, headers });
  }
  
  makePost<T>(methodName: string, body?: any) {
    const headers = this.getHttpHeaders();
    return this.httpClient.post<T>(`${this.serverUrl}/${methodName}`, body, { headers });
  }
  
  private getHttpHeaders(): HttpHeaders {
    const token = localStorage.getItem('ee_user_token');
    
    let headers = new HttpHeaders();
    headers = headers.set('Token', token ?? '');
    
    return headers;
  }
}

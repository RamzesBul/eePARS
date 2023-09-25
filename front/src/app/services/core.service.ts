import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class CoreService {
  public readonly ApplicationInfo: Record<string, unknown> = {
    ServerUrl: 'http://localhost:8080',
    CompanyInfo: {
      Name: 'eePARS',
      Devs: 'E&E DH',
    },
  };

  constructor() { }
}

import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class CoreService {
  public readonly APPLICATION_INFO = {
    HOST: 'http://localhost:8080',
    NAME: 'eePARS',
    DEVS: 'E&E DH',
    NAME_DETAILS: 'Processing and analysis of raw streams',
    DEVS_DATAILS: 'Embedded & enterprise development hub',
  };



  constructor() { }
}

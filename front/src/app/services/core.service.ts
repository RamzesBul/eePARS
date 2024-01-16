import { Injectable } from '@angular/core';

/**
 * User profile.
 */
export interface IUserProfile {
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

@Injectable({
  providedIn: 'root'
})
export class CoreService {
  public readonly applicationInfo: Record<string, string> = {
    serverUrl: 'http://localhost:8080',
  };

  constructor() { }
}

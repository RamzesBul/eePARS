import { inject, Injectable } from '@angular/core';
import { ActivatedRouteSnapshot, RouterStateSnapshot } from '@angular/router';
import { UserService } from './user.service';
import { RouterService } from './router.service';

@Injectable({
  providedIn: 'root'
})
export class GuardService {
  static authorizedOnly(route: ActivatedRouteSnapshot, state: RouterStateSnapshot) {
    const userService = inject(UserService);
    const routerService = inject(RouterService);
    
    return userService.getUserInfo().subscribe((user) => {
      if (!Boolean(user)) {
        routerService.navigate('/authorization');
      }
      
      return true;
    });
  }
  
  static unAuthorizedOnly(route: ActivatedRouteSnapshot, state: RouterStateSnapshot) {
    const userService = inject(UserService);
    const routerService = inject(RouterService);
    
    return userService.getUserInfo().subscribe((user) => {
      if (Boolean(user)) {
        routerService.navigate('/welcome');
      }
      
      return true;
    });
  }
}

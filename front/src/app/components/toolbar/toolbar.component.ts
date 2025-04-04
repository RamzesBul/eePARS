import { Component, inject } from '@angular/core';
import { RouterService } from 'src/app/services/router.service';
import { SessionService } from 'src/app/services/session.service';
import { UserService } from 'src/app/services/user.service';

@Component({
  selector: 'toolbar',
  templateUrl: './toolbar.component.html',
  styleUrl: './toolbar.component.scss'
})
export class ToolbarComponent {
  private readonly sessionService = inject(SessionService);
  private readonly routerService = inject(RouterService);
  private readonly userService = inject(UserService);
  
  protected readonly user$ = this.userService.getUserInfo();
  
  protected onLoginClick() {
    this.routerService.navigate('/authorization');
  }
  
  protected onLogoutClick() {
    this.sessionService.logout();
    this.routerService.reload();
  }

  protected onProfileClick() {
    this.routerService.navigate('/profile');
  }
  
  protected onGameClick() {
    
  }
  
  protected openRepository() {
    open('https://github.com/RamzesBul/eePARS');
  }
}

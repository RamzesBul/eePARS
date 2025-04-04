import { Component, inject, signal } from '@angular/core';
import { HttpService } from 'src/app/services/http.service';
import { RouterService } from 'src/app/services/router.service';
import { SessionService } from 'src/app/services/session.service';

@Component({
  selector: 'registration',
  templateUrl: './registration.component.html',
  styleUrls: ['./registration.component.scss']
})
export class RegistrationComponent {
  private readonly httpService = inject(HttpService);
  private readonly sessionService = inject(SessionService);
  private readonly routerService = inject(RouterService);
  
  private readonly controllerUrl = 'registration';
  
  protected readonly showLoader = signal<boolean>(true);
  
  protected login: string = '';
  protected email: string = '';
  protected password: string = '';
  
  ngOnInit(): void {
    this.showLoader.set(false);
  }
  
  protected onRegisterClick() {
    this.showLoader.set(true);
    
    const body = {
      login: this.login,
      email: this.email,
      password: this.password,
    };
    
    this.httpService.makePost<string>(this.controllerUrl, body).subscribe({
      next: (token) => {
        this.sessionService.login(token);
        this.routerService.reload();
      },
      error: (error) => {
        console.error(error);
        this.showLoader.set(false);
      }
    });
  }
}

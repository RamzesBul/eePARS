import { Component, inject, OnInit, signal } from '@angular/core';
import { UserService } from 'src/app/services/user.service';

@Component({
  selector: 'welcome',
  templateUrl: './welcome.component.html',
  styleUrls: ['./welcome.component.scss']
})
export class WelcomeComponent implements OnInit {
  private readonly userService = inject(UserService);
  
  protected readonly showLoader = signal<boolean>(true);
  
  protected readonly user$ = this.userService.getUserInfo();
  
  ngOnInit(): void {
    this.showLoader.set(false);
  }
}
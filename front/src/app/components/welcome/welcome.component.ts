import { Component } from '@angular/core';

@Component({
  selector: 'app-welcome',
  templateUrl: './welcome.component.html',
  styleUrls: ['./welcome.component.scss']
})
export class WelcomeComponent {
  private getTokenUri: string = 'https://oauth.vk.com/authorize?client_id=51737169&display=page&redirect_uri=http://localhost:4200/authorization&scope=friends&response_type=token&v=5.131&state=123456';

  onLogInButtonClick() {
    open(this.getTokenUri);
  }
}

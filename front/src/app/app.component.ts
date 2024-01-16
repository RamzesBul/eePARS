import { Component } from '@angular/core';

/**
 * Application.
 */
@Component({
  selector: 'ee-app',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss'],
})
export class AppComponent {
  constructor() { }

  public openRepository() {
    open('https://github.com/RamzesBul/eePARS');
  }
}

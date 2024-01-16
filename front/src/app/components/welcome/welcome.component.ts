import { HttpClient } from '@angular/common/http';
import { Component } from '@angular/core';
import { Observable } from 'rxjs';
import { CoreService } from 'src/app/services/core.service';

/**
 * 
 */
@Component({
  selector: 'app-welcome',
  templateUrl: './welcome.component.html',
  styleUrls: ['./welcome.component.scss']
})
export class WelcomeComponent {
  private readonly tokenUrl: string = `${this.coreService.applicationInfo['serverUrl']}/welcome`;

  public tokenUrl$: Observable<string> = this.http.get<string>(this.tokenUrl, { responseType: 'text' as 'json' });

  constructor(private http: HttpClient, private coreService: CoreService) { }

  public onLogInButtonClick(tokenUrl: string): void {
    open(tokenUrl);
  }
}
import {Component, OnInit} from '@angular/core';
import {HttpClient} from '@angular/common/http';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent implements OnInit {
  private HOST: string = 'http://localhost:8080';

  constructor(public http: HttpClient) {
  }

  ngOnInit(): void {
    this.http.get(`${this.HOST}/api/hello`)
      .subscribe((response) => {
        console.log(response);
      });
  }
}

import { Component } from '@angular/core';
import {CoreService} from "../../services/core.service";

@Component({
  selector: 'eepars-header',
  templateUrl: 'header.component.html',
  styleUrls: ['header.component.scss'],
})
export class HeaderComponent {
  readonly headerOptions = {
    color: 'primary',
  };

  readonly applcationInfo = this.core.APPLICATION_INFO;

  constructor(public core: CoreService) {
  }
}

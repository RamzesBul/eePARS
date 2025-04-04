import { inject, Injectable } from '@angular/core';
import { Router } from '@angular/router';

@Injectable({
  providedIn: 'root'
})
export class RouterService {
  private readonly router = inject(Router);

  navigate(url: string) {
    console.log(url);
    
    this.router.navigateByUrl(url);
  }

  reload() {
    window.location.reload()
  }
}

import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { AuthorizationComponent } from './components/authorization/authorization.component';
import { WelcomeComponent } from './components/welcome/welcome.component';
import { RegistrationComponent } from './components/registration/registration.component';
import { ProfileComponent } from './components/profile/profile.component';
import { GuardService } from './services/guard.service';

const routes: Routes = [
  { path: '', redirectTo: 'welcome', pathMatch: 'full' },
  { path: 'profile', component: ProfileComponent, canActivate: [GuardService.authorizedOnly] },
  { path: 'welcome', component: WelcomeComponent },
  { path: 'registration', component: RegistrationComponent, canActivate: [GuardService.unAuthorizedOnly] },
  { path: 'authorization', component: AuthorizationComponent, canActivate: [GuardService.unAuthorizedOnly] },
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }

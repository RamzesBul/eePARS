import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { AuthorizationComponent } from "./components/authorization/authorization.component";
import { WelcomeComponent } from "./components/welcome/welcome.component";

const routes: Routes = [
  { path: 'welcome', component: WelcomeComponent },
  { path: 'authorization', component: AuthorizationComponent },
  { path: '**', redirectTo: 'welcome' },
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }

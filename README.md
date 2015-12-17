# Qt_freesoundAPIv2
A demo app written in c++ and the Qt framework that authenticates with the freesound.org sound repository using the APIv2 and downloads a file.

So as a first step to restoring freesound support to the Kdenlive video editor I have created my self a little demo application that makes use of the ver2 API.

It is written in C++ using the Qt framework. I thought it might be a useful addition to the client libraries at http://www.freesound.org/docs/api/client_libs.html

 This program authenticates at freesound, obtains an authorisation code from freesound - if the user has authorised the app to use freesound.
It then obtains an access_token and finally uses that access token to download a file
 
 Based on code at  https://code.google.com/p/qt-oauth-lib/
  Which is Qt Library created by Integrated Computer Solutions, Inc. (ICS) to provide OAuth2.0 for the Google API.
 
  Licence: GNU Lesser General Public License
  http://www.gnu.org/licenses/lgpl.html
  
  in oauth2.cpp you need to set up your client ID, client secret and redirect URL that you will have obtained from freesound  http://www.freesound.org/apiv2/apply/ 

# Pre Requirements

1. Install [NodeJS](https://nodejs.org/en/) **v8.5.0** or later.

2. Install [Java Development Kit (JDK) 8](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html).

3. Install [Android Studio](https://developer.android.com/studio/index.html). Detailed installation instructions are on Android's developer site.

4. After installing the Android SDK, you must also install the packages for whatever API level you wish to target. It is recommended that you install the highest SDK version that your version of cordova-android supports.

   Open the Android SDK Manager (run android or sdkmanager from the terminal) and make sure the following are installed:

   + Android Platform SDK for your targeted version of Android
   
   + Android SDK build-tools version 19.1.0 or higher
   
   + Android Support Repository (found under "Extras")

# Setup

1. Load Modules

   `npm install`

2. Load Typings

   `./node_modules/.bin/typings install -g`

# Compile

1. Compile just once

   `./node_modules/.bin/gulp --cwd . --gulpfile scripts/gulpfile.js Debug`
   
2. Compile everytime a file has changed

   `./node_modules/.bin/gulp --cwd . --gulpfile scripts/gulpfile.js Watch`

# Debug

1. Just open `./src/client/index.html`

2. Run `node src/app.js` and open http://localhost/src/client/

3. Start [VSCode](https://code.visualstudio.com/) Debugger and open http://localhost/src/client/

# Release

1.  <img alt="In Progress" src="https://i.stack.imgur.com/W08Uq.png" width="48"> `./node_modules/.bin/gulp --cwd . --gulpfile scripts/gulpfile.js Release`

2. `./node_modules/.bin/cordova build`

3. `./node_modules/.bin/cordova emulate android`

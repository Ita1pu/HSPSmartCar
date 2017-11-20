# Pre Requirements

Install [NodeJS](https://nodejs.org/en/) **v8.5.0** or higher

# Setup

1. Load Modules

   `npm install`

2. Load Typings

   `.\node_modules\.bin\typings install -g`

# Compile

1. Compile just once

   `.\node_modules\.bin\gulp --cwd %cd% --gulpfile scripts/gulpfile.js Debug`
   
2. Compile everytime a file has changed

   `.\node_modules\.bin\gulp --cwd %cd% --gulpfile scripts/gulpfile.js Watch`

# Debug

1. Just open `./src/client/index.html`

2. Run `node src/app.js` and open http://localhost/src/client/

3. Start [VSCode](https://code.visualstudio.com/) Debugger and open http://localhost/src/client/

# Release

Not available yet.

